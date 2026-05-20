# OmniHand Pro 2025 (O12) Python API

## Overview

**OmniHand Pro 2025 (O12)** is a 12 DOF dexterous hand with 3D tactile sensors. This document describes the Python API for controlling and interacting with OmniHand Pro 2025 devices.

**Key Features:**
- 12 active degrees of freedom
- 3D tactile sensors (fingers + palm, not dorsum)
- Motor position range: 0-2000
- Supports CAN (ZLG USB CANFD) communication only
- Supports SocketCAN (Linux only)
- Supports temperature and current report period settings

## Import

```python
from omnihand import OmniHandPro2025, HandType, Finger, ControlMode
```

## Enumerations

### HandType

```python
class HandType(IntEnum):
    LEFT = 0              # Left hand
    RIGHT = 1             # Right hand
    UNKNOWN = 255  # Unknown hand type
```

### Finger

```python
class Finger(IntEnum):
    THUMB = 1
    INDEX = 2
    MIDDLE = 3
    RING = 4
    LITTLE = 5
    PALM = 6      # Not supported by O12
    DORSUM = 7    # Not supported by O12
    UNKNOWN = 255
```

**Note**: O12 supports fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM), not dorsum.

### ControlMode

```python
class ControlMode(IntEnum):
    POSITIONTION = 0
    SERVO = 1            # Servo mode
    VELOCITYCITY = 2
    TORQUE = 3           # Not supported (use mixed modes instead)
    POSITIONTION_TORQUE = 4  # Mixed control
    VELOCITYCITY_TORQUE = 5  # Mixed control
    POSITIONTION_VELOCITYCITY_TORQUE = 6  # Mixed control
    UNKNOWN = 10
```

**Note**: 
- **SERVO mode**: Servo control mode
- **Pure torque control (TORQUE)**: Not supported. Use mixed control modes instead

## Data Structures

### VendorInfo

```python
class VendorInfo:
    product_model: str
    product_seq_num: str
    hardware_version: Version
    software_version: Version
    voltage: int
    dof: int  # 12 for O12

    def __str__(self) -> str: ...
```

### DeviceInfo

```python
class DeviceInfo:
    hand_device_id: int
    commu_params: CommuParams

    def __str__(self) -> str: ...
```

### TactileSensor3DData

```python
class TactileSensor3DData:
    online_state: int             # 1=online, 0=offline
    channel_values: List[int]      # Six native channel values
    normal_force: int              # Normal force (0-3000, unit: 0.1N)
    tangent_force: int             # Tangent force
    tangent_force_angle: int       # Tangent force angle (0-359 degrees)
    capacitive_approach: List[int] # 4 channels
```

## Factory Methods

### Recommended: ZLG USB CANFD (Zero Configuration)

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """Creates a dexterous hand object (Recommended: Zero configuration).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: Hand device ID, defaults to 1.
        canfd_device_id: USB CANFD adapter device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandPro2025: Dexterous hand instance.
    """
```

**Example:**
```python
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

