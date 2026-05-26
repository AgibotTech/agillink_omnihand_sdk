# OmniHand 2025 (O10) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The O10 ROS2 node provides a unified topic interface for the 10 DOF dexterous hand, following the [Unified ROS2 Interface Specification](API_ROS2.md).

## ROS2 Topics

All topics are prefixed with `/o10/<side>/`, where `<side>` is `left` or `right`.

| Topic | Message Type | Direction | Description |
|-------|-------------|-----------|-------------|
| `joint_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | `position[0..9]` = rad, triggers control + readback |
| `joint_states` | `sensor_msgs/JointState` | Publish (you sub) | `position[0..9]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | Position+torque mixed control (see below) |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllErrorReport()` |
| `joint_error_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | `data[]` = error bitmask (5 bit) |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllTemperatureReport()` |
| `joint_temperature_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | `data[]` = temperature |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe (you pub) | Triggers `GetAllCurrentReport()` |
| `joint_current_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | `data[]` = current |
| `joint_current_threshold_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | Write current threshold `data[0..9]` |
| `joint_current_threshold_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | Readback current threshold `data[0..9]` |
| `tactile_cmd` | `std_msgs/Float32` | Subscribe (you pub) | Stream rate in Hz (`>0` start, `0` stop); max **50 Hz** (hardcoded in node) |
| `tactile_states` | `omnihand_2025_node_msgs/TactileSensor` | Publish (you sub) | 1D tactile while stream active (Raw full resolution) |

**Note**: O10 has 10 degrees of freedom. All arrays contain 10 elements.

**Trigger-based Readback**: Temperature, current, and error codes use one-shot `*_cmd` → `*_states`. **Tactile** uses `tactile_cmd` (`std_msgs/Float32`, Hz) to start/stop an internal read timer; the node does **not** stream tactile until you publish a positive rate. `joint_cmd` still auto-publishes `joint_states` after control.

## Mixed Control

`joint_mix_control_cmd` uses `sensor_msgs/JointState`; the node picks the mix mode from the arrays you publish:

| Published fields | Mode |
|------------------|------|
| `position[]` + `effort[]` only (no `velocity[]`, or too short) | **POSITION_TORQUE** (position + current) |
| `position[]` + `velocity[]` + `effort[]` (`velocity` length ≥ number of joints commanded) | **POSITION_VELOCITY_TORQUE** (position + velocity + current) |

- `position[]` = raw motor position (int16, 0–4095)
- `velocity[]` = raw motor velocity (int16; required for position+velocity+torque mode)
- `effort[]` = motor current (int16, **mA**, 0–1000, not standard N·m)

> **Note**: `effort` is current in mA, not ROS2 torque (N·m). All joints in one message share the same mode (one CAN mix-control frame).

The node calls `MixControlByPT` / `MixControlByPVT` (arrays must have length 10). **No readback**.

## Tactile Sensor (1D)

O10 is equipped with 1D tactile sensors across 7 regions. The ROS node calls SDK **`GetAllTactileSensorDataRaw()`**; `tactile_states` carries **full-resolution raw** counts (same as Toolbox raw tab / C++ `GetAllTactileSensorDataRaw()`). For downsampled data use SDK `GetTactileSensorData()` / `GetAllTactileSensorData()` — the ROS node does not publish those.

| Region | `tactile_states` `uint8[]` length (raw points) |
|--------|-----------------------------------------------|
| THUMB | 16 |
| INDEX | 18 |
| MIDDLE | 18 |
| RING | 18 |
| LITTLE | 18 |
| PALM | 78 |
| DORSUM | 102 |

> **Connection**: Raw uses one multi-frame read; requires the **public CAN API** (`socketcan` / `zlgcan`, `OmniHand2025CanImpl`). On **private protocol** (`PrivateOmniHand2025`), Raw is not implemented and `tactile_states` may stay empty.

Message type `omnihand_2025_node_msgs/TactileSensor`:
- `header` (std_msgs/Header)
- One `uint8[]` per region (1g, max 255g): `thumb`, `index`, `middle`, `ring`, `little`, `palm`, `dorsum`. Lengths as in the table above; empty if that region was not returned for the readback.

**Stream control** (publish once; node reads periodically):

```bash
# Start 10 Hz tactile stream (immediate first sample, then timer)
ros2 topic pub --once /o10/left/tactile_cmd std_msgs/msg/Float32 "{data: 10.0}"

# Subscribe
ros2 topic echo /o10/left/tactile_states

# Stop
ros2 topic pub --once /o10/left/tactile_cmd std_msgs/msg/Float32 "{data: 0.0}"
```

## O10 Error Bitmask

O10 error codes use a 5-bit bitmask:

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
ros2 launch omnihand_node omnihand_2025_node.launch.py
```

### Python Script Examples

```bash
# Send position command + subscribe to readback (10 DOF, rad)
python3 scripts/omnihand_2025/joint_cmd.py left

# Trigger and view error report (1Hz)
python3 scripts/omnihand_2025/joint_error.py left 1

# Trigger and view temperature
python3 scripts/omnihand_2025/joint_temperature.py left

# Trigger and view current
python3 scripts/omnihand_2025/joint_current.py left

# Set current threshold
python3 scripts/omnihand_2025/joint_current_threshold_pub.py 500 left

# Mixed control (position + force)
python3 scripts/omnihand_2025/mix_control_pub.py left

# Start/stop tactile stream (Hz on tactile_cmd; echo tactile_states separately)
python3 scripts/omnihand_2025/tactile.py left o10 10
python3 scripts/omnihand_2025/tactile.py left o10 0
```

