# OmniHand 3 Lite (H3L) Python API

## Overview

**OmniHand 3 Lite (H3L)** is a 4 DOF lightweight dexterous hand. This document describes the Python API for controlling and interacting with OmniHand 3 Lite devices.

**Key Features:**
- 4 active degrees of freedom
- Motor position range: 0-4096 (int16 motor ticks)
- Supports CAN (ZLG USB CANFD / HCAN) communication
- Supports SocketCAN (Linux only)
- Supports ZLG CAN TCP (Linux x64 / Windows only)
- Supports RS485 communication (default 460800 baud)
- Six U16 1D tactile sensors: five fingers and palm
- Active-joint angle control and offline motor/angle conversion through `OmniHand3LiteSolver`
- No full active-plus-passive kinematics solver; `get_all_joint_angles()` and `get_all_joint_angles_from_motor_positions()` are not available for H3L

Calibration, zero-point, parameter-persistence, and firmware-maintenance APIs
are intentionally not exposed by the H3L Python interface.

## Import

```python
from omnihand.omnihand_3_lite import (
    OmniHand3Lite,
    OmniHand3LiteGesture,
    OmniHand3LiteSolver,
    TactileSensorDataU16,
)
from omnihand import Finger, HandType
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

### RS485

```python
@staticmethod
def create_hand_by_rs485(
    hand_type: int = 0,
    hand_device_id: int = 1,
    serial_port: str = "/dev/ttyUSB0",
    baud_rate: int = 460800,
) -> "OmniHand3Lite": ...
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

def get_hand_gesture(self, gesture: OmniHand3LiteGesture) -> List[int]: ...
```

### Motor Position Control (Recommended)

Motor indices are 1-based. Motor positions use the range 0-4096.

```python
def set_joint_position(self, joint_motor_index: int, posi: int) -> None:
    """Sets the position of a single joint motor.

    Args:
        joint_motor_index: Joint motor index (1-4).
        posi: Motor position (0-4096).

    """

def set_all_joint_positions(self, positions: List[int]) -> List[int]:
    """Sets positions of all joint motors in batch.

    Args:
        positions: Target position list, 4 ints, range 0-4096.

    Returns:
        List[int]: Actual position list returned by the device. Empty on failure.
    """

def get_joint_position(self, joint_motor_index: int) -> int:
    """Gets the position of a single joint motor.

    Args:
        joint_motor_index: Joint motor index (1-4).

    Returns:
        int: Current motor position (0-4096).
    """

def get_all_joint_positions(self) -> List[int]:
    """Gets positions of all joint motors in batch.

    Returns:
        List[int]: Current motor position list (4 values).
    """
```

### Active Joint Angles and Offline Solver

```python
def set_all_active_joint_angles(self, angles: List[float]) -> List[float]: ...
def get_all_active_joint_angles(self) -> List[float]: ...

class OmniHand3LiteSolver:
    def __init__(self, is_left_hand: bool) -> None: ...
    def active_joint_pos_to_actuator_input(
        self, active_joint_pos: List[float]
    ) -> List[int]: ...
    def actuator_input_to_active_joint_pos(
        self, actuator_input: List[int]
    ) -> List[float]: ...
    def single_active_joint_pos_to_actuator_input(
        self, joint_index: int, angle: float
    ) -> int: ...
    def single_actuator_input_to_active_joint_pos(
        self, joint_index: int, actuator_input: int
    ) -> float: ...
    def set_hand_gesture(self, gesture: OmniHand3LiteGesture) -> List[int]: ...
```

The solver methods above are pure calculations and do not communicate with the
device. H3L does not currently provide full active-plus-passive joint-angle
kinematics.

### Velocity Control

```python
def set_joint_velocity(self, joint_motor_index: int, velocity: int) -> None: ...
def get_joint_velocity(self, joint_motor_index: int) -> int: ...
def set_all_joint_velocities(self, velocities: List[int]) -> None: ...
def get_all_joint_velocities(self) -> List[int]: ...
```

### Current Threshold Control

> **Note**: O10/H3L: not allowed to use current threshold control and MixControl at the same time.
> If you want to use MixControl, please do not call the current threshold interfaces.

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None: ...
def get_current_threshold(self, joint_motor_index: int) -> int: ...
def set_all_current_thresholds(self, thresholds: List[int]) -> None: ...
def get_all_current_thresholds(self) -> List[int]: ...
```

### Mixed Control

