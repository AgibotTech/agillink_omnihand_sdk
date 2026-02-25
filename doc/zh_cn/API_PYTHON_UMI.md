# OmniHand Dex UMI (O10 UMI) Python API

## 概述

**OmniHand Dex UMI (O10 UMI)** 是一款使用 UMI 协议的 10 自由度灵巧手。本文档描述了用于控制和交互 OmniHand Dex UMI (O10 UMI) 设备的 Python API。

**主要特性：**
- 10 个主动自由度
- 1D 触觉传感器（手指、手心，无手背）
- UMI 协议（Pn1-Pn8 寄存器）
- 主动查询位置信息（无周期上报）
- 仅支持 CAN（ZLG USB CANFD）通信
- 支持 SocketCAN（仅 Linux）
- **只读位置信息**（不支持位置/速度/力矩控制）

## 导入

```python
from omnihand import OmniHandDexUMI, HandType, Finger
```

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_id: int = 0,
                canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """创建灵巧手对象（推荐：零配置）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 设备 ID，默认为 1。
        canfd_device_id: USB CANFD 适配器设备索引，默认为 0。
        channel_id: CAN 通道索引，默认为 0。
    
    Returns:
        OmniHandDexUMI: 灵巧手实例。
    """
```

### HCAN USB CANFD

```python
@staticmethod
def create_hand_by_hcan(hand_type: HandType = HandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """创建灵巧手对象（HCAN USB CANFD，通过设备 ID）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 手部设备 ID，默认为 1。
        canfd_device_id: HCAN 设备索引，默认为 0。
        canfd_channel_id: CAN 通道索引，默认为 0。双通道适配器(USBCANFD-200U）： can0=0, can1=1；单通道适配器(USBCANFD-100U）： 始终为0。
    
    Returns:
        OmniHandDexUMI: 灵巧手实例。
    """
```

**示例：**
```python
hand = OmniHandDexUMI.create_hand_by_hcan(
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
                canfd_canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """创建灵巧手对象（HCAN USB CANFD，通过序列号）。

    Args:
        hand_type: 手型。
        device_id: 手部设备 ID。
        hcan_serial_number: HCAN 设备序列号（支持部分匹配）。
        canfd_channel_id: CAN 通道索引，默认为 0。双通道适配器(USBCANFD-200U）： can0=0, can1=1；单通道适配器(USBCANFD-100U）： 始终为0。
    
    Returns:
        OmniHandDexUMI: 灵巧手实例，如果找不到设备则返回 None。
    """
```

## UMI 特有接口

### 位置校准

```python
def set_min_position_calibration(self) -> None:
    """设置最小位置校准（UMI 协议 Pn7，子寄存器 0x00）。
    
    Note:
        这是位置校准的只写操作。设备应处于最小位置时调用此函数。
    """

def set_max_position_calibration(self) -> None:
    """设置最大位置校准（UMI 协议 Pn7，子寄存器 0x01）。
    
    Note:
        这是位置校准的只写操作。设备应处于最大位置时调用此函数。
    """
```

### 位置查询

```python
def get_joint_position(self, joint_motor_index: int) -> int:
    """获取单个关节电机位置（UMI 协议 Pn3，子寄存器 0x01-0x0A）。
    
    Args:
        joint_motor_index: 关节电机索引（0-9）。
    
    Returns:
        int: 关节位置（0-4096）。
    """

def get_all_joint_positions(self) -> List[int]:
    """获取所有关节电机位置（UMI 协议 Pn3，子寄存器 0x00）。
    
    Returns:
        List[int]: 10 个关节位置（0-4096）。
    """
```


### 触觉传感器数据

```python
def get_all_tactile_sensor_data_raw(self) -> List[TactileSensorData]:
    """一次性获取所有 1D 触觉传感器原始数据。
    
    Returns:
        List[TactileSensorData]: TactileSensorData 结构列表。
    
    Note:
        这返回完整分辨率数据。使用 UMI 协议 Pn6。
    """

def get_tactile_sensor_data_raw(self, eFinger: Finger) -> TactileSensorData:
    """获取单个传感器的 1D 触觉传感器原始数据。
    
    Args:
        eFinger: 手指/手心枚举值。
    
    Returns:
        TactileSensorData: 包含完整分辨率数据的 TactileSensorData 结构。
    
    Note:
        使用 UMI 协议 Pn6。
    """
```

## 重要注意事项

1. **无位置/速度/力矩控制**：OmniHand Dex UMI (O10 UMI) 是**只读**设备。它不支持位置、速度或力矩控制。它仅通过主动查询提供位置信息。

2. **主动查询位置**：UMI 协议支持通过 `get_joint_position()` 或 `get_all_joint_positions()` 主动查询关节位置。位置值范围为 0-4096。

3. **位置校准**：位置校准（最小/最大）是只写操作。调用校准函数时，设备应处于适当的位置。

## 完整示例

```python
from omnihand import OmniHandDexUMI, HandType, Finger
import time

# 创建手部实例
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("初始化 OmniHand Dex UMI (O10 UMI) 失败")
    exit(1)

# 查询关节位置（主动查询）
positions = hand.get_all_joint_positions()
print(f"所有关节位置 ({len(positions)} 个值): {positions}")

# 获取触觉传感器数据
tactile_data = hand.get_all_tactile_sensor_data_raw()
print(f"触觉传感器: {len(tactile_data)} 个传感器")
```

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
