# OmniHand 3 Ultra M（H3UM/O20）C++ API

## 概述

`OmniHand3UltraM` 通过 CAN FD 控制 20 自由度 H3UM/O20，并可读取五个 Xense 手指传感器和手掌触觉服务。电机位置使用 `[0, 4095]` 原始刻度，主动关节角单位为弧度。

## 包含头文件

```cpp
#include "omnihand/omnihand_3_ultra_m.h"
using namespace agilink::omnihand;
```

## 工厂函数

所有工厂函数都可在通信参数后接受一个可选的触觉 SoC 端点：

```cpp
const std::string& tactile_soc_ip = OmniHand3UltraM::kDefaultTactileSocIp;
uint16_t tactile_soc_port = OmniHand3UltraM::kDefaultTactileSocPort;
```

默认值为 `192.168.99.2:19009`。SDK 在构造期间探测该端点，仅在可通信时初始化 Xense 和手掌服务。使用标准端点的应用无需传入这两个参数。

```cpp
static std::unique_ptr<OmniHand3UltraM> createHandByZlgcan(
    HandType hand_type, uint8_t hand_device_id,
    uint8_t canfd_device_id, uint8_t canfd_channel_id = 0,
    const std::string& tactile_soc_ip = kDefaultTactileSocIp,
    uint16_t tactile_soc_port = kDefaultTactileSocPort);

static std::unique_ptr<OmniHand3UltraM> createHandByZlgcan(
    HandType hand_type, uint8_t hand_device_id,
    const std::string& usbcanfd_serial_number,
    uint8_t canfd_channel_id = 0,
    const std::string& tactile_soc_ip = kDefaultTactileSocIp,
    uint16_t tactile_soc_port = kDefaultTactileSocPort);

static std::unique_ptr<OmniHand3UltraM> createHandByHcan(
    HandType hand_type, uint8_t hand_device_id,
    uint8_t canfd_device_id, uint8_t canfd_channel_id = 0,
    const std::string& tactile_soc_ip = kDefaultTactileSocIp,
    uint16_t tactile_soc_port = kDefaultTactileSocPort);

static std::unique_ptr<OmniHand3UltraM> createHandByHcan(
    HandType hand_type, uint8_t hand_device_id,
    const std::string& hcan_serial_number,
    uint8_t canfd_channel_id = 0,
    const std::string& tactile_soc_ip = kDefaultTactileSocIp,
    uint16_t tactile_soc_port = kDefaultTactileSocPort);
```

`createHandByZlgCanTcp(...)` 和 Linux 下的 `createHandSocketCan(...)` 也使用相同的最后两个触觉端点参数。

## 设备信息与地址

```cpp
VendorInfo GetVendorInfo() const;
std::string GetSN() const;
DeviceInfo GetDeviceInfo() const;

uint8_t GetNonPrivateHandDeviceIdByBroadcast();
int GetHandDeviceIdByBroadcast();
bool SetHandDeviceIdByBroadcast(uint8_t id);
void SetBroadcast(bool broadcast);
```

H3UM 只有标准协议。设备 ID `0` 为广播地址，出厂默认单播 ID 为 `9`。

## 位置与角度控制

```cpp
int16_t SetJointMotorPosi(unsigned char joint_motor_index, int16_t position);
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;
std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& positions);
std::vector<int16_t> GetAllJointMotorPosi() const;

std::vector<double> SetAllActiveJointAngles(const std::vector<double>& angles);
std::vector<double> GetAllActiveJointAngles() const;
```

手支持 CSP（`ControlMode::POSITION`，值为 `0`）和 PP（`ControlMode::PROFILE_POSITION`，值为 `7`）。CSP/PP 与“仅位置”或“位置+力矩”的控制类型相互独立。

## 混合控制

```cpp
MixCtrl MixControlByPT(uint8_t joint_motor_index,
                       int16_t position, int16_t torque);
std::vector<MixCtrl> MixControlByPT(
    const std::vector<int16_t>& positions,
    const std::vector<int16_t>& torques);

MixCtrl MixControlByPVT(uint8_t joint_motor_index,
                        int16_t position, int16_t velocity,
                        int16_t torque);
std::vector<MixCtrl> MixControlByPVT(
    const std::vector<int16_t>& positions,
    const std::vector<int16_t>& velocities,
    const std::vector<int16_t>& torques);
```

力矩按有符号 16 位电流值直接下发，单位为 mA。`GetMinMaxDefaultMixCtrlTorque()` 返回范围 `0..10000`、默认值 `1000`。

## 零位与标定

```cpp
bool SetAllMotor2Zero();
bool SetMotor2Zero(unsigned char joint_motor_index);  // 1..20
bool SetAxisHoming(uint8_t axis_index, int16_t position);
bool SetAxisMinPos(uint8_t axis_index, int16_t min_position);
bool SetAxisMaxPos(uint8_t axis_index, int16_t max_position);
bool SaveParam();
```

## 触觉数据

```cpp
bool GetFingerTactile(uint8_t finger_index, XenseFrame& frame); // 0=拇指 .. 4=小指
bool GetPalmTactile(PalmFrame& frame);
bool GetAllTactile(AllTactileFrame& frame);
```

`XenseFrame::timestamp` 单位为纳秒。`PalmFrame` 包含力场、`resultant[3]` 和触觉服务提供的 `timestamp`。

## 相关文档

- [C++ API 总览](API_CPP.md)
- [从 1.1.8 迁移到 1.1.9](MIGRATION_1_1_8_TO_1_1_9.md)
- [ROS2 API](API_ROS2.md)
