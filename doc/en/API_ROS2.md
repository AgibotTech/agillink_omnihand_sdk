# OmniHand SDK ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The OmniHand SDK provides unified ROS2 interfaces for four product models:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand
- **OmniHand 3 Lite (H3L)**: 4 DOF lightweight dexterous hand
- **OmniHand 3 Ultra M (H3U_M / O20)**: 20 DOF dexterous hand

All products share the same topic naming convention and message types, differing only in DOF count and connection options.

## Product-Specific ROS2 Documentation

- **[OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md)** - 10 DOF
- **[OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md)** - 12 DOF
- **[OmniHand 3 Lite (H3L) ROS2 Interface](API_ROS2_H3L.md)** - 4 DOF

## Unified Topic Specification

All products follow the same topic naming and interaction pattern:

### Topic Naming

```
/<product>/<side>/<topic_name>
```

- `<product>`: `o10` / `o12` / `h3l` / `h3u_m`
- `<side>`: `left` / `right`
- `<topic_name>`: see table below

### Topic List

| Topic | Message Type | Direction | Trigger | Description |
|-------|-------------|-----------|---------|-------------|
| `joint_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | — | Position command `position[]` = rad |
| `joint_states` | `sensor_msgs/JointState` | Publish (you sub) | On `joint_cmd` received | Position readback `position[]` = rad |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe (you pub) | — | Position+torque mixed control (O10/O12 only, see below) |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger error report query |
| `joint_error_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | On `joint_error_cmd` received | `data[]` = error bitmask |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger temperature query |
| `joint_temperature_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | On `joint_temperature_cmd` received | `data[]` = temperature |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe (you pub) | — | Trigger current query |
| `joint_current_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | On `joint_current_cmd` received | `data[]` = current |
| `joint_control_mode_cmd` | `std_msgs/Int8MultiArray` | Subscribe (you pub) | — | Write control mode `data[]` (O12/H3U_M only) |
| `joint_control_mode_states` | `std_msgs/Int8MultiArray` | Publish (you sub) | On `joint_control_mode_cmd` received | Readback control mode `data[]` (O12/H3U_M only) |
| `joint_voltage_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | — | Write voltage command `data[]` (O12 only) |
| `joint_voltage_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | On `joint_voltage_cmd` received | Readback voltage command `data[]` (O12 only) |
| `joint_current_threshold_cmd` | `std_msgs/Int16MultiArray` | Subscribe (you pub) | — | Write current threshold `data[]` |
| `joint_current_threshold_states` | `std_msgs/Int16MultiArray` | Publish (you sub) | On `joint_current_threshold_cmd` received | Readback current threshold `data[]` |
| `tactile_cmd` | `std_msgs/Float32` | Subscribe (you pub) | — | Tactile stream: `data` = Hz (>0 start/restart, 0 stop); max **50 Hz** (O10) / **100 Hz** (O12), hardcoded in node |
| `tactile_states` | Product-specific msg (see below) | Publish (you sub) | While tactile stream active | Tactile sensor data at the requested rate |

**Design Principle — Trigger-based Readback**: The OmniHand ROS2 node **does not use periodic timers by default**. Temperature, current, error codes, etc. are triggered externally — publish the corresponding `*_cmd` once to get one `*_states` message. **Exception — tactile (O10/O12)**: publish `std_msgs/Float32` to `tactile_cmd` once to set the stream rate in Hz; the node runs an internal timer until you publish `0`. **Exception — joint position**: `joint_cmd` automatically publishes `joint_states` after control.

The rationale behind this design:
- **No interference with the control loop**: Periodic autonomous queries would consume CAN bus bandwidth and may affect the real-time performance of position control commands.
- **User-controlled resource usage**: The state query frequency is entirely determined by the user — trigger on demand, avoid unnecessary bus load.
- **Usage pattern**: For example, to read temperature, publish a `std_msgs/Empty` to `joint_temperature_cmd`; the node will query the hardware and publish one reading on `joint_temperature_states`. The `joint_cmd` topic is special — after sending a position command, the node automatically publishes position readback on `joint_states` without an extra trigger.

### Control Mode

Different product models have different control mode support:

**O10**: No `joint_control_mode_cmd`. On `joint_mix_control_cmd`, `position[]`+`effort[]` only → position+current mix; add `velocity[]` (sufficient length) → position+velocity+current mix. **H3L**: position+current mix only. In mixed control, `effort` is motor current in **mA** (0–1000), not standard N·m.

**O12**: Supports switching control modes via `joint_control_mode_cmd` topic (0=POSITION, 1=SERVO, 2=VELOCITY, 3=TORQUE, 4=VOLTAGE, 7=PROFILE_POSITION). Voltage commands use `joint_voltage_cmd`; switch the target joints to mode `4` before sending voltage commands. O12 also supports position + force mixed control via `joint_mix_control_cmd`. In mixed control, the `effort` field is in **0.01 N** (correlated with tactile sensor normal force).

