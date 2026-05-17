# OmniHand Pro 2025 (O12) Kinematics Solver C++ API

## Introduction

The **OmniHandPro2025Solver** class (namespace: `agilink::omnihand::omnihand::o12`) provides control functionalities for the OmniHand Pro 2025 (O12) robotic hand, including gesture setting and joint position conversion.

## Include Header

```cpp
#include "kinematics/omnihand_pro_2025/omnihand_pro_2025_solver.h"
```

## Constructor

```cpp
agilink::omnihand::omnihand::o12::OmniHandPro2025Solver(const bool& hand_type);
```

- **`hand_type`**: Set to `true` for a left-hand and `false` for a right-hand.

## Methods

### 1. CheckJointPos

```cpp
bool CheckJointPos(std::vector<double> &active_joint_angles);
```

- Checks if the number of active joints is reasonable. If not, returns `false`.
- Checks if the positions of active joints are reasonable. If not, modifies them.

### 2. SetHandGesture

```cpp
std::vector<int> SetHandGesture(OmniHandPro2025Gesture gesture);
```

- Sets a predefined hand gesture.
- Returns the corresponding actuator input values.

| Gesture Enum                                      | Gesture Name |
|---------------------------------------------------|--------------|
| OmniHandPro2025Gesture::OMNIHAND_PRO_2025_GESTURE_ALL_ZERO | ALL_ZERO     |
| OmniHandPro2025Gesture::OMNIHAND_PRO_2025_GESTURE_PAPER    | PAPER        |
| OmniHandPro2025Gesture::OMNIHAND_PRO_2025_GESTURE_FIST     | FIST         |
| OmniHandPro2025Gesture::OMNIHAND_PRO_2025_GESTURE_OK       | OK           |
| OmniHandPro2025Gesture::OMNIHAND_PRO_2025_GESTURE_PACK     | PACK         |

### 3. ConvertJoint2Actuator

```cpp
std::vector<int> ConvertJoint2Actuator(const std::vector<double>& active_joint_angles);
```

- Converts active joint positions into actuator input values.
- **Note**: O12 motor input range is 0-2000, different from O10 which is 0-4096.

### 4. ConvertActuator2Joint

```cpp
std::vector<double> ConvertActuator2Joint(const std::vector<int>& actuator_input);
```

- Converts actuator input values back into active joint positions.
- **Note**: O12 motor input range is 0-2000, different from O10 which is 0-4096.

### 5. ActiveJoint2MotorLength

```cpp
std::vector<double> ActiveJoint2MotorLength(const std::vector<double> &active_joint_angles);
```

- Converts active joint positions into motor length.

### 6. MotorLength2ActiveJoint

```cpp
std::vector<double> MotorLength2ActiveJoint(const std::vector<double> &motor_length);
```

- Converts motor length into active joint positions.

### 7. MotorLength2MotorInput

```cpp
std::vector<int> MotorLength2MotorInput(const std::vector<double> &motor_length);
```

- Converts the motor length to motor input value.

### 8. MotorInput2MotorLength

```cpp
std::vector<double> MotorInput2MotorLength(const std::vector<int> &motor_input);
```

- Converts the motor input value to motor length.

### 9. GetAllJointAngles

```cpp
std::vector<double> GetAllJointAngles(const std::vector<double>& active_joint_angles);
```

- Retrieves the angles of all joints, including passive joints, based on the given active joint positions.

## Code Example

```cpp
#include "kinematics/omnihand_pro_2025/omnihand_pro_2025_solver.h"

// Create solver for right hand
agilink::omnihand::omnihand::o12::OmniHandPro2025Solver solver(false);

// Check and validate joint positions
std::vector<double> active_joint_angles = {0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.5, 0.3};
bool valid = solver.CheckJointPos(active_joint_angles);

// Set a gesture
std::vector<int> actuator_input = solver.SetHandGesture(OmniHandPro2025Gesture::OMNIHAND_PRO_2025_GESTURE_PAPER); // PAPER gesture

// Convert joint positions to actuator input
std::vector<int> input = solver.ConvertJoint2Actuator(active_joint_angles);

// Get all joint positions (including passive joints)
std::vector<double> all_joints = solver.GetAllJointAngles(active_joint_angles);
```

## Related Documentation

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md) - Main C++ API documentation
- [OmniHand Pro 2025 (O12) Kinematics Solver Python API](API_KINEMATICS_PYTHON_O12.md) - Python version of this API
