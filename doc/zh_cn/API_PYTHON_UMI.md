# OmniHand Dex UMI (O10 UMI) Python API

## 概述

**OmniHand Dex UMI (O10 UMI)** 是一款使用 UMI 协议的 10 自由度灵巧手。本文档描述了用于控制和交互 OmniHand Dex UMI (O10 UMI) 设备的 Python API。

**主要特性：**
- 10 个主动自由度
- 1D 触觉传感器（手指、手心、手背）
- UMI 协议（Pn1-Pn7 寄存器）
- 通过回调函数进行位置和触觉传感器周期上报
- 仅支持 CAN（ZLG USB CANFD）通信
- 支持 SocketCAN（仅 Linux）
- **只读位置信息**（不支持位置/速度/力矩控制）

## 导入

```python
from omnihand import OmniHandDexUMI, EHandType, EFinger
```

## 工厂方法

### 推荐：ZLG USB CANFD（零配置）

```python
@staticmethod
def create_hand_by_zlgcan(hand_type: EHandType = EHandType.LEFT,
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
def create_hand_by_hcan(hand_type: EHandType = EHandType.LEFT,
                hand_device_id: int = 1,
                canfd_device_id: int = 0,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """创建灵巧手对象（HCAN USB CANFD，通过设备 ID）。

    Args:
        hand_type: 手型，默认为左手。
        device_id: 手部设备 ID，默认为 1。
        canfd_device_id: HCAN 设备索引，默认为 0。
        canfd_channel_id: CAN 通道索引，默认为 0。
    
    Returns:
        OmniHandDexUMI: 灵巧手实例。
    """
```

**示例：**
```python
hand = OmniHandDexUMI.create_hand_by_hcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_canfd_channel_id=0
)
```

### 通过 HCAN 序列号创建

```python
@staticmethod
def create_hand_by_hcan(hand_type: EHandType,
                hand_device_id: int,
                hcan_usbcanfd_serial_number: str,
                canfd_canfd_channel_id: int = 0) -> 'OmniHandDexUMI':
    """创建灵巧手对象（HCAN USB CANFD，通过序列号）。

    Args:
        hand_type: 手型。
        device_id: 手部设备 ID。
        hcan_serial_number: HCAN 设备序列号（支持部分匹配）。
        canfd_channel_id: CAN 通道索引，默认为 0。
    
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

### 周期上报频率设置

```python
def set_position_report_frequency(self, frequency: int) -> None:
    """设置位置上报频率（UMI 协议 Pn2.03）。
    
    Args:
        frequency: 上报频率（Hz，默认：100）。
    
    Note:
        将频率设置为 0 将禁用周期上报。
    """

def set_tactile_sensor_report_frequency(self, frequency: int) -> None:
    """设置触觉传感器上报频率（UMI 协议 Pn2.04）。
    
    Args:
        frequency: 上报频率（Hz，默认：100）。
    
    Note:
        将频率设置为 0 将禁用周期上报。
    """
```

### 周期上报回调

```python
def set_position_report_callback(self, callback: Optional[Callable[[List[int]], None]], 
                                 frequency: Optional[int] = None) -> None:
    """注册位置周期上报回调函数（UMI 协议 Pn3，Pn2.03 设置频率）。
    
    Args:
        callback: 接收到位置数据时调用的回调函数。
                 回调函数接收位置值列表（电压值，单位：mV）。
                 如果为 None，将取消注册回调。
        frequency: 可选频率（Hz，如果提供，在注册回调前设置 Pn2.03，默认：100）。
    
    Note:
        回调在后台线程中执行，因此应该是线程安全的。
    """

def set_tactile_sensor_report_callback(self, 
                                       callback: Optional[Callable[[TactileSensorData, int], None]], 
                                       frequency: Optional[int] = None) -> None:
    """注册触觉传感器周期上报回调函数（UMI 协议 Pn6，Pn2.04 设置频率）。
    
    Args:
        callback: 接收到触觉传感器数据时调用的回调函数。
                 回调函数接收 (sensor_data: TactileSensorData, sensor_id: int)。
                 sensor_id 是子寄存器地址（0x01~0x07）。
                 如果为 None，将取消注册回调。
        frequency: 可选频率（Hz，如果提供，在注册回调前设置 Pn2.04，默认：100）。
    
    Note:
        回调在后台线程中执行，因此应该是线程安全的。
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

def get_tactile_sensor_data_raw(self, eFinger: EFinger) -> TactileSensorData:
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

1. **无位置/速度/力矩控制**：OmniHand Dex UMI (O10 UMI) 是**只读**设备。它不支持位置、速度或力矩控制。它仅通过周期上报提供位置信息。

2. **周期上报**：UMI 协议支持位置和触觉传感器数据的周期上报。使用回调函数异步接收此数据。

3. **线程安全**：回调函数在后台线程中执行。确保您的回调是线程安全的。

4. **位置校准**：位置校准（最小/最大）是只写操作。调用校准函数时，设备应处于适当的位置。

## 完整示例

```python
from omnihand import OmniHandDexUMI, EHandType, EFinger
import time

# 创建手部实例
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand.init():
    print("初始化 OmniHand Dex UMI (O10 UMI) 失败")
    exit(1)

# 注册位置上报回调
def position_callback(positions):
    print(f"位置上报: {len(positions)} 个值")

hand.set_position_report_callback(position_callback, frequency=100)  # 100 Hz

# 注册触觉传感器上报回调
def tactile_callback(sensor_data, sensor_id):
    print(f"触觉传感器上报: sensor_id={sensor_id}, data_size={len(sensor_data.data)}")

hand.set_tactile_sensor_report_callback(tactile_callback, frequency=100)  # 100 Hz

# 保持运行以接收周期上报
time.sleep(10)

# 取消注册回调
hand.set_position_report_callback(None)
hand.set_tactile_sensor_report_callback(None)
```

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
