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

#### First Hand Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `hand_type` | string | "left" | Hand type: "left" or "right" |
| `hand_device_id` | int | 1 | Hand device ID (1-255) |
| `connection_type` | string | "zlg_can" | Connection type: "zlg_can", "hcan", or "rs485" |
| `canfd_serial_number` | string | "" | CANFD adapter serial number (recommended, stable after reboot/unplug) |
| `canfd_device_id` | int | 0 | CANFD adapter device index (alternative, may change after reboot/unplug) |
| `canfd_channel_id` | int | 0 | CAN channel index (0 or 1) |
| `uart_port` | string | "/dev/ttyUSB0" | Serial port path (rs485 only) |
| `baudrate` | int | 460800 | Baudrate (rs485 only) |

#### Second Hand Parameters (Optional)

If second hand parameters are configured, the second hand will be automatically started.

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `second_hand_type` | string | "" | Second hand type: "left" or "right" |
| `second_hand_device_id` | int | 1 | Second hand device ID |
| `second_connection_type` | string | "zlg_can" | Second hand connection type |
| `second_canfd_serial_number` | string | "" | Second hand CANFD adapter serial number (recommended) |
| `second_canfd_device_id` | int | 0 | Second hand CANFD adapter device index (alternative) |
| `second_canfd_channel_id` | int | 1 | Second hand CAN channel index |
| `second_uart_port` | string | "/dev/ttyUSB1" | Second hand serial port path (rs485 only) |
| `second_baudrate` | int | 460800 | Second hand baudrate (rs485 only) |

**Note**:
- It is recommended to use `canfd_serial_number` (serial number) because it remains stable after device reboot or unplug
- `canfd_device_id` is an alternative, but may change after device reboot or unplug

### Usage Examples

**Default: Single hand mode** - Both `ros2 run` and `ros2 launch` default to single hand mode (left hand). Dual-hand mode is enabled by configuring `second_hand_type` parameter.

**1. Using ros2 launch (recommended):**

```bash
# Use default configuration file (single hand, left)
# Default loads config/omnihand_2025_node.yaml (single hand config)
ros2 launch omnihand_node omnihand_2025_node.launch.py

# Use specified configuration file (relative path, relative to package share directory)
ros2 launch omnihand_node omnihand_2025_node.launch.py config_file:=config/omnihand_2025_node.yaml

# Use specified configuration file (absolute path)
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  config_file:=$(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml

# Override config via parameters (using serial number, recommended)
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  hand_type:=left \
  canfd_serial_number:="12345678" \
  canfd_channel_id:=0

# Override config via parameters (using device ID, alternative)
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  hand_type:=left \
  canfd_device_id:=0 \
  canfd_channel_id:=0

# Configure two hands (using serial numbers)
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  hand_type:=left \
  canfd_serial_number:="12345678" \
  canfd_channel_id:=0 \
  second_hand_type:=right \
  second_canfd_serial_number:="87654321" \
  second_canfd_channel_id:=1
```

**2. Using ros2 run:**

```bash
# Direct run (uses default parameters from code, not recommended)
# Default parameters are defined in node/src/omnihand_2025/main.cpp
# Recommended to use config file or launch file
ros2 run omnihand_node omnihand_2025_node

# Use configuration file (recommended)
ros2 run omnihand_node omnihand_2025_node --ros-args \
  --params-file $(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml

# Configure via parameters (using serial number, recommended)
ros2 run omnihand_node omnihand_2025_node --ros-args \
  -p hand_type:=left \
  -p canfd_serial_number:="12345678" \
  -p canfd_channel_id:=0

# Configure via parameters (using device ID, alternative)
ros2 run omnihand_node omnihand_2025_node --ros-args \
  -p hand_type:=left \
  -p canfd_device_id:=0 \
  -p canfd_channel_id:=0

# Configure two hands (using serial numbers)
ros2 run omnihand_node omnihand_2025_node --ros-args \
  -p hand_type:=left \
  -p canfd_serial_number:="12345678" \
  -p canfd_channel_id:=0 \
  -p second_hand_type:=right \
  -p second_canfd_serial_number:="87654321" \
  -p second_canfd_channel_id:=1
```

**3. Example YAML configuration:**

```yaml
# omnihand_2025_node.yaml (single hand, left - default)
omnihand_2025_param_reader:
  ros__parameters:
    # First hand (required)
    hand_type: "left"                    # "left" or "right"
    hand_device_id: 1                     # Hand device ID (1-255)
    connection_type: "zlg_can"            # "zlg_can", "hcan", or "rs485" (O10 only)
    
    # CANFD connection (recommended: use serial number)
    canfd_serial_number: ""               # CANFD device serial number (recommended, more stable)
    canfd_device_id: 0                    # CANFD device index (alternative if serial number not provided)
    canfd_channel_id: 0                   # CAN channel (0 or 1)
    
    # RS485 connection (O10 only, if connection_type is "rs485")
    uart_port: "/dev/ttyUSB0"             # Serial port
    baudrate: 460800                      # Baudrate
    
    # Second hand (optional - if second_hand_type is set, dual-hand mode is enabled)
    # Uncomment below to enable dual-hand mode:
    # second_hand_type: "right"            # "left" or "right" (empty = single hand mode)
    # second_hand_device_id: 1
    # second_connection_type: "zlg_can"
    # second_canfd_serial_number: ""
    # second_canfd_device_id: 0
    # second_canfd_channel_id: 1
    # second_uart_port: "/dev/ttyUSB1"
    # second_baudrate: 460800
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
