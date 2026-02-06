# OmniHand 2025 (O10) C++ API

## 概述

**OmniHand 2025 (O10)** 是一款 10 自由度灵巧手，配备 1D 触觉传感器。本文档描述了用于控制和交互 OmniHand 2025 设备的 C++ API。

**主要特性：**
- 10 个主动 + 6 个被动自由度
- 1D 触觉传感器（手指、手心、手背）
- 电机位置范围：0-4096
- 支持 CAN（ZLG USB CANFD）和 RS485 通信
- 支持 SocketCAN（仅 Linux）

## 包含头文件

```cpp
#include "omnihand/omnihand_2025.h"
```

## 枚举类型

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

**注意**： 
- **eServo 模式 (1)**：伺服控制模式
- **纯力控模式 (eTorque) 不支持**：请使用混合控制模式（ePosiTorque, eVeloTorque, ePosiVeloTorque）

## 数据结构

### VendorInfo

```cpp
struct VendorInfo {
    std::string productModel;     // 产品型号
    std::string productSeqNum;    // 产品序列号
    Version hardwareVersion;      // 硬件版本
    Version softwareVersion;      // 软件版本
    int16_t voltage;              // 供电电压 (mV)
    unsigned char dof;            // 自由度（O10 为 10）

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

### TactileSensorData

```cpp
struct TactileSensorData {
    EFinger sensor_id_;           // 传感器 ID（手指/手心/手背）
    std::vector<uint8_t> data_;   // 传感器数据（单位：1g，最大值：255g）
};
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
 * @return OmniHand2025 实例的唯一指针
 * @note 设备类型（200U/100U/MINI）会自动检测，无需手动指定
 * @note ✅ 推荐：零配置，开箱即用。无需 root 权限。
 */
