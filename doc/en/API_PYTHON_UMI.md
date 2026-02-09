# OmniHand Dex UMI (O10 UMI) Python API

## Overview

**OmniHand Dex UMI (O10 UMI)** is a 10 DOF dexterous hand using the UMI protocol. This document describes the Python API for controlling and interacting with OmniHand Dex UMI (O10 UMI) devices.

**Key Features:**
- 10 active degrees of freedom
- 1D tactile sensors (fingers and palm, no dorsum)
- UMI protocol (Pn1-Pn8 registers)
- Active position query (no periodic reports)
- Supports CAN (ZLG USB CANFD) communication only
- Supports SocketCAN (Linux only)
- **Read-only position information** (no position/velocity/torque control)

## Import

```python
from omnihand import OmniHandDexUMI, EHandType, EFinger
```

## Enumerations

### EHandType

```python
class EHandType(IntEnum):
    LEFT = 0      # Left hand
    RIGHT = 1     # Right hand
    UNKNOWN = 10
```

### EFinger

```python
class EFinger(IntEnum):
    THUMB = 1
    INDEX = 2
    MIDDLE = 3
    RING = 4
    LITTLE = 5
    PALM = 6
    # Note: UMI does not have DORSUM sensor
    UNKNOWN = 255
```

## Data Structures

### VendorInfo

```python
class VendorInfo:
    product_model: str
    product_seq_num: str
    hardware_version: Version
    software_version: Version
    voltage: int
    dof: int  # 10 for UMI

    def __str__(self) -> str: ...
```

### DeviceInfo

```python
class DeviceInfo:
    hand_device_id: int
    commu_params: CommuParams

    def __str__(self) -> str: ...
```

### TactileSensorData

```python
class TactileSensorData:
    sensor_id: int  # EFinger enum value
    data: List[int]  # Raw sensor data (unit: 1g, max: 255g)
```

## Factory Methods

### Recommended: ZLG USB CANFD (Zero Configuration)

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: EHandType = EHandType.LEFT,
                hand_device_id: int = 1,
                canfd_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """Creates a dexterous hand object (Recommended: Zero configuration).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: Hand device ID, defaults to 1.
        canfd_device_id: USB CANFD adapter device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance.
    """
```

**Example:**
```python
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

