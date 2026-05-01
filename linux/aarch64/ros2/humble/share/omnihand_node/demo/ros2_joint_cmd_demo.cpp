// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file ros2_joint_cmd_demo.cpp
 * @brief ROS2 C++ 示例 - 通过 ROS2 topic 控制 OmniHand 并读取传感器数据
 *
 * 此示例演示 C++ 开发者如何通过 ROS2 接口控制 OmniHand 灵巧手，
 * 不直接依赖 OmniHand C++ SDK，仅通过标准 ROS2 消息通信。
 *
 * 使用的消息类型:
 *   - sensor_msgs/JointState        : 关节位置控制 (position[] = rad)
 *   - omnihand_msgs/JointStateInt16 : 电流读取 (data[] = int16)
 *   - omnihand_msgs/JointStateInt8  : 温度/错误码读取 (data[] = int8)
 *
 * ======================================================================
 * 快速开始 (拿到 OmniHand release 包后)
 * ======================================================================
 *
 * 1. 确保已安装 ROS2 Humble 并 source 了环境:
 *      source /opt/ros/humble/setup.bash
 *
 * 2. Source OmniHand release 的 ROS2 环境:
 *      source /path/to/omnihand_sdk_release/ros2/setup.bash
 *    这一步将 omnihand_msgs 等包加入 CMAKE_PREFIX_PATH，
 *    使得你的程序可以 find_package(omnihand_msgs)。
 *
 * 3. 创建你的 colcon workspace 并拷入 demo 包:
 *      mkdir -p ~/omnihand_ws/src
 *      cp -r /path/to/omnihand_release/ros2/humble/share/omnihand_node/demo \
 *            ~/omnihand_ws/src/omnihand_ros2_demo
 *
 * 4. 编译:
 *      cd ~/omnihand_ws
 *      colcon build --packages-select omnihand_ros2_demo
 *      source install/setup.bash
 *
 * 5. 启动 OmniHand ROS2 node (另一个终端):
 *      source /opt/ros/humble/setup.bash
 *      source /path/to/omnihand_release/ros2/setup.bash
 *      ros2 run omnihand_node omnihand_2025_node \
 *          --ros-args --params-file /path/to/omnihand_release/ros2/humble/share/omnihand_node/config/omnihand_2025_node.yaml
 *
 * 6. 运行 demo:
 *      ros2 run omnihand_ros2_demo ros2_joint_cmd_demo left o10
 *
 * ======================================================================
 * topic 列表 (<product> = o10/o12/h3u_m, <side> = left/right)
 * ======================================================================
 *   pub: /<product>/<side>/joint_cmd              (sensor_msgs/JointState)
 *   pub: /<product>/<side>/joint_temperature_cmd  (omnihand_msgs/JointStateInt8, trigger)
 *   pub: /<product>/<side>/joint_current_cmd      (omnihand_msgs/JointStateInt16, trigger)
 *   sub: /<product>/<side>/joint_states           (sensor_msgs/JointState)
 *   sub: /<product>/<side>/joint_temperature_states (omnihand_msgs/JointStateInt8)
 *   sub: /<product>/<side>/joint_current_states     (omnihand_msgs/JointStateInt16)
 *
 * ======================================================================
 * 自己从零写 ROS2 C++ 包
 * ======================================================================
 *
 * 只需两个文件:
 *
 * package.xml:
 *   <depend>rclcpp</depend>
 *   <depend>sensor_msgs</depend>
 *   <depend>omnihand_msgs</depend>     <!-- OmniHand 自定义消息 -->
 *
 * CMakeLists.txt:
 *   find_package(rclcpp REQUIRED)
 *   find_package(sensor_msgs REQUIRED)
 *   find_package(omnihand_msgs REQUIRED)   # 由 release/ros2/setup.bash 提供
 *   ament_target_dependencies(your_node rclcpp sensor_msgs omnihand_msgs)
 *
 * 如果只需关节位置控制，可以不依赖 omnihand_msgs，仅用 sensor_msgs 即可。
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

class JointCmdDemo : public rclcpp::Node {
 public:
  JointCmdDemo(const std::string& product, const std::string& side,
               int num_joints)
      : Node(product + "_" + side + "_joint_cmd_demo"),
        product_(product),
        side_(side),
        num_joints_(num_joints),
        cycle_(0) {
    std::string prefix = "/" + product + "/" + side;

    // --- joint position: pub cmd + sub states ---
    joint_cmd_pub_ = this->create_publisher<JointState>(
        prefix + "/joint_cmd", 10);
    joint_states_sub_ = this->create_subscription<JointState>(
        prefix + "/joint_states", 10,
        std::bind(&JointCmdDemo::OnJointStates, this, std::placeholders::_1));

    // --- temperature: pub trigger + sub states (omnihand_msgs/JointStateInt8) ---
    temp_cmd_pub_ = this->create_publisher<JointStateInt8>(
        prefix + "/joint_temperature_cmd", 10);
    temp_states_sub_ = this->create_subscription<JointStateInt8>(
        prefix + "/joint_temperature_states", 10,
        std::bind(&JointCmdDemo::OnTemperature, this, std::placeholders::_1));

    // --- current: pub trigger + sub states (omnihand_msgs/JointStateInt16) ---
    current_cmd_pub_ = this->create_publisher<JointStateInt16>(
        prefix + "/joint_current_cmd", 10);
    current_states_sub_ = this->create_subscription<JointStateInt16>(
        prefix + "/joint_current_states", 10,
        std::bind(&JointCmdDemo::OnCurrent, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(1500ms,
                                     std::bind(&JointCmdDemo::OnTimer, this));

    RCLCPP_INFO(this->get_logger(),
                "Demo started: %s/%s (%d DOF)",
                product.c_str(), side.c_str(), num_joints);
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_cmd (sensor_msgs/JointState)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_temperature_cmd (omnihand_msgs/JointStateInt8, trigger)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_current_cmd (omnihand_msgs/JointStateInt16, trigger)",
                prefix.c_str());
  }

 private:
  void OnTimer() {
    // 1. Send joint position command
    JointState cmd;
    cmd.header.stamp = this->now();
    if (cycle_ % 2 == 0) {
      cmd.position.assign(num_joints_, 0.0);
      RCLCPP_INFO(this->get_logger(), "Sending: OPEN (all 0.0 rad)");
    } else {
      cmd.position.assign(num_joints_, 0.6);
      RCLCPP_INFO(this->get_logger(), "Sending: CLOSE (all 0.6 rad)");
    }
    joint_cmd_pub_->publish(cmd);

    // 2. Trigger temperature readback (send empty msg as trigger)
    JointStateInt8 temp_trigger;
    temp_trigger.header.stamp = this->now();
    temp_cmd_pub_->publish(temp_trigger);

    // 3. Trigger current readback (send empty msg as trigger)
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

  // Joint position
  rclcpp::Publisher<JointState>::SharedPtr joint_cmd_pub_;
  rclcpp::Subscription<JointState>::SharedPtr joint_states_sub_;
  // Temperature (omnihand_msgs/JointStateInt8)
  rclcpp::Publisher<JointStateInt8>::SharedPtr temp_cmd_pub_;
  rclcpp::Subscription<JointStateInt8>::SharedPtr temp_states_sub_;
  // Current (omnihand_msgs/JointStateInt16)
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

  auto node = std::make_shared<JointCmdDemo>(product, side, num_joints);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
