# OmniHand Dex UMI (O10 UMI) Python API

## Overview

**OmniHand Dex UMI (O10 UMI)** is a 10 DOF dexterous hand using the UMI protocol. This document describes the Python API for controlling and interacting with OmniHand Dex UMI (O10 UMI) devices.

**Key Features:**
- 10 active degrees of freedom
- 1D tactile sensors (fingers, palm, dorsum)
- UMI protocol (Pn1-Pn7 registers)
- Periodic position and tactile sensor reports via callbacks
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
    DORSUM = 7
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
    # UMI-specific fields (optional)
    position_report_frequency: Optional[int]  # Pn2.03: Position report frequency (Hz, default 100)
    tactile_sensor_report_frequency: Optional[int]  # Pn2.04: Tactile sensor report frequency (Hz, default 100)
    adc_channel_count: Optional[int]  # Pn2.05: ADC channel count (read-only)
    tactile_sensor_info: Optional[List[int]]  # Pn2.06: Tactile sensor information (read-only)

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
        DeviceInfo: Device info structure containing device ID, communication parameters,
                    and UMI-specific fields (position_report_frequency, tactile_sensor_report_frequency,
                    adc_channel_count, tactile_sensor_info).
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
    """Set maximum position calibration (UMI Protocol Pn7, sub-register 0x01).
    
    Note:
        This is a write-only operation for position calibration.
        The device should be in maximum position when calling this function.
    """
```

## Periodic Report Frequency Settings

```python
def set_position_report_frequency(self, frequency: int) -> None:
    """Set position report frequency (UMI Protocol Pn2.03).
    
    Args:
        frequency: Report frequency in Hz (default: 100).
    
    Note:
        Setting frequency to 0 will disable periodic reports.
    """

def set_tactile_sensor_report_frequency(self, frequency: int) -> None:
    """Set tactile sensor report frequency (UMI Protocol Pn2.04).
    
    Args:
        frequency: Report frequency in Hz (default: 100).
    
    Note:
        Setting frequency to 0 will disable periodic reports.
    """
```

## Periodic Report Callbacks

**Note**: UMI protocol supports periodic reports via callbacks. The callbacks are executed in a background thread, so they should be thread-safe.

```python
def set_position_report_callback(self, callback: Optional[Callable[[List[int]], None]], 
                                 frequency: Optional[int] = None) -> None:
    """Register callback function for position periodic report (UMI Protocol Pn3, Pn2.03 sets frequency).
    
    Args:
        callback: Callback function to be called when position data is received.
                 The callback receives a list of position values (voltage values in mV).
                 If None, the callback will be unregistered.
        frequency: Optional frequency in Hz (if provided, sets Pn2.03 before registering callback, default: 100).
    
    Note:
        The callback will be called in a background thread, so it should be thread-safe.
    """

def set_tactile_sensor_report_callback(self, 
                                       callback: Optional[Callable[[TactileSensorData, int], None]], 
                                       frequency: Optional[int] = None) -> None:
    """Register callback function for tactile sensor periodic report (UMI Protocol Pn6, Pn2.04 sets frequency).
    
    Args:
        callback: Callback function to be called when tactile sensor data is received.
                 The callback receives (sensor_data: TactileSensorData, sensor_id: int).
                 sensor_id is the sub-register address (0x01~0x07).
                 If None, the callback will be unregistered.
        frequency: Optional frequency in Hz (if provided, sets Pn2.04 before registering callback, default: 100).
    
    Note:
        The callback will be called in a background thread, so it should be thread-safe.
    """
```

## Tactile Sensor Data

OmniHand Dex UMI (O10 UMI) uses **1D tactile sensors** similar to OmniHand 2025 (O10):
- **Data unit**: 1g
- **Max value**: 255g
- **Sensor locations**: Fingers, Palm, Dorsum
- **Protocol**: UMI Protocol Pn6 (read-only, sub-register 0x01~0x07)

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
    """

@staticmethod
def get_sensor_order() -> List[int]:
    """Get sensor order vector (static method).
    
    Returns:
        List[int]: Reference to sensor order vector.
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

1. **No Position/Velocity/Torque Control**: OmniHand Dex UMI (O10 UMI) is a **read-only** device. It does not support position, velocity, or torque control. It only provides position information via periodic reports.

2. **Periodic Reports**: UMI protocol supports periodic reports for position and tactile sensor data. Use callbacks to receive this data asynchronously.

3. **Thread Safety**: Callback functions are executed in a background thread. Ensure your callbacks are thread-safe.

4. **Position Calibration**: Position calibration (min/max) is a write-only operation. The device should be in the appropriate position when calling calibration functions.

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

# Get device information (includes UMI-specific fields)
device_info = hand.get_device_info()
print(device_info)

# Register position report callback
def position_callback(positions):
    print(f"Position report: {len(positions)} values")

hand.set_position_report_callback(position_callback, frequency=100)  # 100 Hz

# Register tactile sensor report callback
def tactile_callback(sensor_data, sensor_id):
    print(f"Tactile sensor report: sensor_id={sensor_id}, data_size={len(sensor_data.data)}")

hand.set_tactile_sensor_report_callback(tactile_callback, frequency=100)  # 100 Hz

# Keep running to receive periodic reports
time.sleep(10)

# Unregister callbacks
hand.set_position_report_callback(None)
hand.set_tactile_sensor_report_callback(None)
```

## UMI Protocol Register Reference

- **Pn1**: Vendor information (read-only)
- **Pn2**: Device information (read-only)
  - **Pn2.03**: Position report frequency (read-write, 2 bytes, Hz, default 100)
  - **Pn2.04**: Tactile sensor report frequency (read-write, 2 bytes, Hz, default 100)
  - **Pn2.05**: ADC channel count (read-only, 1 byte)
  - **Pn2.06**: Tactile sensor information (read-only, variable length)
- **Pn3**: Position information (read-only, periodic report)
- **Pn6**: Tactile sensor data (read-only, periodic report, sub-register 0x01~0x07)
- **Pn7**: Position calibration (write-only)
  - **Pn7.00**: Minimum position calibration
  - **Pn7.01**: Maximum position calibration

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
