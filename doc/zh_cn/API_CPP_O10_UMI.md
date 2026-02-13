# OmniHand Dex UMI (O10 UMI) C++ API

## 概述

**OmniHand Dex UMI (O10 UMI)** 是一款使用 UMI 协议的 10 自由度灵巧手。本文档描述了用于控制和交互 OmniHand Dex UMI (O10 UMI) 设备的 C++ API。

**主要特性：**
- 10 个主动自由度
- 1D 触觉传感器（手指、手心，无手背）
- UMI 协议（Pn1-Pn8 寄存器）
- 主动查询位置信息（无周期上报）
- 仅支持 CAN（ZLG USB CANFD）通信
- 支持 SocketCAN（仅 Linux）
- **只读位置信息**（不支持位置/速度/力矩控制）

## 包含头文件

```cpp
#include "omnihand/omnihand_dex_umi.h"
```

## 枚举类型

### HandType

```cpp
namespace agilink {
namespace omnihand {
enum class HandType : unsigned char {
    LEFT = 0,      // 左手
    RIGHT = 1,     // 右手
    UNKNOWN = 255  // 未知手型
};
}  // namespace omnihand
}  // namespace agilink
```

### Finger

```cpp
namespace agilink {
namespace omnihand {
enum class Finger : unsigned char {
    THUMB = 0x01,    // 拇指
    INDEX = 0x02,    // 食指
    MIDDLE = 0x03,   // 中指
    RING = 0x04,     // 无名指
    LITTLE = 0x05,   // 小指
    PALM = 0x06,     // 手心
    DORSUM = 0x07,   // 手背（UMI 不支持）
    UNKNOWN = 0xff   // 未知
};
}  // namespace omnihand
}  // namespace agilink
```

**注意**：UMI 支持手指和手心传感器（THUMB, INDEX, MIDDLE, RING, LITTLE, PALM），但不支持手背（DORSUM）传感器。

### ControlMode

```cpp
namespace agilink {
namespace omnihand {
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
}  // namespace omnihand
}  // namespace agilink
```

**注意**： 
- **UMI 是只读设备**：OmniHand Dex UMI (O10 UMI) 不支持位置、速度或力矩控制，因此控制模式枚举ControlMode在 UMI 设备上不可用。
- UMI 仅支持主动查询位置信息，不支持任何控制模式。

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```cpp
/**
 * @brief 工厂方法 - CAN 通信（ZLG USB CANFD），通过 canfd_device_id
 * @param hand_type 手型（左手/右手）
 * @param hand_device_id 设备 ID（默认：1）
 * @param canfd_device_id USB CANFD 适配器设备索引（默认：0）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器(USBCANFD-200U): can0=0, can1=1；单通道适配器(USBCANFD-100U): 始终为0。双通道适配器(USBCANFD-200U): can0=0, can1=1；单通道适配器(USBCANFD-100U): 始终为0
 * @return OmniHandDexUMI 实例的唯一指针
 * @note 设备类型（200U/100U/MINI）会自动检测，无需手动指定
 * @note ✅ 推荐：零配置，开箱即用。无需 root 权限。
 */
static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
    HandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHandDexUMI::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### 通过序列号创建

```cpp
/**
 * @brief 工厂方法 - CAN 通信（ZLG USB CANFD），通过序列号
 * @param hand_type 手型（左手/右手）
 * @param hand_device_id 设备 ID
 * @param usbcanfd_serial_number USB CANFD 设备序列号（支持部分匹配）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器(USBCANFD-200U): can0=0, can1=1；单通道适配器(USBCANFD-100U): 始终为0
 * @return OmniHandDexUMI 实例的唯一指针，如果找不到设备则返回 nullptr
 */
static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& usbcanfd_serial_number,
    unsigned char canfd_channel_id = 0);
```

### HCAN USB CANFD

```cpp
/**
 * @brief 工厂方法 - HCAN USB CANFD 通信，通过设备 ID
 * @param hand_type 手型（左手/右手）
 * @param hand_device_id 手部设备 ID
 * @param canfd_device_id HCAN 设备索引
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器(USBCANFD-200U): can0=0, can1=1；单通道适配器(USBCANFD-100U): 始终为0
 * @return OmniHandDexUMI 实例的唯一指针
 */
static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHandDexUMI::createHandByHcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### 通过 HCAN 序列号创建

```cpp
/**
 * @brief 工厂方法 - HCAN USB CANFD 通信，通过序列号
 * @param hand_type 手型（左手/右手）
 * @param hand_device_id 手部设备 ID
 * @param hcan_serial_number HCAN 设备序列号（支持部分匹配）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器(USBCANFD-200U): can0=0, can1=1；单通道适配器(USBCANFD-100U): 始终为0
 * @return OmniHandDexUMI 实例的唯一指针，如果找不到设备则返回 nullptr
 */
static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& hcan_serial_number,
    unsigned char canfd_channel_id = 0);
```

### SocketCAN（仅 Linux）

```cpp
#ifdef __linux__
static std::unique_ptr<OmniHandDexUMI> createHandSocketCan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

## UMI 特有接口

### 位置查询

```cpp
/**
 * @brief 获取单个关节电机位置（UMI 协议 Pn3=0x13，子寄存器 0x01-0x0A）
 * @param joint_motor_index 关节电机索引（1-10）
 * @return 关节位置值（0-4096），错误时返回-1
 */
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;