### CLI Examples

```bash
# Send position command
ros2 topic pub --once /o10/left/joint_cmd sensor_msgs/msg/JointState \
  "{position: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}"

# View readback
ros2 topic echo /o10/left/joint_states

# Trigger error report query
ros2 topic pub --once /o10/left/joint_error_cmd std_msgs/msg/Empty '{}'

# View error reports
ros2 topic echo /o10/left/joint_error_states

# Mixed control: position + force (raw int16, effort=current mA)
ros2 topic pub --once /o10/left/joint_mix_control_cmd sensor_msgs/msg/JointState \
  "{position: [2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000], effort: [100, 100, 100, 100, 100, 100, 100, 100, 100, 100]}"
```

## Configuration

The default config file is [config/omnihand_2025_node.yaml](../../../node/config/omnihand_2025_node.yaml). Edit it directly or override via the `config_file` launch argument.

O10 supports: `zlgcan`, `hcan`, `socketcan`, `rs485`, `usb`.

### Parameters

Parameters are organized under `left_hand` / `right_hand` namespaces. If `connection_type` is empty or not set, that hand is skipped.

| Parameter | Type | Default | Applicable | Description |
|-----------|------|---------|------------|-------------|
| `hand_device_id` | int | 1 | All | Hand device ID (1-255) |
| `connection_type` | string | "" | — | `"zlgcan"` / `"hcan"` / `"socketcan"` / `"rs485"` / `"usb"`; empty = skip |
| `canfd_serial_number` | string | "" | zlgcan / hcan | Adapter serial number (**recommended**, stable across reboots) |
| `canfd_device_id` | int | 0 | zlgcan / hcan | Adapter device index (used when `canfd_serial_number` is empty; may change across reboots) |
| `canfd_channel_id` | int | 0 | zlgcan / hcan | CAN channel index (0 or 1; dual-channel adapters like 200U use 0/1) |
| `can_interface` | string | "can0" | socketcan | SocketCAN interface name (e.g. `can0`, `can1`) |
| `uart_port` | string | "" | rs485 / usb | Serial device path (e.g. `/dev/ttyUSB0`) |
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
      canfd_serial_number: "12345678"    # Adapter serial (recommended, stable)
      canfd_channel_id: 0                # Channel 0
    right_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "12345678"    # Same adapter
      canfd_channel_id: 1                # Channel 1 (dual-channel adapters like 200U)
```

**ZLG CANFD adapter (by device index):**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_device_id: 0                 # First adapter (index may change across reboots)
      canfd_channel_id: 0
    right_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_device_id: 0                 # Same adapter
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

**RS485 Serial (O10 only):**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "rs485"
      uart_port: "/dev/ttyUSB0"
    right_hand:
      hand_device_id: 1
      connection_type: "rs485"
      uart_port: "/dev/ttyUSB1"
```

**USB Direct (O10 only):**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "usb"
      uart_port: "/dev/ttyACM0"
    right_hand:
      hand_device_id: 1
      connection_type: "usb"
      uart_port: "/dev/ttyACM1"
```

**Tuning CAN bus timing:**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "socketcan"
      can_interface: "can0"
      request_interval_ms: 5             # 5ms between requests (reduce bus load)
      frame_recv_timeout_ms: 100         # 100ms per-frame timeout (increase if bus is busy)
      show_data_details: true            # Debug: print TX/RX data
    right_hand:
      hand_device_id: 1
      connection_type: "socketcan"
      can_interface: "can1"
      request_interval_ms: 5
      frame_recv_timeout_ms: 100
```

### Launch

```bash
# Default config file
ros2 launch omnihand_node omnihand_2025_node.launch.py

# Custom config file
ros2 launch omnihand_node omnihand_2025_node.launch.py config_file:=/path/to/your_config.yaml
```

## Demo

| Function | Python | C++ |
|---|---|---|
| Position control + state readback | [joint_cmd.py](../../../node/scripts/omnihand_2025/joint_cmd.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Mixed control (position + force) | [mix_control_pub.py](../../../node/scripts/omnihand_2025/mix_control_pub.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |
| Temperature query | [joint_temperature.py](../../../node/scripts/omnihand_2025/joint_temperature.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Current query | [joint_current.py](../../../node/scripts/omnihand_2025/joint_current.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Error report query | [joint_error.py](../../../node/scripts/omnihand_2025/joint_error.py) | [ros2_joint_cmd_demo.cpp](../../../node/demo/ros2_joint_cmd_demo.cpp) |
| Tactile sensor query | [tactile.py](../../../node/scripts/omnihand_2025/tactile.py) | [ros2_mix_ctrl_pos_torque_demo.cpp](../../../node/demo/ros2_mix_ctrl_pos_torque_demo.cpp) |

> `ros2_joint_cmd_demo.cpp` is a comprehensive demo that includes position control, temperature, current, error report queries, and tactile sensor readback.

## Related Documentation

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md)
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md)
