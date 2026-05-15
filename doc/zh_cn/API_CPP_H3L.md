# OmniHand 3 Lite (H3L) C++ API

## 概述

**OmniHand 3 Lite (H3L)** 是一款 4 自由度轻量级灵巧手。本文档描述了用于控制和交互 OmniHand 3 Lite 设备的 C++ API。

**主要特性：**
- 4 个主动自由度
- 电机位置范围：0–4096（int16 电机 ticks）
- 支持 CAN（ZLG USB CANFD / HCAN）通信
- 支持 SocketCAN（仅 Linux）
- 支持 ZLG CAN TCP（仅 Linux x64 / Windows）
- **无触觉传感器**
- **无运动学求解器**：不支持角度控制（`SetAllActiveJointAngles` 为桩实现）。请使用电机位置控制（`SetAllJointMotorPosi` / `SetAllJointMotorPosi`）

## 包含头文件

```cpp
#include "omnihand/omnihand_3_lite.h"
#include <cstdint>
using namespace agilink::omnihand;
```

## 枚举类型

### HandType

```cpp
enum class HandType : unsigned char {
    LEFT = 0,      // 左手
    RIGHT = 1,     // 右手
    UNKNOWN = 255  // 未知手型
};
```

### ControlMode

```cpp
namespace agilink {
namespace omnihand {
enum class ControlMode : unsigned char {
    POSITION                  = 0,    // 位置模式（默认）
    SERVO                     = 1,    // 伺服模式（H3L 不支持 SetControlMode 切换）
    VELOCITY                  = 2,    // 速度模式（H3L 不支持 SetControlMode 切换）
    TORQUE                    = 3,    // 力控模式（H3L 不支持）
    POSITION_TORQUE           = 4,    // 位置-力控模式（混合控制：位置 + 电流）
    VELOCITY_TORQUE           = 5,    // 速度-力控模式（H3L 暂未开放）
    POSITION_VELOCITY_TORQUE  = 6,    // 位置-速度-力控模式（H3L 暂未开放）
    UNKNOWN                   = 10    // 未知模式
};
}  // namespace omnihand
}  // namespace agilink
```

**H3L 可用模式**：
- `POSITION` (0)：默认位置控制
- `POSITION_TORQUE` (4)：位置+电流混合控制
- 其余模式不可用或暂未开放

## 数据结构

### VendorInfo

```cpp
struct VendorInfo {
    std::string productModel;     // 产品型号
    std::string productSeqNum;    // 产品序列号
    Version hardwareVersion;      // 硬件版本
    Version softwareVersion;      // 软件版本
    int16_t voltage;              // 供电电压 (mV)
    unsigned char dof;            // 自由度（H3L 为 4）

    std::string toString() const;
};
```

### DeviceInfo

```cpp
struct DeviceInfo {
    unsigned char hand_device_id; // 手部设备 ID
    CommuParams commu_params;     // 通信参数
    std::string toString() const;
};
```

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```cpp
/**
 * @brief 工厂方法 - CAN 通信（ZLG USB CANFD），通过 canfd_device_id
 * @param hand_type 手型（左/右手）
 * @param hand_device_id 设备 ID（默认：1）
 * @param canfd_device_id USB CANFD 适配器设备索引（默认：0）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道（200U）：can0=0, can1=1；单通道：始终为 0
 * @return OmniHand3Lite 实例的唯一指针
 * @note 推荐：零配置，开箱即用。无需 root 权限。
 */
static std::unique_ptr<OmniHand3Lite> createHandByZlgcan(
    HandType hand_type,
    uint8_t hand_device_id = 1,
    uint8_t canfd_device_id = 0,
    uint8_t canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHand3Lite::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### 通过序列号创建

```cpp
static std::unique_ptr<OmniHand3Lite> createHandByZlgcan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& usbcanfd_serial_number,
    uint8_t canfd_channel_id = 0);
```

### HCAN USB CANFD

```cpp
static std::unique_ptr<OmniHand3Lite> createHandByHcan(
    HandType hand_type,
    uint8_t hand_device_id,
    uint8_t canfd_device_id,
    uint8_t canfd_channel_id = 0);

static std::unique_ptr<OmniHand3Lite> createHandByHcan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& hcan_serial_number,
    uint8_t canfd_channel_id = 0);
