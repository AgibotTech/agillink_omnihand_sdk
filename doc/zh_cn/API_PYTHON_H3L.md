# OmniHand 3 Lite (H3L) Python API

## 概述

**OmniHand 3 Lite (H3L)** 是一款 4 自由度轻量级灵巧手。本文档描述了用于控制和交互 OmniHand 3 Lite 设备的 Python API。

**主要特性：**
- 4 个主动自由度
- 电机位置范围：0–4096（int16 电机 ticks）
- 支持 CAN（ZLG USB CANFD / HCAN）通信
- 支持 SocketCAN（仅 Linux）
- 支持 ZLG CAN TCP（仅 Linux x64 / Windows）
- **无触觉传感器**
- **无运动学求解器**：不支持角度控制（`set_all_active_joint_angles` 为桩实现）。请使用电机位置控制（`set_all_joint_motor_positions`）

## 导入

```python
from omnihand import OmniHand3Lite, HandType, ControlMode
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
```

### 电机位置控制

```python
def set_joint_motor_posi(self, joint_motor_index: int, posi: int) -> int:
    """设置单个关节电机位置。

    Args:
        joint_motor_index: 关节电机索引（1–4）。
        posi: 电机位置（0–4095）。

    Returns:
        int: 设备返回的实际位置。-1 表示失败。
    """

def set_all_joint_motor_positions(self, positions: List[int]) -> List[int]:
    """批量设置所有关节电机位置。

    Args:
        positions: 目标位置列表，4 个 int，范围 0–4095。

    Returns:
        List[int]: 设备返回的实际位置列表。失败返回空列表。
    """

def get_joint_motor_posi(self, joint_motor_index: int) -> int:
    """获取单个关节电机位置。

    Args:
        joint_motor_index: 关节电机索引（1–4）。

    Returns:
        int: 当前电机位置（0–4095）。
    """

def get_all_joint_motor_positions(self) -> List[int]:
    """批量获取所有关节电机位置。

    Returns:
        List[int]: 当前电机位置列表（4 个值）。
    """
```

### 关节角度控制（不可用）

```python
# 以下方法在 H3L 中为桩实现，不可用。请使用电机位置控制。
def set_all_active_joint_angles(self, angles: List[float]) -> List[float]: ...
def get_all_active_joint_angles(self) -> List[float]: ...
```

### 速度控制

```python
def set_joint_motor_velo(self, joint_motor_index: int, velo: int) -> None: ...
def get_joint_motor_velo(self, joint_motor_index: int) -> int: ...
def set_all_joint_motor_velo(self, velocities: List[int]) -> None: ...
def get_all_joint_motor_velo(self) -> List[int]: ...
```

### 电流阈值控制

```python
def set_current_threshold(self, joint_motor_index: int, current_threshold: int) -> None: ...
def get_current_threshold(self, joint_motor_index: int) -> int: ...
def set_all_current_thresholds(self, thresholds: List[int]) -> None: ...
def get_all_current_thresholds(self) -> List[int]: ...
```

### 混合控制

```python
def mix_ctrl_joint_motor(self, mix_ctrls: List[MixCtrl]) -> None:
    """以混合模式控制关节电机。

    MixCtrl 字段：
        joint_index: 关节索引（1–4）
        ctrl_mode: 控制模式（仅 ControlMode.POSITION_TORQUE 可用）
        tgt_posi: 目标位置（0–4095）
        tgt_torque: 目标电流（mA）
    """
```

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

### 调试功能

```python
def show_data_details(self, show: bool) -> None:
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

# 设置电机位置（4 个关节，0–4095）
positions = [2048, 2048, 2048, 2048]
echo = hand.set_all_joint_motor_positions(positions)
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
