# OmniHand 2025 (O10) Python API

## Overview

**OmniHand 2025 (O10)** is a 10 DOF dexterous hand with 1D tactile sensors. This document describes the Python API for controlling and interacting with OmniHand 2025 devices.

**Key Features:**
- 10 active + 6 passive degrees of freedom
- 1D tactile sensors (fingers, palm, dorsum)
- Motor position range: 0-4096
- Supports CAN (ZLG USB CANFD) and RS485 communication
- Supports SocketCAN (Linux only)

## Import

```python
from omnihand import OmniHand2025, HandType, Finger, ControlMode
```

**Note:** Factory arguments `hand_device_id`, `canfd_device_id`, and `canfd_channel_id` are exposed as Python `int` values; the extension passes them to C++ as `uint8_t`, consistent with `omnihand_2025.h`.

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
    PALM = 6
    DORSUM = 7
    UNKNOWN = 255
```

### ControlMode

```python
class ControlMode(IntEnum):
    POSITIONTION = 0         # Position mode
    SERVO = 1            # Servo mode
    VELOCITYCITY = 2         # Velocity mode
    TORQUE = 3           # Torque mode (pure torque control not supported)
    POSITIONTION_TORQUE = 4  # Position-Torque mixed mode
    VELOCITYCITY_TORQUE = 5  # Velocity-Torque mixed mode
    POSITIONTION_VELOCITYCITY_TORQUE = 6 # Position-Velocity-Torque mixed mode
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
    dof: int

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
    sensor_id: int  # Finger enum value
    data: List[int]  # Raw sensor data (unit: 1g, max: 255g)
```

## Factory Methods

### Recommended: ZLG USB CANFD (Zero Configuration)

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """Creates a dexterous hand object (Recommended: Zero configuration).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        canfd_device_id: USB CANFD adapter device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHand2025: Dexterous hand instance.
    
    Note:
        ✅ Recommended: Zero configuration, ready to use out of the box. 
        No root privileges required.
    """
```

**Example:**
```python
hand = OmniHand2025.create_hand_by_zlgcan(
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
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """Creates a dexterous hand object by serial number.

    Args:
        hand_type: The hand type.
        hand_device_id: The hand device ID.
        usbcanfd_serial_number: USB CANFD device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHand2025: Dexterous hand instance, or None if device not found.
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """Creates a dexterous hand object via HCAN USB CANFD (by device ID).

    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: Hand device ID, defaults to 1.
        canfd_device_id: HCAN device index, defaults to 0.
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHand2025: Dexterous hand instance.
    """
```

**Example:**
```python
hand = OmniHand2025.create_hand_by_hcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

### Factory Method by HCAN Serial Number

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType,
                hand_device_id: int,
                hcan_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """Creates a dexterous hand object via HCAN USB CANFD (by serial number).

    Args:
        hand_type: The hand type.
        hand_device_id: Hand device ID.
        hcan_serial_number: HCAN device serial number (supports partial matching).
        canfd_channel_id: CAN channel index, defaults to 0. For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0.
    
    Returns:
        OmniHand2025: Dexterous hand instance, or None if device not found.
    """
```

### RS485 Communication (O10 Only)

```python
@staticmethod
def create_hand_by_rs485(hand_type: HandType,
                      hand_device_id: int,
                      uart_port: str,
                      baudrate: int = 460800) -> 'OmniHand2025':
    """Creates a dexterous hand object using RS485 communication (O10 only).

    Args:
        hand_type: The hand type.
        hand_device_id: Hand device ID.
        uart_port: Serial port path (e.g., "/dev/ttyUSB0").
        baudrate: Baud rate, defaults to 460800.
    
    Returns:
        OmniHand2025: Dexterous hand instance.
    """
```

### Advanced: SocketCAN (Linux Only)

```python
@staticmethod
def create_hand_socketcan(hand_type: HandType = HandType.LEFT,
                          hand_device_id: int = 1,
                          can_interface: str = "can0") -> 'OmniHand2025':
    """Creates a dexterous hand object using SocketCAN (Linux only, advanced usage).
    
    Args:
        hand_type: The hand type, defaults to the left hand.
        hand_device_id: The hand device ID, defaults to 1.
        can_interface: CAN interface name (e.g., "can0", "can1").
    
    Returns:
        OmniHand2025: Dexterous hand instance.
    
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
        int: ProductType.OMNIHAND_2025
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
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def set_device_id(self, device_id: int) -> None:
    """Sets the device ID.
    
    Args:
        hand_device_id: The hand device ID.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """
```

