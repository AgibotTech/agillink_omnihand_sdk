# OmniHand SDK ROS2 接口

> ⚠️ **仅支持 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## 概述

OmniHand SDK 为四款产品提供统一风格的 ROS2 接口：

- **OmniHand 2025 (O10)**: 10 自由度灵巧手
- **OmniHand Pro 2025 (O12)**: 12 自由度灵巧手
- **OmniHand 3 Lite (H3L)**: 4 自由度轻量级灵巧手
- **OmniHand 3 Ultra M (H3U_M / O20)**: 20 自由度灵巧手

四款产品共享统一的 Topic 命名规范和消息类型，区别仅在于自由度数量和连接方式。

## 产品 ROS2 文档

- **[OmniHand 2025 (O10) ROS2 接口](API_ROS2_O10.md)** - 10 DOF
- **[OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md)** - 12 DOF
- **[OmniHand 3 Lite (H3L) ROS2 接口](API_ROS2_H3L.md)** - 4 DOF

## 统一 Topic 规范

所有产品遵循相同的 Topic 命名和交互模式：

### Topic 命名

```
/<product>/<side>/<topic_name>
```

- `<product>`: `o10` / `o12` / `h3l` / `h3u_m`
- `<side>`: `left` / `right`
- `<topic_name>`: 见下表

### Topic 列表

| Topic | 消息类型 | 方向 | 触发方式 | 说明 |
|-------|---------|------|---------|------|
| `joint_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | — | 位置指令 `position[]` = rad |
| `joint_states` | `sensor_msgs/JointState` | 发布 (你订阅) | 收到 `joint_cmd` 时 | 位置回读 `position[]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | — | 位置+力矩混合控制（仅 O10/O12，见下文） |
| `joint_error_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发错误码查询 |
| `joint_error_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | 收到 `joint_error_cmd` 时 | `data[]` = 错误码 bitmask |
| `joint_temperature_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发温度查询 |
| `joint_temperature_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | 收到 `joint_temperature_cmd` 时 | `data[]` = 温度值 |
| `joint_current_cmd` | `std_msgs/Empty` | 订阅 (你发布) | — | 触发电流查询 |
| `joint_current_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | 收到 `joint_current_cmd` 时 | `data[]` = 电流值 |
| `joint_control_mode_cmd` | `std_msgs/Int8MultiArray` | 订阅 (你发布) | — | 写入控制模式 `data[]`（仅 H3U_M） |
| `joint_control_mode_states` | `std_msgs/Int8MultiArray` | 发布 (你订阅) | 收到 `joint_control_mode_cmd` 时 | 回读控制模式 `data[]`（仅 H3U_M） |
| `joint_current_threshold_cmd` | `std_msgs/Int16MultiArray` | 订阅 (你发布) | — | 写入电流阈值 `data[]` |
| `joint_current_threshold_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | 收到 `joint_current_threshold_cmd` 时 | 回读电流阈值 `data[]` |
| `tactile_cmd` | `std_msgs/Float32` | 订阅 (你发布) | — | 触觉流：`data` = 频率 Hz（>0 启动/改频，0 停止）；上限 **50 Hz**（O10）/ **100 Hz**（O12），节点内写死 |
| `tactile_states` | 产品专属消息（见下文） | 发布 (你订阅) | 触觉流开启期间 | 按设定频率发布触觉数据 |

**设计原则 — 触发式回读**：节点**默认不启用周期定时器**。温度、电流、错误码等需向对应 `*_cmd` 发一次消息，节点读一次硬件并发布一条 `*_states`。**例外 — 触觉（O10/O12）**：向 `tactile_cmd` 发布一次 `std_msgs/Float32`（`data` = Hz）即可在节点内周期读触觉并发布 `tactile_states`，再发 `data: 0` 停止。**例外 — 关节位置**：`joint_cmd` 控制后自动发布 `joint_states`。

这样设计是为了减少不必要的 CAN 占用；触觉频率由你在 `tactile_cmd` 中显式开启，上限为各产品 `hand_node.h` 内写死的 **50 Hz**（O10）或 **100 Hz**（O12）（`kMaxTactilePublishHz`）。

### 控制模式

不同产品型号对控制模式的支持有所不同：

**O10 / H3L**：不支持通过 `joint_control_mode_cmd` topic 切换控制模式，默认位置控制。可通过 `joint_mix_control_cmd` 实现位置+力矩混合控制；位置+速度+力矩模式暂未开放。注意：混合控制中 `effort` 字段实际对应电机电流，单位为 **mA**，范围 **0–1000**（非标准 N·m）。

