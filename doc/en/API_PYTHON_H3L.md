# OmniHand 3 Lite (H3L) Python API

## Overview

**OmniHand 3 Lite (H3L)** is a 4 DOF lightweight dexterous hand. This document describes the Python API for controlling and interacting with OmniHand 3 Lite devices.

**Key Features:**
- 4 active degrees of freedom
- Motor position range: 0-4096 (int16 motor ticks)
- Supports CAN (ZLG USB CANFD / HCAN) communication
- Supports SocketCAN (Linux only)
- Supports ZLG CAN TCP (Linux x64 / Windows only)
- **No tactile sensors**
- **No kinematics solver**: angle-based control is unavailable (`set_all_active_joint_angles` is a stub). Use motor position control (`set_all_joint_motor_positions`) instead.

## Import

```python
from omnihand import OmniHand3Lite, HandType, ControlMode
```

## Factory Methods

### Recommended: ZLG USB CANFD (Zero Configuration)

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """Creates a dexterous hand object (Recommended: Zero configuration).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        canfd_device_id: USB CANFD adapter device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1.

    Returns:
        OmniHand3Lite: Dexterous hand instance.

    Note:
        ✅ Recommended: Zero configuration, ready to use out of the box. No root privileges required.
    """
```

### Factory Method by ZLG Serial Number

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType,
                hand_device_id: int,
                usbcanfd_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """Creates a dexterous hand object by serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        usbcanfd_serial_number: USB CANFD device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0.

    Returns:
        OmniHand3Lite: Dexterous hand instance, or None if device not found.
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """Creates a dexterous hand object (HCAN USB CANFD, by device ID).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        canfd_device_id: HCAN device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0.

    Returns:
        OmniHand3Lite: Dexterous hand instance.
    """
```

### Factory Method by HCAN Serial Number

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType,
                hand_device_id: int,
                hcan_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """Creates a dexterous hand object by HCAN serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        hcan_serial_number: HCAN device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0.

    Returns:
        OmniHand3Lite: Dexterous hand instance, or None if device not found.
    """
```

### SocketCAN (Linux only)

```python
@staticmethod
def create_hand_socketcan(hand_type: HandType,
                hand_device_id: int,
                can_interface: str = "can0") -> 'OmniHand3Lite':
    """Creates a dexterous hand object (SocketCAN communication, Linux only).

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        can_interface: CAN interface name (e.g. "can0", "can1"), default "can0".

    Returns:
        OmniHand3Lite: Dexterous hand instance.
    """
