# OmniHand 2025 (O10) ROS2 接口

> ⚠️ **仅支持 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## 概述

O10 ROS2 节点提供 10 自由度灵巧手的统一 Topic 接口，遵循 [ROS2 接口统一规范](API_ROS2.md)。

## ROS2 Topics

所有 Topic 前缀为 `/o10/<side>/`，其中 `<side>` 为 `left` 或 `right`。

| Topic | 消息类型 | 方向 | 说明 |
|-------|---------|------|------|
| `joint_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | `position[0..9]` = rad，触发控制+回读 |
| `joint_states` | `sensor_msgs/JointState` | 发布 (你订阅) | `position[0..9]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | 位置+力矩混合控制（见下文） |
| `joint_error_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllErrorReport()` |
| `joint_error_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | `data[]` = 错误码 bitmask (5 bit) |
| `joint_temperature_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllTemperatureReport()` |
| `joint_temperature_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | `data[]` = 温度值 |
| `joint_current_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllCurrentReport()` |
| `joint_current_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | `data[]` = 电流值 |
| `joint_current_threshold_cmd` | `omnihand_msgs/JointStateInt16` | 订阅 (你发布) | 写入电流阈值 `data[0..9]` |
| `joint_current_threshold_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 回读电流阈值 `data[0..9]` |
| `tactile_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllTactileSensorData()` |
| `tactile_states` | `omnihand_2025_node_msgs/TactileSensor` | 发布 (你订阅) | 1D 触觉传感器数据 |

**注意**: O10 有 10 个自由度。所有数组包含 10 个元素。

**触发式回读**：节点不会自动周期发布状态。温度、电流、错误码等需要你先发送对应的 `*_cmd`（如 `joint_temperature_cmd`），节点才会查询硬件并在 `*_states` 上发布一次回读。`joint_cmd` 例外——发送位置指令后自动回读 `joint_states`。这样设计是为了避免占用 CAN 总线带宽，保证控制指令的实时性。

## 混合控制

`joint_mix_control_cmd` 使用 `sensor_msgs/JointState` 进行位置+力矩混合控制：

- `position[]` = 电机原始位置 (int16, 范围 0–4095)
- `effort[]` = 电机电流 (int16, 单位 **mA**，范围 **0–1000**，非标准 N·m)

> **注意**：`effort` 字段传递的是电流值（mA，0–1000），而非 ROS2 标准的力矩（N·m）。位置+速度+力矩模式（POSITION_VELOCITY_TORQUE）暂未开放。

节点内部以 POSITION_TORQUE 模式调用 `MixCtrlJointMotor`，**无回读**。

## 触觉传感器 (1D)

O10 配备 1D 触觉传感器，7 个区域：

| 区域 | 传感器点数 |
|------|----------|
| THUMB | 16 |
| INDEX | 18 |
| MIDDLE | 18 |
| RING | 18 |
| LITTLE | 18 |
| PALM | 78 |
| DORSUM | 102 |

消息类型 `omnihand_2025_node_msgs/TactileSensor`：
- `header` (std_msgs/Header)
- `tactile_datas[]` (TactileSensorData[])
  - 每个 `TactileSensorData`: `uint8[] tactiles`（单位: 1g, 最大 255g）

`tactile_datas` 数组按上述区域顺序排列。

## O10 错误码 bitmask

O10 的错误码为 5 位 bitmask：

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
ros2 launch omnihand_node omnihand_2025_node.launch.py
```

### Python 脚本示例

```bash
# 发送位置指令 + 订阅位置回读（10 DOF, rad）
python3 scripts/omnihand_2025/joint_cmd.py left

# 触发并查看错误码 (1Hz)
python3 scripts/omnihand_2025/joint_error.py left 1

# 触发并查看温度
python3 scripts/omnihand_2025/joint_temperature.py left

# 触发并查看电流
python3 scripts/omnihand_2025/joint_current.py left

# 设置电流阈值
python3 scripts/omnihand_2025/joint_current_threshold_pub.py 500 left

# 混合控制 (位置+力)
python3 scripts/omnihand_2025/mix_control_pub.py left

# 触发并查看触觉传感器
python3 scripts/omnihand_2025/tactile.py left
```

### 命令行示例

```bash
# 发送位置指令
ros2 topic pub --once /o10/left/joint_cmd sensor_msgs/msg/JointState \
  "{position: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}"

# 查看回读
ros2 topic echo /o10/left/joint_states

# 触发错误码查询
ros2 topic pub --once /o10/left/joint_error_cmd std_msgs/msg/Empty '{}'

# 查看错误码
ros2 topic echo /o10/left/joint_error_states

# 混合控制: 位置+力 (raw int16, effort=电流mA)
ros2 topic pub --once /o10/left/joint_mix_control_cmd sensor_msgs/msg/JointState \
  "{position: [2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000], effort: [100, 100, 100, 100, 100, 100, 100, 100, 100, 100]}"
```

## 配置

O10 支持的连接方式：zlgcan, hcan, socketcan, rs485, usb。

配置示例：
```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "hcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0
```

详见 [ROS2 接口统一规范](API_ROS2.md) 中的配置说明。

## Demo

| 功能 | Python | C++ |
|---|---|---|
| 位置控制 + 状态回读 | [joint_cmd.py](../../../node/scripts/omnihand_2025/joint_cmd.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 混合控制（位置+力） | [mix_control_pub.py](../../../node/scripts/omnihand_2025/mix_control_pub.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |
| 温度查询 | [joint_temperature.py](../../../node/scripts/omnihand_2025/joint_temperature.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 电流查询 | [joint_current.py](../../../node/scripts/omnihand_2025/joint_current.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 错误码查询 | [joint_error.py](../../../node/scripts/omnihand_2025/joint_error.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| 触觉传感器查询 | [tactile.py](../../../node/scripts/omnihand_2025/tactile.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |

> `ros2_joint_cmd_demo.cpp` 是综合示例，包含位置控制、温度、电流、错误码查询和触觉传感器读取功能。

## 相关文档

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md)
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md)
