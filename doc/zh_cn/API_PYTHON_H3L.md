# OmniHand 3 Lite (H3L) Python API

## 概述

**OmniHand 3 Lite (H3L)** 是一款 4 自由度轻量级灵巧手。本文档描述了用于控制和交互 OmniHand 3 Lite 设备的 Python API。

**主要特性：**
- 4 个主动自由度
- 电机位置范围：0–4096（int16 电机 ticks）
- 支持 CAN（ZLG USB CANFD / HCAN）通信
- 支持 SocketCAN（仅 Linux）
- 支持 ZLG CAN TCP（仅 Linux x64 / Windows）
- 支持 RS485 通信（默认波特率 460800）
- 支持 6 个 U16 一维触觉传感器：五指和掌心
- 支持主动关节角控制，并可通过 `OmniHand3LiteSolver` 离线进行电机位置/角度换算
- 暂不支持包含主动与被动关节的完整运动学；H3L 不支持 `get_all_joint_angles()` 和 `get_all_joint_angles_from_motor_positions()`

H3L Python 接口有意不暴露标定、零点设置、参数保存和固件维护等破坏性能力。

## 导入

```python
from omnihand.omnihand_3_lite import (
    OmniHand3Lite,
    OmniHand3LiteGesture,
    OmniHand3LiteSolver,
    TactileSensorDataU16,
)
from omnihand import Finger, HandType
```

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """创建灵巧手对象（推荐：零配置）。

    Args:
        hand_type: 手型，默认为左手。
        hand_device_id: 设备 ID，默认为 1。
        canfd_device_id: USB CANFD 适配器设备索引，默认为 0。
        canfd_channel_id: CAN 通道索引，默认为 0（USBCANFD-200U 有 2 个通道）。

    Returns:
        OmniHand3Lite: 灵巧手实例。

    Note:
        ✅ 推荐：零配置，开箱即用。无需 root 权限。
    """
```

### 通过 ZLG 序列号创建

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType,
                hand_device_id: int,
                usbcanfd_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """创建灵巧手对象（ZLG USB CANFD，通过序列号）。

    Args:
        hand_type: 手型。
        hand_device_id: 手部设备 ID。
        usbcanfd_serial_number: USB CANFD 设备序列号（支持部分匹配）。
        canfd_channel_id: CAN 通道索引，默认为 0。

    Returns:
        OmniHand3Lite: 灵巧手实例，如果找不到设备则返回 None。
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """创建灵巧手对象（HCAN USB CANFD，通过设备 ID）。

    Args:
        hand_type: 手型，默认为左手。
        hand_device_id: 手部设备 ID，默认为 1。
        canfd_device_id: HCAN 设备索引，默认为 0。
        canfd_channel_id: CAN 通道索引，默认为 0。

    Returns:
        OmniHand3Lite: 灵巧手实例。
    """
```

### 通过 HCAN 序列号创建

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType,
                hand_device_id: int,
                hcan_serial_number: str,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """创建灵巧手对象（HCAN USB CANFD，通过序列号）。

    Args:
        hand_type: 手型。
        hand_device_id: 手部设备 ID。
        hcan_serial_number: HCAN 设备序列号（支持部分匹配）。
        canfd_channel_id: CAN 通道索引，默认为 0。

    Returns:
        OmniHand3Lite: 灵巧手实例，如果找不到设备则返回 None。
    """
```

### SocketCAN（仅 Linux）

```python
@staticmethod
def create_hand_socketcan(hand_type: HandType,
                hand_device_id: int,
                can_interface: str = "can0") -> 'OmniHand3Lite':
    """创建灵巧手对象（SocketCAN 通信，仅 Linux）。

    Args:
        hand_type: 手型。
        hand_device_id: 手部设备 ID。
        can_interface: CAN 接口名（如 "can0"、"can1"），默认 "can0"。

    Returns:
        OmniHand3Lite: 灵巧手实例。
    """
