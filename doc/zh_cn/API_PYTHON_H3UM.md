# OmniHand 3 Ultra M（H3UM/O20）Python API

## 概述

Python 提供 `OmniHand3UltraM` 用于 20 自由度 CAN FD 控制，并提供 `OmniHand3UltraMSolver` 完成关节角与执行器输入之间的转换。

```python
from omnihand import OmniHand3UltraM, OmniHand3UltraMSolver, HandType, ControlMode
```

## 工厂函数

```python
OmniHand3UltraM.create_hand_by_zlgcan(
    hand_type=HandType.LEFT, hand_device_id=9,
    canfd_device_id=0, canfd_channel_id=0)

OmniHand3UltraM.create_hand_by_hcan(
    hand_type=HandType.LEFT, hand_device_id=9,
    canfd_device_id=0, canfd_channel_id=0)
```

ZLG 和 HCAN 也提供按适配器序列号创建的重载。相应平台还提供 `create_hand_by_zlgcan_tcp(...)`，Linux 提供 `create_hand_socketcan(...)`。Python 当前在内部使用 SDK 默认触觉 SoC 端点，不暴露 C++ 的 `tactile_soc_ip`/`tactile_soc_port` 参数。

## 控制与状态

```python
hand.set_joint_position(joint_motor_index, posi)
position = hand.get_joint_position(joint_motor_index)
positions = hand.set_all_joint_positions(values)  # 20 个值
positions = hand.get_all_joint_positions()

hand.set_all_active_joint_angles(angles)           # 20 个弧度值
angles = hand.get_all_active_joint_angles()
all_angles = hand.get_all_joint_angles()

hand.set_control_mode(joint_motor_index, int(ControlMode.POSITION))
mode = hand.get_control_mode(joint_motor_index)
hand.set_all_control_modes(modes)
modes = hand.get_all_control_modes()
```

绑定还提供电流阈值、速度、温度、电流、错误报告、请求间隔、接收超时和详细日志接口。H3UM 的混合控制、零位设置、触觉帧及广播 ID 持久化接口目前仅在 C++ 中提供。

## 运动学求解器

```python
solver = OmniHand3UltraMSolver(is_left_hand=True)
actuator = solver.active_joint_pos_to_actuator_input(active_joint_pos)
angles = solver.actuator_input_to_active_joint_pos(actuator)
```

两个向量均包含 20 个值；角度单位为弧度，执行器输入使用 `[0, 4096]` 协议值。

## 相关文档

- [H3UM C++ API](API_CPP_H3UM.md)
- [Python API 总览](API_PYTHON.md)
- [从 1.1.8 迁移到 1.1.9](MIGRATION_1_1_8_TO_1_1_9.md)
