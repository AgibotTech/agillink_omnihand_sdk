# OmniHand 2025 (O10) 运动学求解器 Python API

## 简介

**OmniHand2025Solver** 类为 OmniHand 2025 (O10) 灵巧手提供控制功能，包括手势设置和关节位置转换。

## 导入

```python
from omnihand.omnihand_2025 import OmniHand2025Solver
```

## 构造函数

```python
OmniHand2025Solver(hand_type: bool)
```

- **`hand_type`**: 设置为 `True` 表示左手，`False` 表示右手。

## 方法

### 1. set_hand_gesture

```python
def set_hand_gesture(gesture: OmniHand2025Gesture) -> List[int]
```

- 设置预定义的手势。
- 返回对应的执行器输入值。
- 从 `omnihand` 导入 `OmniHand2025Gesture`。

| 手势ID | 成员 | 手势名称 |
|--------|------|----------|
| 0 | `OMNIHAND_2025_GESTURE_ALL_ZERO` | 全零位 |
| 1 | `OMNIHAND_2025_GESTURE_PAPER` | 张开手 |
| 2 | `OMNIHAND_2025_GESTURE_FIST1` | 拳头1 |
| 3 | `OMNIHAND_2025_GESTURE_FIST2` | 拳头2 |
| … | … | … |
| 17 | `OMNIHAND_2025_GESTURE_CLASPING` | 抓握 |

请传入 `OmniHand2025Gesture` 枚举成员，勿使用裸数字。

### 2. active_joint_angles_to_actuator_input

```python
def active_joint_angles_to_actuator_input(active_joint_angles: List[float]) -> List[int]
```

- 将主动关节位置转换为执行器输入值。
- **注意**：O10 电机输入范围为 0-4096，与 O12 的 0-2000 不同。

### 3. actuator_input_to_active_joint_angles

```python
def actuator_input_to_active_joint_angles(actuator_input: List[int]) -> List[float]
```

- 将执行器输入值转换回主动关节位置。
- **注意**：O10 电机输入范围为 0-4096，与 O12 的 0-2000 不同。

### 4. get_all_joint_angles

```python
def get_all_joint_angles(active_joint_angles: List[float]) -> List[float]
```

- 根据给定的主动关节位置，获取所有关节（包括被动关节）的角度。

### 5. show_log

```python
def show_log(flag: bool) -> None
```

- 控制日志输出。设置为 `True` 启用日志，`False` 禁用。

## 代码示例

```python
from omnihand.omnihand_2025 import OmniHand2025Solver

# 创建右手求解器
solver = OmniHand2025Solver(hand_type=False)

# 启用日志
solver.show_log(True)

# 设置手势
from omnihand import OmniHand2025Gesture

actuator_input = solver.set_hand_gesture(OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST1)

# 将关节位置转换为执行器输入
active_joint_angles = [0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0]
input_values = solver.active_joint_angles_to_actuator_input(active_joint_angles)

# 获取所有关节位置（包括被动关节）
all_joints = solver.get_all_joint_angles(active_joint_angles)
```

## 相关文档

- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md) - 主要 Python API 文档
- [OmniHand 2025 (O10) 运动学求解器 C++ API](API_KINEMATICS_CPP_O10.md) - 此 API 的 C++ 版本