### Factory Method by Serial Number

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType,
                hand_device_id: int,
                usbcanfd_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """Creates a dexterous hand object by serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: Hand device ID.
        serial_number: USB CANFD device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandPro2025: Dexterous hand instance, or None if device not found.
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """Creates a dexterous hand object via HCAN USB CANFD (by device ID).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: Hand device ID, defaults to 1.
        canfd_device_id: HCAN device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandPro2025: Dexterous hand instance.
    """
```

**Example:**
```python
hand = OmniHandPro2025.create_hand_by_hcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_canfd_channel_id=0
)
```

### Factory Method by HCAN Serial Number

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType,
                hand_device_id: int,
                hcan_usbcanfd_serial_number: str,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """Creates a dexterous hand object via HCAN USB CANFD (by serial number).

    Args:
        hand_type: The hand type.
        hand_device_id: Hand device ID.
        hcan_serial_number: HCAN device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandPro2025: Dexterous hand instance, or None if device not found.
    """
```

### Advanced: SocketCAN (Linux Only)

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """Creates a dexterous hand object (Recommended: Zero configuration).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        canfd_device_id: USB CANFD adapter device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandPro2025: Dexterous hand instance.
    
    Note:
        ✅ Recommended: Zero configuration, ready to use out of the box. 
        No root privileges required.
    """
```

**Example:**
```python
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

### Factory Method by Serial Number

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType,
                hand_device_id: int,
                usbcanfd_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """Creates a dexterous hand object by serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        usbcanfd_serial_number: USB CANFD device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHandPro2025: Dexterous hand instance, or None if device not found.
    """
```

### Advanced: SocketCAN (Linux Only)

```python
@staticmethod
def create_hand_socketcan(hand_type: HandType = HandType.LEFT,
                          hand_device_id: int = 1,
                          can_interface: str = "can0") -> 'OmniHandPro2025':
    """Creates a dexterous hand object using SocketCAN (Linux only, advanced usage).
    
    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        can_interface: CAN interface name (e.g., "can0", "can1").
    
    Returns:
        OmniHandPro2025: Dexterous hand instance.
    
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
        int: ProductType.OMNIHAND_PRO_2025
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

## Joint Angle Control

## Motor Position Control

**Note**: OmniHand Pro 2025 (O12) motor position range is **0-2000** (different from O10 which is 0-4096).

```python
def set_joint_position(self, joint_motor_index: int, position: int) -> None:
    """Sets the position of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        position: Target position (range: 0-2000).
    """

def get_joint_position(self, joint_motor_index: int) -> int:
    """Gets the position of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        int: Current position (range: 0-2000).
    """

def set_all_joint_positions(self, positions: List[int]) -> List[int]:
    """Sets the positions of all joint motors in batch and returns the actual positions.
    
    Args:
        positions: List of target positions. Must have 12 values, each in range 0-2000.
    
    Returns:
        List of actual positions from device response. Empty list on failure.
    """

def get_all_joint_positions(self) -> List[int]:
    """Gets the positions of all joint motors in batch.
    
    Returns:
        List[int]: List of current positions. Returns 12 values, each in range 0-2000.
    """
```

## Joint Angle Control

### Joint Angle I/O Order (Right Hand)

| Index | Joint Name (URDF)       | Min Angle (rad)      | Max Angle (rad)     | Min Angle (°) | Max Angle (°) | Velocity Limit (rad/s) |
| ----- | ----------------------- | -------------------- | ------------------- | ------------ | ------------- | ---------------------- |
| 1     | `R_thumb_roll_joint`   | 0.0                  | 0.9424777960769379  | 0.0          | 54.0          | 2.38                   |
| 2     | `R_thumb_abad_joint`   | -1.387536755335492   | 0.0                 | -79.5        | 0.0           | 2.33                   |
| 3     | `R_thumb_mcp_joint`    | -0.8272860654453121  | 0.0                 | -47.4        | 0.0           | 1.35                   |
| 4     | `R_thumb_pip_joint`    | -1.2915436464758039  | 0.0                 | -74.0        | 0.0           | 1.87                   |
| 5     | `R_index_abad_joint`   | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 6     | `R_index_mcp_joint`    | 0.0                  | 1.3526301702956054  | 0.0          | 77.5          | 2.22                   |
| 7     | `R_index_pip_joint`    | 0.0                  | 1.530653753999027   | 0.0          | 87.7          | 2.49                   |
| 8     | `R_middle_abad_joint`  | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 9     | `R_middle_mcp_joint`   | 0.0                  | 1.3578661580515883  | 0.0          | 77.8          | 2.22                   |
| 10    | `R_middle_pip_joint`   | 0.0                  | 1.8151424220741028  | 0.0          | 104.0         | 2.16                   |
| 11    | `R_ring_mcp_joint`     | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |
| 12    | `R_pinky_mcp_joint`    | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |

### Joint Angle I/O Order (Left Hand)

