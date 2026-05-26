# OmniHand Pro 2025 (O12) ROS2 接口

> ⚠️ **仅支持 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## 概述

O12 ROS2 节点提供 12 自由度灵巧手的统一 Topic 接口，遵循 [ROS2 接口统一规范](API_ROS2.md)。

## ROS2 Topics

所有 Topic 前缀为 `/o12/<side>/`，其中 `<side>` 为 `left` 或 `right`。

| Topic | 消息类型 | 方向 | 说明 |
|-------|---------|------|------|
| `joint_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | `position[0..11]` = rad，触发控制+回读 |
| `joint_states` | `sensor_msgs/JointState` | 发布 (你订阅) | `position[0..11]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | 位置+力混合控制（见下文） |
| `joint_error_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllErrorReport()` |
| `joint_error_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | `data[]` = 错误码 bitmask (5 bit) |
| `joint_temperature_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllTemperatureReport()` |
| `joint_temperature_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | `data[]` = 温度值 |
| `joint_current_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllCurrentReport()` |
| `joint_current_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | `data[]` = 电流值 |
| `joint_current_threshold_cmd` | `std_msgs/Int16MultiArray` | 订阅 (你发布) | 写入电流阈值 `data[0..11]` |
| `joint_current_threshold_states` | `std_msgs/Int16MultiArray` | 发布 (你订阅) | 回读电流阈值 `data[0..11]` |
| `tactile_cmd` | `std_msgs/Float32` | 订阅 (你发布) | 触觉流频率 Hz（>0 启动，0 停止）；上限 **100 Hz**（节点内写死） |
| `tactile_states` | `omnihand_pro_2025_node_msgs/TactileSensor` | 发布 (你订阅) | 流开启期间周期发布 3D 触觉 |

**注意**: O12 有 12 个自由度。所有数组包含 12 个元素。

**触发式回读**：节点不会自动周期发布状态。温度、电流、错误码等需要你先发送对应的 `*_cmd`（如 `joint_temperature_cmd`），节点才会查询硬件并在 `*_states` 上发布一次回读。`joint_cmd` 例外——发送位置指令后自动回读 `joint_states`。这样设计是为了避免占用 CAN 总线带宽，保证控制指令的实时性。

## 混合控制

`joint_mix_control_cmd` 使用 `sensor_msgs/JointState` 进行位置+力混合控制：

- `position[]` = 电机原始位置 (int16, 范围 0–4095)
- `effort[]` = 指尖力 (int16, 单位 **0.01N**，与触觉传感器法向力关联)

节点内部调用 `MixControlByPT`（12 路 position/effort），**无回读**。

## 触觉传感器 (3D)

O12 配备 3D 触觉传感器，5 个手指 + 手心（THUMB, INDEX, MIDDLE, RING, LITTLE, PALM）。

消息类型 `omnihand_pro_2025_node_msgs/TactileSensor`：
- `header` (std_msgs/Header)
- 每指/手心一个 `TactileSensorData` 字段：`thumb`、`index`、`middle`、`ring`、`little`、`palm`。每个子消息含：
  - `online_state` (uint8): 1=在线, 0=离线
  - `channel_value[]` (uint32[]): 6 个通道 24 位值
  - `normal_force` (uint16): 法向力 (0.1N, 最大 3000)
  - `tangent_force` (uint16): 切向力 (0.1N)
  - `tangent_force_angle` (uint16): 切向力角度 (0-359°)
  - `capa_approach[]` (uint8[]): 4 个自电容接近值

## O12 错误码 bitmask

O12 的错误码与 O10 相同，为 5 位 bitmask：

| Bit | 含义 |
|-----|------|
| bit0 | stalled（堵转） |
| bit1 | overheat（过热） |
| bit2 | over_current（过流） |
| bit3 | motor_except（电机异常） |
| bit4 | commu_except（通讯异常） |

## 使用方式

### 启动

```bash
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py
```

### Python 脚本示例

```bash
# 发送位置指令 + 订阅位置回读（12 DOF, rad）
python3 scripts/omnihand_pro_2025/joint_cmd.py left

# 触发并查看错误码 (1Hz)
python3 scripts/omnihand_pro_2025/joint_error.py left 1

# 触发并查看温度
python3 scripts/omnihand_pro_2025/joint_temperature.py left

# 触发并查看电流
python3 scripts/omnihand_pro_2025/joint_current.py left

# 设置电流阈值
python3 scripts/omnihand_pro_2025/joint_current_threshold_pub.py 500 left

# 混合控制 (位置+力, effort=0.01N)
python3 scripts/omnihand_pro_2025/mix_control_pub.py left

# 触发并查看触觉传感器
python3 scripts/omnihand_pro_2025/tactile.py left o12 10
python3 scripts/omnihand_pro_2025/tactile.py left o12 0
```

### 命令行示例

```bash
# 发送位置指令
ros2 topic pub --once /o12/left/joint_cmd sensor_msgs/msg/JointState \
  "{position: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}"

