# OmniHand SDK ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## Overview

The OmniHand SDK provides ROS2 interfaces for two product models:

- **OmniHand 2025 (O10)**: 10 DOF dexterous hand with 1D tactile sensors
- **OmniHand Pro 2025 (O12)**: 12 DOF dexterous hand with 3D tactile sensors

Each product has its own ROS2 node and message types, providing product-specific interfaces.

## Product-Specific ROS2 Documentation

- **[OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md)** - 10 DOF, joint angle topics and set/get joint angle services
- **[OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md)** - 12 DOF, joint angle topics and set/get joint angle services

## Configuration

The ROS2 nodes support YAML configuration files for flexible parameter management. Parameter names are consistent with the Python API.

### Configuration Parameters

Parameters are organized under `left_hand` and `right_hand` namespaces. If a hand's `connection_type` is empty (or the namespace is commented out in YAML), that hand is skipped.

#### Per-Hand Parameters (`left_hand` / `right_hand`)

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `hand_device_id` | int | 1 | Hand device ID (1-255) |
| `connection_type` | string | "" | Connection type: "zlgcan", "hcan", "rs485", or "usb" (rs485/usb: O10 only); empty to skip |
| `canfd_serial_number` | string | "" | CANFD adapter serial number (stable after reboot/unplug; internally scans then opens) |
| `canfd_device_id` | int | 0 | CANFD adapter device index (opens directly without scan; index may change after reboot/unplug) |
| `canfd_channel_id` | int | 0 | CAN channel index (0 or 1) |
| `uart_port` | string | "" | Serial port path (rs485/usb only, O10 only) |
| `baudrate` | int | 460800 | Baudrate (rs485/usb only, O10 only) |

**Note (ZLG CANFD device identification)**:
- **By serial number** (`canfd_serial_number`): Stable after reboot/unplug; internally triggers a scan (open/close to read info) then open for use.
- **By device index** (`canfd_device_id`): Device is opened once without prior scan; index may change after reboot/unplug.

### Usage Examples

**Default: Single hand mode** - The default YAML configuration enables a single left hand. To enable dual-hand mode, uncomment the `right_hand` section in the YAML file.

**1. Using ros2 launch (recommended):**

```bash
# Use default configuration file (single hand, left)
ros2 launch omnihand_node omnihand_2025_node.launch.py

# Use specified configuration file (absolute path)
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  config_file:=/path/to/your/omnihand_2025_node.yaml
```

**2. Using ros2 run:**

```bash
# Direct run (uses code defaults, not recommended)
ros2 run omnihand_node omnihand_2025_node

# Use configuration file (recommended)
ros2 run omnihand_node omnihand_2025_node --ros-args \
  --params-file $(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml
```

**3. Example YAML configurations:**

Single hand (CANFD):
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_device_id: 0
      canfd_channel_id: 0
```

Dual hand (CANFD):
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0

    right_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "87654321"
      canfd_channel_id: 1
```

USB (O10 only):
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "usb"
      uart_port: "/dev/ttyACM0"
      baudrate: 460800
```

RS485 (O10 only):
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "rs485"
      uart_port: "/dev/ttyUSB0"
      baudrate: 460800
```

**4. Connection types:**

- **zlgcan** - ZLG USB CANFD (default)
- **hcan** - HCAN USB CANFD
- **rs485** - RS485 serial (O10 only)
- **usb** - USB serial (O10 only)

Each hand can use a different connection type.

**5. Device identification:**

With ZLG USBCANFD: **200U** has two CAN channels (can0, can1) for left/right hand; **100U / MINI** has a single channel, `canfd_channel_id` is always 0, single hand only.

- **By serial number** (`canfd_serial_number`): Stable after reboot/unplug; internally does a scan (open/close) then open for use.
- **By device index** (`canfd_device_id`): Device opened once without scan; index may change after reboot/unplug.
