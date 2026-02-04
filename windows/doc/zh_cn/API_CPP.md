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
- **[OmniHand Dex UMI (O10 UMI) C++ API](API_CPP_O10_UMI.md)** - 10 自由度，UMI 协议，周期上报，仅支持 CAN

## 通用枚举和数据结构

所有产品共享通用的枚举和数据结构。这些内容在每个产品特定的 API 文档中都有详细说明，但这里提供一个快速参考：

### ProductType（仅供参考，新 API 中不使用）

```cpp
enum class ProductType : unsigned char {
    OMNIHAND_2025        = 0,    // OmniHand 2025 (10 自由度)
    OMNIHAND_PRO_2025    = 1,    // OmniHand Pro 2025 (12 自由度)
    OMNIHAND_DEX_UMI     = 2,    // OmniHand Dex UMI (O10 UMI) (10 自由度, UMI 协议)
    UNKNOWN_PRODUCT_TYPE = 255   // 未知
};
```

**注意**：新的产品特定接口不需要 `ProductType` - 每个类已经为其产品类型化了。

### EHandType

```cpp
enum class EHandType : unsigned char {
    eLeft    = 0,    // 左手
    eRight   = 1,    // 右手
    eUnknown = 10    // 未知
};
```

### EFinger

```cpp
enum class EFinger : unsigned char {
    eThumb   = 0x01,    // 拇指
    eIndex   = 0x02,    // 食指
    eMiddle  = 0x03,    // 中指
    eRing    = 0x04,    // 无名指
    eLittle  = 0x05,    // 小指
    ePalm    = 0x06,    // 手心
    eDorsum  = 0x07,    // 手背
    eUnknown = 0xff     // 未知
};
```

### EControlMode

```cpp
enum class EControlMode : unsigned char {
  ePosi           = 0,    // 位置模式
  eServo          = 1,    // 伺服模式
  eVelo           = 2,    // 速度模式
  eTorque         = 3,    // 力控模式（不支持：纯力控不可用）
  ePosiTorque     = 4,    // 位置-力控模式（混合控制：位置 + 力矩）
  eVeloTorque     = 5,    // 速度-力控模式（混合控制：速度 + 力矩）
  ePosiVeloTorque = 6,    // 位置-速度-力控模式（混合控制：位置 + 速度 + 力矩）
  eUnknown        = 10    // 未知模式
};
```

## 快速开始示例

### OmniHand 2025 (O10)

```cpp
#include "omnihand/omnihand_2025.h"

// 创建手部实例
auto hand = OmniHand2025::createHandByZlgcan(
    EHandType::eLeft,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "初始化失败" << std::endl;
    return -1;
}

// 设置电机位置（O10 范围：0-4096）
std::vector<int16_t> positions{500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094};
hand->SetAllJointMotorPosi(positions);
```

### OmniHand Pro 2025 (O12)

```cpp
#include "omnihand/omnihand_pro_2025.h"

// 创建手部实例
auto hand = OmniHandPro2025::createHandByZlgcan(
    EHandType::eLeft,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);

if (!hand || !hand->Init()) {
    std::cerr << "初始化失败" << std::endl;
    return -1;
}

// 设置电机位置（O12 范围：0-2000）
std::vector<int16_t> positions{500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500};
hand->SetAllJointMotorPosi(positions);
```

### OmniHand Dex UMI

```cpp
#include "omnihand/omnihand_dex_umi.h"

// 创建手部实例
auto hand = OmniHandDexUMI::createHandByZlgcan(
    EHandType::eLeft,
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

## 从统一接口迁移

如果您之前使用的是旧的统一 `OmniHand::createHandByZlgcan(ProductType, ...)` 接口：

**旧代码：**
```cpp
auto hand = OmniHand::createHandByZlgcan(
    ProductType::OMNIHAND_2025,
    EHandType::eLeft,
    1, 0, 0
);
```

**新代码：**
```cpp
auto hand = OmniHand2025::createHandByZlgcan(
    EHandType::eLeft,
    1, 0, 0
);
```

产品类型现在由您使用的类确定，提供了更好的类型安全性和编译时检查。

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
- [OmniHand 2025 (O10) 运动学求解器 C++ API](API_KINEMATICS_CPP_O10.md) - 运动学计算
- [OmniHand Pro 2025 (O12) 运动学求解器 C++ API](API_KINEMATICS_CPP_O12.md) - 运动学计算
