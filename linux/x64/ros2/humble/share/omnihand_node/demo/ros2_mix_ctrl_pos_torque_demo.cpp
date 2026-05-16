// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file ros2_mix_ctrl_pos_torque_demo.cpp
 * @brief ROS2 C++ demo - mix control (position + torque)
 *
 * Demonstrates POSITION_TORQUE mode via the joint_mix_control_cmd topic,
 * while also reading joint states, temperature, and current.
 *
 * ======================================================================
 * Control Modes
 * ======================================================================
 * Each product supports a different subset of ControlMode:
 *
 *   O10 (OmniHand 2025):
 *     - POSITION (0):                 position control
 *     - POSITION_TORQUE (4):          position + torque
 *     - POSITION_VELOCITY_TORQUE (6): position + velocity + torque
 *
 *   O12 (OmniHand Pro 2025):
 *     - POSITION (0):        position control
 *     - POSITION_TORQUE (4): position + torque
 *
 * Note: VELOCITY_TORQUE (5) is defined in the protocol but not supported by
 * either O10 or O12. POSITION_VELOCITY_TORQUE (6) is only supported by O10.
 *
 * ======================================================================
 * Message types used
 * ======================================================================
 *   - sensor_msgs/JointState        : mix control (position[] + effort[])
 *       position[] = raw motor position (0–4095)
 *       effort[]   = motor current in mA (range 0–1000, non-standard, not N·m)
 *   - std_msgs/Int16MultiArray : current / temperature / current-threshold (mA / °C)
 *   - std_msgs/Empty                : temperature / current query triggers
 *
 * ======================================================================
 * Quick Start (after obtaining the OmniHand release package)
 * ======================================================================
 *
 * 1. Ensure ROS2 Humble is installed and sourced:
 *      source /opt/ros/humble/setup.bash
 *
 * 2. Source the OmniHand release ROS2 environment:
 *      source /path/to/omnihand_sdk_release/ros2/setup.bash
 *
 * 3. Create a colcon workspace and copy the demo package:
 *      mkdir -p ~/omnihand_ws/src
 *      cp -r /path/to/omnihand_release/ros2/humble/share/omnihand_node/demo \
 *            ~/omnihand_ws/src/omnihand_ros2_demo
 *
 * 4. Build:
 *      cd ~/omnihand_ws
 *      colcon build --packages-select omnihand_ros2_demo
 *      source install/setup.bash
 *
 * 5. Launch the OmniHand ROS2 node (in another terminal):
 *      source /opt/ros/humble/setup.bash
 *      source /path/to/omnihand_release/ros2/setup.bash
 *      ros2 run omnihand_node omnihand_2025_node \
 *          --ros-args --params-file /path/to/omnihand_release/ros2/humble/share/omnihand_node/config/omnihand_2025_node.yaml
 *
 * 6. Run the demo:
 *      ros2 run omnihand_ros2_demo ros2_mix_ctrl_pos_torque_demo left o10
 *      ros2 run omnihand_ros2_demo ros2_mix_ctrl_pos_torque_demo left o10 500   # optional threshold (mA)
 *
 * Arguments: [left|right] [o10|o12|h3l|h3u_m] [current_threshold_mA]
 *
 * ======================================================================
 * Topic list (<product> = o10/o12/h3l/h3u_m, <side> = left/right)
 * ======================================================================
 *   pub: /<product>/<side>/joint_mix_control_cmd  (sensor_msgs/JointState, position+effort)
 *   pub: /<product>/<side>/joint_temperature_cmd  (std_msgs/Empty, trigger)
 *   pub: /<product>/<side>/joint_current_cmd      (std_msgs/Empty, trigger)
 *   pub: /<product>/<side>/tactile_cmd            (std_msgs/Empty, trigger)
 *   sub: /<product>/<side>/joint_states           (sensor_msgs/JointState)
 *   sub: /<product>/<side>/joint_temperature_states (std_msgs/Int16MultiArray)
 *   sub: /<product>/<side>/joint_current_states     (std_msgs/Int16MultiArray)
 *   pub: /<product>/<side>/joint_current_threshold_cmd  (std_msgs/Int16MultiArray, write)
 *   sub: /<product>/<side>/joint_current_threshold_states (std_msgs/Int16MultiArray, readback)
 *   sub: /<product>/<side>/tactile_states           (omnihand_2025_node_msgs/TactileSensor or omnihand_pro_2025_node_msgs/TactileSensor)
 *
 * Trigger-based Readback:
 *   The OmniHand ROS2 node never publishes state on a periodic timer.
 *   Temperature, current, and other *_states are only published once after
 *   you send the corresponding *_cmd message.
 *   Example: publish Empty to joint_temperature_cmd → receive one joint_temperature_states.
 *   Exception: joint_cmd / joint_mix_control_cmd automatically trigger joint_states readback.
 *   Current threshold: publish Int16MultiArray to joint_current_threshold_cmd once;
 *   the node sets thresholds and publishes readback on joint_current_threshold_states.
 *   Rationale: avoid consuming CAN bus bandwidth, ensure real-time control responsiveness.
 *
 * ======================================================================
 * Writing your own ROS2 C++ package from scratch
 * ======================================================================
 *
 * Only two files are needed:
 *
 * package.xml:
 *   <depend>rclcpp</depend>
 *   <depend>sensor_msgs</depend>
 *   <depend>std_msgs</depend>
 *   <depend>omnihand_2025_node_msgs</depend>
 *   <depend>omnihand_pro_2025_node_msgs</depend>
 *
 * CMakeLists.txt:
 *   find_package(rclcpp REQUIRED)
 *   find_package(sensor_msgs REQUIRED)
 *   find_package(std_msgs REQUIRED)
 *   find_package(omnihand_2025_node_msgs REQUIRED)
 *   find_package(omnihand_pro_2025_node_msgs REQUIRED)
 *   ament_target_dependencies(your_node rclcpp sensor_msgs std_msgs
 *       omnihand_2025_node_msgs omnihand_pro_2025_node_msgs)
 */

