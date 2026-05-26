# OmniPicker 3 (OP3) C++ API

## Overview

**OmniPicker 3 (OP3)** is a 1-DOF gripper exposed via the `OmniPicker3` class and CAN factory methods (see `omni_picker_3.h`).

## Mixed Control

On OP3, the mixed-control bus protocol implements **position + force (`MixControlMode::POSITION_TORQUE`)** only. Torque is **int16** (same as O12, unit 0.01 N); per-joint stride is 5 bytes on the wire.

`OmniHand` also declares `MixControlByPV` and `MixControlByPVT`. On OP3 these log an error and return an empty vector; **no CAN frame is sent**.

| API | Support |
|-----|---------|
| `MixControlByPT(positions, torques)` | Supported |
| `MixControlByPV(velocities, torques)` | **Not supported** |
| `MixControlByPVT(positions, velocities, torques)` | **Not supported** |

```cpp
#include "omnihand/omni_picker_3.h"

// Array index i maps to mix-control joint id i (0-based); OP3 has one joint.
std::vector<MixCtrl> echo = hand->MixControlByPT({pos}, {torque});

// Not available on OP3; returns {}
hand->MixControlByPV({vel}, {torque});
hand->MixControlByPVT({pos}, {vel}, {torque});
```

Echo `joint_index_` is **1-based** in the reply (consistent with O10/O12 mixed-control readback).
