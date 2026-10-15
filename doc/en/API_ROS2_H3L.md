# OmniHand 3 Lite (H3L) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The H3L ROS2 node provides a unified topic interface for the 4 DOF dexterous hand, following the [Unified ROS2 Interface Specification](API_ROS2.md).

> **Important**: H3L has no full kinematics solver for angle-based control. `JointState.position` carries **motor ticks** (int16, range 0-4096), **NOT radians**. The node performs double ↔ int16 transparent conversion internally. Gesture control is available via the SDK's `SetHandGesture()` API.

## ROS2 Topics

All topics are prefixed with `/h3l/<side>/`, where `<side>` is `left` or `right`.

| Topic | Message Type | Direction | Description |
|-------|-------------|-----------|-------------|
| `joint_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | `position[0..3]` = motor ticks (0-4095), triggers control + readback |
| `joint_states` | `sensor_msgs/JointState` | Publish (you sub) | `position[0..3]` = motor ticks (0-4095) |
| `joint_cmd_source` | `std_msgs/Int8` | Subscribe (you pub) | `0=NORMAL`, `1=ANGLE`, `2=POSITION` |
| `joint_angle_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | Source `1`; H3L still interprets values through its available position path |
| `joint_position_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | Source `2`; raw motor positions |
| `joint_position_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish (you sub) | Timestamped raw position readback |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | Position+torque mixed control (see below) |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllErrorReport()` |
| `joint_error_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish (you sub) | `header.stamp` + error bitmask `data[]` |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllTemperatureReport()` |
| `joint_temperature_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish (you sub) | `header.stamp` + temperature `data[]` (°C) |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllCurrentReport()` |
| `joint_current_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish (you sub) | `header.stamp` + current `data[]` |
| `joint_current_threshold_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | Write current threshold `data[0..3]` |
| `joint_current_threshold_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish (you sub) | Timestamped current-threshold readback `data[0..3]` |
| `tactile_cmd` | `std_msgs/Float32` | Subscribe (you pub) | `data` is Hz; >0 starts/changes rate, 0 stops, max 50 Hz |
| `tactile_states` | `omnihand_3_lite_node_msgs/TactileSensor` | Publish (you sub) | `header` plus six raw `uint16[]` tactile regions |

**Note**: H3L has 4 degrees of freedom, so joint arrays contain 4 elements. The tactile message contains six regions: `thumb`, `index`, `middle`, `ring`, `little`, and `palm`, populated by `GetAllTactileSensorDataRaw()`.

**Trigger-based Readback**: Temperature, current, and error reports require a corresponding `*_cmd` and produce one readback. `joint_cmd` automatically produces `joint_states`. Tactile is another exception: publish a rate once on `tactile_cmd` and the node periodically publishes `tactile_states` until it receives `data: 0`.

## Mixed Control

`joint_mix_control_cmd` uses `sensor_msgs/JointState` for position+torque mixed control:

- `position[]` = raw motor position (int16 ticks, range 0-4095)
- `effort[]` = motor current (int16, unit: mA)

> **Note**: The `effort` field carries current values in mA, not ROS2 standard torque (N·m). H3L uses `MixControlByPT` (4 position + effort values). **No readback**.

## H3L Error Bitmask

H3L error codes use a 5-bit bitmask:

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
ros2 launch omnihand_node omnihand_3_lite_node.launch.py
```

### Python Script Examples

```bash
# Send position command + subscribe to readback (4 DOF, motor ticks)
python3 scripts/omnihand_3_lite/joint_cmd.py left

# Mixed control (position + current)
python3 scripts/omnihand_3_lite/mix_control_pt_pub.py left

# Trigger and view error report
python3 scripts/omnihand_3_lite/joint_error.py left

# Trigger and view temperature
python3 scripts/omnihand_3_lite/joint_temperature.py left

# Trigger and view current
python3 scripts/omnihand_3_lite/joint_current.py left

# Set current threshold
python3 scripts/omnihand_3_lite/joint_current_threshold_pub.py 500 left