| Index | Joint Name (URDF)       | Min Angle (rad)      | Max Angle (rad)     | Min Angle (°) | Max Angle (°) | Velocity Limit (rad/s) |
| ----- | ----------------------- | -------------------- | ------------------- | ------------ | ------------- | ---------------------- |
| 1     | `L_thumb_roll_joint`    | -0.9424777960769379  | 0.0                 | -54.0        | 0.0           | 2.38                   |
| 2     | `L_thumb_abad_joint`    | 0.0                  | 1.387536755335492   | 0.0          | 79.5          | 2.33                   |
| 3     | `L_thumb_mcp_joint`     | -0.8272860654453121  | 0.0                 | -47.4        | 0.0           | 1.35                   |
| 4     | `L_thumb_pip_joint`     | -1.2915436464758039  | 0.0                 | -74.0        | 0.0           | 1.87                   |
| 5     | `L_index_abad_joint`    | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 6     | `L_index_mcp_joint`     | 0.0                  | 1.3526301702956054  | 0.0          | 77.5          | 2.22                   |
| 7     | `L_index_pip_joint`     | 0.0                  | 1.530653753999027   | 0.0          | 87.7          | 2.49                   |
| 8     | `L_middle_abad_joint`   | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 9     | `L_middle_mcp_joint`    | 0.0                  | 1.3578661580515883  | 0.0          | 77.8          | 2.22                   |
| 10    | `L_middle_pip_joint`    | 0.0                  | 1.8151424220741028  | 0.0          | 104.0         | 2.16                   |
| 11    | `L_ring_mcp_joint`      | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |
| 12    | `L_pinky_mcp_joint`     | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |

**Note**: The left hand is generated via xacro mirroring, with the same joint limits as the right hand, only the joint name prefix changes from `R_` to `L_`.

```python
def set_all_active_joint_angles(self, angles: List[float]) -> None:
    """Sets the angles of all active joints (in radians).
    
    Args:
        angles: List of joint angles in radians. Must have 12 values.
                The order follows the table above (indices 1-12).
    """

def get_all_active_joint_angles(self) -> List[float]:
    """Gets the angles of all active joints (in radians).
    
    Returns:
        List[float]: List of joint angles in radians. Returns 12 values.
                    The order follows the table above (indices 1-12).
    """

def get_all_joint_angles(self) -> List[float]:
    """Gets the angles of all joints (active and passive, in radians).
    
    Returns:
        List[float]: List of all joint angles in radians. Returns 19 values (12 active + 7 passive).
                    The first 12 values are active joints (order follows the table above), followed by 7 passive joints.
    """
```

## Velocity Control

```python
def set_joint_velocity(self, joint_motor_index: int, velocity: int) -> None:
    """Sets the velocity of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        velocity: Target velocity.
    """

def get_joint_velocity(self, joint_motor_index: int) -> int:
    """Gets the velocity of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        int: Current velocity.
    """

def set_all_joint_velocities(self, velocities: List[int]) -> None:
    """Sets the velocities of all joint motors in batch.
    
    Args:
        velocities: List of target velocities. Must have 12 values.
    """

def get_all_joint_velocities(self) -> List[int]:
    """Gets the velocities of all joint motors in batch.
    
    Returns:
        List[int]: List of current velocities. Returns 12 values.
    """
```

## Tactile Sensor Data

OmniHand Pro 2025 (O12) uses **3D tactile sensors** with the following characteristics:
- **Sensor locations**: Fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM)
- **Not supported**: Dorsum sensor
- **Data structure**: TactileSensor3DData with normal force, tangent force, tangent force angle, etc.

```python
def get_tactile_sensor_3d_data(self, eFinger: Finger) -> TactileSensor3DData:
    """Gets 3D tactile sensor data for the specified finger (O12 only).
    
    Args:
        eFinger: Finger/palm enum value (O12 supports fingers + palm, not dorsum).
    
    Returns:
        TactileSensor3DData: 3D tactile sensor data structure containing:
                           - online_state: Sensor online status
                           - channel_values: Six native sampled channel values
                           - normal_force: Normal force (0-3000, unit: 0.1N)
                           - tangent_force: Tangent force
                           - tangent_force_angle: Tangent force angle (0-359 degrees)
                           - capacitive_approach: Capacitive approach values (4 channels)
    
    Note:
        O12 supports fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM), not dorsum.
    """
```