```python
def mix_control_by_pt(self, positions: List[int], torques: List[int]) -> List[MixCtrl]: ...
def mix_control_by_pvt(self, positions: List[int], velocities: List[int], torques: List[int]) -> List[MixCtrl]: ...
    """Controls joint motors in mixed mode.

    MixCtrl fields:
        joint_index: Joint index (1-4)
        ctrl_mode: Control mode (`MixControlMode.POSITION_TORQUE` or `POSITION_VELOCITY_TORQUE`)
        tgt_posi: Target position (0-4096)
        tgt_torque: Target current (mA)
    """
```

H3L also exposes the 0x25 status-returning variants. H3L does not support PV
mixed control.

```python
def mix_control_with_status(self, controls: List[MixCtrl]) -> MixCtrlResult: ...
def mix_control_by_pt_with_status(
    self, joint_motor_index: int, position: int, torque: int
) -> MixCtrlResult: ...
def mix_control_by_pt_with_status(
    self, positions: List[int], torques: List[int]
) -> MixCtrlResult: ...
def mix_control_by_pvt_with_status(
    self, joint_motor_index: int, position: int, velocity: int, torque: int
) -> MixCtrlResult: ...
def mix_control_by_pvt_with_status(
    self, positions: List[int], velocities: List[int], torques: List[int]
) -> MixCtrlResult: ...
```

### Position Control with Status (0x24)

```python
def set_joint_position_with_status(
    self, joint_motor_index: int, position: int
) -> JointPosiStatus: ...
def get_joint_position_with_status(
    self, joint_motor_index: int
) -> JointPosiStatus: ...
def set_all_joint_positions_with_status(
    self, positions: List[int]
) -> AllJointPosiStatus: ...
def get_all_joint_positions_with_status(self) -> AllJointPosiStatus: ...
```

### Joint Detail Information (0x26)

The all-joint query returns whole-hand bus information. The single-joint query
uses a zero-based `joint_index`.

```python
def get_all_joint_detail_info(self) -> HandDetailInfo: ...
def get_joint_detail_info(self, joint_index: int) -> JointDetailInfo: ...
```

The 0x24/0x25/0x26 interfaces require matching firmware support. Firmware that
does not implement these registers will time out and return empty/default
result objects.

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

### Product Metadata and Ranges

```python
def get_joint_names(self) -> List[str]: ...

@staticmethod
def get_min_max_motor_position(joint_motor_index: int) -> Int16Bound: ...
@staticmethod
def get_min_max_actual_motor_position(joint_motor_index: int) -> Int16Bound: ...
@staticmethod
def get_min_max_default_mix_ctrl_velocity(joint_motor_index: int) -> Int16Range: ...
@staticmethod
def get_min_max_default_mix_ctrl_torque(joint_motor_index: int) -> Int16Range: ...
@staticmethod
def get_min_max_activate_joint_angle(
    joint_motor_index: int, hand_type: HandType
) -> FloatBound: ...
```

### U16 1D Tactile Sensors

```python
class TactileSensorDataU16:
    sensor_id: Finger
    data: List[int]

def get_num_of_tactile_sensors(self) -> int: ...
def get_num_of_tactile_points(self, finger_index: int) -> int: ...
def init_tactile_points_map(self) -> bool: ...
def get_len_of_tactile_datum(self, finger_index: int) -> int: ...
def get_num_of_replied_tactile_frames(self, finger_index: int) -> int: ...
def get_sn_of_tactile_sensor(self, finger_index: int) -> str: ...
def get_sensor_data_length(self, finger_index: int) -> int: ...  # deprecated alias
def get_sensor_order(self) -> List[Finger]: ...
def get_tactile_sensor_data(
    self, finger_index: int
) -> TactileSensorDataU16: ...
def get_tactile_sensor_data_raw(
    self, finger_index: int
) -> TactileSensorDataU16: ...
def get_all_tactile_sensor_data_raw(self) -> List[TactileSensorDataU16]: ...
```

The sensor order is thumb, index, middle, ring, little, and palm. Call
`init_tactile_points_map()` before relying on cached point counts.

### Communication Settings

```python
def set_request_interval(self, milliseconds: int) -> None: ...
def get_request_interval(self) -> int: ...
def set_frame_recv_timeout(self, milliseconds: int) -> None: ...
def get_frame_recv_timeout(self) -> int: ...
def set_frame_send_timeout(self, milliseconds: int) -> None: ...
def get_frame_send_timeout(self) -> int: ...
```

Frame-send timeout is a CAN transport setting. The RS485 implementation treats
the setter as a no-op and returns `0` from the getter.

### Debugging

```python
def show_data_details(self, show: bool, callback: Callable[[str], None] | None = None) -> None:
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

# Set motor positions (4 joints, 0-4096)
positions = [2048, 2048, 2048, 2048]
echo = hand.set_all_joint_positions(positions)
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