# Start tactile streaming at 10 Hz; publish 0 to stop
python3 scripts/omnihand_3_lite/tactile.py left 10
python3 scripts/omnihand_3_lite/tactile.py left 0
```

### CLI Examples

```bash
# Send position command (4 motor ticks, range 0-4095)
ros2 topic pub --once /h3l/left/joint_cmd sensor_msgs/msg/JointState   "{position: [2048, 2048, 2048, 2048]}"

# View readback
ros2 topic echo /h3l/left/joint_states

# Trigger error report query
ros2 topic pub --once /h3l/left/joint_error_cmd std_msgs/msg/Empty '{}'

# View error reports
ros2 topic echo /h3l/left/joint_error_states

# Mixed control: position + force (raw int16, effort=current mA)
ros2 topic pub --once /h3l/left/joint_mix_control_cmd sensor_msgs/msg/JointState   "{position: [2048, 2048, 2048, 2048], effort: [100, 100, 100, 100]}"

# Start tactile streaming and inspect six-region uint16 raw data
ros2 topic pub --once /h3l/left/tactile_cmd std_msgs/msg/Float32 '{data: 10.0}'
ros2 topic echo /h3l/left/tactile_states

# Stop tactile streaming
ros2 topic pub --once /h3l/left/tactile_cmd std_msgs/msg/Float32 '{data: 0.0}'
```

## Configuration

The default config file is [config/omnihand_3_lite_node.yaml](../../../node/omnihand_node/config/omnihand_3_lite_node.yaml). Edit it directly or override via the `config_file` launch argument.

H3L supports: `zlgcan`, `hcan`, `socketcan`. (RS485 and USB direct are not supported.)

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
ros2 launch omnihand_node omnihand_3_lite_node.launch.py

# Custom config file
ros2 launch omnihand_node omnihand_3_lite_node.launch.py config_file:=/path/to/your_config.yaml
```

## Demo

| Function | Python | C++ |
|---|---|---|
| Position control + state readback | [joint_cmd.py](../../../node/omnihand_node/scripts/omnihand_3_lite/joint_cmd.py) | [ros2_joint_cmd_demo.cpp](../../../node/omnihand_node/demo/ros2_joint_cmd_demo.cpp) |
| Mixed control (position + current) | [mix_control_pt_pub.py](../../../node/omnihand_node/scripts/omnihand_3_lite/mix_control_pt_pub.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/omnihand_node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |
| Tactile stream control | [tactile.py](../../../node/omnihand_node/scripts/omnihand_3_lite/tactile.py) | — |
| Temperature query | [joint_temperature.py](../../../node/omnihand_node/scripts/omnihand_3_lite/joint_temperature.py) | [ros2_joint_cmd_demo.cpp](../../../node/omnihand_node/demo/ros2_joint_cmd_demo.cpp) |
| Current query | [joint_current.py](../../../node/omnihand_node/scripts/omnihand_3_lite/joint_current.py) | [ros2_joint_cmd_demo.cpp](../../../node/omnihand_node/demo/ros2_joint_cmd_demo.cpp) |
| Error report query | [joint_error.py](../../../node/omnihand_node/scripts/omnihand_3_lite/joint_error.py) | [ros2_joint_cmd_demo.cpp](../../../node/omnihand_node/demo/ros2_joint_cmd_demo.cpp) |

> `ros2_joint_cmd_demo.cpp` and `ros2_mix_ctrl_pos_torque_demo.cpp` are shared comprehensive demos that support multiple products.

## Key Differences from O10/O12

| Feature | H3L | O10 / O12 |
|---------|----|-----------|
| DOF | 4 | 10 / 12 |
| Angle control | ❌ Unavailable (no kinematics solver) | ✅ rad |
| Motor position control | ✅ Recommended (0-4095 ticks) | ✅ Optional |
| Tactile sensor | ✅ Six-region raw `uint16[]` data | ✅ Product-specific formats |
| RS485 / USB direct | ❌ Not supported | ✅ O10 supported |
| `joint_cmd` unit | motor ticks (int16) | rad (float64) |
| namespace | `/h3l` | `/o10` / `/o12` |

## Related Documentation

- [OmniHand 3 Lite (H3L) C++ API](API_CPP_H3L.md)
- [OmniHand 3 Lite (H3L) Python API](API_PYTHON_H3L.md)
- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md)
- [OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md)
- [OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md)