```

### ZLG CAN TCP（仅部分平台）

```python
@staticmethod
def create_hand_by_zlgcan_tcp(hand_type: HandType,
                hand_device_id: int,
                tcp_host: str,
                tcp_port: int,
                canfd_channel_id: int = 0) -> 'OmniHand3Lite':
    """创建灵巧手对象（ZLG CANFD over TCP）。

    仅支持 Windows 和 Linux x64（不支持 Linux aarch64/arm64）。

    Args:
        hand_type: 手型。
        hand_device_id: 手部设备 ID。
        tcp_host: TCP 服务器 IP 或主机名。
        tcp_port: TCP 服务器端口。
        canfd_channel_id: CAN 通道索引，默认为 0。

    Returns:
        OmniHand3Lite: 灵巧手实例。
    """
```

### RS485

```python
@staticmethod
def create_hand_by_rs485(
    hand_type: int = 0,
    hand_device_id: int = 1,
    serial_port: str = "/dev/ttyUSB0",
    baud_rate: int = 460800,
) -> "OmniHand3Lite": ...
```

## 主要接口

### 初始化

```python
def init(self) -> bool:
    """初始化设备。创建实例后必须调用。失败返回 False。"""
```

### 手势控制

```python
class OmniHand3LiteGesture(enum.IntEnum):
    OMNI_HAND_3_LITE_GESTURE_ALL_ZERO = 0  # 全零位
    OMNI_HAND_3_LITE_GESTURE_FIST = 1      # 握拳
    OMNI_HAND_3_LITE_GESTURE_OPEN = 2      # 张开

def set_hand_gesture(self, gesture: OmniHand3LiteGesture) -> None:
    """设置预设手势。

    Args:
        gesture: OmniHand3LiteGesture 枚举值。

    预设手势位置以右手为基准，左手时求解器自动镜像电机 1 和电机 4。
    """

def get_hand_gesture(self, gesture: OmniHand3LiteGesture) -> List[int]: ...
```

### 电机位置控制

电机索引从 1 开始，电机位置范围为 0–4096。

```python
def set_joint_position(self, joint_motor_index: int, posi: int) -> None:
    """设置单个关节电机位置。

    Args:
        joint_motor_index: 关节电机索引（1–4）。
        posi: 电机位置（0–4096）。

    """

def set_all_joint_positions(self, positions: List[int]) -> List[int]:
    """批量设置所有关节电机位置。

    Args:
        positions: 目标位置列表，4 个 int，范围 0–4096。

    Returns:
        List[int]: 设备返回的实际位置列表。失败返回空列表。
    """

def get_joint_position(self, joint_motor_index: int) -> int:
    """获取单个关节电机位置。

    Args:
        joint_motor_index: 关节电机索引（1–4）。

    Returns:
        int: 当前电机位置（0–4096）。
    """

def get_all_joint_positions(self) -> List[int]:
    """批量获取所有关节电机位置。

    Returns:
        List[int]: 当前电机位置列表（4 个值）。
    """
```

### 主动关节角与离线求解器

```python
def set_all_active_joint_angles(self, angles: List[float]) -> List[float]: ...
def get_all_active_joint_angles(self) -> List[float]: ...

class OmniHand3LiteSolver:
    def __init__(self, is_left_hand: bool) -> None: ...
    def active_joint_pos_to_actuator_input(
        self, active_joint_pos: List[float]
    ) -> List[int]: ...
    def actuator_input_to_active_joint_pos(
        self, actuator_input: List[int]
    ) -> List[float]: ...
    def single_active_joint_pos_to_actuator_input(
        self, joint_index: int, angle: float
    ) -> int: ...
    def single_actuator_input_to_active_joint_pos(
        self, joint_index: int, actuator_input: int
    ) -> float: ...
    def set_hand_gesture(self, gesture: OmniHand3LiteGesture) -> List[int]: ...
```

上述求解器接口仅执行本地计算，不与设备通信。H3L 目前没有包含主动、
被动关节的完整关节角运动学。

### 速度控制

```python
def set_joint_velocity(self, joint_motor_index: int, velocity: int) -> None: ...
def get_joint_velocity(self, joint_motor_index: int) -> int: ...
def set_all_joint_velocities(self, velocities: List[int]) -> None: ...
def get_all_joint_velocities(self) -> List[int]: ...
```

### 电流阈值控制

> **注意**：O10/H3L 不允许同时使用电流阈值控制和混合控制。如需使用混合控制，请勿调用电流阈值接口。

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None: ...
def get_current_threshold(self, joint_motor_index: int) -> int: ...
def set_all_current_thresholds(self, thresholds: List[int]) -> None: ...
def get_all_current_thresholds(self) -> List[int]: ...
```

