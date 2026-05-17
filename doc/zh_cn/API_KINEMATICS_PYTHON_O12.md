# OmniHand Pro 2025 (O12) 运动学求解器 Python API

## 简介

**OmniHandPro2025Solver** 类为 OmniHand Pro 2025 (O12) 灵巧手提供控制功能，包括手势设置和关节位置转换。

## 导入

```python
from omnihand.omnihand_pro_2025 import OmniHandPro2025Solver
```

## 构造函数

```python
OmniHandPro2025Solver(hand_type: bool)
```

- **`hand_type`**: 设置为 `True` 表示左手，`False` 表示右手。

## 方法

### 1. check_joint_pos

```python
def check_joint_pos(active_joint_angles: List[float]) -> bool
```

- 检查主动关节数量是否合理。如果不合理，返回 `False`。
- 检查主动关节位置是否合理。如果不合理，进行修正。

### 2. set_hand_gesture

```python
def set_hand_gesture(gesture: OmniHandPro2025Gesture) -> List[int]
```

- 设置预定义的手势。
- 返回对应的执行器输入值。

| 手势枚举                                           | 手势名称 |
|---------------------------------------------------|----------|
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_ALL_ZERO | ALL_ZERO |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_PAPER    | PAPER    |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_FIST     | FIST     |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_OK       | OK       |
| OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_PACK     | PACK     |

### 3. convert_joint_to_actuator

```python
def convert_joint_to_actuator(active_joint_angles: List[float]) -> List[int]
```

- 将主动关节位置转换为执行器输入值。
- **注意**：O12 电机输入范围为 0-2000，与 O10 的 0-4096 不同。

### 4. convert_actuator_to_joint

```python
def convert_actuator_to_joint(actuator_input: List[int]) -> List[float]
```

- 将执行器输入值转换回主动关节位置。
- **注意**：O12 电机输入范围为 0-2000，与 O10 的 0-4096 不同。

### 5. active_joint_to_motor_length

```python
def active_joint_to_motor_length(active_joint_angles: List[float]) -> List[float]
```

- 将主动关节位置转换为电机长度。

### 6. motor_length_to_active_joint

```python
def motor_length_to_active_joint(motor_length: List[float]) -> List[float]
```

- 将电机长度转换为主动关节位置。

### 7. motor_length_to_motor_input

```python
def motor_length_to_motor_input(motor_length: List[float]) -> List[int]
```

- 将电机长度转换为电机输入值。

### 8. motor_input_to_motor_length

```python
def motor_input_to_motor_length(motor_input: List[int]) -> List[float]
```

- 将电机输入值转换为电机长度。

### 9. get_all_joint_angles

```python
def get_all_joint_angles(active_joint_angles: List[float]) -> List[float]
```

- 根据给定的主动关节位置，获取所有关节（包括被动关节）的角度。

## 代码示例

```python
from omnihand.omnihand_pro_2025 import OmniHandPro2025Solver

# 创建右手求解器
solver = OmniHandPro2025Solver(hand_type=False)

# 检查并验证关节位置
active_joint_angles = [0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.5, 0.3]
valid = solver.check_joint_pos(active_joint_angles)

# 设置手势
actuator_input = solver.set_hand_gesture(OmniHandPro2025Gesture.OMNIHAND_PRO_2025_GESTURE_PAPER)  # PAPER 手势

# 将关节位置转换为执行器输入
input_values = solver.convert_joint_to_actuator(active_joint_angles)

# 获取所有关节位置（包括被动关节）
all_joints = solver.get_all_joint_angles(active_joint_angles)
```

## 相关文档

- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md) - 主要 Python API 文档
- [OmniHand Pro 2025 (O12) 运动学求解器 C++ API](API_KINEMATICS_CPP_O12.md) - 此 API 的 C++ 版本
