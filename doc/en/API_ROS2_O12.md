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
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | Position+force mixed control (see below) |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllErrorReport()` |
| `joint_error_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | `data[]` = error bitmask (5 bit) |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllTemperatureReport()` |
| `joint_temperature_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | `data[]` = temperature |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllCurrentReport()` |
| `joint_current_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | `data[]` = current |
| `joint_control_mode_cmd` | `std_msgs/Int8MultiArray` | Subscribe (you pub) | Write control mode `data[0..11]`; use `4` for voltage mode |
| `joint_control_mode_states` | `std_msgs/Int8MultiArray` | Publish (you sub) | Readback control mode `data[0..11]` |
| `joint_voltage_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | Write voltage command `data[0..11]` |
| `joint_voltage_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | Readback voltage command `data[0..11]` |
| `joint_current_threshold_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | Write current threshold `data[0..11]` |
| `joint_current_threshold_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | Readback current threshold `data[0..11]` |
| `tactile_cmd` | `std_msgs/Float32` | Subscribe (you pub) | Stream rate in Hz (`>0` start, `0` stop); max **100 Hz** (hardcoded in node) |
| `tactile_states` | `omnihand_pro_2025_node_msgs/TactileSensor` | Publish (you sub) | 3D tactile while stream active |

**Note**: O12 has 12 degrees of freedom. All arrays contain 12 elements.

**Trigger-based Readback**: The node never publishes state on a periodic timer. To read temperature, current, error codes, etc., you must first publish the corresponding `*_cmd` message (e.g. `joint_temperature_cmd`); the node will then query the hardware and publish one reading on `*_states`. The exception is `joint_cmd` — after sending a position command, the node automatically publishes position readback on `joint_states`. This avoids consuming CAN bus bandwidth and ensures real-time responsiveness of control commands.

## Mixed Control

`joint_mix_control_cmd` uses `sensor_msgs/JointState` for position+force mixed control:

- `position[]` = raw motor position (int16, range 0–2000)
- `effort[]` = fingertip force (int16, unit: **0.01 N**, correlated with tactile sensor normal force)

The node calls `MixControlByPT` (12 position + effort values). **No readback**.

## Voltage Control

Voltage control uses two topic pairs:

- `joint_control_mode_cmd/states` (`std_msgs/Int8MultiArray`) for switching all 12 joints to `ControlMode.VOLTAGE` (`4`).
- `joint_voltage_cmd/states` (`std_msgs/Int16MultiArray`) for writing and reading back voltage commands.

Publish a full 12-element array to each command topic. The node does not switch modes implicitly; send control mode `4` before publishing non-zero voltage commands.

## Tactile Sensor (3D)

O12 is equipped with 3D tactile sensors on 5 fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM).

Message type `omnihand_pro_2025_node_msgs/TactileSensor`:
- `header` (std_msgs/Header)
- One `TactileSensorData` per finger/palm: `thumb`, `index`, `middle`, `ring`, `little`, `palm`. Each contains:
  - `online_state` (uint8): 1=online, 0=offline
  - `channel_value[]` (uint32[]): 6 channels, 24-bit values
  - `normal_force` (uint16): Normal force (0.1N, max 3000)
  - `tangent_force` (uint16): Tangent force (0.1N)
  - `tangent_force_angle` (uint16): Tangent force angle (0-359°)
  - `capa_approach[]` (uint8[]): 4 self-capacitance approach values

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

# Set all joints to voltage mode, then publish voltage commands
python3 scripts/omnihand_pro_2025/joint_control_mode_pub.py 4 left
python3 scripts/omnihand_pro_2025/joint_voltage_pub.py 0 left

# Mixed control (position + force, effort=0.01N)
python3 scripts/omnihand_pro_2025/mix_control_pub.py left

# Start/stop tactile stream
python3 scripts/omnihand_pro_2025/tactile.py left o12 10
python3 scripts/omnihand_pro_2025/tactile.py left o12 0
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

# Set all joints to voltage mode
ros2 topic pub --once /o12/left/joint_control_mode_cmd std_msgs/msg/Int8MultiArray \
  "{data: [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4]}"

