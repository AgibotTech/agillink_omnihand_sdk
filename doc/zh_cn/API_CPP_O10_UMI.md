# OmniHand Dex UMI (O10 UMI) C++ API

## 概述

**OmniHand Dex UMI (O10 UMI)** 是一款使用 UMI 协议的 10 自由度灵巧手。本文档描述了用于控制和交互 OmniHand Dex UMI (O10 UMI) 设备的 C++ API。

**主要特性：**
- 10 个主动自由度
- 1D 触觉传感器（手指、手心、手背）
- UMI 协议（Pn1-Pn7 寄存器）
- 通过回调函数进行位置和触觉传感器周期上报
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
 * @param canfd_channel_id CAN 通道索引（默认：0，USBCANFD-200U 有 2 个通道：0 和 1）
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
 * @param canfd_channel_id CAN 通道索引（默认：0）
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
 * @param canfd_channel_id CAN 通道索引（默认：0）
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
 * @param canfd_channel_id CAN 通道索引（默认：0）
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

### 位置校准

```cpp
/**
 * @brief 设置最小位置校准（UMI 协议 Pn7，子寄存器 0x00）
 * @note 这是位置校准的只写操作。设备应处于最小位置时调用此函数。
 */
void SetMinPositionCalibration();

/**
 * @brief 设置最大位置校准（UMI 协议 Pn7，子寄存器 0x01）
 * @note 这是位置校准的只写操作。设备应处于最大位置时调用此函数。
 */
void SetMaxPositionCalibration();
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
    
    // UMI 特定字段（可选，仅 OmniHand Dex UMI 填充）
    std::optional<uint16_t> position_report_frequency;        // Pn2.03: 位置上报频率（Hz，默认 100）
    std::optional<uint16_t> tactile_sensor_report_frequency; // Pn2.04: 触觉传感器上报频率（Hz，默认 100）
    std::optional<unsigned char> adc_channel_count;          // Pn2.05: ADC 通道数（只读）
    std::optional<std::vector<unsigned char>> tactile_sensor_info; // Pn2.06: 触觉传感器信息（只读）
};
```

## 重要注意事项

1. **无位置/速度/力矩控制**：OmniHand Dex UMI (O10 UMI) 是**只读**设备。它不支持位置、速度或力矩控制。它仅通过周期上报提供位置信息。

2. **周期上报**：UMI 协议支持位置和触觉传感器数据的周期上报。使用回调函数异步接收此数据。

3. **线程安全**：回调函数在 `RecvFrame` 线程中执行。确保您的回调是线程安全的。

4. **位置校准**：位置校准（最小/最大）是只写操作。调用校准函数时，设备应处于适当的位置。

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
- **Pn6**: 触觉传感器数据（只读，周期上报，子寄存器 0x01~0x07）
- **Pn7**: 位置校准（只写）
  - **Pn7.00**: 最小位置校准
  - **Pn7.01**: 最大位置校准

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