**H3U_M**: Supports switching control modes via `joint_control_mode_cmd` topic (0=CSP position mode, 7=PP profile position mode).

### Mixed Control (O10/O12)

`joint_mix_control_cmd` is a separate topic from `joint_cmd` for mixed control, using `sensor_msgs/JointState`:

- `position[]` = raw int16 motor position
- `effort[]` = raw int16 motor torque (O10/H3L: current in mA, range 0–1000; O12: 0.01 N)

The node calls `MixControlByPT` or `MixControlByPVT` (full-DOF arrays required). **No readback**.

**Note**: `joint_cmd` uses `position[]` in radians with automatic conversion. `joint_mix_control_cmd` uses raw int16 motor values for both `position[]` and `effort[]`, without unit conversion.

### Tactile Sensors (O10/O12)

O10 and O12 have different tactile sensor data structures, each using product-specific message types:

| Product | Message Type | Data Structure |
|---------|-------------|---------------|
| O10 | `omnihand_2025_node_msgs/TactileSensor` | `header` + seven `uint8[]` (Raw full resolution via `GetAllTactileSensorDataRaw()`); typical lengths: thumb 16, four fingers 18 each, palm 78, dorsum 102 (see [API_ROS2_O10.md](API_ROS2_O10.md)) |
| O12 | `omnihand_pro_2025_node_msgs/TactileSensor` | `header` + six `TactileSensorData` fields: `thumb`, `index`, `middle`, `ring`, `little`, `palm` (`online_state`, `channel_value[6]`, forces, angle, `capa_approach[4]`) |

### `std_msgs` array payloads

Error, temperature, current, current-threshold, and O12 voltage topics carry per-joint `int16` values in `std_msgs/Int16MultiArray` (`data[]`, with a 1-D `layout.dim` entry). O12/H3U_M control modes use `std_msgs/Int8MultiArray` the same way. Tactile data remains in product-specific `omnihand_*_node_msgs` types.

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

Tactile stream maximum rate is **50 Hz** (O10) or **100 Hz** (O12), fixed in each product's `hand_node.h` (`kMaxTactilePublishHz`), not a YAML parameter.

### Usage

**1. Using ros2 launch (recommended):**

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

# Start/stop tactile stream (O10/O12)
python3 tactile.py left o10 10
python3 tactile.py left o10 0

# Set current threshold
python3 joint_current_threshold_pub.py 500 left o10

# Set control mode (O12: 4=VOLTAGE; H3U_M: 0=CSP, 7=PP)
python3 joint_control_mode_pub.py 0 left h3u_m

# O12 voltage control: switch to VOLTAGE mode, then send voltage command
python3 joint_control_mode_pub.py 4 left o12
python3 joint_voltage_pub.py 0 left o12
```

**4. Connection types:**

| Connection | O10 | O12 | H3L | H3U_M |
|-----------|-----|-----|-----|-------|
| zlgcan | ✅ | ✅ | ✅ | ✅ |
| hcan | ✅ | ✅ | ✅ | ✅ |
| socketcan | ✅ | ✅ | ✅ | ✅ |
| rs485 | ✅ | ❌ | ❌ | ❌ |
| usb | ✅ | ❌ | ❌ | ❌ |

## C++ Development Demo

The release package includes a ROS2 C++ demo showing how to control OmniHand via standard ROS2 topics (without depending on the OmniHand C++ SDK).

The demo is located at `ros2/humble/share/omnihand_node/demo/` and contains:
- `ros2_joint_cmd_demo.cpp` — Uses `sensor_msgs/JointState` for position control + `std_msgs/Int16MultiArray` for temperature and current readback
- `CMakeLists.txt` / `package.xml` — Standard ament_cmake package configuration

### Quick Start

```bash
# 1. Source ROS2 and OmniHand release environment
source /opt/ros/humble/setup.bash
source /path/to/omnihand_sdk_release/ros2/setup.bash

# 2. Create workspace and copy demo
mkdir -p ~/omnihand_ws/src
cp -r /path/to/omnihand_sdk_release/ros2/humble/share/omnihand_node/demo \
      ~/omnihand_ws/src/omnihand_ros2_demo

# 3. Build and run
cd ~/omnihand_ws
colcon build --packages-select omnihand_ros2_demo
source install/setup.bash
ros2 run omnihand_ros2_demo ros2_joint_cmd_demo left o10
```

### Custom Development

To write your own ROS2 C++ package for OmniHand control, add the dependencies in `package.xml`:

```xml
<depend>rclcpp</depend>
<depend>sensor_msgs</depend>
<depend>std_msgs</depend>
```

And in `CMakeLists.txt`:

```cmake
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
ament_target_dependencies(your_node rclcpp sensor_msgs std_msgs)
```

> 💡 `source ros2/setup.bash` adds the OmniHand message packages and libraries to the search paths, so your package can be built in any workspace location.
>
> If you only need joint position control (no temperature/current/error readback), you can use only `sensor_msgs` and `std_msgs` for triggers.
