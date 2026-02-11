# OmniHand 2025 SDK C++ API

## 概述

OmniHand 2025 SDK 为三种不同的产品提供了**产品特定的接口**：

- **OmniHand 2025 (O10)**: 10 自由度灵巧手，配备 1D 触觉传感器
- **OmniHand Pro 2025 (O12)**: 12 自由度灵巧手，配备 3D 触觉传感器
- **OmniHand Dex UMI (O10 UMI)**: 10 自由度灵巧手，支持 UMI 协议

每个产品都有自己的接口类（`OmniHand2025`、`OmniHandPro2025`、`OmniHandDexUMI`），具有产品特定的工厂方法和 API。与使用 `ProductType` 的统一接口相比，这种设计提供了更好的类型安全性和更清晰的 API 组织。

## 产品特定 API 文档

- **[OmniHand 2025 (O10) C++ API](API_CPP_O10.md)** - 10 自由度，1D 触觉传感器，支持 CAN 和 RS485
- **[OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)** - 12 自由度，3D 触觉传感器，仅支持 CAN
- **[OmniHand Dex UMI (O10 UMI) C++ API](API_CPP_O10_UMI.md)** - 10 自由度，UMI 协议，主动查询，仅支持 CAN

## 通用枚举和数据结构

所有产品共享通用的枚举和数据结构。这些内容在每个产品特定的 API 文档中都有详细说明，但这里提供一个快速参考：

### ProductType（仅供参考，新 API 中不使用）

```cpp
enum class ProductType : unsigned char {
    OMNIHAND_2025        = 0,    // OmniHand 2025 (10 自由度)
    OMNIHAND_PRO_2025    = 1,    // OmniHand Pro 2025 (12 自由度)
    OMNIHAND_DEX_UMI     = 2,    // OmniHand Dex UMI (O10 UMI) (10 自由度, UMI 协议)
    UNKNOWN = 255   // 未知
};
```

**注意**：新的产品特定接口不需要 `ProductType` - 每个类已经为其产品类型化了。

### HandType

```cpp
enum class HandType : unsigned char {
    LEFT = 0,      // 左手
    RIGHT = 1,     // 右手
    UNKNOWN = 255  // 未知手型
};
```

### Finger

```cpp
enum class Finger : unsigned char {
    THUMB = 0x01,    // 拇指
    INDEX = 0x02,    // 食指
    MIDDLE = 0x03,   // 中指
    RING = 0x04,     // 无名指
    LITTLE = 0x05,   // 小指
    PALM = 0x06,     // 手心
    DORSUM = 0x07,   // 手背
    UNKNOWN = 0xff   // 未知
};
```

### ControlMode

```cpp
enum class ControlMode : unsigned char {
    POSITION                  = 0,    // 位置模式
    SERVO                     = 1,    // 伺服模式
    VELOCITY                  = 2,    // 速度模式
    TORQUE                    = 3,    // 力控模式（不支持：纯力控不可用）
    POSITION_TORQUE           = 4,    // 位置-力控模式（混合控制：位置 + 力矩）
    VELOCITY_TORQUE           = 5,    // 速度-力控模式（混合控制：速度 + 力矩）
    POSITION_VELOCITY_TORQUE  = 6,    // 位置-速度-力控模式（混合控制：位置 + 速度 + 力矩）
    UNKNOWN                   = 10    // 未知模式
};
```

## 快速开始示例

### OmniHand 2025 (O10)

```cpp
#include "omnihand/omnihand_2025.h"

// 创建手部实例
auto hand = OmniHand2025::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "初始化失败" << std::endl;
    return -1;
}

// 设置关节角度（单位：弧度，O10 有 10 个关节）
std::vector<double> angles(10, 0.0);  // 所有关节归零
hand->SetAllActiveJointAngles(angles);
```

### OmniHand Pro 2025 (O12)

```cpp
#include "omnihand/omnihand_pro_2025.h"

// 创建手部实例
auto hand = OmniHandPro2025::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "初始化失败" << std::endl;
    return -1;
}

// 设置关节角度（单位：弧度，O12 有 12 个关节）
std::vector<double> angles(12, 0.0);  // 所有关节归零
hand->SetAllActiveJointAngles(angles);
```

### OmniHand Dex UMI

```cpp
#include "omnihand/omnihand_dex_umi.h"

// 创建手部实例
auto hand = OmniHandDexUMI::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "初始化失败" << std::endl;
    return -1;
}

// 注册位置上报回调
hand->SetPositionReportCallback([](const std::vector<int16_t>& positions) {
    std::cout << "收到位置上报: " << positions.size() << " 个值" << std::endl;
}, 100);  // 100 Hz 频率
```


## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
- [OmniHand 2025 (O10) 运动学求解器 C++ API](API_KINEMATICS_CPP_O10.md) - 运动学计算
- [OmniHand Pro 2025 (O12) 运动学求解器 C++ API](API_KINEMATICS_CPP_O12.md) - 运动学计算
