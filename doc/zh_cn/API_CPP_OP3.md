# OmniPicker 3 (OP3) C++ API

## 概述

**OmniPicker 3 (OP3)** 为 1 自由度夹爪，支持 ZLG CAN FD、HCAN、ZLG CAN TCP、SocketCAN 和 RS485。请包含 `omnihand/omnipicker_3.h`；原来的 `omni_picker_3.h` 不再是公开头文件名。

## 工厂函数

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

ZLG 和 HCAN 工厂函数还提供序列号重载。ZLG CAN TCP 和 Linux SocketCAN 工厂函数与其他产品类采用相同形式。

## 设备 ID

OP3 只有标准协议。初始化对象后可调用 `GetNonPrivateHandDeviceIdByBroadcast()`、`GetHandDeviceIdByBroadcast()` 和 `SetHandDeviceIdByBroadcast()`。广播 ID 为 `0`，默认单播 ID 为 `1`。OP3 没有私有协议 ID。

## 位置与状态

```cpp
int16_t SetJointMotorPosi(unsigned char joint_motor_index, int16_t position);
JointPosiStatus SetJointMotorPosiWithStatus(
    uint8_t joint_motor_index, int16_t position);
JointDetailInfo GetJointDetailInfo(uint8_t joint_index) const;
```

`SetJointMotorPosiWithStatus()` 使用从 1 开始的电机索引；`GetJointDetailInfo()` 使用从 0 开始的关节索引。详细状态接口由 RS485 实现，不支持的后端返回全零结构。

## 触觉元数据

`ITactileSensor1DU16` 提供传感器数量、触觉点数、单点数据长度、回复帧数、传感器序列号、传感器顺序和原始触觉帧。`GetSensorDataLength()` 已废弃；获取触觉点数请使用 `GetNumOfTactilePoints()`。

## 混合控制

OP3 在总线混合控制协议上仅实现 **位置 + 电流（`MixControlMode::POSITION_TORQUE`）**，电流字段为 `int16`，单位 mA，范围 `0..6500`，默认值 `3000`。

基类 `OmniHand` 还提供 `MixControlByPV`、`MixControlByPVT` 接口；在 OP3 上调用将打印错误并返回空向量，**不会下发 CAN 指令**。

| 接口 | 支持情况 |
|------|----------|
| `MixControlByPT(positions, torques)` | 支持 |
| `MixControlByPV(velocities, torques)` | **不支持** |
| `MixControlByPVT(positions, velocities, torques)` | **不支持** |

```cpp
#include "omnihand/omnipicker_3.h"

// positions / torques 长度按关节数（OP3 为 1）；数组下标 i 对应混合控制关节 id i（0-based）
std::vector<MixCtrl> echo = hand->MixControlByPT({pos}, {torque});

// 以下在 OP3 上不可用，返回 {}
hand->MixControlByPV({vel}, {torque});
hand->MixControlByPVT({pos}, {vel}, {torque});
```

回读 echo 中 `joint_index_` 为 **1-based**（与 O10/O12 混合控制回读一致）。

## 相关文档

- [C++ API 总览](API_CPP.md)
- [从 1.1.8 迁移到 1.1.9](MIGRATION_1_1_8_TO_1_1_9.md)
