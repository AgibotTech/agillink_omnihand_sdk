// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file ros2_mix_ctrl_pos_vel_torque_demo.cpp
 * @brief ROS2 C++ demo - mix control (position + velocity + torque)
 *
 * Demonstrates POSITION_VELOCITY_TORQUE mode via the joint_mix_control_cmd topic,
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
 *   - sensor_msgs/JointState        : mix control (position[] + velocity[] + effort[])
 *   - omnihand_msgs/JointStateInt16 : current readback (data[] = int16)
 *   - omnihand_msgs/JointStateInt8  : temperature/error readback (data[] = int8)
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
 *      ros2 run omnihand_ros2_demo ros2_mix_ctrl_pos_vel_torque_demo left o10
 *
 * ======================================================================
 * Topic list (<product> = o10/o12/h3u_m, <side> = left/right)
 * ======================================================================
 *   pub: /<product>/<side>/joint_mix_control_cmd  (sensor_msgs/JointState, position+velocity+effort)
 *   pub: /<product>/<side>/joint_temperature_cmd  (omnihand_msgs/JointStateInt8, trigger)
 *   pub: /<product>/<side>/joint_current_cmd      (omnihand_msgs/JointStateInt16, trigger)
 *   sub: /<product>/<side>/joint_states           (sensor_msgs/JointState)
 *   sub: /<product>/<side>/joint_temperature_states (omnihand_msgs/JointStateInt8)
 *   sub: /<product>/<side>/joint_current_states     (omnihand_msgs/JointStateInt16)
 *
 * Trigger-based Readback:
 *   The OmniHand ROS2 node never publishes state on a periodic timer.
 *   Temperature, current, and other *_states are only published once after
 *   you send the corresponding *_cmd message.
 *   Example: publish Empty to joint_temperature_cmd → receive one joint_temperature_states.
 *   Exception: joint_cmd / joint_mix_control_cmd automatically trigger joint_states readback.
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
 *   <depend>omnihand_msgs</depend>
 *
 * CMakeLists.txt:
 *   find_package(rclcpp REQUIRED)
 *   find_package(sensor_msgs REQUIRED)
 *   find_package(omnihand_msgs REQUIRED)
 *   ament_target_dependencies(your_node rclcpp sensor_msgs omnihand_msgs)
 */

#include <chrono>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "omnihand_msgs/msg/joint_state_int16.hpp"
#include "omnihand_msgs/msg/joint_state_int8.hpp"

using namespace std::chrono_literals;
using sensor_msgs::msg::JointState;
using omnihand_msgs::msg::JointStateInt16;
using omnihand_msgs::msg::JointStateInt8;

class MixCtrlPosVelTorqueDemo : public rclcpp::Node {
 public:
  MixCtrlPosVelTorqueDemo(const std::string& product, const std::string& side,
                          int num_joints)
      : Node(product + "_" + side + "_mix_ctrl_pos_vel_torque_demo"),
        product_(product),
        side_(side),
        num_joints_(num_joints),
        cycle_(0) {
    std::string prefix = "/" + product + "/" + side;

    mix_ctrl_pub_ = this->create_publisher<JointState>(
        prefix + "/joint_mix_control_cmd", 10);
    joint_states_sub_ = this->create_subscription<JointState>(
        prefix + "/joint_states", 10,
        std::bind(&MixCtrlPosVelTorqueDemo::OnJointStates, this, std::placeholders::_1));

    temp_cmd_pub_ = this->create_publisher<JointStateInt8>(
        prefix + "/joint_temperature_cmd", 10);
    temp_states_sub_ = this->create_subscription<JointStateInt8>(
        prefix + "/joint_temperature_states", 10,
        std::bind(&MixCtrlPosVelTorqueDemo::OnTemperature, this, std::placeholders::_1));

    current_cmd_pub_ = this->create_publisher<JointStateInt16>(
        prefix + "/joint_current_cmd", 10);
    current_states_sub_ = this->create_subscription<JointStateInt16>(
        prefix + "/joint_current_states", 10,
        std::bind(&MixCtrlPosVelTorqueDemo::OnCurrent, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(1500ms,
                                     std::bind(&MixCtrlPosVelTorqueDemo::OnTimer, this));

    RCLCPP_INFO(this->get_logger(),
                "MixCtrlPosVelTorque demo started: %s/%s (%d DOF)",
                product.c_str(), side.c_str(), num_joints);
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_mix_control_cmd (sensor_msgs/JointState, position+velocity+effort)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_temperature_cmd (omnihand_msgs/JointStateInt8, trigger)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_current_cmd (omnihand_msgs/JointStateInt16, trigger)",
                prefix.c_str());
  }

 private:
  void OnTimer() {
    JointState mix_cmd;
    mix_cmd.header.stamp = this->now();

    if (cycle_ % 2 == 0) {
      mix_cmd.position.assign(num_joints_, 2000.0);
      mix_cmd.velocity.assign(num_joints_, 500.0);
      mix_cmd.effort.assign(num_joints_, 50.0);
    } else {
      mix_cmd.position.assign(num_joints_, 1000.0);
      mix_cmd.velocity.assign(num_joints_, 300.0);
      mix_cmd.effort.assign(num_joints_, 150.0);
    }

    RCLCPP_INFO(this->get_logger(),
                "Sending mix_ctrl [POSITION_VELOCITY_TORQUE]: pos=%.0f, vel=%.0f, effort=%.0f",
                mix_cmd.position[0],
                mix_cmd.velocity[0],
                mix_cmd.effort[0]);
    mix_ctrl_pub_->publish(mix_cmd);

    JointStateInt8 temp_trigger;
    temp_trigger.header.stamp = this->now();
    temp_cmd_pub_->publish(temp_trigger);

    JointStateInt16 current_trigger;
    current_trigger.header.stamp = this->now();
    current_cmd_pub_->publish(current_trigger);

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

  void OnTemperature(const JointStateInt8::SharedPtr msg) {
    std::ostringstream oss;
    oss << "temperature (°C): [";
    for (size_t i = 0; i < msg->data.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << static_cast<int>(msg->data[i]);
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnCurrent(const JointStateInt16::SharedPtr msg) {
    std::ostringstream oss;
    oss << "current (mA): [";
    for (size_t i = 0; i < msg->data.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << msg->data[i];
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  rclcpp::Publisher<JointState>::SharedPtr mix_ctrl_pub_;
  rclcpp::Subscription<JointState>::SharedPtr joint_states_sub_;
  rclcpp::Publisher<JointStateInt8>::SharedPtr temp_cmd_pub_;
  rclcpp::Subscription<JointStateInt8>::SharedPtr temp_states_sub_;
  rclcpp::Publisher<JointStateInt16>::SharedPtr current_cmd_pub_;
  rclcpp::Subscription<JointStateInt16>::SharedPtr current_states_sub_;

  rclcpp::TimerBase::SharedPtr timer_;
  std::string product_;
  std::string side_;
  int num_joints_;
  int cycle_;
};

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  std::string side = "left";
  std::string product = "o10";

  if (argc > 1) side = argv[1];
  if (argc > 2) product = argv[2];

  int num_joints = 10;
  if (product == "o12")
    num_joints = 12;
  else if (product == "h3u_m")
    num_joints = 20;

  auto node = std::make_shared<MixCtrlPosVelTorqueDemo>(product, side, num_joints);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}