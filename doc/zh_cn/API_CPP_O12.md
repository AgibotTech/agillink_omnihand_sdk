# OmniHand Pro 2025 (O12) C++ API

## 概述

**OmniHand Pro 2025 (O12)** 是一款 12 自由度灵巧手，配备 3D 触觉传感器。本文档描述了用于控制和交互 OmniHand Pro 2025 设备的 C++ API。

**主要特性：**
- 12 个主动自由度
- 3D 触觉传感器（仅手指，不支持手心/手背）
- 电机位置范围：0-2000
- 仅支持 CAN（ZLG USB CANFD）通信
- 支持 SocketCAN（仅 Linux）
- 支持温度和电流上报周期设置

## 包含头文件

```cpp
#include "omnihand/omnihand_pro_2025.h"
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
    PALM = 0x06,     // 手心（O12 不支持）
    DORSUM = 0x07,   // 手背（O12 不支持）
    UNKNOWN = 0xff   // 未知
};
}  // namespace omnihand
}  // namespace agilink
```

**注意**：O12 仅支持手指传感器（THUMB, INDEX, MIDDLE, RING, LITTLE），不支持手心（PALM）和手背（DORSUM）传感器。

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
- **SERVO 模式 (1)**：伺服控制模式
- **纯力控模式 (TORQUE) 不支持**：请使用混合控制模式（POSITION_TORQUE, VELOCITY_TORQUE, POSITION_VELOCITY_TORQUE）

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```cpp
/**
 * @brief 工厂方法 - CAN 通信（ZLG USB CANFD），通过 canfd_device_id
 * @param hand_type 手型（左/右手）
 * @param hand_device_id 设备 ID（默认：1）
 * @param canfd_device_id USB CANFD 适配器设备索引（默认：0）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器（USBCANFD-200U）：can0=0, can1=1；单通道适配器（USBCANFD-100U）：始终为 0
 * @return OmniHandPro2025 实例的唯一指针
 * @note 设备类型（200U/100U/MINI）会自动检测，无需手动指定
 * @note 推荐：零配置，开箱即用。无需 root 权限。
 */
static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
    HandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**示例**：
```cpp
auto hand = OmniHandPro2025::createHandByZlgcan(
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
 * @param hand_type 手型（左/右手）
 * @param hand_device_id 设备 ID
 * @param usbcanfd_serial_number USB CANFD 设备序列号（支持部分匹配）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器（USBCANFD-200U）：can0=0, can1=1；单通道适配器（USBCANFD-100U）：始终为 0
 * @return OmniHandPro2025 实例的唯一指针，如果找不到设备则返回 nullptr
 */
static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& usbcanfd_serial_number,
    unsigned char canfd_channel_id = 0);
```

### HCAN USB CANFD

```cpp
/**
 * @brief 工厂方法 - HCAN USB CANFD 通信，通过设备 ID
 * @param hand_type 手型（左/右手）
 * @param hand_device_id 手部设备 ID
 * @param canfd_device_id HCAN 设备索引
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器（USBCANFD-200U）：can0=0, can1=1；单通道适配器（USBCANFD-100U）：始终为 0
 * @return OmniHandPro2025 实例的唯一指针
 */
static std::unique_ptr<OmniHandPro2025> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**示例**：
```cpp
auto hand = OmniHandPro2025::createHandByHcan(
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
 * @param hand_type 手型（左/右手）
 * @param hand_device_id 手部设备 ID
 * @param hcan_serial_number HCAN 设备序列号（支持部分匹配）
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配器（USBCANFD-200U）：can0=0, can1=1；单通道适配器（USBCANFD-100U）：始终为 0
 * @return OmniHandPro2025 实例的唯一指针，如果找不到设备则返回 nullptr
 */
static std::unique_ptr<OmniHandPro2025> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& hcan_serial_number,
    unsigned char canfd_channel_id = 0);
```

### SocketCAN（仅 Linux）