static std::unique_ptr<OmniHand2025> createHandByZlgcan(
    EHandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHand2025::createHandByZlgcan(
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
 * @return OmniHand2025 实例的唯一指针，如果找不到设备则返回 nullptr
 */
static std::unique_ptr<OmniHand2025> createHandByZlgcan(
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
 * @return OmniHand2025 实例的唯一指针
 */
static std::unique_ptr<OmniHand2025> createHandByHcan(
    EHandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**示例：**
```cpp
auto hand = OmniHand2025::createHandByHcan(
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
 * @return OmniHand2025 实例的唯一指针，如果找不到设备则返回 nullptr
 */
static std::unique_ptr<OmniHand2025> createHandByHcan(
    EHandType hand_type,
    unsigned char hand_device_id,
    const std::string& hcan_serial_number,
    unsigned char canfd_channel_id = 0);
```

### RS485 通信（仅 O10）

```cpp
/**
 * @brief 工厂方法 - RS485 通信（仅 OmniHand 2025）
 * @param hand_type 手型（左手/右手）
 * @param hand_device_id 手部设备 ID
 * @param uart_port 串口路径（例如："/dev/ttyUSB0"）
 * @param baudrate 波特率（默认：460800）
 * @return OmniHand2025 实例的唯一指针
 */
static std::unique_ptr<OmniHand2025> createHandByRs485(
    EHandType hand_type,
    unsigned char hand_device_id,
    const std::string& uart_port,
    int32_t baudrate = 460800);
```

### 高级：SocketCAN（仅 Linux）

```cpp
#ifdef __linux__
/**
 * @brief 工厂方法 - SocketCAN 通信（Linux 原生 CAN 接口）
 * @param hand_type 手型（左手/右手）
 * @param hand_device_id 设备 ID
 * @param can_interface CAN 接口名称（例如："can0", "can1"）
 * @return OmniHand2025 实例的唯一指针
 * @warning ⚠️ 高级用法：需要驱动设置和 root 权限。
 */
static std::unique_ptr<OmniHand2025> createHandSocketCan(
    EHandType hand_type,
    unsigned char hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

## 基本信息

```cpp
/**
 * @brief 获取产品类型
 * @return ProductType::OMNIHAND_2025
 */
ProductType GetProductType() const;

/**
 * @brief 检查初始化状态
 * @return 如果初始化成功返回 true，否则返回 false
 */
bool Init() const;
```

## 设备信息

```cpp
/**
 * @brief 获取厂商信息。
 * @return 包含产品型号、序列号、硬件版本、软件版本等的 VendorInfo 结构。
 */
VendorInfo GetVendorInfo() const;

/**
 * @brief 获取设备信息。
 * @return 包含设备 ID 和通信参数的 DeviceInfo 结构。
 * @note 串口通信（RS485）不支持此接口。
 */
DeviceInfo GetDeviceInfo() const;

/**
 * @brief 设置设备 ID。
 * @param hand_device_id 设备 ID。
 * @note 串口通信（RS485）不支持此接口。
 */
void SetDeviceId(unsigned char hand_device_id);

/**
 * @brief 从广播地址（hand_device_id = 0x00）获取设备信息
 * @param canfd_device_id USB CANFD 适配器设备索引
 * @param canfd_channel_id CAN 通道索引（默认：0）
 * @return DeviceInfo 结构，如果请求失败则返回空的 DeviceInfo
 * @note 此函数发送广播请求以发现 CAN 总线上的设备
 * @note 仅适用于 CAN 通信，RS485 不支持
 */
static DeviceInfo GetDeviceInfoFromBroadcast(
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);

/**
 * @brief 从广播地址（hand_device_id = 0x00）通过序列号获取设备信息
 * @param usbcanfd_serial_number USB CANFD 设备序列号（支持部分匹配）
 * @param canfd_channel_id CAN 通道索引（默认：0）
 * @return DeviceInfo 结构，如果找不到设备或请求失败则返回空的 DeviceInfo
 * @note 此函数发送广播请求以发现 CAN 总线上的设备
 * @note 仅适用于 CAN 通信，RS485 不支持
 */
static DeviceInfo GetDeviceInfoFromBroadcast(
    const std::string& usbcanfd_serial_number,
    unsigned char canfd_channel_id = 0);

#ifdef __linux__
/**
 * @brief 从广播地址（hand_device_id = 0x00）通过 SocketCAN 获取设备信息
 * @param can_interface CAN 接口名称（例如："can0", "can1"）
 * @return DeviceInfo 结构，如果请求失败则返回空的 DeviceInfo
 * @note 此函数发送广播请求以发现 CAN 总线上的设备
 * @note 仅适用于 SocketCAN（Linux 专用）
 */
static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
    const std::string& can_interface);
#endif
```

## 关节角度控制

### 关节角度 I/O 顺序（右手）

| 索引 | 关节名称         | 最小角度 (rad) | 最大角度 (rad) | 最小角度 (°) | 最大角度 (°) | 速度限制 (rad/s) |
| ---- | ---------------- | ------------- | ------------- | ----------- | ----------- | --------------- |
| 1    | R_thumb_roll_joint | -0.03        | 1.12          | -2          | 64          | 0.164           |
| 2    | R_thumb_abad_joint | -1.64        | 0.05          | -94         | 3           | 0.164           |
| 3    | R_thumb_mcp_joint  | 0            | 0.84          | 0           | 48          | 0.308           |
| 4    | R_index_abad_joint | -0.16        | 0             | -9          | 0           | 0.164           |
| 5    | R_index_pip_joint  | 0            | 1.48          | 0           | 85          | 0.308           |
| 6    | R_middle_pip_joint | 0            | 1.48          | 0           | 85          | 0.308           |
| 7    | R_ring_abad_joint  | 0            | 0.17          | 0           | 10          | 0.164           |
| 8    | R_ring_pip_joint   | 0            | 1.48          | 0           | 85          | 0.308           |
| 9    | R_pinky_abad_joint | 0            | 0.19          | 0           | 11          | 0.164           |
| 10   | R_pinky_pip_joint  | 0            | 1.48          | 0           | 85          | 0.308           |

### 关节角度 I/O 顺序（左手）

| 索引 | 关节名称         | 最小角度 (rad) | 最大角度 (rad) | 最小角度 (°) | 最大角度 (°) | 速度限制 (rad/s) |
| ---- | ---------------- | ------------- | ------------- | ----------- | ----------- | --------------- |
| 1    | L_thumb_roll_joint | -1.12        | 0.03          | -64         | 2           | 0.164           |
| 2    | L_thumb_abad_joint | -0.05        | 1.64          | -3          | 94          | 0.164           |
| 3    | L_thumb_mcp_joint  | -0.84        | 0             | -48         | 0           | 0.308           |
| 4    | L_index_abad_joint | 0            | 0.16          | 0           | 9           | 0.164           |
| 5    | L_index_pip_joint  | 0            | 1.48          | 0           | 85          | 0.308           |
| 6    | L_middle_pip_joint | 0            | 1.48          | 0           | 85          | 0.308           |
| 7    | L_ring_abad_joint  | -0.17        | 0             | -10         | 0           | 0.164           |
| 8    | L_ring_pip_joint   | 0            | 1.48          | 0           | 85          | 0.308           |
| 9    | L_pinky_abad_joint | -0.19        | 0             | -11         | 0           | 0.164           |
| 10   | L_pinky_pip_joint  | 0            | 1.48          | 0           | 85          | 0.308           |

```cpp
/**
 * @brief 设置所有主动关节的角度。
 * @param angles 关节角度向量（单位：弧度）。必须包含 10 个值。
 * @note 有关具体顺序和限制，请参考上表。
 */
void SetAllActiveJointAngles(const std::vector<double>& angles);

/**
 * @brief 获取所有主动关节的角度。
 * @return 关节角度向量（单位：弧度）。返回 10 个值。
 * @note 有关具体顺序和限制，请参考上表。
 */
std::vector<double> GetAllActiveJointAngles() const;

/**
 * @brief 获取所有关节的角度（包括主动和被动关节）。
 * @return 所有关节角度向量（单位：弧度）。返回 16 个值（10 个主动 + 6 个被动）。
 * @note 前 10 个值为主动关节（顺序参考上表），后 6 个值为被动关节。
 */
std::vector<double> GetAllJointAngles() const;

/**
 * @brief 从主动关节角度计算所有关节角度（包括主动和被动关节）。
 * @param active_joint_pos 主动关节角度向量（单位：弧度）。必须包含 10 个值。
 * @return 所有关节角度向量（单位：弧度），包括主动和被动关节。
 * @note 此函数不进行硬件通信，仅执行运动学计算。
 */
std::vector<double> GetAllJointPos(const std::vector<double>& active_joint_pos) const;
```

## 电机位置控制

**注意**：OmniHand 2025 (O10) 电机位置范围为 **0-4096**。

```cpp
/**
 * @brief 设置单个关节电机的位置。
 * @param joint_motor_index 关节电机索引（1-10）。
 * @param posi 电机位置（范围：0-4096）。
 */
void SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi);

/**
 * @brief 获取单个关节电机的位置。
 * @param joint_motor_index 关节电机索引（1-10）。
 * @return 当前位置值（范围：0-4096）。
 */
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;

/**
 * @brief 批量设置所有关节电机的位置。
 * @param vec_posi 目标位置向量。必须包含 10 个值，每个值在 0-4096 范围内。
 */
void SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi);

/**
 * @brief 批量获取所有关节电机的位置。
 * @return 当前位置向量。返回 10 个值，每个值在 0-4096 范围内。
 */
std::vector<int16_t> GetAllJointMotorPosi() const;
```

## 速度控制

```cpp
/**
 * @brief 设置单个关节电机的速度。
 * @param joint_motor_index 关节电机索引（1-10）。
 * @param velo 目标速度值。
 * @note 串口通信（RS485）不支持此接口。
 */
void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo);

