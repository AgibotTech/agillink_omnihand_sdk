# OmniHand Pro 2025 (O12) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The O12 ROS2 node provides a unified topic interface for the 12 DOF dexterous hand, following the [Unified ROS2 Interface Specification](API_ROS2.md).

## ROS2 Topics

All topics are prefixed with `/o12/<side>/`, where `<side>` is `left` or `right`.

| Topic | Message Type | Direction | Description |
|-------|-------------|-----------|-------------|
| `joint_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | `position[0..11]` = rad, triggers control + readback |
| `joint_states` | `sensor_msgs/JointState` | Publish (you sub) | `position[0..11]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | Position+torque mixed control (see below) |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllErrorReport()` |
| `joint_error_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | `data[]` = error bitmask (5 bit) |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllTemperatureReport()` |
| `joint_temperature_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | `data[]` = temperature |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllCurrentReport()` |
| `joint_current_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | `data[]` = current |
| `joint_current_threshold_cmd` | `omnihand_msgs/JointStateInt16` | Subscribe (you pub) | Write current threshold `data[0..11]` |
| `joint_current_threshold_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | Readback current threshold `data[0..11]` |
| `motor_pos_cmd` | `omnihand_msgs/JointStateInt16` | Subscribe (you pub) | Write raw motor position `data[0..11]` (int16 tick) |
| `motor_pos_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | Readback raw motor position `data[0..11]` (int16 tick) |
| `tactile_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Trigger 3D tactile sensor query |
| `tactile_states` | `omnihand_pro_2025_node_msgs/TactileSensor` | Publish (you sub) | 3D tactile sensor data |

**Note**: O12 has 12 degrees of freedom. All arrays contain 12 elements.

## Mixed Control

`joint_mix_control_cmd` uses `sensor_msgs/JointState` for position+torque mixed control:

- `position[]` = raw int16 motor position
- `effort[]` = raw int16 motor torque

The node internally calls `MixCtrlJointMotor` in POSITION_TORQUE mode. **No readback**.

`joint_cmd` uses `position[]` in radians with automatic conversion. `joint_mix_control_cmd` uses raw int16 motor values.

## Tactile Sensor (3D)

O12 is equipped with 3D tactile sensors on 5 fingers (THUMB, INDEX, MIDDLE, RING, LITTLE).

Message type `omnihand_pro_2025_node_msgs/TactileSensor`:
- `header` (std_msgs/Header)
- `tactile_datas[]` (TactileSensorData[])
  - `online_state` (uint8): 1=online, 0=offline
  - `channel_value[]` (uint32[]): 6 channels, 24-bit values
  - `normal_force` (uint16): Normal force (0.1N, max 3000)
  - `tangent_force` (uint16): Tangent force (0.1N)
  - `tangent_force_angle` (uint16): Tangent force angle (0-359°)
  - `capa_approach[]` (uint8[]): 4 self-capacitance approach values

`tactile_datas` array is ordered: THUMB, INDEX, MIDDLE, RING, LITTLE.

## O12 Error Bitmask

O12 error codes are the same as O10, using a 5-bit bitmask:

| Bit | Meaning |
|-----|---------|
| bit0 | stalled |
| bit1 | overheat |
| bit2 | over_current |
| bit3 | motor_except |
| bit4 | commu_except |

## Usage

### Launch

```bash
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py
```

### Python Script Examples

```bash
# Send position command + subscribe to readback (12 DOF, rad)
python3 scripts/omnihand_pro_2025/joint_cmd.py left

# Trigger and view error report (1Hz)
python3 scripts/omnihand_pro_2025/joint_error.py left 1

# Trigger and view temperature
python3 scripts/omnihand_pro_2025/joint_temperature.py left

# Trigger and view current
python3 scripts/omnihand_pro_2025/joint_current.py left

# Set current threshold
python3 scripts/omnihand_pro_2025/joint_current_threshold_pub.py 500 left

# Send raw motor position (int16 tick) + subscribe to readback
python3 scripts/omnihand_pro_2025/motor_pos.py left

# Mixed control (position + torque)
python3 scripts/omnihand_pro_2025/mix_control_pub.py left

# Trigger and view tactile sensor
python3 scripts/omnihand_pro_2025/tactile.py left
```

### CLI Examples

```bash
# Send position command
ros2 topic pub --once /o12/left/joint_cmd sensor_msgs/msg/JointState \
  "{position: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}"

# View readback
ros2 topic echo /o12/left/joint_states

# Trigger error report query
ros2 topic pub --once /o12/left/joint_error_cmd std_msgs/msg/Empty '{}'

# View error reports
ros2 topic echo /o12/left/joint_error_states

# Mixed control: position + torque (raw int16)
ros2 topic pub --once /o12/left/joint_mix_control_cmd sensor_msgs/msg/JointState \
  "{position: [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000], effort: [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100]}"
```

## Configuration

O12 supports: zlgcan, hcan, socketcan (does NOT support rs485/usb).

Example:
```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "hcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0
```

See [Unified ROS2 Interface Specification](API_ROS2.md) for full configuration details.

## Differences from O10

1. **DOF**: O12 has 12 degrees of freedom (vs 10 for O10)
2. **Connection**: O12 does not support rs485/usb
3. **Topic prefix**: `/o12/<side>/` (vs `/o10/<side>/` for O10)

## Related Documentation

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)
