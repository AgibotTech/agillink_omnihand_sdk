# OmniHand Pro 2025 (O12) C++ API

## 概述

**OmniHand Pro 2025 (O12)** 是一�?12 自由度灵巧手，配�?3D 触觉传感器。本文档描述了用于控制和交互 OmniHand Pro 2025 设备�?C++ API�?

**主要特性：**
- 12 个主动自由度
- 3D 触觉传感器（仅手指，不支持手�?手背�?
- 电机位置范围�?-2000
- 仅支�?CAN（ZLG USB CANFD）通信
- 支持 SocketCAN（仅 Linux�?
- 支持温度和电流上报周期设�?

## 包含头文�?

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
    RING = 0x04,     // 无名�?
    LITTLE = 0x05,   // 小指
    PALM = 0x06,     // 手心（O12 不支持）
    DORSUM = 0x07,   // 手背（O12 不支持）
    UNKNOWN = 0xff   // 未知
};
}  // namespace omnihand
}  // namespace agilink
```

**注意**：O12 仅支持手指传感器（THUMB, INDEX, MIDDLE, RING, LITTLE），不支持手心（PALM）和手背（DORSUM）传感器�?

### ControlMode

```cpp
namespace agilink {
namespace omnihand {
enum class ControlMode : unsigned char {
    POSITION                  = 0,    // 位置模式
    SERVO                     = 1,    // 伺服模式
    VELOCITY                  = 2,    // 速度模式
    TORQUE                    = 3,    // 力控模式（不支持：纯力控不可用）
    POSITION_TORQUE           = 4,    // 位置-力控模式（混合控制：位置 + 力矩�?
    VELOCITY_TORQUE           = 5,    // 速度-力控模式（混合控制：速度 + 力矩�?
    POSITION_VELOCITY_TORQUE  = 6,    // 位置-速度-力控模式（混合控制：位置 + 速度 + 力矩�?
    UNKNOWN                   = 10    // 未知模式
};
}  // namespace omnihand
}  // namespace agilink
```

**注意**�?
- **SERVO 模式 (1)**：伺服控制模�?
- **纯力控模�?(TORQUE) 不支�?*：请使用混合控制模式（POSITION_TORQUE, VELOCITY_TORQUE, POSITION_VELOCITY_TORQUE�?

## 工厂方法

### 推荐：ZLG USB CANFD（零配置�?

```cpp
/**
 * @brief 工厂方法 - CAN 通信（ZLG USB CANFD），通过 canfd_device_id
 * @param hand_type 手型（左�?右手�?
 * @param hand_device_id 设备 ID（默认：1�?
 * @param canfd_device_id USB CANFD 适配器设备索引（默认�?�?
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配�?USBCANFD-200U): can0=0, can1=1；单通道适配�?USBCANFD-100U): 始终�?。双通道适配�?USBCANFD-200U): can0=0, can1=1；单通道适配�?USBCANFD-100U): 始终�?
 * @return OmniHandPro2025 实例的唯一指针
 * @note 设备类型�?00U/100U/MINI）会自动检测，无需手动指定
 * @note �?推荐：零配置，开箱即用。无需 root 权限�?
 */
static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
    HandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**示例�?*
```cpp
auto hand = OmniHandPro2025::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### 通过序列号创�?

```cpp
/**
 * @brief 工厂方法 - CAN 通信（ZLG USB CANFD），通过序列�?
 * @param hand_type 手型（左�?右手�?
 * @param hand_device_id 设备 ID
 * @param usbcanfd_serial_number USB CANFD 设备序列号（支持部分匹配�?
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配�?USBCANFD-200U): can0=0, can1=1；单通道适配�?USBCANFD-100U): 始终�?
 * @return OmniHandPro2025 实例的唯一指针，如果找不到设备则返�?nullptr
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
 * @param hand_type 手型（左�?右手�?
 * @param hand_device_id 手部设备 ID
 * @param canfd_device_id HCAN 设备索引
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配�?USBCANFD-200U): can0=0, can1=1；单通道适配�?USBCANFD-100U): 始终�?
 * @return OmniHandPro2025 实例的唯一指针
 */