/**
 * @brief 获取单个关节电机的速度。
 * @param joint_motor_index 关节电机索引（1-10）。
 * @return 当前速度值。
 * @note 串口通信（RS485）不支持此接口。
 */
int16_t GetJointMotorVelo(unsigned char joint_motor_index) const;

/**
 * @brief 批量设置所有关节电机的速度。
 * @param vec_velo 目标速度向量。必须包含 10 个值。
 */
void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo);

/**
 * @brief 批量获取所有关节电机的速度。
 * @return 当前速度向量。返回 10 个值。
 */
std::vector<int16_t> GetAllJointMotorVelo() const;
```

## 触觉传感器数据

OmniHand 2025 (O10) 使用 **1D 触觉传感器**，具有以下特性：
- **数据单位**：1g
- **最大值**：255g
- **采样频率**：10Hz
- **传感器位置**：手指（每个 16 个点）、手心（78 个点）、手背（102 个点）

```cpp
/**
 * @brief 获取指定部位的触觉传感器数据（仅 O10）。
 * @param eFinger 手指/手心枚举值。
 * @return 指定部位的触觉传感器数据向量。
 * @note 数据单位：1g，最大值：255g，采样频率：10Hz
 *       - 手指：返回 16 个数据点，每个传感器点一个
 *       - 手心：返回 25 个数据点，每 3 个传感器点一个（降采样）
 *       - 手背：返回 25 个数据点，每 4 个传感器点一个（降采样）
 */
