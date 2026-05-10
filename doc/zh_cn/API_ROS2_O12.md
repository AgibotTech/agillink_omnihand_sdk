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
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | 订阅 (你发布) | 位置+力矩混合控制（见下文） |
| `joint_error_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllErrorReport()` |
| `joint_error_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | `data[]` = 错误码 bitmask (5 bit) |
| `joint_temperature_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllTemperatureReport()` |
| `joint_temperature_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | `data[]` = 温度值 |
| `joint_current_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 `GetAllCurrentReport()` |
| `joint_current_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | `data[]` = 电流值 |
| `joint_current_threshold_cmd` | `omnihand_msgs/JointStateInt16` | 订阅 (你发布) | 写入电流阈值 `data[0..11]` |
| `joint_current_threshold_states` | `omnihand_msgs/JointStateInt16` | 发布 (你订阅) | 回读电流阈值 `data[0..11]` |
| `tactile_cmd` | `std_msgs/Empty` | 订阅 (你发布) | 触发 3D 触觉传感器查询 |
| `tactile_states` | `omnihand_pro_2025_node_msgs/TactileSensor` | 发布 (你订阅) | 3D 触觉传感器数据 |

**注意**: O12 有 12 个自由度。所有数组包含 12 个元素。

**触发式回读**：节点不会自动周期发布状态。温度、电流、错误码等需要你先发送对应的 `*_cmd`（如 `joint_temperature_cmd`），节点才会查询硬件并在 `*_states` 上发布一次回读。`joint_cmd` 例外——发送位置指令后自动回读 `joint_states`。这样设计是为了避免占用 CAN 总线带宽，保证控制指令的实时性。

## 混合控制

`joint_mix_control_cmd` 使用 `sensor_msgs/JointState` 进行位置+力矩混合控制：

- `position[]` = 电机原始位置 (int16)
- `effort[]` = 电机原始力矩 (int16)

节点内部以 POSITION_TORQUE 模式调用 `MixCtrlJointMotor`，**无回读**。

`joint_cmd` 的 `position[]` 单位是弧度，自动转换。`joint_mix_control_cmd` 的值都是电机原始 int16 值。

## 触觉传感器 (3D)

O12 配备 3D 触觉传感器，5 个手指（THUMB, INDEX, MIDDLE, RING, LITTLE）。

消息类型 `omnihand_pro_2025_node_msgs/TactileSensor`：
- `header` (std_msgs/Header)
- `tactile_datas[]` (TactileSensorData[])
  - `online_state` (uint8): 1=在线, 0=离线
  - `channel_value[]` (uint32[]): 6 个通道 24 位值
  - `normal_force` (uint16): 法向力 (0.1N, 最大 3000)
  - `tangent_force` (uint16): 切向力 (0.1N)
  - `tangent_force_angle` (uint16): 切向力角度 (0-359°)
  - `capa_approach[]` (uint8[]): 4 个自电容接近值

`tactile_datas` 数组按 THUMB, INDEX, MIDDLE, RING, LITTLE 顺序排列。

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

# 发送电机原始位置 (int16 tick) + 订阅回读
python3 scripts/omnihand_pro_2025/motor_pos.py left

# 混合控制 (位置+力矩)
python3 scripts/omnihand_pro_2025/mix_control_pub.py left

# 触发并查看触觉传感器
python3 scripts/omnihand_pro_2025/tactile.py left
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

# 混合控制: 位置+力矩 (raw int16)
ros2 topic pub --once /o12/left/joint_mix_control_cmd sensor_msgs/msg/JointState \
  "{position: [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000], effort: [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100]}"
```

## 配置

O12 支持的连接方式：zlgcan, hcan, socketcan（不支持 rs485/usb）。

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

## 与 O10 的区别

1. **自由度**: O12 有 12 个自由度 (O10 有 10 个)
2. **连接方式**: O12 不支持 rs485/usb
3. **Topic 前缀**: `/o12/<side>/` (O10 为 `/o10/<side>/`)

## 相关文档

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)
