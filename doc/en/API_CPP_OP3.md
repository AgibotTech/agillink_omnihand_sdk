# OmniPicker 3 (OP3) C++ API

## Overview

**OmniPicker 3 (OP3)** is a 1-DOF gripper supporting ZLG CAN FD, HCAN, ZLG CAN TCP, SocketCAN, and RS485. Include `omnihand/omnipicker_3.h`; the former `omni_picker_3.h` name is no longer public.

## Factory Methods

```cpp
static std::unique_ptr<OmniPicker3> createHandByZlgcan(
    HandType hand_type, uint8_t hand_device_id,
    uint8_t canfd_device_id, uint8_t canfd_channel_id = 0);

static std::unique_ptr<OmniPicker3> createHandByHcan(
    HandType hand_type, uint8_t hand_device_id,
    uint8_t canfd_device_id, uint8_t canfd_channel_id = 0);

static std::unique_ptr<OmniPicker3> createHandByRs485(
    HandType hand_type, uint8_t hand_device_id,
    const std::string& uart_port, int32_t baudrate = 460800);
```

The ZLG and HCAN factories also have serial-number overloads. ZLG CAN TCP and Linux SocketCAN factories follow the same conventions as the other product classes.

## Device ID

OP3 uses only the standard protocol. The initialized instance provides `GetNonPrivateHandDeviceIdByBroadcast()`, `GetHandDeviceIdByBroadcast()`, and `SetHandDeviceIdByBroadcast()`. The broadcast ID is `0`, and the default unicast ID is `1`. OP3 has no private-protocol ID.

## Position and Status

```cpp
int16_t SetJointMotorPosi(unsigned char joint_motor_index, int16_t position);
JointPosiStatus SetJointMotorPosiWithStatus(
    uint8_t joint_motor_index, int16_t position);
JointDetailInfo GetJointDetailInfo(uint8_t joint_index) const;
```

`SetJointMotorPosiWithStatus()` uses a 1-based motor index. `GetJointDetailInfo()` uses a 0-based joint index. The detailed status calls are implemented by RS485; unsupported backends return an all-zero structure.

## Tactile Metadata

`ITactileSensor1DU16` provides sensor count, point count, datum size, returned-frame count, sensor serial number, sensor order, and raw tactile frames. `GetSensorDataLength()` is deprecated; use `GetNumOfTactilePoints()` for point counts.

## Mixed Control

On OP3, the mixed-control bus protocol implements **position + current (`MixControlMode::POSITION_TORQUE`)** only. The current field is `int16` in mA; its range is `0..6500`, default `3000`.

`OmniHand` also declares `MixControlByPV` and `MixControlByPVT`. On OP3 these log an error and return an empty vector; **no CAN frame is sent**.

| API | Support |
|-----|---------|
| `MixControlByPT(positions, torques)` | Supported |
| `MixControlByPV(velocities, torques)` | **Not supported** |
| `MixControlByPVT(positions, velocities, torques)` | **Not supported** |

```cpp
#include "omnihand/omnipicker_3.h"

// Array index i maps to mix-control joint id i (0-based); OP3 has one joint.
std::vector<MixCtrl> echo = hand->MixControlByPT({pos}, {torque});

// Not available on OP3; returns {}
hand->MixControlByPV({vel}, {torque});
hand->MixControlByPVT({pos}, {vel}, {torque});
```

Echo `joint_index_` is **1-based** in the reply (consistent with O10/O12 mixed-control readback).

## Related Documentation

- [C++ API index](API_CPP.md)
- [1.1.8 to 1.1.9 migration](MIGRATION_1_1_8_TO_1_1_9.md)