std::vector<uint8_t> GetTactileSensorData(EFinger eFinger) const;

/**
 * @brief 一次性获取所有 1D 触觉传感器原始数据。
 * @return TactileSensorData 结构向量
 * @note 这返回完整分辨率数据，与返回降采样数据的 GetTactileSensorData() 不同。
 */
std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const;

/**
 * @brief 获取单个传感器的 1D 触觉传感器原始数据。
 * @param eFinger 手指/手心枚举值
 * @return 包含完整分辨率数据的 TactileSensorData 结构
 */
TactileSensorData GetTactileSensorDataRaw(EFinger eFinger) const;
```

**⚠️ 重要建议：获取多个传感器数据时，强烈推荐使用 `GetAllTactileSensorDataRaw()` 而不是循环调用 `GetTactileSensorDataRaw()`。**

以下表格对比了两种方式的差异：

| 方面 | 循环调用 `GetTactileSensorDataRaw()` (7次) | 使用 `GetAllTactileSensorDataRaw()` (1次) |
|------|-------------------------------------------|-------------------------------------------|
| **请求间隔累积** | 7 × interval_ms (例如：7 × 3ms = 21ms) | 1 × interval_ms (例如：1 × 3ms = 3ms) |
| **独立超时检查次数** | 7次（每次请求都有独立超时风险） | 1次（多帧接收在单次请求内完成） |
| **CAN总线占用** | 7次请求 + 7次响应 = 14次帧传输 | 1次请求 + 5次响应 = 6次帧传输 |
| **通信开销** | 高（14次帧传输） | 低（6次帧传输） |
| **超时风险累积** | 高（7次独立超时风险叠加） | 低（1次请求，多帧接收） |
| **设备处理压力** | 高（7次独立处理请求） | 低（1次批量处理） |
| **总耗时** | 长（累积请求间隔 + 多次通信） | 短（单次请求 + 多帧接收） |

**💡 建议：在需要获取多个传感器数据时，始终优先使用 `GetAllTactileSensorDataRaw()`，以获得更好的性能和可靠性。**

```cpp
/**
 * @brief 获取特定手指的传感器数据长度（静态方法）
 * @param eFinger 手指枚举值
 * @return 传感器数据长度（字节）
 */
static size_t GetSensorDataLength(EFinger eFinger);

/**
 * @brief 获取传感器顺序向量（静态方法）
 * @return 传感器顺序向量的引用
 */
static const std::vector<EFinger>& GetSensorOrder();
```

## 控制模式

```cpp
/**
 * @brief 设置单个关节电机的控制模式。
 * @param joint_motor_index 关节电机索引（1-10）。
 * @param mode 控制模式枚举值。
 * @note 纯力控模式 (eTorque) 不支持。请使用混合控制模式（ePosiTorque, eVeloTorque, ePosiVeloTorque）。
 */
