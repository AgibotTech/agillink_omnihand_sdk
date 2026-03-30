# OmniHand Pro 2025 (O12) Python API

## 概述

**OmniHand Pro 2025 (O12)** 是一款 12 自由度灵巧手，配备 3D 触觉传感器。本文档描述了用于控制和交互 OmniHand Pro 2025 设备的 Python API。

**主要特性：**
- 12 个主动自由度
- 3D 触觉传感器（仅手指，不支持手心/手背）
- 电机位置范围：0-2000
- 仅支持 CAN（ZLG USB CANFD）通信
- 支持 SocketCAN（仅 Linux）
- 支持温度和电流上报周期设置

## 导入

```python
from omnihand import OmniHandPro2025, HandType, Finger, ControlMode
```

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """创建灵巧手对象（推荐：零配置）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 设备 ID，默认为 1。
        canfd_device_id: USB CANFD 适配器设备索引，默认为 0。
        channel_id: CAN 通道索引，默认为 0。
    
    Returns:
        OmniHandPro2025: 灵巧手实例。
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """创建灵巧手对象（HCAN USB CANFD，通过设备 ID）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 手部设备 ID，默认为 1。
        canfd_device_id: HCAN 设备索引，默认为 0。
        canfd_channel_id: CAN 通道索引，默认为 0。双通道适配器(USBCANFD-200U）： can0=0, can1=1；单通道适配器(USBCANFD-100U）： 始终为0。
    
    Returns:
        OmniHandPro2025: 灵巧手实例。
    """
```

**示例：**
```python
hand = OmniHandPro2025.create_hand_by_hcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_canfd_channel_id=0
)
```

### 通过 HCAN 序列号创建

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType,
                hand_device_id: int,
                hcan_usbcanfd_serial_number: str,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandPro2025':
    """创建灵巧手对象（HCAN USB CANFD，通过序列号）。

    Args:
        hand_type: 手型。
        device_id: 手部设备 ID。
        hcan_serial_number: HCAN 设备序列号（支持部分匹配）。
        canfd_channel_id: CAN 通道索引，默认为 0。双通道适配器(USBCANFD-200U）： can0=0, can1=1；单通道适配器(USBCANFD-100U）： 始终为0。
    
    Returns:
        OmniHandPro2025: 灵巧手实例，如果找不到设备则返回 None。
    """
```

## 主要接口

### 关节角度控制

#### 关节角输出/输入顺序（右手）

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

#### 关节角输出/输入顺序（左手）

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

**注意**：左手通过 xacro 镜像生成，关节限制与右手相同，仅关节名称前缀从 `R_` 变为 `L_`。

```python
def set_all_active_joint_angles(self, angles: List[float]) -> None:
    """设置所有主动关节角度（单位：弧度）。
    
    Args:
        angles: 关节角度列表。必须包含 12 个值，顺序见上表。
    """

def get_all_active_joint_angles(self) -> List[float]:
    """获取所有主动关节角度（单位：弧度）。
    
    Returns:
        List[float]: 关节角度列表。返回 12 个值，顺序见上表。
    """

def get_all_joint_angles(self) -> List[float]:
    """获取所有关节角度（包括主动和被动，单位：弧度）。
    
    Returns:
        List[float]: 关节角度列表。返回 19 个值（12 主动 + 7 被动）。
                    前 12 个值为主动关节（顺序见上表），后 7 个为被动关节。
    """
```

### 电机位置控制

**注意**：OmniHand Pro 2025 (O12) 电机位置范围为 **0-2000**（与 O10 的 0-4096 不同）。

```python
def set_joint_position(self, joint_motor_index: int, position: int) -> None:
    """设置单个关节电机的位置。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        position: 目标位置（范围：0-2000）。
    """

def get_joint_position(self, joint_motor_index: int) -> int:
    """获取单个关节电机的位置。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        int: 当前位置（范围：0-2000）。
    """

def set_all_joint_positions(self, positions: List[int]) -> List[int]:
    """批量设置所有关节电机的位置并返回实际位置。
    
    Args:
        positions: 目标位置列表。必须包含 12 个值，每个值在 0-2000 范围内。
    
    Returns:
        设备响应的实际位置列表。失败时返回空列表。
    """

def get_all_joint_positions(self) -> List[int]:
    """批量获取所有关节电机的位置。
    
    Returns:
        List[int]: 当前位置列表。返回 12 个值，每个值在 0-2000 范围内。
    """
```

### 3D 触觉传感器

```python
def get_tactile_sensor_3d_data(self, eFinger: Finger) -> TactileSensor3DData:
    """获取指定手指的 3D 触觉传感器数据（仅 O12）。
    
    Args:
        eFinger: 手指枚举值（O12 仅支持手指，不支持手心/手背）。
    
    Returns:
        TactileSensor3DData: 3D 触觉传感器数据结构，包含：
                           - online_state: 传感器在线状态
                           - channel_values: 六个原生通道值
                           - normal_force: 法向力（0-3000，单位：0.1N）
                           - tangent_force: 切向力
                           - tangent_force_angle: 切向力角度（0-359 度）
                           - capacitive_approach: 电容接近值（4 个通道）
    
    Note:
        O12 仅支持手指（THUMB, INDEX, MIDDLE, RING, LITTLE），不支持手心或手背。
    """
```

### 温度和电流上报周期设置（O12 特有）

```python
def set_temperature_report_period(self, joint_motor_index: int, period: int) -> None:
    """设置单个关节电机的温度上报周期（仅 O12）。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        period: 上报周期（毫秒）。
    """

def set_all_temperature_report_periods(self, periods: List[int]) -> None:
    """批量设置所有关节电机的温度上报周期（仅 O12）。
    
    Args:
        periods: 上报周期列表。必须包含 12 个值。
    """

def set_current_report_period(self, joint_motor_index: int, period: int) -> None:
    """设置单个关节电机的电流上报周期（仅 O12）。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        period: 上报周期（毫秒）。
    """

def set_all_current_report_periods(self, periods: List[int]) -> None:
    """批量设置所有关节电机的电流上报周期（仅 O12）。
    
    Args:
        periods: 上报周期列表。必须包含 12 个值。
    """
```

## 完整示例

```python
from omnihand import OmniHandPro2025, HandType, Finger

# 创建手部实例
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("初始化 OmniHand Pro 2025 失败")
    exit(1)

# 设置关节角度
angles = [0.3, -0.5, -0.3, -0.5, 0.0, 0.6, 0.7, 0.0, 0.6, 0.7, 0.7, 0.7]  # 12 个关节角度（单位：弧度）
hand.set_all_active_joint_angles(angles)
print(f"已设置关节角度: {angles} (rad)")

# 获取 3D 触觉传感器数据
thumb_data = hand.get_tactile_sensor_3d_data(Finger.THUMB)
print(f"拇指法向力: {thumb_data.normal_force} (0.1N)")
```

## 速度控制

```python
def set_joint_velocity(self, joint_motor_index: int, velocity: int) -> None:
    """设置单个关节电机的速度。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        velocity: 目标速度。
    """

def get_joint_velocity(self, joint_motor_index: int) -> int:
    """获取单个关节电机的速度。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        int: 当前速度。
    """

def set_all_joint_velocities(self, velocities: List[int]) -> None:
    """批量设置所有关节电机的速度。
    
    Args:
        velocities: 目标速度列表。必须包含 12 个值。
    """

def get_all_joint_velocities(self) -> List[int]:
    """批量获取所有关节电机的速度。
    
    Returns:
        List[int]: 当前速度列表。返回 12 个值。
    """
```

## 控制模式

```python
def set_control_mode(self, joint_motor_index: int, mode: int) -> None:
    """设置单个关节电机的控制模式。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        mode: 控制模式（参见 ControlMode）。
    
    Note:
        - 所有控制模式都支持
        - 纯 TORQUE 模式不支持
    """

def get_control_mode(self, joint_motor_index: int) -> int:
    """获取单个关节电机的控制模式。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        int: 当前控制模式（参见 ControlMode）。
    """

def set_all_control_modes(self, ctrl_modes: List[int]) -> None:
    """批量设置所有关节电机的控制模式。
    
    Args:
        ctrl_modes: 控制模式列表。必须包含 12 个值。
    """

def get_all_control_modes(self) -> List[int]:
    """批量获取所有关节电机的控制模式。
    
    Returns:
        List[int]: 控制模式列表。返回 12 个值。
    """
```

## 电流阈值控制

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None:
    """设置单个关节电机的电流阈值。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        current_threshold: 电流阈值。
    """

def get_current_threshold(self, joint_motor_index: int) -> int:
    """获取单个关节电机的电流阈值。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        int: 当前电流阈值。
    """

def set_all_current_thresholds(self, current_thresholds: List[int]) -> None:
    """批量设置所有关节电机的电流阈值。
    
    Args:
        current_thresholds: 电流阈值列表。必须包含 12 个值。
    """

def get_all_current_thresholds(self) -> List[int]:
    """批量获取所有关节电机的电流阈值。
    
    Returns:
        List[int]: 电流阈值列表。返回 12 个值。
    """
```

## 混合控制

```python
def mix_ctrl_joint_motor(self, mix_ctrls: List[MixCtrl]) -> None:
    """以混合模式控制关节电机。
    
    Args:
        mix_ctrls: 混合控制参数列表。
    
    Note:
        纯力控模式 (TORQUE) 不支持。请使用混合控制模式。
    """
```

## 错误处理

```python
def get_error_report(self, joint_motor_index: int) -> JointMotorErrorReport:
    """获取单个关节电机的错误报告。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        JointMotorErrorReport: 错误报告结构。
    """

def get_all_error_reports(self) -> List[JointMotorErrorReport]:
    """批量获取所有关节电机的错误报告。
    
    Returns:
        List[JointMotorErrorReport]: 错误报告列表。返回 12 个值。
    """

def set_error_report_period(self, joint_motor_index: int, period: int) -> None:
    """设置单个关节电机的错误报告周期。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        period: 上报周期（毫秒）。
    """

def set_all_error_report_periods(self, periods: List[int]) -> None:
    """批量设置所有关节电机的错误报告周期。
    
    Args:
        periods: 上报周期列表。必须包含 12 个值。
    """
```

## 温度监控

```python
def get_temperature_report(self, joint_motor_index: int) -> int:
    """获取单个关节电机的温度报告。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        int: 当前温度值（摄氏度）。
    """

def get_all_temperature_reports(self) -> List[int]:
    """批量获取所有关节电机的温度报告。
    
    Returns:
        List[int]: 温度值列表。返回 12 个值。
    """

def set_temperature_report_period(self, joint_motor_index: int, period: int) -> None:
    """设置单个关节电机的温度上报周期（仅 O12）。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        period: 上报周期（毫秒）。
    """

def set_all_temperature_report_periods(self, periods: List[int]) -> None:
    """批量设置所有关节电机的温度上报周期（仅 O12）。
    
    Args:
        periods: 上报周期列表。必须包含 12 个值。
    """
```

## 电流监控

```python
def get_current_report(self, joint_motor_index: int) -> int:
    """获取单个关节电机的电流报告。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
    
    Returns:
        int: 当前电流值。
    """

def get_all_current_reports(self) -> List[int]:
    """批量获取所有关节电机的电流报告。
    
    Returns:
        List[int]: 电流值列表。返回 12 个值。
    """

def set_current_report_period(self, joint_motor_index: int, period: int) -> None:
    """设置单个关节电机的电流上报周期（仅 O12）。
    
    Args:
        joint_motor_index: 关节电机索引（1-12）。
        period: 上报周期（毫秒）。
    """

def set_all_current_report_periods(self, periods: List[int]) -> None:
    """批量设置所有关节电机的电流上报周期（仅 O12）。
    
    Args:
        periods: 上报周期列表。必须包含 12 个值。
    """
```

## 调试功能

```python
def show_data_details(self, show: bool) -> None:
    """切换显示原始发送/接收数据详情。
    
    Args:
        show: 是否显示数据详情。
    """
```

## 相关文档

- [OmniHand Pro 2025 (O12) 运动学求解器 Python API](API_KINEMATICS_PYTHON_O12.md) - 运动学计算
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