# Send zero voltage command
ros2 topic pub --once /o12/left/joint_voltage_cmd std_msgs/msg/Int16MultiArray \
  "{data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}"

# View voltage readback
ros2 topic echo /o12/left/joint_voltage_states

# Mixed control: position + force (raw int16)
ros2 topic pub --once /o12/left/joint_mix_control_cmd sensor_msgs/msg/JointState \
  "{position: [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000], effort: [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100]}"
```

## Configuration

The default config file is [config/omnihand_pro_2025_node.yaml](../../../node/config/omnihand_pro_2025_node.yaml). Edit it directly or override via the `config_file` launch argument.

O12 supports: `zlgcan`, `hcan`, `socketcan` (does **NOT** support rs485/usb).

### Parameters

Parameters are organized under `left_hand` / `right_hand` namespaces. If `connection_type` is empty or not set, that hand is skipped.

| Parameter | Type | Default | Applicable | Description |
|-----------|------|---------|------------|-------------|
| `hand_device_id` | int | 1 | All | Hand device ID (1-255) |
| `connection_type` | string | "" | — | `"zlgcan"` / `"hcan"` / `"socketcan"`; empty = skip |
| `canfd_serial_number` | string | "" | zlgcan / hcan | Adapter serial number (**recommended**, stable across reboots) |
| `canfd_device_id` | int | 0 | zlgcan / hcan | Adapter device index (used when `canfd_serial_number` is empty; may change across reboots) |
| `canfd_channel_id` | int | 0 | zlgcan / hcan | CAN channel index (0 or 1; dual-channel adapters like 200U use 0/1) |
| `can_interface` | string | "can0" | socketcan | SocketCAN interface name (e.g. `can0`, `can1`) |
| `request_interval_ms` | int | -1 | All | Minimum request interval (ms), 0=no limit, range 0-100, -1=SDK default |
| `frame_recv_timeout_ms` | int | -1 | All | Per-frame receive timeout (ms), range 10-1000, -1=SDK default (50ms) |
| `show_data_details` | bool | false | All | Print TX/RX data to terminal (for debugging) |

> **Adapter selection priority**: `canfd_serial_number` takes priority over `canfd_device_id`. If `canfd_serial_number` is non-empty, the adapter is found by serial number; otherwise by device index. Serial number is recommended as device indices may change across reboots.

### YAML Examples

> ⚠️ YAML is strict about indentation — use **spaces only** (no tabs), and keep indentation consistent for parameters at the same level.

> All examples below show dual-hand configuration. For single-hand use, simply remove the `right_hand` (or `left_hand`) namespace.

**ZLG CANFD adapter (by serial number, recommended):**

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

**ZLG CANFD adapter (by device index):**

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

**HCAN adapter (by serial number):**

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

**SocketCAN:**

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

### Launch

```bash
# Default config file
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py

# Custom config file
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py config_file:=/path/to/your_config.yaml
```

## Differences from O10

1. **DOF**: O12 has 12 degrees of freedom (vs 10 for O10)
2. **Connection**: O12 does not support rs485/usb
3. **Topic prefix**: `/o12/<side>/` (vs `/o10/<side>/` for O10)

## Demo

| Function | Python | C++ |
|---|---|---|
| Position control + state readback | [joint_cmd.py](../../../node/scripts/omnihand_pro_2025/joint_cmd.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Mixed control (position + force) | [mix_control_pub.py](../../../node/scripts/omnihand_pro_2025/mix_control_pub.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |
| Temperature query | [joint_temperature.py](../../../node/scripts/omnihand_pro_2025/joint_temperature.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Current query | [joint_current.py](../../../node/scripts/omnihand_pro_2025/joint_current.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Error report query | [joint_error.py](../../../node/scripts/omnihand_pro_2025/joint_error.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Tactile sensor query | [tactile.py](../../../node/scripts/omnihand_pro_2025/tactile.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |

> `ros2_joint_cmd_demo.cpp` is a comprehensive demo that includes position control, temperature, current, error report queries, and tactile sensor readback.

## Related Documentation

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)