```cpp
#ifdef __linux__
static std::unique_ptr<OmniHandPro2025> createHandSocketCan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

## 主要接口

### 关节角度控制

#### 关节角输入顺序（右手）

| 索引 | 关节名称 (URDF)       | 最小角度 (rad)      | 最大角度 (rad)     | 最小角度 (°) | 最大角度 (°) | 速度限制 (rad/s) |
| ---- | --------------------- | ------------------- | ------------------ | ------------ | ------------ | ---------------- |
| 1    | `R_thumb_roll_joint`  | 0.0                 | 0.9424777960769379 | 0.0          | 54.0         | 2.38             |
| 2    | `R_thumb_abad_joint`  | -1.387536755335492  | 0.0                | -79.5        | 0.0          | 2.33             |
| 3    | `R_thumb_mcp_joint`   | -0.8272860654453121 | 0.0                | -47.4        | 0.0          | 1.35             |
| 4    | `R_thumb_pip_joint`   | -1.2915436464758039 | 0.0                | -74.0        | 0.0          | 1.87             |
| 5    | `R_index_abad_joint`  | -0.2617993877991494 | 0.2617993877991494 | -15.0        | 15.0         | 2.16             |
| 6    | `R_index_mcp_joint`   | 0.0                 | 1.3526301702956054 | 0.0          | 77.5         | 2.22             |
| 7    | `R_index_pip_joint`   | 0.0                 | 1.530653753999027  | 0.0          | 87.7         | 2.49             |
| 8    | `R_middle_abad_joint` | -0.2617993877991494 | 0.2617993877991494 | -15.0        | 15.0         | 2.16             |
| 9    | `R_middle_mcp_joint`  | 0.0                 | 1.3578661580515883 | 0.0          | 77.8         | 2.22             |
| 10   | `R_middle_pip_joint`  | 0.0                 | 1.8151424220741028 | 0.0          | 104.0        | 2.16             |
| 11   | `R_ring_mcp_joint`    | 0.0                 | 1.53588974175501   | 0.0          | 88.0         | 2.54             |
| 12   | `R_pinky_mcp_joint`   | 0.0                 | 1.53588974175501   | 0.0          | 88.0         | 2.54             |

#### 关节角输入顺序（左手）

| 索引 | 关节名称 (URDF)       | 最小角度 (rad)      | 最大角度 (rad)     | 最小角度 (°) | 最大角度 (°) | 速度限制 (rad/s) |
| ---- | --------------------- | ------------------- | ------------------ | ------------ | ------------ | ---------------- |
| 1    | `L_thumb_roll_joint`  | -0.9424777960769379 | 0.0                | -54.0        | 0.0          | 2.38             |
| 2    | `L_thumb_abad_joint`  | 0.0                 | 1.387536755335492  | 0.0          | 79.5         | 2.33             |
| 3    | `L_thumb_mcp_joint`   | -0.8272860654453121 | 0.0                | -47.4        | 0.0          | 1.35             |
| 4    | `L_thumb_pip_joint`   | -1.2915436464758039 | 0.0                | -74.0        | 0.0          | 1.87             |
| 5    | `L_index_abad_joint`  | -0.2617993877991494 | 0.2617993877991494 | -15.0        | 15.0         | 2.16             |
| 6    | `L_index_mcp_joint`   | 0.0                 | 1.3526301702956054 | 0.0          | 77.5         | 2.22             |
| 7    | `L_index_pip_joint`   | 0.0                 | 1.530653753999027  | 0.0          | 87.7         | 2.49             |
| 8    | `L_middle_abad_joint` | -0.2617993877991494 | 0.2617993877991494 | -15.0        | 15.0         | 2.16             |
| 9    | `L_middle_mcp_joint`  | 0.0                 | 1.3578661580515883 | 0.0          | 77.8         | 2.22             |
| 10   | `L_middle_pip_joint`  | 0.0                 | 1.8151424220741028 | 0.0          | 104.0        | 2.16             |
| 11   | `L_ring_mcp_joint`    | 0.0                 | 1.53588974175501   | 0.0          | 88.0         | 2.54             |
| 12   | `L_pinky_mcp_joint`   | 0.0                 | 1.53588974175501   | 0.0          | 88.0         | 2.54             |

**注意**：左手通过 xacro 镜像生成，关节限制与右手相同，仅关节名称前缀由 `R_` 变为 `L_`

```cpp
void SetAllActiveJointAngles(const std::vector<double>& angles);  // 12 个值，顺序见上表
std::vector<double> GetAllActiveJointAngles() const;  // 返回 12 个值
std::vector<double> GetAllJointAngles() const;  // 返回 19 个值（12 主动 + 7 被动）
```

### 电机位置控制

**注意**：OmniHand Pro 2025 (O12) 电机位置范围为 **0-2000**（与 O10 的 0-4096 不同）

```cpp
void SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi);  // 1-12
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;
std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi);  // 12 个值，返回实际位置
std::vector<int16_t> GetAllJointMotorPosi() const;
```

### 3D 触觉传感器

```cpp
/**
 * @brief 获取指定手指的 3D 触觉传感器数据（仅 O12）。
 * @param eFinger 手指枚举值（O12 仅支持手指，不支持手心/手背）
 * @return TactileSensor3DData 结构，包含：
 *         - online_state: 传感器在线状态
 *         - channel_value[9]: 通道值（9 个通道）
 *         - normal_force: 法向力（0-3000，单位：0.1N）
 *         - tangent_force: 切向力
 *         - tangent_force_angle: 切向力角度（0-359 度）
 *         - capa_approach[4]: 电容接近值
 */
