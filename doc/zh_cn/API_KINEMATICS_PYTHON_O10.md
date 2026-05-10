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
def set_hand_gesture(gesture: int) -> List[int]
```

- 设置预定义的手势。
- 返回对应的执行器输入值。

| 手势ID | 手势名称 | 手势图片 |
|--------|----------|----------|
| 0      | 张开手   | <img src="../pic/open_hand.jpg" width="100" /> |
| 1      | 拳头1    | <img src="../pic/fist_1.jpg" width="100" /> |
| 2      | 拳头2    | <img src="../pic/fist_2.jpg" width="100" /> |
| 3      | OK       | <img src="../pic/OK.jpg" width="100" /> |
| 4      | 单手比心  | <img src="../pic/One-handed_finger_heart.jpg" width="100" /> |
| 5      | 点赞     | <img src="../pic/like.jpg" width="100" /> |
| 6      | ILY      | <img src="../pic/ILY.jpg" width="100" /> |
| 7      | 数字1    | <img src="../pic/number_1.jpg" width="100" /> |
| 8      | 数字2    | <img src="../pic/number_2.jpg" width="100" /> |
| 9      | 数字3    | <img src="../pic/number_3.jpg" width="100" /> |
| 10     | 数字4    | <img src="../pic/number_4.jpg" width="100" /> |
| 11     | 数字6    | <img src="../pic/number_6.jpg" width="100" /> |
| 12     | 数字8    | <img src="../pic/number_8.jpg" width="100" /> |
| 13     | 双手比心1 | <img src="../pic/hand_heart_1.jpg" width="100" /> |
| 14     | 双手比心2 | <img src="../pic/hand_heart_2.jpg" width="100" /> |
| 15     | 双手比心3 | <img src="../pic/hand_heart_3.jpg" width="100" /> |
| 16     | 合掌     | <img src="../pic/clasping.jpg" width="100" /> |

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
actuator_input = solver.set_hand_gesture(1)  # 拳头手势

# 将关节位置转换为执行器输入
active_joint_angles = [0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0]
input_values = solver.active_joint_angles_to_actuator_input(active_joint_angles)

# 获取所有关节位置（包括被动关节）
all_joints = solver.get_all_joint_angles(active_joint_angles)
```

## 相关文档

- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md) - 主要 Python API 文档
- [OmniHand 2025 (O10) 运动学求解器 C++ API](API_KINEMATICS_CPP_O10.md) - 此 API 的 C++ 版本
