# OmniHand SDK ROS2 接口

> ⚠️ **仅支持 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## 概述

OmniHand SDK 为三款产品提供统一风格的 ROS2 接口：

- **OmniHand 2025 (O10)**: 10 自由度灵巧手
- **OmniHand Pro 2025 (O12)**: 12 自由度灵巧手
- **OmniHand 3 Ultra M (H3U_M / O20)**: 20 自由度灵巧手

三款产品共享统一的 Topic 命名规范和消息类型，区别仅在于自由度数量和连接方式。

## 产品 ROS2 文档

- **[OmniHand 2025 (O10) ROS2 接口](API_ROS2_O10.md)** - 10 DOF
- **[OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md)** - 12 DOF

## 统一 Topic 规范

所有产品遵循相同的 Topic 命名和交互模式：

### Topic 命名

```
/<product>/<side>/<topic_name>
```

- `<product>`: `o10` / `o12` / `h3u_m`
- `<side>`: `left` / `right`
- `<topic_name>`: 见下表

### Topic 列表

| Topic | 消息类型 | 方向 | 触发方式 | 说明 |
|-------|---------|------|---------|------|
| `joint_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | — | 位置指令 `position[]` = rad |
| `joint_states` | `sensor_msgs/JointState` | 发布 (你订阅) | 收到 `joint_cmd` 时 | 位置回读 `position[]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | — | 位置+力矩混合控制（仅 O10/O12，见下文） |
| `joint_error_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发错误码查询 |
| `joint_error_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 收到 `joint_error_cmd` 时 | `data[]` = 错误码 bitmask |
| `joint_temperature_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发温度查询 |
| `joint_temperature_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 收到 `joint_temperature_cmd` 时 | `data[]` = 温度值 |
| `joint_current_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发电流查询 |
| `joint_current_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 收到 `joint_current_cmd` 时 | `data[]` = 电流值 |
| `joint_control_mode_cmd` | `omnihand_msgs/JointStateInt8` | 订阅 (你发布) | — | 写入控制模式 `data[]`（仅 H3U_M） |
| `joint_control_mode_states` | `omnihand_msgs/JointStateInt8` | 发布 (你订阅) | 收到 `joint_control_mode_cmd` 时 | 回读控制模式 `data[]`（仅 H3U_M） |
| `joint_current_threshold_cmd` | `omnihand_msgs/JointStateInt16` | 订阅 (你发布) | — | 写入电流阈值 `data[]` |
| `joint_current_threshold_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 收到 `joint_current_threshold_cmd` 时 | 回读电流阈值 `data[]` |
| `motor_pos_cmd` | `omnihand_msgs/JointStateInt16` | 订阅 (你发布) | — | 写入电机原始位置 `data[]` (int16 tick)（仅 O10/O12） |
| `motor_pos_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 收到 `motor_pos_cmd` 时 | 回读电机原始位置 `data[]` (int16 tick)（仅 O10/O12） |
| `tactile_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发触觉传感器查询（仅 O10/O12） |
| `tactile_states` | 产品专属消息（见下文） | 发布 (你订阅) | 收到 `tactile_cmd` 时 | 触觉传感器数据 |

**设计原则**：节点不自驱发布。所有状态回读都由外部触发（发送 cmd），不会干扰控制回路的工作节奏。

### 控制模式

不同产品型号对控制模式的支持有所不同：

**O10 / H3L**：不支持通过 `joint_control_mode_cmd` topic 切换控制模式，默认位置控制。可通过 `joint_mix_control_cmd` 实现位置+力矩、位置+速度+力矩混合控制（支持 3 种模式）。

**O12**：支持通过 `SetControlMode` 接口设置位置模式、伺服模式、速度模式、力矩模式，也可通过 `joint_mix_control_cmd` 实现位置+力矩混合控制（支持 5 种模式）。注意 O12 的 ROS2 node 当前未暴露 `joint_control_mode_cmd` topic，控制模式切换需通过 C++/Python SDK 直接调用。

**H3U_M**：支持通过 `joint_control_mode_cmd` topic 切换控制模式（0=CSP 位置模式，7=PP 规划位置模式）。

### 混合控制（O10/O12）

`joint_mix_control_cmd` 是独立于 `joint_cmd` 的混合控制 topic，使用 `sensor_msgs/JointState`：

- `position[]` = 电机原始位置 (int16)
- `effort[]` = 电机原始力矩 (int16)

节点内部以 POSITION_TORQUE 模式调用 `MixCtrlJointMotor`，**无回读**。

**注意**：`joint_cmd` 的 `position[]` 单位是弧度 (rad)，会自动转换。`joint_mix_control_cmd` 的 `position[]` 和 `effort[]` 都使用电机原始 int16 值，不做单位转换。

### 触觉传感器（O10/O12）

O10 和 O12 的触觉传感器数据结构不同，各自使用产品专属的消息类型：

| 产品 | 消息类型 | 数据结构 |
|------|---------|---------|
| O10 | `omnihand_2025_node_msgs/TactileSensor` | 7 个区域（THUMB/INDEX/MIDDLE/RING/LITTLE/PALM/DORSUM），每个区域 `uint8[]` 压力值（1g, 最大 255g） |
| O12 | `omnihand_pro_2025_node_msgs/TactileSensor` | 5 个手指，每个包含 `online_state`, `channel_value[6]`, `normal_force`, `tangent_force`, `tangent_force_angle`, `capa_approach[4]` |

### 自定义消息类型

`omnihand_msgs` 包定义了通用的整型关节状态消息，所有产品共享：

| 消息类型 | 字段 |
|---------|------|
| `omnihand_msgs/JointStateInt8` | `std_msgs/Header header` + `string[] name` + `int8[] data` |
| `omnihand_msgs/JointStateInt16` | `std_msgs/Header header` + `string[] name` + `int16[] data` |

## 配置

ROS2 节点通过 YAML 配置文件管理参数，参数名与 Python API 一致。

### 配置参数

参数按 `left_hand` 和 `right_hand` 命名空间组织。如果某只手的 `connection_type` 为空（或在 YAML 中注释掉），则跳过该手。

#### 每只手的参数（`left_hand` / `right_hand`）

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `hand_device_id` | int | 1 | 手设备 ID (1-255) |
| `connection_type` | string | "" | 连接方式: "zlgcan", "hcan", "socketcan", "rs485", "usb"；为空则跳过 |
| `canfd_serial_number` | string | "" | CANFD 适配器序列号（推荐，重启后稳定） |
| `canfd_device_id` | int | 0 | CANFD 适配器设备索引（不推荐，重启后可能变化） |
| `canfd_channel_id` | int | 0 | CAN 通道索引 (0 或 1) |
| `can_interface` | string | "can0" | SocketCAN 接口名 (仅 socketcan) |
| `uart_port` | string | "" | 串口路径 (仅 rs485/usb, O10 专用) |
| `baudrate` | int | 460800 | 波特率 (仅 rs485/usb, O10 专用) |

### 使用方式

**1. 使用 ros2 launch（推荐）：**

```bash
# O10
ros2 launch omnihand_node omnihand_2025_node.launch.py

