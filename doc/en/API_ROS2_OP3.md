# OmniPicker 3 (OP3) ROS2 Interface

> ⚠️ **Linux only**: the ROS2 node is included in Linux x64/aarch64 releases for Humble and Jazzy.

## Overview

The OP3 ROS2 node provides single-axis position and PT control, stroke control, gentle gripping, on-demand state queries, and two-region tactile streaming. Topics use:

```text
/op3/<side>/
```

`<side>` is `left` or `right`. Supported transports are `zlgcan`, `hcan`, `socketcan`, and `rs485`.

## Topics

| Topic | Message Type | Direction | Description |
|---|---|---|---|
| `joint_position_cmd` | `std_msgs/Int16MultiArray` | Subscribe | One raw motor-position value in the 0–4095 range |
| `joint_position_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish | One position value returned by the position command |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | Subscribe | Single-axis PT; `position[0]` is ticks and `effort[0]` is current in mA |
| `stroke_cmd` | `omnipicker_3_node_msgs/StrokeCommand` | Subscribe | Target stroke in mm with optional speed/current limits |
| `stroke_states` | `omnipicker_3_node_msgs/StrokeState` | Publish | `header` plus actual `stroke_mm` |
| `gentle_grip_cmd` | `omnipicker_3_node_msgs/GentleGripCommand` | Subscribe | Enter gentle-grip mode with an optional current limit |
| `gentle_grip_states` | `omnipicker_3_node_msgs/GentleGripState` | Publish | `header` plus raw firmware reply in `raw_response` |
| `joint_error_cmd` | `std_msgs/Empty` | Subscribe | Trigger an error query |
| `joint_error_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish | One error value |
| `joint_temperature_cmd` | `std_msgs/Empty` | Subscribe | Trigger a temperature query |
| `joint_temperature_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish | One temperature value |
| `joint_current_cmd` | `std_msgs/Empty` | Subscribe | Trigger a current query |
| `joint_current_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | Publish | One current value |
| `tactile_cmd` | `std_msgs/Float32` | Subscribe | `data` is Hz; >0 starts/changes rate, 0 stops, max 50 Hz |
| `tactile_states` | `omnipicker_3_node_msgs/TactileSensor` | Publish | `header` plus raw `uint16[]` regions `thumb` and `index` |

State reports use trigger-based readback. Tactile data streams periodically after a nonzero rate is received and stops when the node receives 0.

PT control accepts 0–4095 position ticks and 0–6500 mA current; the node clamps out-of-range values.

## Product-specific messages

### StrokeCommand

```text
float64 stroke_mm
bool use_speed
int16 speed_rpm
bool use_current
int16 current_ma
```

The stroke range is -5.00–112.00 mm. The node converts millimetres to the SDK's 0.01 mm unit and clamps the result. Speed is 0–5000 rpm and current is 0–3500 mA.

### GentleGripCommand

```text
bool use_current_limit
uint16 current_limit_ma
```

> **Important**: the OP3 protocol provides a command to enter gentle-grip mode but no exit command. Stroke commands can reopen the gripper, but gentle-grip mode remains active; power-cycle the device to leave it completely.

### TactileSensor

```text
std_msgs/Header header
uint16[] thumb
uint16[] index
```

## Launch

```bash
# Default configuration
ros2 launch omnihand_node omnipicker_3_node.launch.py

# Custom configuration
ros2 launch omnihand_node omnipicker_3_node.launch.py \
  config_file:=/path/to/omnipicker_3_node.yaml
```

The default configuration is `share/omnihand_node/config/omnipicker_3_node.yaml`. Configure only the required `left_hand` and/or `right_hand`; a side with an empty `connection_type` is skipped.

## Parameters

| Parameter | Type | Default | Description |
|---|---|---|---|
| `hand_device_id` | int | 1 | Device ID |
| `connection_type` | string | `""` | `zlgcan` / `hcan` / `socketcan` / `rs485` |
| `canfd_serial_number` | string | `""` | ZLG/HCAN serial number; takes priority over device index |
| `canfd_device_id` | int | 0 | ZLG/HCAN adapter index |
| `canfd_channel_id` | int | 0 | CAN channel index |
| `can_interface` | string | `can0` | SocketCAN interface |
| `uart_port` | string | `""` | RS485 serial path |
| `request_interval_ms` | int | -1 | SDK minimum request interval; -1 uses the SDK default |
| `frame_recv_timeout_ms` | int | -1 | Receive timeout; -1 uses the SDK default |
| `show_data_details` | bool | false | Print low-level traffic |

## Python test scripts

Scripts are installed under `lib/omnihand_node/scripts/omnipicker_3/`:

```bash
# Raw-position cycle: 0 ↔ 2048
python3 joint_position.py left 2048 1.5

# PT cycle: position 0 ↔ 2048 at 1000 mA
python3 mix_control_pt.py left 2048 1000 1.5

# Stroke cycle: 0 ↔ 50 mm
python3 stroke.py left 50 3000 1000 2

# Alternate stroke-open and gentle grip
python3 gentle_grip_stroke.py left \
  --open-mm 50 --grip-current-ma 1000 --period 2

# Start tactile streaming at 10 Hz; publish 0 to stop
python3 tactile.py left 10
python3 tactile.py left 0
```

## CLI examples

```bash
# Set raw position
ros2 topic pub --once /op3/left/joint_position_cmd \
  std_msgs/msg/Int16MultiArray '{data: [2048]}'

# Query temperature
ros2 topic pub --once /op3/left/joint_temperature_cmd std_msgs/msg/Empty '{}'
ros2 topic echo /op3/left/joint_temperature_states

# Stream tactile data at 10 Hz
ros2 topic pub --once /op3/left/tactile_cmd std_msgs/msg/Float32 '{data: 10.0}'
ros2 topic echo /op3/left/tactile_states
```

## Related documentation

- [ROS2 interface index](API_ROS2.md)
- [OmniPicker 3 C++ API](API_CPP_OP3.md)
- [SocketCAN setup guide](SOCKETCAN_SETUP.md)