## Joint Angle Control

## Motor Position Control

**Note**: OmniHand 2025 (O10) motor position range is **0-4096**.

```python
def set_joint_position(self, joint_motor_index: int, position: int) -> None:
    """Sets the position of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
        position: Target position (range: 0-4096).
    """

def get_joint_position(self, joint_motor_index: int) -> int:
    """Gets the position of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        int: Current position (range: 0-4096).
    """

def set_all_joint_positions(self, positions: List[int]) -> List[int]:
    """Sets the positions of all joint motors in batch and returns the actual positions.
    
    Args:
        positions: List of target positions. Must have 10 values, each in range 0-4096.
    
    Returns:
        List of actual positions from device response. Empty list on failure.
    """

def get_all_joint_positions(self) -> List[int]:
    """Gets the positions of all joint motors in batch.
    
    Returns:
        List[int]: List of current positions. Returns 10 values, each in range 0-4096.
    """
```

## Joint Angle Control

### Joint Angle I/O Order (Right Hand)

| Index | Joint Name         | Min Angle (rad) | Max Angle (rad) | Min Angle (°) | Max Angle (°) | Velocity Limit (rad/s) |
| ----- | ------------------ | --------------- | --------------- | ------------- | ------------- | ---------------------- |
| 1     | R_thumb_roll_joint | -0.03           | 1.12            | -2            | 64            | 0.164                  |
| 2     | R_thumb_abad_joint | -1.64           | 0.05            | -94           | 3             | 0.164                  |
| 3     | R_thumb_mcp_joint  | 0               | 0.84            | 0             | 48            | 0.308                  |
| 4     | R_index_abad_joint | -0.16           | 0               | -9            | 0             | 0.164                  |
| 5     | R_index_pip_joint  | 0               | 1.48            | 0             | 85            | 0.308                  |
| 6     | R_middle_pip_joint | 0               | 1.48            | 0             | 85            | 0.308                  |
| 7     | R_ring_abad_joint  | 0               | 0.17            | 0             | 10            | 0.164                  |
| 8     | R_ring_pip_joint   | 0               | 1.48            | 0             | 85            | 0.308                  |
| 9     | R_pinky_abad_joint | 0               | 0.19            | 0             | 11            | 0.164                  |
| 10    | R_pinky_pip_joint  | 0               | 1.48            | 0             | 85            | 0.308                  |

### Joint Angle I/O Order (Left Hand)

| Index | Joint Name         | Min Angle (rad) | Max Angle (rad) | Min Angle (°) | Max Angle (°) | Velocity Limit (rad/s) |
| ----- | ------------------ | --------------- | --------------- | ------------- | ------------- | ---------------------- |
| 1     | L_thumb_roll_joint | -1.12           | 0.03            | -64           | 2             | 0.164                  |
| 2     | L_thumb_abad_joint | -0.05           | 1.64            | -3            | 94            | 0.164                  |
| 3     | L_thumb_mcp_joint  | -0.84           | 0               | -48           | 0             | 0.308                  |
| 4     | L_index_abad_joint | 0               | 0.16            | 0             | 9             | 0.164                  |
| 5     | L_index_pip_joint  | 0               | 1.48            | 0             | 85            | 0.308                  |
| 6     | L_middle_pip_joint | 0               | 1.48            | 0             | 85            | 0.308                  |
| 7     | L_ring_abad_joint  | -0.17           | 0               | -10           | 0             | 0.164                  |
| 8     | L_ring_pip_joint   | 0               | 1.48            | 0             | 85            | 0.308                  |
| 9     | L_pinky_abad_joint | -0.19           | 0               | -11           | 0             | 0.164                  |
| 10    | L_pinky_pip_joint  | 0               | 1.48            | 0             | 85            | 0.308                  |