/**
 * @brief 获取所有关节电机位置（UMI 协议 Pn3=0x13，子寄存器 0x00）
 * @return 所有关节位置向量（10个值，范围0-4096），错误时返回空向量
 */
std::vector<int16_t> GetAllJointMotorPosi() const;
```

### 位置校准

```cpp
/**
 * @brief 设置所有10个关节的最小位置校准（UMI 协议 Pn8=0x08，子寄存器 0x00）
 * @note 这是位置校准的只写操作。设备应处于最小位置时调用此函数。
 */
void SetMinPositionCalibration();

/**
 * @brief 设置单个关节的最小位置校准（UMI 协议 Pn8=0x08，子寄存器 0x01-0x0A）
 * @param joint_index 关节索引（1-10，其中1为第一个关节）
 * @note 这是位置校准的只写操作。
 */
void SetMinPositionCalibration(unsigned char joint_index);

/**
 * @brief 设置所有10个关节的最大位置校准（UMI 协议 Pn7=0x07，子寄存器 0x00）
 * @note 这是位置校准的只写操作。设备应处于最大位置时调用此函数。
 */
void SetMaxPositionCalibration();

/**
 * @brief 设置单个关节的最大位置校准（UMI 协议 Pn7=0x07，子寄存器 0x01-0x0A）
 * @param joint_index 关节索引（1-10，其中1为第一个关节）
 * @note 这是位置校准的只写操作。
 */
void SetMaxPositionCalibration(unsigned char joint_index);
```


### 触觉传感器数据

```cpp
/**
 * @brief 一次性获取所有 1D 触觉传感器原始数据
 * @return TactileSensorData 结构向量
 * @note 返回完整分辨率数据。使用 UMI 协议 Pn6。
 * @note UMI 设备只有 6 个传感器：Thumb, Index, Middle, Ring, Little, Palm（无手背传感器）
 */
std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const;

/**
 * @brief 获取单个传感器的 1D 触觉传感器原始数据
 * @param eFinger 手指/手心枚举值
 * @return 包含完整分辨率数据的 TactileSensorData 结构
 * @note 使用 UMI 协议 Pn6。
 * @note UMI 不支持 DORSUM（手背传感器）
 */
TactileSensorData GetTactileSensorDataRaw(Finger eFinger) const;

/**
 * @brief 获取特定手指的传感器数据长度（静态方法）
 * @param eFinger 手指枚举值
 * @return 传感器数据长度（字节）
 * @note 对于 UMI：DORSUM 返回 0（UMI 没有手背传感器）
 */
static size_t GetSensorDataLength(Finger eFinger);

/**
 * @brief 获取传感器顺序向量（静态方法）
 * @return 传感器顺序向量的引用
 * @note 对于 UMI：返回的向量包含 DORSUM，但 UMI 设备没有手背传感器。
 *       使用 GetAllTactileSensorDataRaw() 时，只返回 UMI 上可用的传感器（Thumb, Index, Middle, Ring, Little, Palm）。
 */
static const std::vector<Finger>& GetSensorOrder();
```

## DeviceInfo 的 UMI 特定字段

```cpp
struct DeviceInfo {
    unsigned char hand_device_id; // 手部设备 ID
    CommuParams commu_params;     // 通信参数
};
```

## 重要注意事项

1. **无位置/速度/力矩控制**：OmniHand Dex UMI (O10 UMI) 是**只读**设备。它不支持位置、速度或力矩控制。它通过主动查询提供位置信息。

2. **主动查询位置**：UMI 协议支持通过 `GetJointMotorPosi()` 和 `GetAllJointMotorPosi()` 主动查询关节位置。

3. **位置校准**：位置校准（最小/最大）是只写操作。调用校准函数时，设备应处于适当的位置。

## 完整示例

```cpp
#include "omnihand/omnihand_dex_umi.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
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

    // 查询关节位置（主动查询）
    auto positions = hand->GetAllJointMotorPosi();
    std::cout << "所有关节位置 (" << positions.size() << " 个值): ";
    for (size_t i = 0; i < positions.size(); ++i) {
        std::cout << positions[i];
        if (i < positions.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    // 获取触觉传感器数据
    auto tactile_data = hand->GetAllTactileSensorDataRaw();
    std::cout << "触觉传感器: " << tactile_data.size() << " 个传感器" << std::endl;

    return 0;
}
```

## UMI 协议寄存器参考

- **Pn1**: 厂商信息（只读）
- **Pn2**: 设备信息（只读）
- **Pn3**: 位置信息（只读，主动查询）
  - **Pn3.00**: 读取所有关节位置（10个值）
  - **Pn3.01~Pn3.0A**: 读取单个关节位置（关节1-10）
- **Pn6**: 触觉传感器数据（只读）
  - **Pn6.00**: 读取所有传感器数据（6个传感器：Thumb, Index, Middle, Ring, Little, Palm，UMI无手背传感器）
  - **Pn6.01~Pn6.06**: 读取单个传感器数据（传感器1-6）
- **Pn7**: 最大位置校准（只写）
  - **Pn7.00**: 一次性设置所有关节的最大位置
  - **Pn7.01~Pn7.0A**: 设置单个关节的最大位置（关节 1-10）
- **Pn8**: 最小位置校准（只写）
  - **Pn8.00**: 一次性设置所有关节的最小位置
  - **Pn8.01~Pn8.0A**: 设置单个关节的最小位置（关节 1-10）

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