# O12
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py

# H3U_M
ros2 launch omnihand_node omnihand_3_ultra_m_node.launch.py
```

**2. YAML 配置示例：**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "hcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0
```

**3. Python 脚本示例：**

```bash
# 发送位置指令 + 订阅位置回读
python3 joint_cmd.py left o10

# 触发并查看错误码
python3 joint_error.py left o10

# 触发并查看温度
python3 joint_temperature.py left o10

# 触发并查看电流
python3 joint_current.py left o10

# 混合控制 (位置+力矩, 仅 O10/O12)
python3 mix_control_pub.py left o10

# 发送电机原始位置 (int16 tick) + 订阅回读 (仅 O10/O12)
python3 motor_pos.py left o10

# 触发并查看触觉传感器 (仅 O10/O12)
python3 tactile.py left o10

# 设置电流阈值
python3 joint_current_threshold_pub.py 500 left o10

# 设置控制模式 (仅 H3U_M, 0=CSP, 7=PP)
python3 joint_control_mode_pub.py 0 left h3u_m
```

**4. 连接方式：**

| 连接方式 | O10 | O12 | H3U_M |
|---------|-----|-----|-------|
| zlgcan | ✅ | ✅ | ✅ |
| hcan | ✅ | ✅ | ✅ |
| socketcan | ✅ | ✅ | ✅ |
| rs485 | ✅ | ❌ | ❌ |
| usb | ✅ | ❌ | ❌ |

## C++ 开发示例

Release 包中提供了一个 ROS2 C++ demo，展示如何通过标准 ROS2 topic 控制 OmniHand（不依赖 OmniHand C++ SDK）。

Demo 位于 `ros2/humble/share/omnihand_node/demo/`，包含：
- `ros2_joint_cmd_demo.cpp` — 使用 `sensor_msgs/JointState` 发送关节位置指令 + 使用 `omnihand_msgs` 读取温度和电流
- `CMakeLists.txt` / `package.xml` — 标准 ament_cmake 包配置

### 快速开始

```bash
# 1. source ROS2 和 OmniHand release 环境
source /opt/ros/humble/setup.bash
source /path/to/omnihand_sdk_release/ros2/setup.bash

# 2. 创建 workspace 并拷入 demo
mkdir -p ~/omnihand_ws/src
cp -r /path/to/omnihand_sdk_release/ros2/humble/share/omnihand_node/demo \
      ~/omnihand_ws/src/omnihand_ros2_demo

# 3. 编译并运行
cd ~/omnihand_ws
colcon build --packages-select omnihand_ros2_demo
source install/setup.bash
ros2 run omnihand_ros2_demo ros2_joint_cmd_demo left o10
```

### 自定义开发

如果你想从零写自己的 ROS2 C++ 包来控制 OmniHand，只需在 `package.xml` 中添加依赖：

```xml
<depend>rclcpp</depend>
<depend>sensor_msgs</depend>
<depend>omnihand_msgs</depend>
```

在 `CMakeLists.txt` 中：

```cmake
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(omnihand_msgs REQUIRED)
ament_target_dependencies(your_node rclcpp sensor_msgs omnihand_msgs)
```

> 💡 `source ros2/setup.bash` 会将 `omnihand_msgs` 的 CMake 配置、头文件和库文件加入搜索路径，你的包可以在任意 workspace 位置编译。
>
> 如果只需关节位置控制（不读取温度/电流/错误码），可以不依赖 `omnihand_msgs`，仅用 `sensor_msgs` 即可。
