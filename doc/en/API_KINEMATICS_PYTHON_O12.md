# OmniHand Pro 2025 (O12) Kinematics Solver Python API

## Introduction

The **OmniHandPro2025Solver** class provides control functionalities for the OmniHand Pro 2025 (O12) robotic hand, including gesture setting and joint position conversion.

## Import

```python
from omnihand.omnihand_pro_2025 import OmniHandPro2025Solver
```

## Constructor

```python
OmniHandPro2025Solver(hand_type: bool)
```

- **`hand_type`**: Set to `True` for a left-hand and `False` for a right-hand.

## Methods

### 1. check_joint_pos

```python
def check_joint_pos(active_joint_angles: List[float]) -> bool
```

- Checks if the number of active joints is reasonable. If not, returns `False`.
- Checks if the positions of active joints are reasonable. If not, modifies them.

### 2. set_hand_gesture

```python
def set_hand_gesture(gesture: OmniHandPro2025Gesture) -> List[int]
```

- Sets a predefined hand gesture.
- Returns the corresponding actuator input values.

| Gesture Enum                                      | Gesture Name |
|---------------------------------------------------|--------------|
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_HOME  | HOME         |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_PAPER | PAPER        |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_FIST  | FIST         |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_OK    | OK           |

### 3. convert_joint_to_actuator

```python
def convert_joint_to_actuator(active_joint_angles: List[float]) -> List[int]
```

- Converts active joint positions into actuator input values.
- **Note**: O12 motor input range is 0-2000, different from O10 which is 0-4096.

### 4. convert_actuator_to_joint

```python
def convert_actuator_to_joint(actuator_input: List[int]) -> List[float]
```

- Converts actuator input values back into active joint positions.
- **Note**: O12 motor input range is 0-2000, different from O10 which is 0-4096.

### 5. active_joint_to_motor_length

```python
def active_joint_to_motor_length(active_joint_angles: List[float]) -> List[float]
```

- Converts active joint positions into motor length.

### 6. motor_length_to_active_joint

```python
def motor_length_to_active_joint(motor_length: List[float]) -> List[float]
```

- Converts motor length into active joint positions.

### 7. motor_length_to_motor_input

```python
def motor_length_to_motor_input(motor_length: List[float]) -> List[int]
```

- Converts the motor length to motor input value.

### 8. motor_input_to_motor_length

```python
def motor_input_to_motor_length(motor_input: List[int]) -> List[float]
```

- Converts the motor input value to motor length.

### 9. get_all_joint_angles

```python
def get_all_joint_angles(active_joint_angles: List[float]) -> List[float]
```

- Retrieves the angles of all joints, including passive joints, based on the given active joint positions.

## Code Example

```python
from omnihand.omnihand_pro_2025 import OmniHandPro2025Solver

# Create solver for right hand
solver = OmniHandPro2025Solver(hand_type=False)

# Check and validate joint positions
active_joint_angles = [0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.5, 0.3]
valid = solver.check_joint_pos(active_joint_angles)

# Set a gesture
actuator_input = solver.set_hand_gesture(OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_PAPER)  # PAPER gesture

# Convert joint positions to actuator input
input_values = solver.convert_joint_to_actuator(active_joint_angles)

# Get all joint positions (including passive joints)
all_joints = solver.get_all_joint_angles(active_joint_angles)
```

## Related Documentation

- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md) - Main Python API documentation
- [OmniHand Pro 2025 (O12) Kinematics Solver C++ API](API_KINEMATICS_CPP_O12.md) - C++ version of this API
