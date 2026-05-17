# OmniHand 2025 (O10) 运动学求解器 C++ API

## 简介

**OmniHand2025Solver** 类（命名空间：`agilink::omnihand::o10`）为 OmniHand 2025 (O10) 灵巧手提供控制功能，包括手势设置和关节位置转换。

## 包含头文件

```cpp
#include "kinematics/omnihand_2025/omnihand_2025_solver.h"
```

## 构造函数

```cpp
agilink::omnihand::o10::OmniHand2025Solver(const bool& is_left_hand);
```

- **`is_left_hand`**: 设置为 `true` 表示左手，`false` 表示右手。

## 方法

### 1. SetHandGesture

```cpp
std::vector<int> SetHandGesture(OmniHand2025Gesture gesture);
```

- 设置预定义的手势。
- 返回对应的执行器输入值。
- 使用 `agilink::omnihand::o10::OmniHand2025Gesture`（`enum class`）。

| 手势ID | 枚举量 | 手势名称 | 手势图片 |
|--------|--------|----------|----------|
| 0 | `OMNIHAND_2025_GESTURE_ALL_ZERO` | 全零位（各主动关节角为 0） | |
| 1 | `OMNIHAND_2025_GESTURE_PAPER` | 张开手 | <img src="../pic/open_hand.jpg" width="100" /> |
| 2 | `OMNIHAND_2025_GESTURE_FIST1` | 拳头1 | <img src="../pic/fist_1.jpg" width="100" /> |
| 3 | `OMNIHAND_2025_GESTURE_FIST2` | 拳头2 | <img src="../pic/fist_2.jpg" width="100" /> |
| 4 | `OMNIHAND_2025_GESTURE_OK` | OK | <img src="../pic/OK.jpg" width="100" /> |
| 5 | `OMNIHAND_2025_GESTURE_ONE_HANDED_FINGER_HEART` | 单手比心 | <img src="../pic/One-handed_finger_heart.jpg" width="100" /> |
| 6 | `OMNIHAND_2025_GESTURE_LIKE` | 点赞 | <img src="../pic/like.jpg" width="100" /> |
| 7 | `OMNIHAND_2025_GESTURE_ILY` | ILY | <img src="../pic/ILY.jpg" width="100" /> |
| 8 | `OMNIHAND_2025_GESTURE_NUM1` | 数字1 | <img src="../pic/number_1.jpg" width="100" /> |
| 9 | `OMNIHAND_2025_GESTURE_NUM2` | 数字2 | <img src="../pic/number_2.jpg" width="100" /> |
| 10 | `OMNIHAND_2025_GESTURE_NUM3` | 数字3 | <img src="../pic/number_3.jpg" width="100" /> |
| 11 | `OMNIHAND_2025_GESTURE_NUM4` | 数字4 | <img src="../pic/number_4.jpg" width="100" /> |
| 12 | `OMNIHAND_2025_GESTURE_NUM6` | 数字6 | <img src="../pic/number_6.jpg" width="100" /> |
| 13 | `OMNIHAND_2025_GESTURE_NUM8` | 数字8 | <img src="../pic/number_8.jpg" width="100" /> |
| 14 | `OMNIHAND_2025_GESTURE_HAND_HEART1` | 双手比心1 | <img src="../pic/hand_heart_1.jpg" width="100" /> |
| 15 | `OMNIHAND_2025_GESTURE_HAND_HEART2` | 双手比心2 | <img src="../pic/hand_heart_2.jpg" width="100" /> |
| 16 | `OMNIHAND_2025_GESTURE_HAND_HEART3` | 双手比心3 | <img src="../pic/hand_heart_3.jpg" width="100" /> |
| 17 | `OMNIHAND_2025_GESTURE_CLASPING` | 抓握 | <img src="../pic/clasping.jpg" width="100" /> |

### 2. ActiveJointPos2ActuatorInput

```cpp
std::vector<int> ActiveJointPos2ActuatorInput(const std::vector<double>& active_joint_angles);
```

- 将主动关节位置转换为执行器输入值。
- **注意**：O10 电机输入范围为 0-4096，与 O12 的 0-2000 不同。

### 3. ActuatorInput2ActiveJointPos

```cpp
std::vector<double> ActuatorInput2ActiveJointPos(const std::vector<int>& actuator_input);
```

- 将执行器输入值转换回主动关节位置。
- **注意**：O10 电机输入范围为 0-4096，与 O12 的 0-2000 不同。

### 4. GetAllJointAngles

```cpp
std::vector<double> GetAllJointAngles(const std::vector<double>& active_joint_angles);
```

- 根据给定的主动关节位置，获取所有关节（包括被动关节）的角度。

### 5. show_log

```cpp
void show_log(bool flag);
```

- 控制日志输出。设置为 `true` 启用日志，`false` 禁用。

## 代码示例

```cpp
#include "kinematics/omnihand_2025/omnihand_2025_solver.h"

// 创建右手求解器
agilink::omnihand::o10::OmniHand2025Solver solver(false);

// 启用日志
solver.show_log(true);

// 设置手势
std::vector<int> actuator_input = solver.SetHandGesture(
    o10::OmniHand2025Gesture::OMNIHAND_2025_GESTURE_FIST1);

// 将关节位置转换为执行器输入
std::vector<double> active_joint_angles = {0.5, -0.3, 0.6, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0};
std::vector<int> input = solver.ActiveJointPos2ActuatorInput(active_joint_angles);

// 获取所有关节位置（包括被动关节）
std::vector<double> all_joints = solver.GetAllJointAngles(active_joint_angles);
```

## 相关文档

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md) - 主要 C++ API 文档
- [OmniHand 2025 (O10) 运动学求解器 Python API](API_KINEMATICS_PYTHON_O10.md) - 此 API 的 Python 版本