### Factory Method by Serial Number

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: EHandType,
                hand_device_id: int,
                usbcanfd_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """Creates a dexterous hand object by serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: Hand device ID.
        serial_number: USB CANFD device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance, or None if device not found.
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: EHandType = EHandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """Creates a dexterous hand object via HCAN USB CANFD (by device ID).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: Hand device ID, defaults to 1.
        canfd_device_id: HCAN device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance.
    """
```

**Example:**
```python
hand = OmniHandDexUMI.create_hand_by_hcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_canfd_channel_id=0
)
```

### Factory Method by HCAN Serial Number

```python
@staticmethod
def create_hand_by_hcan(hand_type: EHandType,
                hand_device_id: int,
                hcan_usbcanfd_serial_number: str,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """Creates a dexterous hand object via HCAN USB CANFD (by serial number).

    Args:
        hand_type: The hand type.
        hand_device_id: Hand device ID.
        hcan_serial_number: HCAN device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance, or None if device not found.
    """
```

### Advanced: SocketCAN (Linux Only)

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: EHandType = EHandType.LEFT,
                hand_device_id: int = 1,
                canfd_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """Creates a dexterous hand object (Recommended: Zero configuration).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        canfd_device_id: USB CANFD adapter device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance.
    
    Note:
        ✅ Recommended: Zero configuration, ready to use out of the box. 
        No root privileges required.
    """
```

**Example:**
```python
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

### Factory Method by Serial Number

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: EHandType,
                hand_device_id: int,
                usbcanfd_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """Creates a dexterous hand object by serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        usbcanfd_serial_number: USB CANFD device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance, or None if device not found.
    """
```

### Advanced: SocketCAN (Linux Only)

```python
@staticmethod
def create_hand_socketcan(hand_type: EHandType = EHandType.LEFT,
                          hand_device_id: int = 1,
                          can_interface: str = "can0") -> 'OmniHandDexUMI':
    """Creates a dexterous hand object using SocketCAN (Linux only, advanced usage).
    
    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        can_interface: CAN interface name (e.g., "can0", "can1").
    
    Returns:
        OmniHandDexUMI: Dexterous hand instance.
    
    Warning:
        ⚠️ Advanced Usage: Requires driver setup and root privileges.
    
    Note:
        **For detailed SocketCAN setup instructions, see**: [SocketCAN Setup Guide](SOCKETCAN_SETUP.md)
    """
```

## Basic Information

```python
def get_product_type(self) -> int:
    """Get product type.
    
    Returns:
        int: ProductType.OMNIHAND_DEX_UMI
    """

def init(self) -> bool:
    """Check initialization status.
    
    Returns:
        bool: True if initialized successfully, False otherwise.
    """
```

## Device Information

```python
def get_vendor_info(self) -> VendorInfo:
    """Gets vendor information.
    
    Returns:
        VendorInfo: Vendor info structure containing product model,
                    serial number, hardware version, software version, etc.
    """

def get_device_info(self) -> DeviceInfo:
    """Gets device information.
    
    Returns:
        DeviceInfo: Device info structure containing device ID and communication parameters.
    """

def set_device_id(self, device_id: int) -> None:
    """Sets the device ID.
    
    Args:
        hand_device_id: The hand device ID.
    """
```

## Position Calibration

**Note**: UMI protocol supports position calibration via Pn7 register. This is a write-only operation.

```python
def set_min_position_calibration(self) -> None:
    """Set minimum position calibration (UMI Protocol Pn7, sub-register 0x00).
    
    Note:
        This is a write-only operation for position calibration.
        The device should be in minimum position when calling this function.
    """

def set_max_position_calibration(self) -> None:
    """Set maximum position calibration (UMI Protocol Pn8, sub-register 0x00).
    
    Note:
        This is a write-only operation for position calibration.
        The device should be in maximum position when calling this function.
    """
```

## Position Query

**Note**: UMI protocol supports active position query. Use `get_joint_position()` or `get_all_joint_positions()` to query joint positions.

```python
def get_joint_position(self, joint_motor_index: int) -> int:
    """Get single joint motor position (UMI Protocol Pn3, sub-register 0x01-0x0A).
    
    Args:
        joint_motor_index: Joint motor index (0-9).
    
    Returns:
        int: Joint position (0-4096).
    """

def get_all_joint_positions(self) -> List[int]:
    """Get all joint motor positions (UMI Protocol Pn3, sub-register 0x00).
    
    Returns:
        List[int]: List of 10 joint positions (0-4096).
    """
```

## Tactile Sensor Data

OmniHand Dex UMI (O10 UMI) uses **1D tactile sensors** similar to OmniHand 2025 (O10):
- **Data unit**: 1g
- **Max value**: 255g
- **Sensor locations**: Fingers (Thumb, Index, Middle, Ring, Little), Palm (Note: UMI has no Dorsum sensor)
- **Protocol**: UMI Protocol Pn6 (read-only)
  - **Pn6.00**: Read all sensor data (6 sensors)
  - **Pn6.01~Pn6.06**: Read individual sensor data (sensor 1-6)

```python
def get_all_tactile_sensor_data_raw(self) -> List[TactileSensorData]:
    """Gets all 1D tactile sensor raw data from all sensors at once.
    
    Returns:
        List[TactileSensorData]: List of TactileSensorData structures.
    
    Note:
        This returns full resolution data.
        Uses UMI Protocol Pn6.
    """

def get_tactile_sensor_data_raw(self, eFinger: EFinger) -> TactileSensorData:
    """Gets 1D tactile sensor raw data for a single sensor.
    
    Args:
        eFinger: Finger/palm enum value.
    
    Returns:
        TactileSensorData: TactileSensorData structure containing full resolution data.
    
    Note:
        Uses UMI Protocol Pn6.
    """

@staticmethod
def get_sensor_data_length(finger_index: int) -> int:
    """Get sensor data length for a specific finger (static method).
    
    Args:
        finger_index: Finger enum value (EFinger).
    
    Returns:
        int: Sensor data length in bytes.
    
    Note:
        For UMI: Returns 0 for EFinger.DORSUM (UMI does not have dorsum sensor).
    """

@staticmethod
def get_sensor_order() -> List[int]:
    """Get sensor order vector (static method).
    
    Returns:
        List[int]: Reference to sensor order vector.
    
    Note:
        For UMI: The returned list includes EFinger.DORSUM, but UMI devices do not have dorsum sensor.
        When using get_all_tactile_sensor_data_raw(), only sensors available on UMI 
        (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM) are returned.
    """
```

## Debugging Features

```python
def show_data_details(self, show: bool) -> None:
    """Toggles the display of raw send/receive data details.
    
    Args:
        show: Whether to show the data details.
    """
```

## Important Notes

1. **No Position/Velocity/Torque Control**: OmniHand Dex UMI (O10 UMI) is a **read-only** device. It does not support position, velocity, or torque control. It only provides position information via active query.

2. **Active Position Query**: UMI protocol supports actively querying joint positions using `get_joint_position()` or `get_all_joint_positions()`. Position values range from 0-4096.

3. **Position Calibration**: Position calibration (min/max) is a write-only operation. The device should be in the appropriate position when calling calibration functions.

## Complete Example

```python
from omnihand import OmniHandDexUMI, EHandType, EFinger
import time

# Create hand instance
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("Failed to initialize OmniHand Dex UMI (O10 UMI)")
    exit(1)

# Get vendor information
vendor = hand.get_vendor_info()
print(vendor)

# Get device information
device_info = hand.get_device_info()
print(device_info)

# Query joint positions (active query)
positions = hand.get_all_joint_positions()
print(f"All joint positions ({len(positions)} values): {positions}")

# Get tactile sensor data
tactile_data = hand.get_all_tactile_sensor_data_raw()
print(f"Tactile sensors: {len(tactile_data)} sensors")
```

## UMI Protocol Register Reference

- **Pn1**: Vendor information (read-only)
- **Pn2**: Device information (read-only)
- **Pn3**: Position information (read-only, active query)
  - **Pn3.00**: Read all joint positions (10 values)
  - **Pn3.01~Pn3.0A**: Read individual joint position (joint 1-10)
- **Pn6**: Tactile sensor data (read-only)
  - **Pn6.00**: Read all sensor data (6 sensors: Thumb, Index, Middle, Ring, Little, Palm, UMI has no Dorsum)
  - **Pn6.01~Pn6.06**: Read individual sensor data (sensor 1-6)
- **Pn7**: Maximum position calibration (write-only)
  - **Pn7.00**: Set all joints max position at once
  - **Pn7.01~Pn7.0A**: Set individual joint max position (joint 1-10)
- **Pn8**: Minimum position calibration (write-only)
  - **Pn8.00**: Set all joints min position at once
  - **Pn8.01~Pn8.0A**: Set individual joint min position (joint 1-10)

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