## Control Mode

O12 supports switching control modes via `set_control_mode`. The following 5 control modes are supported:

| Mode Enum | Value | Description |
|---|---|---|
| `ControlMode.POSITION` | 0 | Position control (default) |
| `ControlMode.SERVO` | 1 | Servo control mode |
| `ControlMode.VELOCITY` | 2 | Velocity control mode |
| `ControlMode.TORQUE` | 3 | Torque control mode |
| `ControlMode.POSITION_TORQUE` | 4 | Position + force mixed control (via `mix_ctrl_joint_motor`, force unit: 0.01 N) |

**Note**: In mixed control, `tgt_torque` is in **0.01 N**, correlated with tactile sensor normal force. Pure torque control (TORQUE) can be set via `set_control_mode`, but mixed control modes (POSITION_TORQUE, VELOCITY_TORQUE, POSITION_VELOCITY_TORQUE) should be used through `mix_ctrl_joint_motor`.

```python
def set_control_mode(self, joint_motor_index: int, mode: int) -> None:
    """Sets the control mode of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        mode: Control mode (see ControlMode).
    
    Note:
        - All control modes are supported
        - Pure TORQUE mode is not supported
    """

def get_control_mode(self, joint_motor_index: int) -> int:
    """Gets the control mode of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        int: Current control mode (see ControlMode).
    """

def set_all_control_modes(self, ctrl_modes: List[int]) -> None:
    """Sets the control modes of all joint motors in batch.
    
    Args:
        ctrl_modes: List of control modes. Must have 12 values.
    """

def get_all_control_modes(self) -> List[int]:
    """Gets the control modes of all joint motors in batch.
    
    Returns:
        List[int]: List of control modes. Returns 12 values.
    """
```

## Current Threshold Control

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None:
    """Sets the current threshold of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        current_threshold: Current threshold value.
    """

def get_current_threshold(self, joint_motor_index: int) -> int:
    """Gets the current threshold of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        int: Current threshold value.
    """

def set_all_current_thresholds(self, current_thresholds: List[int]) -> None:
    """Sets the current thresholds of all joint motors in batch.
    
    Args:
        current_thresholds: List of current thresholds. Must have 12 values.
    """

def get_all_current_thresholds(self) -> List[int]:
    """Gets the current thresholds of all joint motors in batch.
    
    Returns:
        List[int]: List of current thresholds. Returns 12 values.
    """
```

## Mixed Control

```python
def mix_ctrl_joint_motor(self, mix_ctrls: List[MixCtrl]) -> None:
    """Controls joint motors in mixed mode.
    
    Args:
        mix_ctrls: List of mixed control parameters.
    
    Note:
        Pure torque control (TORQUE) is not supported. Use mixed control modes instead.
    """
```

## Error Handling

```python
def get_error_report(self, joint_motor_index: int) -> JointMotorErrorReport:
    """Gets the error report for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        JointMotorErrorReport: Error report structure.
    """

def get_all_error_reports(self) -> List[JointMotorErrorReport]:
    """Gets the error reports for all joint motors.
    
    Returns:
        List[JointMotorErrorReport]: List of error reports. Returns 12 values.
    """

def set_error_report_period(self, joint_motor_index: int, period: int) -> None:
    """Sets the error report period for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        period: Reporting period in milliseconds.
    """

def set_all_error_report_periods(self, periods: List[int]) -> None:
    """Sets the error report periods for all joint motors.
    
    Args:
        periods: List of reporting periods. Must have 12 values.
    """
```

## Temperature Monitoring

```python
def get_temperature_report(self, joint_motor_index: int) -> int:
    """Gets the temperature report for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        int: Current temperature value in degrees Celsius.
    """

def get_all_temperature_reports(self) -> List[int]:
    """Gets the temperature reports for all joint motors.
    
    Returns:
        List[int]: List of temperature values. Returns 12 values.
    """

