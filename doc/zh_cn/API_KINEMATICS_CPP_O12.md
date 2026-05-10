# OmniHand Pro 2025 (O12) 运动学求解器 C++ API

## 简介

**OmniHandPro2025Solver** 类（命名空间：`agilink::omnihand::o12`）为 OmniHand Pro 2025 (O12) 灵巧手提供控制功能，包括手势设置和关节位置转换。

## 包含头文件

```cpp
#include "kinematics/omnihand_pro_2025/omnihand_pro_2025_solver.h"
```

## 构造函数

```cpp
agilink::o12::OmniHandPro2025Solver(const bool& hand_type);
```

- **`hand_type`**: 设置为 `true` 表示左手，`false` 表示右手。

## 方法

### 1. CheckJointPos

```cpp
bool CheckJointPos(std::vector<double> &active_joint_angles);
```

- 检查主动关节数量是否合理。如果不合理，返回 `false`。
- 检查主动关节位置是否合理。如果不合理，进行修正。

### 2. SetHandGesture

```cpp
std::vector<int> SetHandGesture(const int& gesture);
```

- 设置预定义的手势。
- 返回对应的执行器输入值。

| 手势ID | 手势名称 |
|--------|----------|
| 0      | HOME     |
| 1      | PAPER    |
| 2      | FIST     |
| 3      | OK       |

### 3. ConvertJoint2Actuator

```cpp
std::vector<int> ConvertJoint2Actuator(const std::vector<double>& active_joint_angles);
```

- 将主动关节位置转换为执行器输入值。
- **注意**：O12 电机输入范围为 0-2000，与 O10 的 0-4096 不同。

### 4. ConvertActuator2Joint

```cpp
std::vector<double> ConvertActuator2Joint(const std::vector<int>& actuator_input);
```

- 将执行器输入值转换回主动关节位置。
- **注意**：O12 电机输入范围为 0-2000，与 O10 的 0-4096 不同。

### 5. ActiveJoint2MotorLength

```cpp
std::vector<double> ActiveJoint2MotorLength(const std::vector<double> &active_joint_angles);
```

- 将主动关节位置转换为电机长度。

### 6. MotorLength2ActiveJoint

```cpp
std::vector<double> MotorLength2ActiveJoint(const std::vector<double> &motor_length);
```

- 将电机长度转换为主动关节位置。

### 7. MotorLength2MotorInput

```cpp
std::vector<int> MotorLength2MotorInput(const std::vector<double> &motor_length);
```

- 将电机长度转换为电机输入值。

### 8. MotorInput2MotorLength

```cpp
std::vector<double> MotorInput2MotorLength(const std::vector<int> &motor_input);
```

- 将电机输入值转换为电机长度。

### 9. GetAllJointAngles

```cpp
std::vector<double> GetAllJointAngles(const std::vector<double>& active_joint_angles);
```

- 根据给定的主动关节位置，获取所有关节（包括被动关节）的角度。

## 代码示例

```cpp
#include "kinematics/omnihand_pro_2025/omnihand_pro_2025_solver.h"

// 创建右手求解器
agilink::omnihand::o12::OmniHandPro2025Solver solver(false);

// 检查并验证关节位置
std::vector<double> active_joint_angles = {0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.5, 0.3};
bool valid = solver.CheckJointPos(active_joint_angles);

// 设置手势
std::vector<int> actuator_input = solver.SetHandGesture(1); // PAPER 手势

// 将关节位置转换为执行器输入
std::vector<int> input = solver.ConvertJoint2Actuator(active_joint_angles);

// 获取所有关节位置（包括被动关节）
std::vector<double> all_joints = solver.GetAllJointAngles(active_joint_angles);
```

## 相关文档

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md) - 主要 C++ API 文档
- [OmniHand Pro 2025 (O12) 运动学求解器 Python API](API_KINEMATICS_PYTHON_O12.md) - 此 API 的 Python 版本
