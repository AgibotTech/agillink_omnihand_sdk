# OmniHand SDK ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The OmniHand SDK provides unified ROS2 interfaces for three product models:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand
- **OmniHand 3 Ultra M (H3U_M / O20)**: 20 DOF dexterous hand

All products share the same topic naming convention and message types, differing only in DOF count and connection options.

## Product-Specific ROS2 Documentation

- **[OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md)** - 10 DOF
- **[OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md)** - 12 DOF

## Unified Topic Specification

All products follow the same topic naming and interaction pattern:

### Topic Naming

```
/<product>/<side>/<topic_name>
```

- `<product>`: `o10` / `o12` / `h3u_m`
- `<side>`: `left` / `right`
- `<topic_name>`: see table below

### Topic List

| Topic | Message Type | Direction | Trigger | Description |
|-------|-------------|-----------|---------|-------------|
| `joint_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | — | Position command `position[]` = rad |
| `joint_states` | `sensor_msgs/JointState` | Publish (you sub) | On `joint_cmd` received | Position readback `position[]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | — | Position+torque mixed control (O10/O12 only, see below) |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger error report query |
| `joint_error_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | On `joint_error_cmd` received | `data[]` = error bitmask |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger temperature query |
| `joint_temperature_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | On `joint_temperature_cmd` received | `data[]` = temperature |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger current query |
| `joint_current_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | On `joint_current_cmd` received | `data[]` = current |
| `joint_control_mode_cmd` | `omnihand_msgs/JointStateInt8` | Subscribe (you pub) | — | Write control mode `data[]` (H3U_M only) |
| `joint_control_mode_states` | `omnihand_msgs/JointStateInt8` | Publish (you sub) | On `joint_control_mode_cmd` received | Readback control mode `data[]` (H3U_M only) |
| `joint_current_threshold_cmd` | `omnihand_msgs/JointStateInt16` | Subscribe (you pub) | — | Write current threshold `data[]` |
| `joint_current_threshold_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | On `joint_current_threshold_cmd` received | Readback current threshold `data[]` |
| `motor_pos_cmd` | `omnihand_msgs/JointStateInt16` | Subscribe (you pub) | — | Write raw motor position `data[]` (int16 tick) (O10/O12 only) |
| `motor_pos_states` | `omnihand_msgs/JointStateInt16` | Publish (you sub) | On `motor_pos_cmd` received | Readback raw motor position `data[]` (int16 tick) (O10/O12 only) |
| `tactile_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger tactile sensor query (O10/O12 only) |
| `tactile_states` | Product-specific msg (see below) | Publish (you sub) | On `tactile_cmd` received | Tactile sensor data |

**Design principle**: The node never publishes autonomously. All state readbacks are triggered externally (by sending a cmd), so they never interfere with the control loop's rhythm.

### Mixed Control (O10/O12)

`joint_mix_control_cmd` is a separate topic from `joint_cmd` for mixed control, using `sensor_msgs/JointState`:

- `position[]` = raw int16 motor position
- `effort[]` = raw int16 motor torque

The node internally calls `MixCtrlJointMotor` in POSITION_TORQUE mode. **No readback**.

**Note**: `joint_cmd` uses `position[]` in radians with automatic conversion. `joint_mix_control_cmd` uses raw int16 motor values for both `position[]` and `effort[]`, without unit conversion.

### Tactile Sensors (O10/O12)

O10 and O12 have different tactile sensor data structures, each using product-specific message types:

| Product | Message Type | Data Structure |
|---------|-------------|---------------|
| O10 | `omnihand_2025_node_msgs/TactileSensor` | 7 regions (THUMB/INDEX/MIDDLE/RING/LITTLE/PALM/DORSUM), each with `uint8[]` pressure values (1g, max 255g) |
| O12 | `omnihand_pro_2025_node_msgs/TactileSensor` | 5 fingers, each with `online_state`, `channel_value[6]`, `normal_force`, `tangent_force`, `tangent_force_angle`, `capa_approach[4]` |

### Custom Message Types

The `omnihand_msgs` package defines generic integer joint state messages shared by all products:

| Message Type | Fields |
|-------------|--------|
| `omnihand_msgs/JointStateInt8` | `std_msgs/Header header` + `string[] name` + `int8[] data` |
| `omnihand_msgs/JointStateInt16` | `std_msgs/Header header` + `string[] name` + `int16[] data` |

## Configuration

ROS2 nodes use YAML configuration files for parameter management. Parameter names are consistent with the Python API.

### Configuration Parameters

Parameters are organized under `left_hand` and `right_hand` namespaces. If a hand's `connection_type` is empty (or commented out in YAML), that hand is skipped.

#### Per-Hand Parameters (`left_hand` / `right_hand`)

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `hand_device_id` | int | 1 | Hand device ID (1-255) |
| `connection_type` | string | "" | Connection type: "zlgcan", "hcan", "socketcan", "rs485", "usb"; empty to skip |
| `canfd_serial_number` | string | "" | CANFD adapter serial number (recommended, stable after reboot) |
| `canfd_device_id` | int | 0 | CANFD adapter device index (not recommended, may change after reboot) |
| `canfd_channel_id` | int | 0 | CAN channel index (0 or 1) |
| `can_interface` | string | "can0" | SocketCAN interface name (socketcan only) |
| `uart_port` | string | "" | Serial port path (rs485/usb only, O10 only) |
| `baudrate` | int | 460800 | Baudrate (rs485/usb only, O10 only) |

### Usage

**1. Using ros2 launch (recommended):**

```bash
# O10
ros2 launch omnihand_node omnihand_2025_node.launch.py

# O12
ros2 launch omnihand_node omnihand_pro_2025_node.launch.py

# H3U_M
ros2 launch omnihand_node omnihand_3_ultra_m_node.launch.py
```

**2. YAML configuration example:**

```yaml
/**:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "hcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0
```

**3. Python script examples:**

```bash
# Send position command + subscribe to readback
python3 joint_cmd.py left o10

# Trigger and view error report
python3 joint_error.py left o10

# Trigger and view temperature
python3 joint_temperature.py left o10

# Trigger and view current
python3 joint_current.py left o10

# Mixed control (position + torque, O10/O12 only)
python3 mix_control_pub.py left o10

# Send raw motor position (int16 tick) + subscribe to readback (O10/O12 only)
python3 motor_pos.py left o10

# Trigger and view tactile sensor (O10/O12 only)
python3 tactile.py left o10

# Set current threshold
python3 joint_current_threshold_pub.py 500 left o10

# Set control mode (H3U_M only, 0=CSP, 7=PP)
python3 joint_control_mode_pub.py 0 left h3u_m
```

**4. Connection types:**

| Connection | O10 | O12 | H3U_M |
|-----------|-----|-----|-------|
| zlgcan | ✅ | ✅ | ✅ |
| hcan | ✅ | ✅ | ✅ |
| socketcan | ✅ | ✅ | ✅ |
| rs485 | ✅ | ❌ | ❌ |
| usb | ✅ | ❌ | ❌ |