```

### ZLG CAN TCP (partial platform support)

```python
@staticmethod
def create_hand_by_zlgcan_tcp(hand_type: HandType,
                hand_device_id: int,
                tcp_host: str,
                tcp_port: int,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """Creates a dexterous hand object (ZLG CANFD over TCP).

    Only supported on Windows and Linux x64 (not Linux aarch64/arm64).

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        tcp_host: TCP server IP or hostname.
        tcp_port: TCP server port.
        canfd_channel_id: CAN channel index, defaults to 0.

    Returns:
        OmniHand3Lite: Dexterous hand instance.
    """
```

## Core API

### Initialization

```python
def init(self) -> bool:
    """Initializes the device. Must be called after creating an instance. Returns False on failure."""
```

### Gesture Control

```python
class OmniHand3LiteGesture(enum.IntEnum):
    OMNI_HAND_3_LITE_GESTURE_ALL_ZERO = 0  # All-zero position
    OMNI_HAND_3_LITE_GESTURE_FIST = 1      # Fist
    OMNI_HAND_3_LITE_GESTURE_OPEN = 2      # Open hand

def set_hand_gesture(self, gesture: OmniHand3LiteGesture) -> None:
    """Sets a predefined hand gesture.

    Args:
        gesture: OmniHand3LiteGesture enum value.

    Preset gesture positions are defined for the right hand. The solver automatically mirrors motors 1 and 4 for the left hand.
    """
```

### Motor Position Control (Recommended)

> **H3L has no full kinematics solver — use motor position control (ticks).** Angle control methods are stubs. Gesture control is available via `set_hand_gesture()`.

```python
def set_joint_motor_posi(self, joint_motor_index: int, posi: int) -> int:
    """Sets the position of a single joint motor.

    Args:
        joint_motor_index: Joint motor index (1-4).
        posi: Motor position (0-4095).

    Returns:
        int: Actual position returned by the device. -1 on failure.
    """

def set_all_joint_motor_positions(self, positions: List[int]) -> List[int]:
    """Sets positions of all joint motors in batch.

    Args:
        positions: Target position list, 4 ints, range 0-4095.

    Returns:
        List[int]: Actual position list returned by the device. Empty on failure.
    """

def get_joint_motor_posi(self, joint_motor_index: int) -> int:
    """Gets the position of a single joint motor.

    Args:
        joint_motor_index: Joint motor index (1-4).

    Returns:
        int: Current motor position (0-4095).
    """

def get_all_joint_motor_positions(self) -> List[int]:
    """Gets positions of all joint motors in batch.

    Returns:
        List[int]: Current motor position list (4 values).
    """
```

### Joint Angle Control (unavailable)

```python
# These methods are stubs on H3L. Use motor position control instead.
def set_all_active_joint_angles(self, angles: List[float]) -> List[float]: ...
def get_all_active_joint_angles(self) -> List[float]: ...
```

### Velocity Control

```python
def set_joint_motor_velo(self, joint_motor_index: int, velo: int) -> None: ...
def get_joint_motor_velo(self, joint_motor_index: int) -> int: ...
def set_all_joint_motor_velo(self, velocities: List[int]) -> None: ...
def get_all_joint_motor_velo(self) -> List[int]: ...
```

### Current Threshold Control

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None: ...
def get_current_threshold(self, joint_motor_index: int) -> int: ...
def set_all_current_thresholds(self, thresholds: List[int]) -> None: ...
def get_all_current_thresholds(self) -> List[int]: ...
```

### Mixed Control

```python
def mix_ctrl_joint_motor(self, mix_ctrls: List[MixCtrl]) -> None:
    """Controls joint motors in mixed mode.

    MixCtrl fields:
        joint_index: Joint index (1-4)
        ctrl_mode: Control mode (only ControlMode.POSITION_TORQUE is available)
        tgt_posi: Target position (0-4095)
        tgt_torque: Target current (mA)
    """
```

### Error Handling

```python
def get_error_report(self, joint_motor_index: int) -> JointMotorErrorReport:
    """Gets the error report for a single joint motor.

    Returns:
        JointMotorErrorReport: Contains stalled, overheat, over_current, motor_except, commu_except bits.
    """

def get_all_error_reports(self) -> List[JointMotorErrorReport]:
    """Gets the error reports for all joint motors. Returns 4 values."""
```

### Temperature Monitoring

```python
def get_temperature_report(self, joint_motor_index: int) -> int:
    """Gets the temperature report for a single joint motor (°C)."""

def get_all_temperature_reports(self) -> List[int]:
    """Gets the temperature reports for all joint motors. Returns 4 values."""
```

### Current Monitoring

```python
def get_current_report(self, joint_motor_index: int) -> int:
    """Gets the current report for a single joint motor."""

def get_all_current_reports(self) -> List[int]:
    """Gets the current reports for all joint motors. Returns 4 values."""
```

### Vendor & Device Information

```python
def get_vendor_info(self) -> VendorInfo: ...
def get_device_info(self) -> DeviceInfo: ...
def set_device_id(self, device_id: int) -> None: ...
```

### Debugging

```python
def show_data_details(self, show: bool) -> None:
    """Toggles the display of raw send/receive data details."""
```

## Complete Example

```python
from omnihand import OmniHand3Lite, HandType

# Create hand instance
hand = OmniHand3Lite.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("Failed to initialize OmniHand 3 Lite")
    exit(1)

# Get vendor information
vendor = hand.get_vendor_info()
print(vendor)

# Set a predefined gesture
from omnihand import OmniHand3LiteGesture
hand.set_hand_gesture(OmniHand3LiteGesture.OMNI_HAND_3_LITE_GESTURE_FIST)

# Set motor positions (4 joints, 0-4095)
positions = [2048, 2048, 2048, 2048]
echo = hand.set_all_joint_motor_positions(positions)
print(f"Motor positions: {echo}")

# Get temperature
temps = hand.get_all_temperature_reports()
print(f"Temperature: {temps}")
```

## Demo Files

The SDK release package includes ready-to-run Python demo scripts (under `python/demo/omnihand_3_lite/`):

| Demo | File |
|------|------|
| CAN FD (connect by ID) | [demo_canfd_id.py](../../../python/demo/omnihand_3_lite/demo_canfd_id.py) |
| CAN FD (connect by serial) | [demo_canfd_serial.py](../../../python/demo/omnihand_3_lite/demo_canfd_serial.py) |
| SocketCAN | [demo_socketcan.py](../../../python/demo/omnihand_3_lite/demo_socketcan.py) |
| ZLG CAN TCP | [demo_zlgcan_tcp.py](../../../python/demo/omnihand_3_lite/demo_zlgcan_tcp.py) |
| Motor position control | [demo_set_motor.py](../../../python/demo/omnihand_3_lite/demo_set_motor.py) |
| Hardware info | [demo_get_hardware_info.py](../../../python/demo/omnihand_3_lite/demo_get_hardware_info.py) |
| Dual-hand control (multi CAN) | [demo_set_motor_via_multicans.py](../../../python/demo/omnihand_3_lite/demo_set_motor_via_multicans.py) |
| Reliability test | [demo_set_get_position_reliability.py](../../../python/demo/omnihand_3_lite/demo_set_get_position_reliability.py) |
| Reliability test CSV | [demo_set_get_reliability_csv.py](../../../python/demo/omnihand_3_lite/demo_set_get_reliability_csv.py) |

## Related Documentation

- [OmniHand 3 Lite (H3L) C++ API](API_CPP_H3L.md)
- [OmniHand 3 Lite (H3L) ROS2 Interface](API_ROS2_H3L.md)
- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md)
