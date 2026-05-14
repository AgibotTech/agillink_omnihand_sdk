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
 *   - std_msgs/Empty                : 触觉 / 温度 / 电流 查询触发
 *   - std_msgs/Int16MultiArray       : 温度与电流回读 (data[] = int16, °C / mA)
 *   - omnihand_2025_node_msgs/TactileSensor     : O10 触觉传感器数据
 *   - omnihand_pro_2025_node_msgs/TactileSensor : O12 触觉传感器数据 (3D)
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
 *    这一步将 omnihand_*_node_msgs 等包加入 CMAKE_PREFIX_PATH。
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
 *   pub: /<product>/<side>/joint_temperature_cmd  (std_msgs/Empty, trigger)
 *   pub: /<product>/<side>/joint_current_cmd      (std_msgs/Empty, trigger)
 *   pub: /<product>/<side>/tactile_cmd            (std_msgs/Empty, trigger)
 *   sub: /<product>/<side>/joint_states           (sensor_msgs/JointState)
 *   sub: /<product>/<side>/joint_temperature_states (std_msgs/Int16MultiArray)
 *   sub: /<product>/<side>/joint_current_states     (std_msgs/Int16MultiArray)
 *   sub: /<product>/<side>/tactile_states           (omnihand_2025_node_msgs/TactileSensor or omnihand_pro_2025_node_msgs/TactileSensor)
 *
 * 触发式回读 (Trigger-based Readback):
 *   OmniHand ROS2 节点不会自动周期发布状态。温度、电流等 *_states
 *   只在你发送对应的 *_cmd 后才会查询硬件并发布一次。
 *   例如：发一条 Empty 到 joint_temperature_cmd → 收到一条 joint_temperature_states。
 *   joint_cmd 例外：发送位置指令后，节点自动回读位置到 joint_states。
 *   设计目的：避免占用 CAN 总线带宽，保证控制指令实时性。
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
 *   <depend>std_msgs</depend>
 *   <depend>omnihand_2025_node_msgs</depend>    <!-- O10 触觉消息 (可选) -->
 *   <depend>omnihand_pro_2025_node_msgs</depend><!-- O12 触觉消息 (可选) -->
 *
 * CMakeLists.txt:
 *   find_package(rclcpp REQUIRED)
 *   find_package(sensor_msgs REQUIRED)
 *   find_package(std_msgs REQUIRED)
 *   find_package(omnihand_2025_node_msgs REQUIRED)     # O10 触觉消息
 *   find_package(omnihand_pro_2025_node_msgs REQUIRED)  # O12 触觉消息
 *   ament_target_dependencies(your_node rclcpp sensor_msgs std_msgs
 *       omnihand_2025_node_msgs omnihand_pro_2025_node_msgs)
 *
 * 如果只需关节位置控制，可以不依赖触觉消息包，仅用 sensor_msgs 与 std_msgs 即可。
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

using namespace std::chrono_literals;
using sensor_msgs::msg::JointState;
using std_msgs::msg::Empty;
using std_msgs::msg::Int16MultiArray;
using O10Tactile = omnihand_2025_node_msgs::msg::TactileSensor;
using O12Tactile = omnihand_pro_2025_node_msgs::msg::TactileSensor;

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

    // --- temperature: pub Empty trigger + sub states (std_msgs/Int16MultiArray) ---
    temp_cmd_pub_ = this->create_publisher<Empty>(
        prefix + "/joint_temperature_cmd", 10);
    temp_states_sub_ = this->create_subscription<Int16MultiArray>(
        prefix + "/joint_temperature_states", 10,
        std::bind(&JointCmdDemo::OnTemperature, this, std::placeholders::_1));

    // --- current: pub Empty trigger + sub states (std_msgs/Int16MultiArray) ---
    current_cmd_pub_ = this->create_publisher<Empty>(
        prefix + "/joint_current_cmd", 10);
    current_states_sub_ = this->create_subscription<Int16MultiArray>(
        prefix + "/joint_current_states", 10,
        std::bind(&JointCmdDemo::OnCurrent, this, std::placeholders::_1));

    // --- tactile: pub trigger (std_msgs/Empty) + sub states ---
    tactile_cmd_pub_ = this->create_publisher<Empty>(
        prefix + "/tactile_cmd", 10);
    if (product == "o10") {
      o10_tactile_sub_ = this->create_subscription<O10Tactile>(
          prefix + "/tactile_states", 10,
          std::bind(&JointCmdDemo::OnO10Tactile, this, std::placeholders::_1));
    } else if (product == "o12") {
      o12_tactile_sub_ = this->create_subscription<O12Tactile>(
          prefix + "/tactile_states", 10,
          std::bind(&JointCmdDemo::OnO12Tactile, this, std::placeholders::_1));
    }

    timer_ = this->create_wall_timer(1500ms,
                                     std::bind(&JointCmdDemo::OnTimer, this));

    RCLCPP_INFO(this->get_logger(),
                "Demo started: %s/%s (%d DOF)",
                product.c_str(), side.c_str(), num_joints);
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_cmd (sensor_msgs/JointState)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_temperature_cmd (std_msgs/Empty, trigger)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/joint_current_cmd (std_msgs/Empty, trigger)",
                prefix.c_str());
    RCLCPP_INFO(this->get_logger(), "  pub -> %s/tactile_cmd (std_msgs/Empty, trigger)",
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

    // 2. Trigger temperature readback (Empty, matches omnihand_*_node)
    temp_cmd_pub_->publish(Empty());

    // 3. Trigger current readback (Empty, matches omnihand_*_node)
    current_cmd_pub_->publish(Empty());

    // 4. Trigger tactile sensor readback
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

  void OnO10Tactile(const O10Tactile::SharedPtr msg) {
    std::ostringstream oss;
    oss << "tactile (O10, 1D): [";
    for (size_t f = 0; f < msg->tactile_datas.size(); ++f) {
      if (f > 0) oss << " | ";
      for (size_t i = 0; i < msg->tactile_datas[f].tactiles.size(); ++i) {
        if (i > 0) oss << ",";
        oss << static_cast<int>(msg->tactile_datas[f].tactiles[i]);
      }
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  void OnO12Tactile(const O12Tactile::SharedPtr msg) {
    std::ostringstream oss;
    oss << "tactile (O12, 3D): [";
    for (size_t f = 0; f < msg->tactile_datas.size(); ++f) {
      if (f > 0) oss << " | ";
      oss << "Fn=" << msg->tactile_datas[f].normal_force * 0.1 << "N"
          << ",Ft=" << msg->tactile_datas[f].tangent_force;
    }
    oss << "]";
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }

  // Joint position
  rclcpp::Publisher<JointState>::SharedPtr joint_cmd_pub_;
  rclcpp::Subscription<JointState>::SharedPtr joint_states_sub_;
  rclcpp::Publisher<Empty>::SharedPtr temp_cmd_pub_;
  rclcpp::Publisher<Empty>::SharedPtr current_cmd_pub_;
  // Temperature / current states: std_msgs/Int16MultiArray
  rclcpp::Subscription<Int16MultiArray>::SharedPtr temp_states_sub_;
  rclcpp::Subscription<Int16MultiArray>::SharedPtr current_states_sub_;
  // Tactile sensor
  rclcpp::Publisher<Empty>::SharedPtr tactile_cmd_pub_;
  rclcpp::Subscription<O10Tactile>::SharedPtr o10_tactile_sub_;
  rclcpp::Subscription<O12Tactile>::SharedPtr o12_tactile_sub_;

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