# 查看回读
ros2 topic echo /o12/left/joint_states

# 触发错误码查询
ros2 topic pub --once /o12/left/joint_error_cmd std_msgs/msg/Empty '{}'

# 查看错误码
ros2 topic echo /o12/left/joint_error_states

# 混合控制: 位置+力 (raw int16)
ros2 topic pub --once /o12/left/joint_mix_control_cmd sensor_msgs/msg/JointState \
  "{position: [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000], effort: [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100]}"
```

## 配置

配置文件默认位于 [config/omnihand_pro_2025_node.yaml](../../../node/config/omnihand_pro_2025_node.yaml)，可直接修改或通过启动参数 `config_file` 指定。

O12 支持的连接方式：`zlgcan`、`hcan`、`socketcan`（不支持 rs485/usb）。

### 参数说明

参数按 `left_hand` / `right_hand` 命名空间组织。如果 `connection_type` 为空或未设置，则跳过该手。

| 参数 | 类型 | 默认值 | 适用连接方式 | 说明 |
|------|------|--------|-------------|------|
| `hand_device_id` | int | 1 | 全部 | 手设备 ID（1-255） |
| `connection_type` | string | "" | — | `"zlgcan"` / `"hcan"` / `"socketcan"`；为空则跳过 |
| `canfd_serial_number` | string | "" | zlgcan / hcan | 适配器序列号（**推荐**，重启后稳定） |
| `canfd_device_id` | int | 0 | zlgcan / hcan | 适配器设备索引（`canfd_serial_number` 为空时使用，重启后可能变化） |
| `canfd_channel_id` | int | 0 | zlgcan / hcan | CAN 通道索引（0 或 1，双通道适配器如 200U 选 0/1，单通道始终为 0） |
| `can_interface` | string | "can0" | socketcan | SocketCAN 接口名（如 `can0`、`can1`） |
| `request_interval_ms` | int | -1 | 全部 | 请求最小间隔（ms），0=不限速，范围 0-100，-1=使用 SDK 默认值 |
| `frame_recv_timeout_ms` | int | -1 | 全部 | 单帧接收超时（ms），范围 10-1000，-1=使用 SDK 默认值（50ms） |
| `show_data_details` | bool | false | 全部 | 是否在终端打印收发数据详情（调试用） |

> **适配器选择优先级**：`canfd_serial_number` 优先于 `canfd_device_id`。如果 `canfd_serial_number` 非空，则按序列号查找适配器；否则按 `canfd_device_id` 索引。推荐使用序列号，因为设备索引在重启后可能变化。

### YAML 配置示例

> ⚠️ YAML 对缩进要求严格，必须使用**空格**（不能用 Tab），且同级参数缩进必须一致。

> 以下示例均为双手配置。如只需单手，删除 `right_hand`（或 `left_hand`）整个命名空间即可。

**ZLG CANFD 适配器（按序列号，推荐）：**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0
    right_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 1
```

**ZLG CANFD 适配器（按设备索引）：**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_device_id: 0
      canfd_channel_id: 0
    right_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_device_id: 0
      canfd_channel_id: 1
```

**HCAN 适配器（按序列号）：**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "hcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0
    right_hand:
      hand_device_id: 1
      connection_type: "hcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 1
```

**SocketCAN：**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "socketcan"
      can_interface: "can0"
    right_hand:
      hand_device_id: 1
      connection_type: "socketcan"
      can_interface: "can1"
```

### 启动

```bash
# 使用默认配置文件
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py

# 指定自定义配置文件
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py config_file:=/path/to/your_config.yaml
```

## 与 O10 的区别

1. **自由度**: O12 有 12 个自由度 (O10 有 10 个)
2. **连接方式**: O12 不支持 rs485/usb
3. **Topic 前缀**: `/o12/<side>/` (O10 为 `/o10/<side>/`)

## Demo

| 功能 | Python | C++ |
|---|---|---|
| 位置控制 + 状态回读 | [joint_cmd.py](../../../node/scripts/omnihand_pro_2025/joint_cmd.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 混合控制（位置+力） | [mix_control_pub.py](../../../node/scripts/omnihand_pro_2025/mix_control_pub.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |
| 温度查询 | [joint_temperature.py](../../../node/scripts/omnihand_pro_2025/joint_temperature.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 电流查询 | [joint_current.py](../../../node/scripts/omnihand_pro_2025/joint_current.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 错误码查询 | [joint_error.py](../../../node/scripts/omnihand_pro_2025/joint_error.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 触觉传感器查询 | [tactile.py](../../../node/scripts/omnihand_pro_2025/tactile.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |

> `ros2_joint_cmd_demo.cpp` 是综合示例，包含位置控制、温度、电流、错误码查询和触觉传感器读取功能。

## 相关文档

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)