void SetControlMode(unsigned char joint_motor_index, EControlMode mode);

/**
 * @brief 获取单个关节电机的控制模式。
 * @param joint_motor_index 关节电机索引（1-10）。
 * @return 当前控制模式。
 * @note 串口通信（RS485）不支持此接口。
 */
EControlMode GetControlMode(unsigned char joint_motor_index) const;

/**
 * @brief 批量设置所有关节电机的控制模式。
 * @param ctrl_modes 控制模式向量。必须包含 10 个值。
 * @note 串口通信（RS485）不支持此接口。
 */
void SetAllControlMode(const std::vector<unsigned char>& ctrl_modes);

/**
 * @brief 批量获取所有关节电机的控制模式。
 * @return 控制模式向量。返回 10 个值。
 * @note 串口通信（RS485）不支持此接口。
 */
std::vector<unsigned char> GetAllControlMode() const;
```

## 电流阈值控制

```cpp
void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold);
int16_t GetCurrentThreshold(unsigned char joint_motor_index) const;
void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds);
std::vector<int16_t> GetAllCurrentThreshold() const;
```

## 混合控制

```cpp
/**
 * @brief 以混合模式控制关节电机。
 * @param mix_ctrls 混合控制参数向量。
 * @note 纯力控模式 (eTorque) 不支持。请使用混合控制模式。
 * @note 串口通信（RS485）不支持此接口。
 */
void MixCtrlJointMotor(const std::vector<MixCtrl>& mix_ctrls);
```

## 错误处理

```cpp
JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const;
std::vector<JointMotorErrorReport> GetAllErrorReport() const;
void SetErrorReportPeriod(unsigned char joint_motor_index, uint16_t period);
void SetAllErrorReportPeriod(std::vector<uint16_t> vec_period);
```

## 温度监控

```cpp
uint16_t GetTemperatureReport(unsigned char joint_motor_index) const;
std::vector<uint16_t> GetAllTemperatureReport() const;
```

**注意**：OmniHand 2025 (O10) 不支持设置温度上报周期。此功能仅适用于 OmniHand Pro 2025 (O12)。

## 电流监控

```cpp
int16_t GetCurrentReport(unsigned char joint_motor_index) const;
std::vector<uint16_t> GetAllCurrentReport() const;
```

**注意**：OmniHand 2025 (O10) 不支持设置电流上报周期。此功能仅适用于 OmniHand Pro 2025 (O12)。

## 调试功能

```cpp
void ShowDataDetails(bool show) const;
```

## 完整示例

```cpp
#include "omnihand/omnihand_2025.h"
#include <iostream>

int main() {
    // 创建手部实例
    auto hand = OmniHand2025::createHandByZlgcan(
        EHandType::eLeft,
        1,      // hand_device_id
        0,      // canfd_device_id
        0       // canfd_channel_id
    );

    if (!hand || !hand->Init()) {
        std::cerr << "初始化 OmniHand 2025 失败" << std::endl;
        return -1;
    }

    // 获取厂商信息
    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // 设置关节角度
    std::vector<double> angles{0.0, 0.0, 0.5, 0.0, 0.8, 0.8, 0.0, 0.8, 0.0, 0.8};  // 10 个关节角度（单位：弧度）
    hand->SetAllActiveJointAngles(angles);
    std::cout << "已设置关节角度: ";
    for (size_t i = 0; i < angles.size(); ++i) {
        std::cout << angles[i];
        if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << " (rad)" << std::endl;

    // 获取触觉传感器数据
    auto thumb_data = hand->GetTactileSensorData(EFinger::eThumb);
    std::cout << "拇指传感器数据: " << thumb_data.size() << " 个点" << std::endl;

    return 0;
}
```

## 相关文档

- [OmniHand 2025 (O10) 运动学求解器 C++ API](API_KINEMATICS_CPP_O10.md) - 运动学计算
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