```python
def set_all_active_joint_angles(self, angles: List[float]) -> None:
    """Sets the angles of all active joints (in radians).
    
    Args:
        angles: List of joint angles in radians. Must have 10 values.
                The order follows the table above (indices 1-10).
    
    Note:
        For specific order and limits, please refer to the table above.
    """

def get_all_active_joint_angles(self) -> List[float]:
    """Gets the angles of all active joints (in radians).
    
    Returns:
        List[float]: List of joint angles in radians. Returns 10 values.
                    The order follows the table above (indices 1-10).
    
    Note:
        For specific order and limits, please refer to the table above.
    """

def get_all_joint_angles(self) -> List[float]:
    """Gets the angles of all joints (active and passive, in radians).
    
    Returns:
        List[float]: List of all joint angles in radians. Returns 16 values (10 active + 6 passive).
                    The first 10 values are active joints (order follows the table above), followed by 6 passive joints.
    
    Note:
        For passive joint order and limits, please refer to the assets model files.
    """
```

## Velocity Control

```python
def set_joint_velocity(self, joint_motor_index: int, velocity: int) -> None:
    """Sets the velocity of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
        velocity: Target velocity.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def get_joint_velocity(self, joint_motor_index: int) -> int:
    """Gets the velocity of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        int: Current velocity.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def set_all_joint_velocities(self, velocities: List[int]) -> None:
    """Sets the velocities of all joint motors in batch.
    
    Args:
        velocities: List of target velocities. Must have 10 values.
    """

def get_all_joint_velocities(self) -> List[int]:
    """Gets the velocities of all joint motors in batch.
    
    Returns:
        List[int]: List of current velocities. Returns 10 values.
    """
```

## Tactile Sensor Data

OmniHand 2025 (O10) uses **1D tactile sensors** with the following characteristics:
- **Data unit**: 1g
- **Max value**: 255g
- **Sampling frequency**: 10Hz
- **Sensor locations**: Fingers (16 points each), Palm (78 points), Dorsum (102 points)

```python
def get_tactile_sensor_data(self, eFinger: Finger) -> List[int]:
    """Gets the tactile sensor data for a specified part (O10 only).
    
    Args:
        eFinger: Finger/palm enum value.
    
    Returns:
        List[int]: List of tactile sensor data for the specified part.
                   - Fingers: Returns 16 data points, one per sensor point
                   - Palm: Returns 25 data points, one per 3 sensor points (downsampled)
                   - Dorsum: Returns 25 data points, one per 4 sensor points (downsampled)
    
    Note:
        Data unit: 1g, Max value: 255g, Sampling frequency: 10Hz
    """

def get_all_tactile_sensor_data_raw(self) -> List[TactileSensorData]:
    """Gets all 1D tactile sensor raw data from all sensors at once.
    
    Returns:
        List[TactileSensorData]: List of TactileSensorData structures.
    
    Note:
        This returns full resolution data, unlike get_tactile_sensor_data() which returns downsampled data.
    """

def get_tactile_sensor_data_raw(self, eFinger: Finger) -> TactileSensorData:
    """Gets 1D tactile sensor raw data for a single sensor.
    
    Args:
        eFinger: Finger/palm enum value.
    
    Returns:
        TactileSensorData: TactileSensorData structure containing full resolution data.
    """
```

**⚠️ Important Recommendation: When retrieving data from multiple sensors, strongly prefer using `get_all_tactile_sensor_data_raw()` over looping `get_tactile_sensor_data_raw()`.**

The following table compares the differences between the two approaches:

| Aspect | Loop `get_tactile_sensor_data_raw()` (7 times) | Use `get_all_tactile_sensor_data_raw()` (1 time) |
|--------|------------------------------------------------|--------------------------------------------------|
| **Request Interval Accumulation** | 7 × interval_ms (e.g., 7 × 3ms = 21ms) | 1 × interval_ms (e.g., 1 × 3ms = 3ms) |
| **Independent Timeout Checks** | 7 times (each request has independent timeout risk) | 1 time (multi-frame reception within single request) |
| **CAN Bus Occupancy** | 7 requests + 7 responses = 14 frame transmissions | 1 request + 5 responses = 6 frame transmissions |
| **Communication Overhead** | High (14 frame transmissions) | Low (6 frame transmissions) |
| **Timeout Risk Accumulation** | High (7 independent timeout risks stacked) | Low (1 request, multi-frame reception) |
| **Device Processing Load** | High (7 independent processing requests) | Low (1 batch processing) |
| **Total Time Cost** | Long (accumulated request intervals + multiple communications) | Short (single request + multi-frame reception) |

**💡 Recommendation: Always prefer `get_all_tactile_sensor_data_raw()` when retrieving data from multiple sensors for better performance and reliability.**