**O12**：支持通过 `SetControlMode` 接口设置位置模式、伺服模式、速度模式、力矩模式，也可通过 `joint_mix_control_cmd` 实现位置+力混合控制（支持 5 种模式）。混合控制中 `effort` 字段单位为 **0.01N**（与触觉传感器法向力关联）。注意 O12 的 ROS2 node 当前未暴露 `joint_control_mode_cmd` topic，控制模式切换需通过 C++/Python SDK 直接调用。

**H3U_M**：支持通过 `joint_control_mode_cmd` topic 切换控制模式（0=CSP 位置模式，7=PP 规划位置模式）。

### 混合控制（O10/O12）

`joint_mix_control_cmd` 是独立于 `joint_cmd` 的混合控制 topic，使用 `sensor_msgs/JointState`：

- `position[]` = 电机原始位置 (int16)
- `effort[]` = 电机原始力矩 (int16, O10/H3L: 电流 mA 范围 0–1000, O12: 0.01N)

节点内部以 POSITION_TORQUE 模式调用 `MixCtrlJointMotor`，**无回读**。

**注意**：`joint_cmd` 的 `position[]` 单位是弧度 (rad)，会自动转换。`joint_mix_control_cmd` 的 `position[]` 和 `effort[]` 都使用电机原始 int16 值，不做单位转换。

### 触觉传感器（O10/O12）

O10 和 O12 的触觉传感器数据结构不同，各自使用产品专属的消息类型：

| 产品 | 消息类型 | 数据结构 |
|------|---------|---------|
| O10 | `omnihand_2025_node_msgs/TactileSensor` | `header` + 七个 `uint8[]` 区域字段：`thumb`、`index`、`middle`、`ring`、`little`、`palm`、`dorsum`（1g，最大 255g） |
| O12 | `omnihand_pro_2025_node_msgs/TactileSensor` | `header` + 五个 `TactileSensorData` 字段：`thumb`、`index`、`middle`、`ring`、`little`（`online_state`、`channel_value[6]`、力、角度、`capa_approach[4]`） |

### `std_msgs` 数组载荷

错误、温度、电流与电流阈值 topic 使用 `std_msgs/Int16MultiArray` 承载每个关节的 `int16`（`data[]`，`layout.dim` 为一维）。H3U_M 的控制模式使用 `std_msgs/Int8MultiArray`，用法相同。触觉数据仍使用各产品 `omnihand_*_node_msgs` 中的类型。

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

# H3L
ros2 launch omnihand_node omnihand_3_lite_node.launch.py

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

# 触发并查看触觉传感器 (仅 O10/O12)
python3 tactile.py left o10 10
python3 tactile.py left o10 0

# 设置电流阈值
python3 joint_current_threshold_pub.py 500 left o10

# 设置控制模式 (仅 H3U_M, 0=CSP, 7=PP)
python3 joint_control_mode_pub.py 0 left h3u_m
```

**4. 连接方式：**

| 连接方式 | O10 | O12 | H3L | H3U_M |
|---------|-----|-----|-----|-------|
| zlgcan | ✅ | ✅ | ✅ | ✅ |
| hcan | ✅ | ✅ | ✅ | ✅ |
| socketcan | ✅ | ✅ | ✅ | ✅ |
| rs485 | ✅ | ❌ | ❌ | ❌ |
| usb | ✅ | ❌ | ❌ | ❌ |

## C++ 开发示例

Release 包中提供了一个 ROS2 C++ demo，展示如何通过标准 ROS2 topic 控制 OmniHand（不依赖 OmniHand C++ SDK）。

Demo 位于 `ros2/humble/share/omnihand_node/demo/`，包含：
- `ros2_joint_cmd_demo.cpp` — 使用 `sensor_msgs/JointState` 发送关节位置指令 + 使用 `std_msgs/Int16MultiArray` 读取温度和电流
- `CMakeLists.txt` / `package.xml` — 标准 ament_cmake 包配置

### 快速开始

```bash
# 1. source OmniHand release 环境
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
<depend>std_msgs</depend>
```

在 `CMakeLists.txt` 中：

```cmake
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
ament_target_dependencies(your_node rclcpp sensor_msgs std_msgs)
```

> 💡 `source ros2/setup.bash` 会将 OmniHand 消息包与库加入搜索路径，你的包可以在任意 workspace 位置编译。
>
> 如果只需关节位置控制（不读取温度/电流/错误码），可仅依赖 `sensor_msgs` 与 `std_msgs`（用于触发 `Empty` 等）。