static std::unique_ptr<OmniHandPro2025> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**示例�?*
```cpp
auto hand = OmniHandPro2025::createHandByHcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### 通过 HCAN 序列号创�?

```cpp
/**
 * @brief 工厂方法 - HCAN USB CANFD 通信，通过序列�?
 * @param hand_type 手型（左�?右手�?
 * @param hand_device_id 手部设备 ID
 * @param hcan_serial_number HCAN 设备序列号（支持部分匹配�?
 * @param canfd_channel_id CAN 通道索引（默认：0）。双通道适配�?USBCANFD-200U): can0=0, can1=1；单通道适配�?USBCANFD-100U): 始终�?
 * @return OmniHandPro2025 实例的唯一指针，如果找不到设备则返�?nullptr
 */
static std::unique_ptr<OmniHandPro2025> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& hcan_serial_number,
    unsigned char canfd_channel_id = 0);
```

### SocketCAN（仅 Linux�?

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

#### 关节角输�?输入顺序（右手）

| 索引 | 关节名称 (URDF)       | 最小角�?(rad)      | 最大角�?(rad)     | 最小角�?(°) | 最大角�?(°) | 速度限制 (rad/s) |
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

#### 关节角输�?输入顺序（左手）

| 索引 | 关节名称 (URDF)       | 最小角�?(rad)      | 最大角�?(rad)     | 最小角�?(°) | 最大角�?(°) | 速度限制 (rad/s) |
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

**注意**：左手通过 xacro 镜像生成，关节限制与右手相同，仅关节名称前缀�?`R_` 变为 `L_`�?

```cpp
void SetAllActiveJointAngles(const std::vector<double>& angles);  // 12 个值，顺序见上�?
std::vector<double> GetAllActiveJointAngles() const;  // 返回 12 个�?
std::vector<double> GetAllJointAngles() const;  // 返回 18 个值（12 主动 + 6 被动�?
```

### 电机位置控制

**注意**：OmniHand Pro 2025 (O12) 电机位置范围�?**0-2000**（与 O10 �?0-4096 不同）�?

```cpp
void SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi);  // 1-12
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;
std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi);  // 12 个值，返回实际位置
std::vector<int16_t> GetAllJointMotorPosi() const;
```

### 3D 触觉传感�?

```cpp
/**
 * @brief 获取指定手指�?3D 触觉传感器数据（�?O12�?
 * @param eFinger 手指枚举值（O12 仅支持手指，不支持手�?手背�?
 * @return TactileSensor3DData 结构，包含：
 *         - online_state: 传感器在线状�?
 *         - channel_value[9]: 通道值（9 个通道�?
 *         - normal_force: 法向力（0-3000，单位：0.1N�?
 *         - tangent_force: 切向�?
 *         - tangent_force_angle: 切向力角度（0-359 度）
 *         - capa_approach[4]: 电容接近�?
 */
TactileSensor3DData GetTactileSensor3DData(Finger eFinger) const;
```

### 温度和电流上报周期设置（O12 特有�?

```cpp
void SetTemperReportPeriod(unsigned char joint_motor_index, uint16_t period);
void SetAllTemperReportPeriod(std::vector<uint16_t> vec_period);
void SetCurrentReportPeriod(unsigned char joint_motor_index, uint16_t period);
void SetAllCurrentReportPeriod(std::vector<uint16_t> vec_period);
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
        std::cerr << "初始化失�? << std::endl;
        return -1;
    }

    // 设置关节角度
    std::vector<double> angles{0.3, -0.5, -0.3, -0.5, 0.0, 0.6, 0.7, 0.0, 0.6, 0.7, 0.7, 0.7};  // 12 个关节角度（单位：弧度）
    hand->SetAllActiveJointAngles(angles);
    std::cout << "已设置关节角�? ";
    for (size_t i = 0; i < angles.size(); ++i) {
        std::cout << angles[i];
        if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << " (rad)" << std::endl;

    // 获取 3D 触觉传感器数�?
    auto thumb_data = hand->GetTactileSensor3DData(Finger::THUMB);
    std::cout << "拇指法向�? " << thumb_data.normal_force_ << " (0.1N)" << std::endl;

    return 0;
}
```

**注意**：O12 的其他接口（关节角度控制、速度控制、控制模式等）与 O10 类似，但参数数量�?12 而不�?10。详细接口说明请参�?[OmniHand 2025 (O10) C++ API](API_CPP_O10.md) 文档�?

## 相关文档

- [OmniHand Pro 2025 (O12) 运动学求解器 C++ API](API_KINEMATICS_CPP_O12.md) - 运动学计�?
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
