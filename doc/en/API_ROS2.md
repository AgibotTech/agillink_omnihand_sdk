# OmniHand 2025 SDK ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The OmniHand 2025 SDK provides ROS2 interfaces for two product models:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand with 1D tactile sensors
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand with 3D tactile sensors

Each product has its own ROS2 node and message types, providing product-specific interfaces.

## Product-Specific ROS2 Documentation

- **[OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md)** - 10 DOF, 1D tactile sensors, supports mix control
- **[OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md)** - 12 DOF, 3D tactile sensors, no mix control

## Configuration

The ROS2 nodes support YAML configuration files for flexible parameter management. Parameter names are consistent with the Python API.

### Configuration Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `hand_type` | string | "left" | Hand type: "left" or "right" |
| `hand_device_id` | int | 1 | Hand device ID (1-255) |
| `canfd_device_id` | int | 0 | USB CANFD adapter device index |
| `canfd_channel_id` | int | 0 | CAN channel index (0 or 1) |
| `enable_both_hands` | bool | true | Enable dual-hand mode |
| `second_hand_type` | string | "right" | Second hand type (for dual mode) |
| `second_hand_device_id` | int | 1 | Second hand device ID |
| `second_canfd_device_id` | int | 0 | Second hand CANFD adapter device index |
| `second_canfd_channel_id` | int | 1 | Second hand channel index |

### Usage Examples

**Default: Both hands mode** - Both `ros2 run` and `ros2 launch` default to dual-hand mode (left hand on channel 0, right hand on channel 1).

**1. Using ros2 run (default: both hands):**

```bash
# Default: both hands (left on channel 0, right on channel 1)
ros2 run omnihand_node omnihand_2025_node

# Single hand only
ros2 run omnihand_node omnihand_2025_node --ros-args -p enable_both_hands:=false -p hand_type:=left
```

**2. Using ros2 launch (default: both hands):**

```bash
# Default: both hands (left on channel 0, right on channel 1)
ros2 launch omnihand_node omnihand_2025.launch.py

# Single left hand
ros2 launch omnihand_node omnihand_2025.launch.py enable_both_hands:=false hand_type:=left canfd_channel_id:=0

# Single right hand
ros2 launch omnihand_node omnihand_2025.launch.py enable_both_hands:=false hand_type:=right canfd_channel_id:=1

# Custom dual-hand configuration
ros2 launch omnihand_node omnihand_2025.launch.py canfd_channel_id:=0 second_canfd_channel_id:=1
```

**3. Using YAML configuration file:**

```bash
# Use pre-configured files from installed ROS2 package (recommended)
ros2 run omnihand_node omnihand_2025_node --ros-args \
    --params-file $(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml

# Or use absolute path to config file
ros2 run omnihand_node omnihand_2025_node --ros-args \
    --params-file /path/to/your/config/omnihand_2025_node.yaml
```

**4. Example YAML configuration:**

```yaml
# omnihand_2025_left.yaml
omnihand_2025_param_reader:
  ros__parameters:
    hand_type: "left"
    hand_device_id: 1
    canfd_device_id: 0
    canfd_channel_id: 0
    enable_both_hands: false

# omnihand_2025_both.yaml
omnihand_2025_param_reader:
  ros__parameters:
    hand_type: "left"
    hand_device_id: 1
    canfd_device_id: 0
    canfd_channel_id: 0
    enable_both_hands: true
    second_hand_type: "right"
    second_hand_device_id: 1
    second_canfd_device_id: 0
    second_canfd_channel_id: 1
```

### Pre-configured YAML Files

The SDK includes pre-configured YAML files in the ROS2 package share directory:

- `omnihand_2025_node.yaml` - Default configuration (single hand, left - can be configured for dual hands)
- `omnihand_pro_2025_node.yaml` - O12 default configuration (single hand, left - can be configured for dual hands)

Access via: `$(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/`

## Quick Reference

### O10 Topics
- Topic prefix: `/omnihand/omnihand_2025/`
- Message namespace: `omnihand_2025_node_msgs`
- DOF: 10
- Motor position range: 0-4096
- Supports mix control: Yes

### O12 Topics
- Topic prefix: `/omnihand/omnihand_pro_2025/`
- Message namespace: `omnihand_pro_2025_node_msgs`
- DOF: 12
- Motor position range: 0-2000
- Supports mix control: No

For detailed topic lists and message definitions, please refer to the product-specific documentation above.