def set_temperature_report_period(self, joint_motor_index: int, period: int) -> None:
    """Sets the temperature report period for a single joint motor (O12 only).
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        period: Reporting period in milliseconds.
    """

def set_all_temperature_report_periods(self, periods: List[int]) -> None:
    """Sets the temperature report periods for all joint motors (O12 only).
    
    Args:
        periods: List of reporting periods. Must have 12 values.
    """
```

## Current Monitoring

```python
def get_current_report(self, joint_motor_index: int) -> int:
    """Gets the current report for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-12).
    
    Returns:
        int: Current value.
    """

def get_all_current_reports(self) -> List[int]:
    """Gets the current reports for all joint motors.
    
    Returns:
        List[int]: List of current values. Returns 12 values.
    """

def set_current_report_period(self, joint_motor_index: int, period: int) -> None:
    """Sets the current report period for a single joint motor (O12 only).
    
    Args:
        joint_motor_index: Joint motor index (1-12).
        period: Reporting period in milliseconds.
    """

def set_all_current_report_periods(self, periods: List[int]) -> None:
    """Sets the current report periods for all joint motors (O12 only).
    
    Args:
        periods: List of reporting periods. Must have 12 values.
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

## Complete Example

```python
from omnihand import OmniHandPro2025, HandType, Finger

# Create hand instance
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("Failed to initialize OmniHand Pro 2025")
    exit(1)

# Get vendor information
vendor = hand.get_vendor_info()
print(vendor)

# Set joint angles
angles = [0.3, -0.5, -0.3, -0.5, 0.0, 0.6, 0.7, 0.0, 0.6, 0.7, 0.7, 0.7]  # 12 joint angles (in radians)
hand.set_all_active_joint_angles(angles)
print(f"Set joint angles: {angles} (rad)")

# Get 3D tactile sensor data
thumb_data = hand.get_tactile_sensor_3d_data(Finger.THUMB)
print(f"Thumb normal force: {thumb_data.normal_force} (0.1N)")
```

## Demo Files

The SDK release package includes ready-to-run Python demo scripts (under `python/demo/omnihand_pro_2025/`):

| Demo | File |
|------|------|
| CAN FD (connect by ID) | [demo_canfd_id.py](../../../python/demo/omnihand_pro_2025/demo_canfd_id.py) |
| CAN FD (connect by serial) | [demo_canfd_serial.py](../../../python/demo/omnihand_pro_2025/demo_canfd_serial.py) |
| SocketCAN | [demo_socketcan.py](../../../python/demo/omnihand_pro_2025/demo_socketcan.py) |
| ZLG CAN TCP | [demo_zlgcan_tcp.py](../../../python/demo/omnihand_pro_2025/demo_zlgcan_tcp.py) |
| Position + Torque mixed control | [demo_mix_ctrl_pos_torque.py](../../../python/demo/omnihand_pro_2025/demo_mix_ctrl_pos_torque.py) |
| Position control | [demo_set_position.py](../../../python/demo/omnihand_pro_2025/demo_set_position.py) |
| Velocity control | [demo_set_velocity.py](../../../python/demo/omnihand_pro_2025/demo_set_velocity.py) |
| Angle control | [demo_set_angle.py](../../../python/demo/omnihand_pro_2025/demo_set_angle.py) |
| Current monitoring | [demo_monitor_current.py](../../../python/demo/omnihand_pro_2025/demo_monitor_current.py) |
| Temperature monitoring | [demo_monitor_temperature.py](../../../python/demo/omnihand_pro_2025/demo_monitor_temperature.py) |
| Error monitoring | [demo_monitor_error.py](../../../python/demo/omnihand_pro_2025/demo_monitor_error.py) |
| Tactile sensor | [demo_sensor_touch.py](../../../python/demo/omnihand_pro_2025/demo_sensor_touch.py) |
| Hardware info | [demo_get_hardware_info.py](../../../python/demo/omnihand_pro_2025/demo_get_hardware_info.py) |

## Related Documentation

- [OmniHand Pro 2025 (O12) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O12.md) - For kinematics calculations
- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