@staticmethod
def get_sensor_data_length(finger_index: int) -> int:
    """Get sensor data length for a specific finger (static method).
    
    Args:
        finger_index: Finger enum value (Finger).
    
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

## Control Mode

```python
def set_control_mode(self, joint_motor_index: int, mode: int) -> None:
    """Sets the control mode of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
        mode: Control mode (see ControlMode).
    
    Note:
        - SERVO mode is O10-only
        - Pure TORQUE mode is not supported
    """

def get_control_mode(self, joint_motor_index: int) -> int:
    """Gets the control mode of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        int: Current control mode (see ControlMode).
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def set_all_control_modes(self, ctrl_modes: List[int]) -> None:
    """Sets the control modes of all joint motors in batch.
    
    Args:
        ctrl_modes: List of control modes. Must have 10 values.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def get_all_control_modes(self) -> List[int]:
    """Gets the control modes of all joint motors in batch.
    
    Returns:
        List[int]: List of control modes. Returns 10 values.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """
```

## Current Threshold Control

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None:
    """Sets the current threshold of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
        current_threshold: Current threshold value.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def get_current_threshold(self, joint_motor_index: int) -> int:
    """Gets the current threshold of a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        int: Current threshold value.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def set_all_current_thresholds(self, current_thresholds: List[int]) -> None:
    """Sets the current thresholds of all joint motors in batch.
    
    Args:
        current_thresholds: List of current thresholds. Must have 10 values.
    
    Note:
        This interface is not supported for serial port communication (RS485).
    """

def get_all_current_thresholds(self) -> List[int]:
    """Gets the current thresholds of all joint motors in batch.
    
    Returns:
        List[int]: List of current thresholds. Returns 10 values.
    
    Note:
        This interface is not supported for serial port communication (RS485).
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
        This interface is not supported for serial port communication (RS485).
    """
```

## Error Handling

```python
def get_error_report(self, joint_motor_index: int) -> JointMotorErrorReport:
    """Gets the error report for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        JointMotorErrorReport: Error report structure.
    """

def get_all_error_reports(self) -> List[JointMotorErrorReport]:
    """Gets the error reports for all joint motors.
    
    Returns:
        List[JointMotorErrorReport]: List of error reports. Returns 10 values.
    """

def set_error_report_period(self, joint_motor_index: int, period: int) -> None:
    """Sets the error report period for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
        period: Reporting period in milliseconds.
    """

def set_all_error_report_periods(self, periods: List[int]) -> None:
    """Sets the error report periods for all joint motors.
    
    Args:
        periods: List of reporting periods. Must have 10 values.
    """
```

## Temperature Monitoring

```python
def get_temperature_report(self, joint_motor_index: int) -> int:
    """Gets the temperature report for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        int: Current temperature value in degrees Celsius.
    """

def get_all_temperature_reports(self) -> List[int]:
    """Gets the temperature reports for all joint motors.
    
    Returns:
        List[int]: List of temperature values. Returns 10 values.
    """
```

**Note**: OmniHand 2025 (O10) does not support setting temperature report periods. This feature is only available for OmniHand Pro 2025 (O12).

## Current Monitoring

```python
def get_current_report(self, joint_motor_index: int) -> int:
    """Gets the current report for a single joint motor.
    
    Args:
        joint_motor_index: Joint motor index (1-10).
    
    Returns:
        int: Current value.
    """

def get_all_current_reports(self) -> List[int]:
    """Gets the current reports for all joint motors.
    
    Returns:
        List[int]: List of current values. Returns 10 values.
    """
```

**Note**: OmniHand 2025 (O10) does not support setting current report periods. This feature is only available for OmniHand Pro 2025 (O12).

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
from omnihand import OmniHand2025, HandType, Finger

# Create hand instance
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("Failed to initialize OmniHand 2025")
    exit(1)

# Get vendor information
vendor = hand.get_vendor_info()
print(vendor)

# Set joint angles
angles = [0.0, 0.0, 0.5, 0.0, 0.8, 0.8, 0.0, 0.8, 0.0, 0.8]  # 10 joint angles (in radians)
hand.set_all_active_joint_angles(angles)
print(f"Set joint angles: {angles} (rad)")

# Get tactile sensor data
thumb_data = hand.get_tactile_sensor_data(Finger.THUMB)
print(f"Thumb sensor data: {len(thumb_data)} points")
```

## Related Documentation

- [OmniHand 2025 (O10) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O10.md) - For kinematics calculations
- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