### 混合控制

```python
def mix_control_by_pt(self, positions: List[int], torques: List[int]) -> List[MixCtrl]: ...
def mix_control_by_pvt(self, positions: List[int], velocities: List[int], torques: List[int]) -> List[MixCtrl]: ...
    """以混合模式控制关节电机。

    MixCtrl 字段：
        joint_index: 关节索引（1–4）
        ctrl_mode: 控制模式（`MixControlMode.POSITION_TORQUE` 或 `POSITION_VELOCITY_TORQUE`）
        tgt_posi: 目标位置（0–4096）
        tgt_torque: 目标电流（mA）
    """
```

H3L 同时暴露带状态回复的 0x25 接口；H3L 不支持 PV 混合控制。

```python
def mix_control_with_status(self, controls: List[MixCtrl]) -> MixCtrlResult: ...
def mix_control_by_pt_with_status(
    self, joint_motor_index: int, position: int, torque: int
) -> MixCtrlResult: ...
def mix_control_by_pt_with_status(
    self, positions: List[int], torques: List[int]
) -> MixCtrlResult: ...
def mix_control_by_pvt_with_status(
    self, joint_motor_index: int, position: int, velocity: int, torque: int
) -> MixCtrlResult: ...
def mix_control_by_pvt_with_status(
    self, positions: List[int], velocities: List[int], torques: List[int]
) -> MixCtrlResult: ...
```

### 带状态回复的位置控制（0x24）

```python
def set_joint_position_with_status(
    self, joint_motor_index: int, position: int
) -> JointPosiStatus: ...
def get_joint_position_with_status(
    self, joint_motor_index: int
) -> JointPosiStatus: ...
def set_all_joint_positions_with_status(
    self, positions: List[int]
) -> AllJointPosiStatus: ...
def get_all_joint_positions_with_status(self) -> AllJointPosiStatus: ...
```

### 关节详细信息（0x26）

整手查询返回整手母线信息；单关节查询中的 `joint_index` 从 0 开始。

```python
def get_all_joint_detail_info(self) -> HandDetailInfo: ...
def get_joint_detail_info(self, joint_index: int) -> JointDetailInfo: ...
```

0x24/0x25/0x26 接口需要固件支持。未实现相应寄存器的固件会请求超时，
并返回空或默认结果对象。

### 错误处理

```python
def get_error_report(self, joint_motor_index: int) -> JointMotorErrorReport:
    """获取单个关节电机错误报告。

    Returns:
        JointMotorErrorReport: 含 stalled、overheat、over_current、motor_except、commu_except 位。
    """

def get_all_error_reports(self) -> List[JointMotorErrorReport]:
    """批量获取所有关节电机错误报告。返回 4 个值。"""
```

### 温度监控

```python
def get_temperature_report(self, joint_motor_index: int) -> int:
    """获取单个关节电机温度（°C）。"""

def get_all_temperature_reports(self) -> List[int]:
    """批量获取所有关节电机温度。返回 4 个值。"""
```

### 电流监控

```python
def get_current_report(self, joint_motor_index: int) -> int:
    """获取单个关节电机电流。"""

def get_all_current_reports(self) -> List[int]:
    """批量获取所有关节电机电流。返回 4 个值。"""
```

### 厂商与设备信息

```python
def get_vendor_info(self) -> VendorInfo: ...
def get_device_info(self) -> DeviceInfo: ...
def set_device_id(self, device_id: int) -> None: ...
```

### 产品元数据与范围

```python
def get_joint_names(self) -> List[str]: ...

@staticmethod
def get_min_max_motor_position(joint_motor_index: int) -> Int16Bound: ...
@staticmethod
def get_min_max_actual_motor_position(joint_motor_index: int) -> Int16Bound: ...
@staticmethod
def get_min_max_default_mix_ctrl_velocity(joint_motor_index: int) -> Int16Range: ...
@staticmethod
def get_min_max_default_mix_ctrl_torque(joint_motor_index: int) -> Int16Range: ...
@staticmethod
def get_min_max_activate_joint_angle(
    joint_motor_index: int, hand_type: HandType
) -> FloatBound: ...
```

### U16 1D 触觉传感器