TactileSensor3DData GetTactileSensor3DData(Finger eFinger) const;
```

### 速度控制

```cpp
void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo);   // 设置单个关节电机速度，索引 1-12
int16_t GetJointMotorVelo(unsigned char joint_motor_index) const;        // 获取单个关节电机速度
void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo);         // 批量设置速度，12 个值
std::vector<int16_t> GetAllJointMotorVelo() const;                       // 批量获取速度，返回 12 个值
```

### 控制模式

**注意**：纯力控模式 (TORQUE) 不支持，请使用混合控制模式（POSITION_TORQUE、VELOCITY_TORQUE、POSITION_VELOCITY_TORQUE）。

```cpp
void SetControlMode(unsigned char joint_motor_index, ControlMode mode);  // 设置单个关节控制模式，索引 1-12
ControlMode GetControlMode(unsigned char joint_motor_index) const;       // 获取单个关节控制模式
void SetAllControlMode(const std::vector<unsigned char>& ctrl_modes);    // 批量设置控制模式，12 个值
std::vector<unsigned char> GetAllControlMode() const;                    // 批量获取控制模式，返回 12 个值
```

### 电流阈值控制

```cpp
void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold);  // 索引 1-12
int16_t GetCurrentThreshold(unsigned char joint_motor_index) const;
void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds);           // 12 个值
std::vector<int16_t> GetAllCurrentThreshold() const;                                     // 返回 12 个值
```

### 混合控制

**注意**：仅支持混合控制模式（位置+力矩、速度+力矩、位置+速度+力矩），不支持纯力矩模式。

```cpp
void MixCtrlJointMotor(const std::vector<MixCtrl>& mix_ctrls);  // 混合控制，每项含 joint_index_(1-12)、ctrl_mode_、tgt_posi_/tgt_velo_/tgt_torque_
```

### 错误上报

```cpp
JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const;  // 索引 1-12
std::vector<JointMotorErrorReport> GetAllErrorReport() const;                  // 返回 12 个值
void SetErrorReportPeriod(unsigned char joint_motor_index, uint16_t period);
void SetAllErrorReportPeriod(std::vector<uint16_t> vec_period);               // 12 个值
```

### 温度与电流监控

```cpp
uint16_t GetTemperatureReport(unsigned char joint_motor_index) const;   // 索引 1-12，单位 ℃
std::vector<uint16_t> GetAllTemperatureReport() const;                  // 返回 12 个值
void SetTemperReportPeriod(unsigned char joint_motor_index, uint16_t period);   // O12 可设置上报周期
void SetAllTemperReportPeriod(std::vector<uint16_t> vec_period);               // 12 个值

int16_t GetCurrentReport(unsigned char joint_motor_index) const;
std::vector<uint16_t> GetAllCurrentReport() const;                       // 返回 12 个值
void SetCurrentReportPeriod(unsigned char joint_motor_index, uint16_t period);  // O12 可设置上报周期
void SetAllCurrentReportPeriod(std::vector<uint16_t> vec_period);               // 12 个值
```

### 调试

```cpp
void ShowDataDetails(bool show) const;  // 是否打印收发原始数据详情
```

## 完整示例

```cpp
#include "omnihand/omnihand_pro_2025.h"
#include <iostream>

int main() {
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

    // 设置关节角度
    std::vector<double> angles{0.3, -0.5, -0.3, -0.5, 0.0, 0.6, 0.7, 0.0, 0.6, 0.7, 0.7, 0.7};  // 12 个关节角度（单位：弧度）
    hand->SetAllActiveJointAngles(angles);
    std::cout << "已设置关节角度 ";
    for (size_t i = 0; i < angles.size(); ++i) {
        std::cout << angles[i];
        if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << " (rad)" << std::endl;

    // 获取 3D 触觉传感器数据
    auto thumb_data = hand->GetTactileSensor3DData(Finger::THUMB);
    std::cout << "拇指法向力: " << thumb_data.normal_force_ << " (0.1N)" << std::endl;

    return 0;
}
```

## 相关文档

- [OmniHand Pro 2025 (O12) 运动学求解器 C++ API](API_KINEMATICS_CPP_O12.md) - 运动学计算
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
