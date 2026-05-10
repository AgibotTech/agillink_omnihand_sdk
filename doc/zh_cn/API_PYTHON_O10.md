# OmniHand 2025 (O10) Python API

## 概述

**OmniHand 2025 (O10)** 是一款 10 自由度灵巧手，配备 1D 触觉传感器。本文档描述了用于控制和交互 OmniHand 2025 设备的 Python API。

**主要特性：**
- 10 个主动 + 6 个被动自由度
- 1D 触觉传感器（手指、手心、手背）
- 电机位置范围：0-4096
- 支持 CAN（ZLG USB CANFD）和 RS485 通信
- 支持 SocketCAN（仅 Linux）

## 导入

```python
from omnihand import OmniHand2025, HandType, Finger, ControlMode
```

**说明：** 工厂方法中的 `hand_device_id`、`canfd_device_id`、`canfd_channel_id` 在 Python 侧为 `int`；绑定层传入 C++ 时为 `uint8_t`，与 `omnihand_2025.h` 一致。

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """创建灵巧手对象（推荐：零配置）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 设备 ID，默认为 1。
        canfd_device_id: USB CANFD 适配器设备索引，默认为 0。
        channel_id: CAN 通道索引，默认为 0（USBCANFD-200U 有 2 个通道）。
    
    Returns:
        OmniHand2025: 灵巧手实例。
    
    Note:
        ✅ 推荐：零配置，开箱即用。无需 root 权限。
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """创建灵巧手对象（HCAN USB CANFD，通过设备 ID）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 手部设备 ID，默认为 1。
        canfd_device_id: HCAN 设备索引，默认为 0。
        canfd_channel_id: CAN 通道索引，默认为 0。双通道适配器(USBCANFD-200U）： can0=0, can1=1；单通道适配器(USBCANFD-100U）： 始终为0。
    
    Returns:
        OmniHand2025: 灵巧手实例。
    """
```

**示例：**
```python
hand = OmniHand2025.create_hand_by_hcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
```

### 通过 HCAN 序列号创建

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType,
                hand_device_id: int,
                hcan_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHand2025':
    """创建灵巧手对象（HCAN USB CANFD，通过序列号）。

    Args:
        hand_type: 手型。
        device_id: 手部设备 ID。
        hcan_serial_number: HCAN 设备序列号（支持部分匹配）。
        canfd_channel_id: CAN 通道索引，默认为 0。双通道适配器(USBCANFD-200U）： can0=0, can1=1；单通道适配器(USBCANFD-100U）： 始终为0。
    
    Returns:
        OmniHand2025: 灵巧手实例，如果找不到设备则返回 None。
    """
```

### RS485 通信（仅 O10）

```python
@staticmethod
def create_hand_by_rs485(hand_type: HandType,
                      hand_device_id: int,
                      uart_port: str,
                      baudrate: int = 460800) -> 'OmniHand2025':
    """使用 RS485 通信创建灵巧手对象（仅 O10）。

    Args:
        hand_type: 手型。
        device_id: 手部设备 ID。
        uart_port: 串口路径（例如："/dev/ttyUSB0"）。
        baudrate: 波特率，默认为 460800。
    
    Returns:
        OmniHand2025: 灵巧手实例。
    """
```

## 主要接口

### 关节角度控制

#### 关节角度 I/O 顺序（右手）

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

#### 关节角度 I/O 顺序（左手）

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

```python
def set_all_active_joint_angles(self, angles: List[float]) -> None:
    """设置所有主动关节的角度（单位：弧度）。
    
    Args:
        angles: 关节角度列表（单位：弧度）。必须包含 10 个值。
               顺序参考上表（索引 1-10）。
    
    Note:
        有关具体顺序和限制，请参考上表。
    """

def get_all_active_joint_angles(self) -> List[float]:
    """获取所有主动关节的角度（单位：弧度）。
    
    Returns:
        List[float]: 关节角度列表（单位：弧度）。返回 10 个值。
                    顺序参考上表（索引 1-10）。
    
    Note:
        有关具体顺序和限制，请参考上表。
    """

def get_all_joint_angles(self) -> List[float]:
    """获取所有关节的角度（包括主动和被动关节，单位：弧度）。
    
    Returns:
        List[float]: 所有关节角度列表（单位：弧度）。返回 16 个值（10 个主动 + 6 个被动）。
                    前 10 个值为主动关节（顺序参考上表），后 6 个值为被动关节。
    
    Note:
        被动关节的顺序和限制，请参考资产模型文件。
    """
```

### 电机位置控制

**注意**：OmniHand 2025 (O10) 电机位置范围为 **0-4096**。

```python
def set_joint_position(self, joint_motor_index: int, position: int) -> None:
    """设置单个关节电机的位置。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
        position: 目标位置（范围：0-4096）。
    """

def get_joint_position(self, joint_motor_index: int) -> int:
    """获取单个关节电机的位置。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
    
    Returns:
        int: 当前位置（范围：0-4096）。
    """

def set_all_joint_positions(self, positions: List[int]) -> List[int]:
    """批量设置所有关节电机的位置并返回实际位置。
    
    Args:
        positions: 目标位置列表。必须包含 10 个值，每个值在 0-4096 范围内。
    
    Returns:
        设备响应的实际位置列表。失败时返回空列表。
    """

def get_all_joint_positions(self) -> List[int]:
    """批量获取所有关节电机的位置。
    
    Returns:
        List[int]: 当前位置列表。返回 10 个值，每个值在 0-4096 范围内。
    """
```

### 触觉传感器数据

```python
def get_tactile_sensor_data(self, eFinger: Finger) -> List[int]:
    """获取指定部位的触觉传感器数据（仅 O10）。
    
    Args:
        eFinger: 手指/手心枚举值。
    
    Returns:
        List[int]: 指定部位的触觉传感器数据列表。
                   - 手指：返回 16 个数据点
                   - 手心：返回 25 个数据点（降采样）
                   - 手背：返回 25 个数据点（降采样）
    
    Note:
        数据单位：1g，最大值：255g，采样频率：10Hz
    """

def get_all_tactile_sensor_data_raw(self) -> List[TactileSensorData]:
    """一次性获取所有 1D 触觉传感器原始数据。
    
    Returns:
        List[TactileSensorData]: TactileSensorData 结构列表。
    
    Note:
        这返回完整分辨率数据，与返回降采样数据的 get_tactile_sensor_data() 不同。
    """

def get_tactile_sensor_data_raw(self, eFinger: Finger) -> TactileSensorData:
    """获取单个传感器的 1D 触觉传感器原始数据。
    
    Args:
        eFinger: 手指/手心枚举值。
    
    Returns:
        TactileSensorData: 包含完整分辨率数据的 TactileSensorData 结构。
    """
```

**⚠️ 重要建议：获取多个传感器数据时，强烈推荐使用 `get_all_tactile_sensor_data_raw()` 而不是循环调用 `get_tactile_sensor_data_raw()`。**

以下表格对比了两种方式的差异：

| 方面 | 循环调用 `get_tactile_sensor_data_raw()` (7次) | 使用 `get_all_tactile_sensor_data_raw()` (1次) |
|------|-----------------------------------------------|-----------------------------------------------|
| **请求间隔累积** | 7 × interval_ms (例如：7 × 3ms = 21ms) | 1 × interval_ms (例如：1 × 3ms = 3ms) |
| **独立超时检查次数** | 7次（每次请求都有独立超时风险） | 1次（多帧接收在单次请求内完成） |
| **CAN总线占用** | 7次请求 + 7次响应 = 14次帧传输 | 1次请求 + 5次响应 = 6次帧传输 |
| **通信开销** | 高（14次帧传输） | 低（6次帧传输） |
| **超时风险累积** | 高（7次独立超时风险叠加） | 低（1次请求，多帧接收） |
| **设备处理压力** | 高（7次独立处理请求） | 低（1次批量处理） |
| **总耗时** | 长（累积请求间隔 + 多次通信） | 短（单次请求 + 多帧接收） |

**💡 建议：在需要获取多个传感器数据时，始终优先使用 `get_all_tactile_sensor_data_raw()`，以获得更好的性能和可靠性。**

@staticmethod
def get_sensor_data_length(finger_index: int) -> int:
    """获取特定手指的传感器数据长度（静态方法）。
    
    Args:
        finger_index: 手指枚举值（Finger）。
    
    Returns:
        int: 传感器数据长度（字节）。
    """

@staticmethod
def get_sensor_order() -> List[int]:
    """获取传感器顺序向量（静态方法）。
    
    Returns:
        List[int]: 传感器顺序向量的引用。
    """
```

## 速度控制

```python
def set_joint_velocity(self, joint_motor_index: int, velocity: int) -> None:
    """设置单个关节电机的速度。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
        velocity: 目标速度。
    
    Note:
        串口通信（RS485）不支持此接口。
    """

def get_joint_velocity(self, joint_motor_index: int) -> int:
    """获取单个关节电机的速度。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
    
    Returns:
        int: 当前速度。
    
    Note:
        串口通信（RS485）不支持此接口。
    """

def set_all_joint_velocities(self, velocities: List[int]) -> None:
    """批量设置所有关节电机的速度。
    
    Args:
        velocities: 目标速度列表。必须包含 10 个值。
    """

def get_all_joint_velocities(self) -> List[int]:
    """批量获取所有关节电机的速度。
    
    Returns:
        List[int]: 当前速度列表。返回 10 个值。
    """
```

## 控制模式

O10 不支持通过 `set_control_mode` 指令切换控制模式，默认工作在**位置控制模式**。可通过混合控制指令 `mix_ctrl_joint_motor` 实现多种控制方式，支持以下控制模式：

| 模式枚举 | 值 | 说明 |
|---|---|---|
| `ControlMode.POSITION` | 0 | 位置控制（默认） |
| `ControlMode.POSITION_TORQUE` | 4 | 位置 + 力矩混合控制 |
| `ControlMode.POSITION_VELOCITY_TORQUE` | 6 | 位置 + 速度 + 力矩混合控制（**暂未开放**） |

> **非标单位说明**：`POSITION_TORQUE` 模式中的"力矩"实际对应电机电流值，单位为 **mA**，范围 **0–1000**，而非 ROS2 标准的 N·m。

## 电流阈值控制

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None:
    """设置单个关节电机的电流阈值。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
        current_threshold: 电流阈值。
    
    Note:
        串口通信（RS485）不支持此接口。
    """

def get_current_threshold(self, joint_motor_index: int) -> int:
    """获取单个关节电机的电流阈值。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
    
    Returns:
        int: 当前电流阈值。
    
    Note:
        串口通信（RS485）不支持此接口。
    """

def set_all_current_thresholds(self, current_thresholds: List[int]) -> None:
    """批量设置所有关节电机的电流阈值。
    
    Args:
        current_thresholds: 电流阈值列表。必须包含 10 个值。
    
    Note:
        串口通信（RS485）不支持此接口。
    """

def get_all_current_thresholds(self) -> List[int]:
    """批量获取所有关节电机的电流阈值。
    
    Returns:
        List[int]: 电流阈值列表。返回 10 个值。
    
    Note:
        串口通信（RS485）不支持此接口。
    """
```

## 混合控制

> **注意**：O10/H3L 的混合控制中，`tgt_torque` 字段实际对应电机电流，单位为 **mA**，范围 **0–1000**（非标准 N·m）。`POSITION_VELOCITY_TORQUE` 模式暂未开放（速度值当前被内部写死）。

```python
def mix_ctrl_joint_motor(self, mix_ctrls: List[MixCtrl]) -> None:
    """以混合模式控制关节电机。
    
    Args:
        mix_ctrls: 混合控制参数列表。
            - ctrl_mode: 控制模式（仅 POSITION_TORQUE 可用）
            - tgt_posi: 目标位置（0–4095 编码器原始值）
            - tgt_torque: 目标电流（单位 mA，范围 0–1000，非 N·m）
    
    Note:
        纯力控模式 (TORQUE) 不支持。
        串口通信（RS485）不支持此接口。
    """
```

## 错误处理

```python
def get_error_report(self, joint_motor_index: int) -> JointMotorErrorReport:
    """获取单个关节电机的错误报告。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
    
    Returns:
        JointMotorErrorReport: 错误报告结构。
    """

def get_all_error_reports(self) -> List[JointMotorErrorReport]:
    """批量获取所有关节电机的错误报告。
    
    Returns:
        List[JointMotorErrorReport]: 错误报告列表。返回 10 个值。
    """

def set_error_report_period(self, joint_motor_index: int, period: int) -> None:
    """设置单个关节电机的错误报告周期。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
        period: 上报周期（毫秒）。
    """

def set_all_error_report_periods(self, periods: List[int]) -> None:
    """批量设置所有关节电机的错误报告周期。
    
    Args:
        periods: 上报周期列表。必须包含 10 个值。
    """
```

## 温度监控

```python
def get_temperature_report(self, joint_motor_index: int) -> int:
    """获取单个关节电机的温度报告。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
    
    Returns:
        int: 当前温度值（摄氏度）。
    """

def get_all_temperature_reports(self) -> List[int]:
    """批量获取所有关节电机的温度报告。
    
    Returns:
        List[int]: 温度值列表。返回 10 个值。
    """
```

**注意**：OmniHand 2025 (O10) 不支持设置温度上报周期。此功能仅适用于 OmniHand Pro 2025 (O12)。

## 电流监控

```python
def get_current_report(self, joint_motor_index: int) -> int:
    """获取单个关节电机的电流报告。
    
    Args:
        joint_motor_index: 关节电机索引（1-10）。
    
    Returns:
        int: 当前电流值。
    """

def get_all_current_reports(self) -> List[int]:
    """批量获取所有关节电机的电流报告。
    
    Returns:
        List[int]: 电流值列表。返回 10 个值。
    """
```

**注意**：OmniHand 2025 (O10) 不支持设置电流上报周期。此功能仅适用于 OmniHand Pro 2025 (O12)。

## 调试功能

```python
def show_data_details(self, show: bool) -> None:
    """切换显示原始发送/接收数据详情。
    
    Args:
        show: 是否显示数据详情。
    """
```

## 完整示例

```python
from omnihand import OmniHand2025, HandType, Finger

# 创建手部实例
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("初始化 OmniHand 2025 失败")
    exit(1)

# 获取厂商信息
vendor = hand.get_vendor_info()
print(vendor)

# 设置关节角度
angles = [0.0, 0.0, 0.5, 0.0, 0.8, 0.8, 0.0, 0.8, 0.0, 0.8]  # 10 个关节角度（单位：弧度）
hand.set_all_active_joint_angles(angles)
print(f"已设置关节角度: {angles} (rad)")

# 获取触觉传感器数据
thumb_data = hand.get_tactile_sensor_data(Finger.THUMB)
print(f"拇指传感器数据: {len(thumb_data)} 个点")
```

## Demo 文件

SDK 发布包中提供了可直接运行的 Python demo 脚本（位于 `python/demo/omnihand_2025/`）：

| Demo | 文件 |
|------|------|
| CAN FD（按 ID 连接） | [demo_canfd_id.py](../../../python/demo/omnihand_2025/demo_canfd_id.py) |
| CAN FD（按串号连接） | [demo_canfd_serial.py](../../../python/demo/omnihand_2025/demo_canfd_serial.py) |
| SocketCAN | [demo_socketcan.py](../../../python/demo/omnihand_2025/demo_socketcan.py) |
| ZLG CAN TCP | [demo_zlgcan_tcp.py](../../../python/demo/omnihand_2025/demo_zlgcan_tcp.py) |
| 位置+力矩混合控制 | [demo_mix_ctrl_pos_torque.py](../../../python/demo/omnihand_2025/demo_mix_ctrl_pos_torque.py) |
| 位置+速度+力矩混合控制 | [demo_mix_ctrl_pos_vel_torque.py](../../../python/demo/omnihand_2025/demo_mix_ctrl_pos_vel_torque.py) |
| 电流监控 | [demo_monitor_current.py](../../../python/demo/omnihand_2025/demo_monitor_current.py) |
| 温度监控 | [demo_monitor_temperature.py](../../../python/demo/omnihand_2025/demo_monitor_temperature.py) |
| 错误监控 | [demo_monitor_error.py](../../../python/demo/omnihand_2025/demo_monitor_error.py) |
| 角度控制 | [demo_set_angle.py](../../../python/demo/omnihand_2025/demo_set_angle.py) |
| 电机位置控制 | [demo_set_motor.py](../../../python/demo/omnihand_2025/demo_set_motor.py) |
| 运动序列 | [demo_set_motion.py](../../../python/demo/omnihand_2025/demo_set_motion.py) |
| 触觉传感器 | [demo_tactile_sensor.py](../../../python/demo/omnihand_2025/demo_tactile_sensor.py) |
| 硬件信息 | [demo_get_hardware_info.py](../../../python/demo/omnihand_2025/demo_get_hardware_info.py) |
| 双手控制（多 CAN） | [demo_set_motor_via_multicans.py](../../../python/demo/omnihand_2025/demo_set_motor_via_multicans.py) |
| 双手控制（多通道） | [demo_set_motor_via_multichannels.py](../../../python/demo/omnihand_2025/demo_set_motor_via_multichannels.py) |
| 双手控制（多 SocketCAN） | [demo_set_motor_via_multisocketcans.py](../../../python/demo/omnihand_2025/demo_set_motor_via_multisocketcans.py) |

## 相关文档

- [OmniHand 2025 (O10) 运动学求解器 Python API](API_KINEMATICS_PYTHON_O10.md) - 运动学计算
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