```python
class TactileSensorDataU16:
    sensor_id: Finger
    data: List[int]

def get_num_of_tactile_sensors(self) -> int: ...
def get_num_of_tactile_points(self, finger_index: int) -> int: ...
def init_tactile_points_map(self) -> bool: ...
def get_len_of_tactile_datum(self, finger_index: int) -> int: ...
def get_num_of_replied_tactile_frames(self, finger_index: int) -> int: ...
def get_sn_of_tactile_sensor(self, finger_index: int) -> str: ...
def get_sensor_data_length(self, finger_index: int) -> int: ...  # 已弃用别名
def get_sensor_order(self) -> List[Finger]: ...
def get_tactile_sensor_data(
    self, finger_index: int
) -> TactileSensorDataU16: ...
def get_tactile_sensor_data_raw(
    self, finger_index: int
) -> TactileSensorDataU16: ...
def get_all_tactile_sensor_data_raw(self) -> List[TactileSensorDataU16]: ...
```

传感器顺序依次为拇指、食指、中指、无名指、小指和掌心。依赖缓存点数前，
应先调用 `init_tactile_points_map()`。

### 通信设置

```python
def set_request_interval(self, milliseconds: int) -> None: ...
def get_request_interval(self) -> int: ...
def set_frame_recv_timeout(self, milliseconds: int) -> None: ...
def get_frame_recv_timeout(self) -> int: ...
def set_frame_send_timeout(self, milliseconds: int) -> None: ...
def get_frame_send_timeout(self) -> int: ...
```

发送超时仅适用于 CAN 传输。RS485 实现中的 setter 不执行操作，getter 返回 `0`。

### 调试功能

```python
def show_data_details(self, show: bool, callback: Callable[[str], None] | None = None) -> None:
    """切换显示原始收发数据详情。"""
```

## 完整示例

```python
from omnihand import OmniHand3Lite, HandType, OmniHand3LiteGesture

# 创建手部实例
hand = OmniHand3Lite.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("初始化 OmniHand 3 Lite 失败")
    exit(1)

# 获取厂商信息
vendor = hand.get_vendor_info()
print(vendor)

# 使用预设手势（右手基准，左手自动镜像电机1/4）
hand.set_hand_gesture(OmniHand3LiteGesture.OMNI_HAND_3_LITE_GESTURE_FIST)

# 设置电机位置（4 个关节，0–4096）
positions = [2048, 2048, 2048, 2048]
echo = hand.set_all_joint_positions(positions)
print(f"电机位置：{echo}")

# 获取温度
temps = hand.get_all_temperature_reports()
print(f"温度：{temps}")
```

## Demo 文件

SDK 发布包中提供了可直接运行的 Python demo 脚本（位于 `python/demo/omnihand_3_lite/`）：

| Demo | 文件 |
|------|------|
| CAN FD（按 ID 连接） | [demo_canfd_id.py](../../../python/demo/omnihand_3_lite/demo_canfd_id.py) |
| CAN FD（按串号连接） | [demo_canfd_serial.py](../../../python/demo/omnihand_3_lite/demo_canfd_serial.py) |
| SocketCAN | [demo_socketcan.py](../../../python/demo/omnihand_3_lite/demo_socketcan.py) |
| ZLG CAN TCP | [demo_zlgcan_tcp.py](../../../python/demo/omnihand_3_lite/demo_zlgcan_tcp.py) |
| 电机位置控制 | [demo_set_motor.py](../../../python/demo/omnihand_3_lite/demo_set_motor.py) |
| 硬件信息 | [demo_get_hardware_info.py](../../../python/demo/omnihand_3_lite/demo_get_hardware_info.py) |
| 双手控制（多 CAN） | [demo_set_motor_via_multicans.py](../../../python/demo/omnihand_3_lite/demo_set_motor_via_multicans.py) |
| 可靠性测试 | [demo_set_get_position_reliability.py](../../../python/demo/omnihand_3_lite/demo_set_get_position_reliability.py) |
| 可靠性测试 CSV | [demo_set_get_reliability_csv.py](../../../python/demo/omnihand_3_lite/demo_set_get_reliability_csv.py) |

## 相关文档

- [OmniHand 3 Lite (H3L) C++ API](API_CPP_H3L.md)
- [OmniHand 3 Lite (H3L) ROS2 接口](API_ROS2_H3L.md)
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md)