```

### SocketCAN（仅 Linux）

```cpp
#ifdef __linux__
static std::unique_ptr<OmniHand3Lite> createHandSocketCan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

### ZLG CAN TCP（仅部分平台）

```cpp
#if OMNIHAND_ZLG_TCP_SUPPORTED
static std::unique_ptr<OmniHand3Lite> createHandByZlgCanTcp(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& tcp_host,
    uint16_t tcp_port,
    uint8_t canfd_channel_id = 0);
#endif
```

## 主要接口

### 初始化

```cpp
bool Init();
```

所有工厂方法返回的实例在创建后需调用 `Init()` 完成初始化。`Init()` 失败时返回 `false`。

### 电机位置控制（推荐）

> **H3L 无运动学求解器，请使用电机位置控制（ticks）。** 角度控制方法（`SetAllActiveJointAngles` 等）均为桩实现，打印 warning 并返回空。

```cpp
int16_t SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi);
std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi);
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;
std::vector<int16_t> GetAllJointMotorPosi() const;
```

### 关节角度控制（不可用）

```cpp
// 以下方法在 H3L 中为桩实现，不可用。请使用电机位置控制。
std::vector<double> SetAllActiveJointAngles(const std::vector<double>& angles);
std::vector<double> GetAllActiveJointAngles() const;
```

### 速度控制

```cpp
void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo);
int16_t GetJointMotorVelo(unsigned char joint_motor_index) const;
void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo);
std::vector<int16_t> GetAllJointMotorVelo() const;
```

### 电流阈值控制

```cpp
void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold);
int16_t GetCurrentThreshold(unsigned char joint_motor_index) const;
void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds);
std::vector<int16_t> GetAllCurrentThreshold() const;
```

### 混合控制

```cpp
void MixCtrlJointMotor(const std::vector<MixCtrl>& mix_ctrls);
```
- `ctrl_mode_`：仅 `POSITION_TORQUE` 可用
- `tgt_posi_`：目标位置（0–4095 编码器原始值）
- `tgt_torque_`：目标电流（单位 mA）

### 错误处理

```cpp
JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const;
std::vector<JointMotorErrorReport> GetAllErrorReport() const;
```

### 温度监控

```cpp
int16_t GetTemperatureReport(unsigned char joint_motor_index) const;
std::vector<int16_t> GetAllTemperatureReport() const;
```

### 电流监控

```cpp
int16_t GetCurrentReport(unsigned char joint_motor_index) const;
std::vector<int16_t> GetAllCurrentReport() const;
```

### 厂商与设备信息

```cpp
VendorInfo GetVendorInfo() const;
DeviceInfo GetDeviceInfo() const;
void SetDeviceId(unsigned char device_id);
```

### 调试功能

```cpp
void ShowDataDetails(bool show) const;
```

## 完整示例

```cpp
#include "omnihand/omnihand_3_lite.h"
#include <iostream>

int main() {
    auto hand = OmniHand3Lite::createHandByZlgcan(
        HandType::LEFT, 1, 0, 0);

    if (!hand || !hand->Init()) {
        std::cerr << "初始化失败" << std::endl;
        return -1;
    }

    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // 设置电机位置（4 个关节，0–4095）
    std::vector<int16_t> positions = {2048, 2048, 2048, 2048};
    auto echo = hand->SetAllJointMotorPosi(positions);

    auto temps = hand->GetAllTemperatureReport();
    return 0;
}
```

## Demo 文件

| Demo | 路径 |
|------|------|
| CAN FD（按 ID 连接） | [H3L_demo_canfd_id.cc](../../../cpp/demo/omnihand_3_lite/H3L_demo_canfd_id.cc) |
| CAN FD（按串号连接） | [H3L_demo_canfd_serial.cc](../../../cpp/demo/omnihand_3_lite/H3L_demo_canfd_serial.cc) |

编译方式参见 [CMakeLists.txt](../../../cpp/demo/omnihand_3_lite/CMakeLists.txt)。

## 相关文档

- [OmniHand 3 Lite (H3L) Python API](API_PYTHON_H3L.md)
- [OmniHand 3 Lite (H3L) ROS2 接口](API_ROS2_H3L.md)
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md)