#include <chrono>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/empty.hpp"
#include "std_msgs/msg/int16_multi_array.hpp"
#include "omnihand_2025_node_msgs/msg/tactile_sensor.hpp"
#include "omnihand_pro_2025_node_msgs/msg/tactile_sensor.hpp"
#include "ros_multi_array_demo.hpp"

using namespace std::chrono_literals;
using agilink::omnihand::ros2::PackInt16MultiArray1D;
using sensor_msgs::msg::JointState;
using std_msgs::msg::Empty;
using std_msgs::msg::Int16MultiArray;
using O10Tactile = omnihand_2025_node_msgs::msg::TactileSensor;
using O12Tactile = omnihand_pro_2025_node_msgs::msg::TactileSensor;

class MixCtrlPosTorqueDemo : public rclcpp::Node {
 public:
  MixCtrlPosTorqueDemo(const std::string& product, const std::string& side,
                       int num_joints, int16_t current_threshold_ma)
      : Node(product + "_" + side + "_mix_ctrl_pos_torque_demo"),
        product_(product),
        side_(side),
        num_joints_(num_joints),
        current_threshold_ma_(current_threshold_ma),
        cycle_(0) {
    std::string prefix = "/" + product + "/" + side;

    mix_ctrl_pub_ = this->create_publisher<JointState>(
        prefix + "/joint_mix_control_cmd", 10);
    joint_states_sub_ = this->create_subscription<JointState>(
        prefix + "/joint_states", 10,
        std::bind(&MixCtrlPosTorqueDemo::OnJointStates, this, std::placeholders::_1));

    temp_cmd_pub_ = this->create_publisher<Empty>(
        prefix + "/joint_temperature_cmd", 10);
    temp_states_sub_ = this->create_subscription<Int16MultiArray>(
        prefix + "/joint_temperature_states", 10,
        std::bind(&MixCtrlPosTorqueDemo::OnTemperature, this, std::placeholders::_1));

    current_cmd_pub_ = this->create_publisher<Empty>(
        prefix + "/joint_current_cmd", 10);
    current_states_sub_ = this->create_subscription<Int16MultiArray>(
        prefix + "/joint_current_states", 10,
        std::bind(&MixCtrlPosTorqueDemo::OnCurrent, this, std::placeholders::_1));

    current_threshold_cmd_pub_ = this->create_publisher<Int16MultiArray>(
        prefix + "/joint_current_threshold_cmd", 10);
    current_threshold_states_sub_ = this->create_subscription<Int16MultiArray>(
        prefix + "/joint_current_threshold_states", 10,
        std::bind(&MixCtrlPosTorqueDemo::OnCurrentThreshold, this, std::placeholders::_1));

    tactile_cmd_pub_ = this->create_publisher<Empty>(
        prefix + "/tactile_cmd", 10);
    if (product == "o10") {
      o10_tactile_sub_ = this->create_subscription<O10Tactile>(
          prefix + "/tactile_states", 10,
          std::bind(&MixCtrlPosTorqueDemo::OnO10Tactile, this, std::placeholders::_1));
    } else if (product == "o12") {
      o12_tactile_sub_ = this->create_subscription<O12Tactile>(
          prefix + "/tactile_states", 10,
          std::bind(&MixCtrlPosTorqueDemo::OnO12Tactile, this, std::placeholders::_1));
    }

    timer_ = this->create_wall_timer(1500ms,
                                     std::bind(&MixCtrlPosTorqueDemo::OnTimer, this));

    RCLCPP_INFO(this->get_logger(),
                "MixCtrlPosTorque demo started: %s/%s (%d DOF)",
                product.c_str(), side.c_str(), num_joints);
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_mix_control_cmd (sensor_msgs/JointState, position+effort)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_temperature_cmd (std_msgs/Empty, trigger)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_current_cmd (std_msgs/Empty, trigger)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(),
                "  pub -> %s/joint_current_threshold_cmd (std_msgs/Int16MultiArray, %d mA x %d joints)",
                prefix.c_str(), current_threshold_ma_, num_joints_);
  }

 private:
  void OnTimer() {
    JointState mix_cmd;
    mix_cmd.header.stamp = this->now();

    if (cycle_ % 2 == 0) {
      mix_cmd.position.assign(num_joints_, 2000.0);
      mix_cmd.effort.assign(num_joints_, 50.0);
    } else {
      mix_cmd.position.assign(num_joints_, 1000.0);
      mix_cmd.effort.assign(num_joints_, 150.0);
    }

    RCLCPP_INFO(this->get_logger(),
                "Sending mix_ctrl [POSITION_TORQUE]: pos=%.0f, effort=%.0f",
                mix_cmd.position[0],
                mix_cmd.effort[0]);
    mix_ctrl_pub_->publish(mix_cmd);

    // Write current thresholds once on the first cycle (node readbacks on *_states).
    if (cycle_ == 0) {
      std::vector<int16_t> thresholds(num_joints_, current_threshold_ma_);
      current_threshold_cmd_pub_->publish(PackInt16MultiArray1D(thresholds));
    }

    temp_cmd_pub_->publish(Empty());

    current_cmd_pub_->publish(Empty());

    Empty tactile_trigger;
    tactile_cmd_pub_->publish(tactile_trigger);

    cycle_++;
  }

  void OnJointStates(const JointState::SharedPtr msg) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << "joint_states (rad): [";
    for (size_t i = 0; i < msg->position.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << msg->position[i];
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnTemperature(const Int16MultiArray::SharedPtr msg) {
    std::ostringstream oss;
    oss << "temperature (°C): [";
    for (size_t i = 0; i < msg->data.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << static_cast<int>(msg->data[i]);
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnCurrent(const Int16MultiArray::SharedPtr msg) {
    std::ostringstream oss;
    oss << "current (mA): [";
    for (size_t i = 0; i < msg->data.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << msg->data[i];
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnCurrentThreshold(const Int16MultiArray::SharedPtr msg) {
    std::ostringstream oss;
    oss << "current_threshold (mA): [";
    for (size_t i = 0; i < msg->data.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << msg->data[i];
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnO10Tactile(const O10Tactile::SharedPtr msg) {
    std::ostringstream oss;
    oss << "tactile (O10, 1D):";
    bool first = true;
    auto append = [&oss, &first](const char* label, const std::vector<uint8_t>& v) {
      if (v.empty()) return;
      if (!first) oss << " | ";
      first = false;
      oss << " " << label << "[";
      for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) oss << ",";
        oss << static_cast<int>(v[i]);
      }
      oss << "]";
    };
    append("thumb", msg->thumb);
    append("index", msg->index);
    append("middle", msg->middle);
    append("ring", msg->ring);
    append("little", msg->little);
    append("palm", msg->palm);
    append("dorsum", msg->dorsum);
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnO12Tactile(const O12Tactile::SharedPtr msg) {
    std::ostringstream oss;
    oss << "tactile (O12, 3D): [";
    auto one = [&](const char* lab, const omnihand_pro_2025_node_msgs::msg::TactileSensorData& td) {
      oss << lab << ":Fn=" << td.normal_force * 0.1 << "N,Ft=" << td.tangent_force << " | ";
    };
    one("thumb", msg->thumb);
    one("index", msg->index);
    one("middle", msg->middle);
    one("ring", msg->ring);
    one("little", msg->little);
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  rclcpp::Publisher<JointState>::SharedPtr mix_ctrl_pub_;
  rclcpp::Subscription<JointState>::SharedPtr joint_states_sub_;
  rclcpp::Publisher<Empty>::SharedPtr temp_cmd_pub_;
  rclcpp::Subscription<Int16MultiArray>::SharedPtr temp_states_sub_;
  rclcpp::Publisher<Empty>::SharedPtr current_cmd_pub_;
  rclcpp::Subscription<Int16MultiArray>::SharedPtr current_states_sub_;
  rclcpp::Publisher<Int16MultiArray>::SharedPtr current_threshold_cmd_pub_;
  rclcpp::Subscription<Int16MultiArray>::SharedPtr current_threshold_states_sub_;
  rclcpp::Publisher<Empty>::SharedPtr tactile_cmd_pub_;
  rclcpp::Subscription<O10Tactile>::SharedPtr o10_tactile_sub_;
  rclcpp::Subscription<O12Tactile>::SharedPtr o12_tactile_sub_;

  rclcpp::TimerBase::SharedPtr timer_;
  std::string product_;
  std::string side_;
  int num_joints_;
  int16_t current_threshold_ma_;
  int cycle_;
};

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  std::string side = "left";
  std::string product = "o10";

  if (argc > 1) side = argv[1];
  if (argc > 2) product = argv[2];

  int16_t current_threshold_ma = 500;
  if (argc > 3) current_threshold_ma = static_cast<int16_t>(std::stoi(argv[3]));

  int num_joints = 10;
  if (product == "o12")
    num_joints = 12;
  else if (product == "h3l")
    num_joints = 4;
  else if (product == "h3u_m")
    num_joints = 20;

  auto node = std::make_shared<MixCtrlPosTorqueDemo>(product, side, num_joints, current_threshold_ma);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}