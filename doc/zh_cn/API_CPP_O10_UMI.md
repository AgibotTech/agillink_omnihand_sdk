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
    EHandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHandDexUMI::createHandByZlgcan(
    EHandType::eLeft,
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
    EHandType hand_type,
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
    EHandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHandDexUMI::createHandByHcan(
    EHandType::eLeft,
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
    EHandType hand_type,
    unsigned char hand_device_id,
    const std::string& hcan_serial_number,
    unsigned char canfd_channel_id = 0);
```

### SocketCAN（仅 Linux）

```cpp
#ifdef __linux__
static std::unique_ptr<OmniHandDexUMI> createHandSocketCan(
    EHandType hand_type,
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

### 周期上报频率设置

```cpp
/**
 * @brief 设置位置上报频率（UMI 协议 Pn2.03）
 * @param frequency 上报频率（Hz，默认：100）
 * @note 将频率设置为 0 将禁用周期上报
 */
void SetPositionReportFrequency(uint16_t frequency);

/**
 * @brief 设置触觉传感器上报频率（UMI 协议 Pn2.04）
 * @param frequency 上报频率（Hz，默认：100）
 * @note 将频率设置为 0 将禁用周期上报
 */
void SetTactileSensorReportFrequency(uint16_t frequency);
```

### 周期上报回调

```cpp
/**
 * @brief 注册位置周期上报回调函数（UMI 协议 Pn3，Pn2.03 设置频率）
 * @param callback 接收到位置数据时调用的回调函数
 * @param frequency 可选频率（Hz，如果提供，在注册回调前设置 Pn2.03，默认：100）
 * @note 回调在 RecvFrame 线程中执行，因此应该是线程安全的
 * @note 如果 callback 为 nullptr，将取消注册回调
 */
void SetPositionReportCallback(PositionReportCallback callback, std::optional<uint16_t> frequency = std::nullopt);

/**
 * @brief 注册触觉传感器周期上报回调函数（UMI 协议 Pn6，Pn2.04 设置频率）
 * @param callback 接收到触觉传感器数据时调用的回调函数
 * @param frequency 可选频率（Hz，如果提供，在注册回调前设置 Pn2.04，默认：100）
 * @note 回调在 RecvFrame 线程中执行，因此应该是线程安全的
 * @note 如果 callback 为 nullptr，将取消注册回调
 */
void SetTactileSensorReportCallback(TactileSensorReportCallback callback, std::optional<uint16_t> frequency = std::nullopt);
```

### 触觉传感器数据

```cpp
std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const;
TactileSensorData GetTactileSensorDataRaw(EFinger eFinger) const;
static size_t GetSensorDataLength(EFinger eFinger);
static const std::vector<EFinger>& GetSensorOrder();
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
    hand->SetPositionReportCallback(
        [](const std::vector<int16_t>& positions) {
            std::cout << "位置上报: " << positions.size() << " 个值" << std::endl;
        },
        100  // 100 Hz 频率
    );

    // 注册触觉传感器上报回调
    hand->SetTactileSensorReportCallback(
        [](const TactileSensorData& sensor_data, unsigned char sensor_id) {
            std::cout << "触觉传感器上报: sensor_id=" << static_cast<int>(sensor_id)
                      << ", data_size=" << sensor_data.data_.size() << std::endl;
        },
        100  // 100 Hz 频率
    );

    // 保持运行以接收周期上报
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 取消注册回调
    hand->SetPositionReportCallback(nullptr);
    hand->SetTactileSensorReportCallback(nullptr);

    return 0;
}
```

## UMI 协议寄存器参考

- **Pn1**: 厂商信息（只读）
- **Pn2**: 设备信息（只读）
  - **Pn2.03**: 位置上报频率（读写，2 字节，Hz，默认 100）
  - **Pn2.04**: 触觉传感器上报频率（读写，2 字节，Hz，默认 100）
  - **Pn2.05**: ADC 通道数（只读，1 字节）
  - **Pn2.06**: 触觉传感器信息（只读，可变长度）
- **Pn3**: 位置信息（只读，周期上报）
- **Pn6**: 触觉传感器数据（只读，子寄存器 0x01~0x06，UMI 无手背传感器）
- **Pn7**: 位置校准（只写）
  - **Pn7.00**: 最小位置校准
  - **Pn7.01**: 最大位置校准

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
