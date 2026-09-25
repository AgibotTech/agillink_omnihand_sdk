# OmniHand 3 Ultra M (H3UM/O20) Python API

## Overview

Python exposes `OmniHand3UltraM` for 20-DOF CAN FD control and `OmniHand3UltraMSolver` for angle/actuator conversion.

```python
from omnihand import OmniHand3UltraM, OmniHand3UltraMSolver, HandType, ControlMode
```

## Factories

```python
OmniHand3UltraM.create_hand_by_zlgcan(
    hand_type=HandType.LEFT, hand_device_id=9,
    canfd_device_id=0, canfd_channel_id=0)

OmniHand3UltraM.create_hand_by_hcan(
    hand_type=HandType.LEFT, hand_device_id=9,
    canfd_device_id=0, canfd_channel_id=0)
```

The ZLG and HCAN overloads also accept adapter serial numbers. Supported builds additionally expose `create_hand_by_zlgcan_tcp(...)` and Linux `create_hand_socketcan(...)`. Python currently uses the SDK's default tactile SoC endpoint internally and does not expose the C++ `tactile_soc_ip`/`tactile_soc_port` parameters.

## Control and State

```python
hand.set_joint_position(joint_motor_index, posi)
position = hand.get_joint_position(joint_motor_index)
positions = hand.set_all_joint_positions(values)  # 20 values
positions = hand.get_all_joint_positions()

hand.set_all_active_joint_angles(angles)           # 20 radians
angles = hand.get_all_active_joint_angles()
all_angles = hand.get_all_joint_angles()

hand.set_control_mode(joint_motor_index, int(ControlMode.POSITION))
mode = hand.get_control_mode(joint_motor_index)
hand.set_all_control_modes(modes)
modes = hand.get_all_control_modes()
```

The binding also exposes current-threshold, velocity, temperature, current, error-report, request-interval, receive-timeout, and detail-log APIs. H3UM mixed-control, zeroing, tactile-frame, and broadcast-ID persistence methods are currently C++-only.

## Kinematics Solver

```python
solver = OmniHand3UltraMSolver(is_left_hand=True)
actuator = solver.active_joint_pos_to_actuator_input(active_joint_pos)
angles = solver.actuator_input_to_active_joint_pos(actuator)
```

Both vectors contain 20 values; angles are radians and actuator inputs use `[0, 4096]` protocol units.

## Related Documentation

- [H3UM C++ API](API_CPP_H3UM.md)
- [Python API index](API_PYTHON.md)
- [1.1.8 to 1.1.9 migration](MIGRATION_1_1_8_TO_1_1_9.md)
