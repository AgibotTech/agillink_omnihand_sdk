# OmniHand Dex UMI 示例程序

本目录包含 OmniHand Dex UMI 产品的 Python 示例程序。

## 示例列表

### 综合示例（推荐）

以下示例覆盖多个场景，类似于 C++ demo，展示了完整的功能流程：

#### 1. `demo_canfd_id.py`
**CANFD 通信（通过 canfd_id）**

使用 USB CANFD 适配器设备索引创建和读取灵巧手数据。

**运行方式：**
```bash
# 读取左手数据
python3 demo_canfd_id.py left

# 读取右手数据
python3 demo_canfd_id.py right

# 同时读取左右手数据（使用同一适配器的不同通道）
python3 demo_canfd_id.py both
```

**功能覆盖：**
- 获取设备信息（厂家信息、设备信息，包括 UMI 特有字段）
- 读取触觉传感器数据（1D Raw）
- 注册位置周期上报回调
- 注册触觉传感器周期上报回调
- 支持 left/right/both 三种模式

**适用场景：**
- 单个 USB CANFD 适配器
- 知道适配器索引（通常为 0）
- `both` 模式需要多通道适配器（如 USBCANFD-200U 有 2 个通道）

#### 2. `demo_canfd_serial.py`
**CANFD 通信（通过 serial_number）**

使用 USB CANFD 适配器序列号创建和读取灵巧手数据。

**运行方式：**
```bash
python3 demo_canfd_serial.py left
python3 demo_canfd_serial.py right
python3 demo_canfd_serial.py both
```

**功能覆盖：**
- 获取设备信息（厂家信息、设备信息，包括 UMI 特有字段）
- 读取触觉传感器数据（1D Raw）
- 注册位置周期上报回调
- 注册触觉传感器周期上报回调
- 支持 left/right/both 三种模式

**适用场景：**
- 多个 USB CANFD 适配器
- 需要通过序列号精确指定设备
- 序列号支持部分匹配（例如 "201BFF2A" 可以匹配 "201BFF2AF01202D44690USBCANFD-200U"）

**注意**：代码中的序列号需要根据实际情况修改。

#### 3. `demo_socketcan.py`（仅 Linux）
**SocketCAN 通信综合示例**

使用 Linux SocketCAN 接口创建和读取灵巧手数据。

**运行方式：**
```bash
# 首先配置 CAN 接口
sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can0 up
sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can1 up

# 运行示例
python3 demo_socketcan.py left    # 使用 can0
python3 demo_socketcan.py right   # 使用 can0
python3 demo_socketcan.py both    # 使用 can0 和 can1
```

**功能覆盖：**
- 获取设备信息（厂家信息、设备信息，包括 UMI 特有字段）
- 读取触觉传感器数据（1D Raw）
- 注册位置周期上报回调
- 注册触觉传感器周期上报回调
- 支持 left/right/both 三种模式

**适用场景：**
- 已有 SocketCAN 环境（板载 CAN、其他 SocketCAN 设备）
- 对于 USB CANFD 设备，推荐使用 ZLG 库方式（无需驱动配置）

### 基础功能示例

#### 1. `demo_get_hardware_info.py`
获取 OmniHand Dex UMI 产品的厂家信息和设备信息。
- 厂家信息（Pn1）：产品型号、序列号、硬件版本、软件版本、供电电压、自由度
- 设备信息（Pn2）：设备ID、通讯参数配置

**运行方式：**
```bash
python3 demo_get_hardware_info.py
```

#### 2. `demo_position_report.py`
OmniHand Dex UMI 位置周期上报示例。
- 演示如何接收和处理位置周期上报数据
- 位置周期上报（Pn3 = 0x13，频率由 Pn2.03 设置，默认 100Hz）
- 位置数据为电压值（单位：mV）
- 周期上报是设备主动发送的数据，不需要发送请求
- 数据通过 ProcessMsg 回调函数自动接收和存储
- 支持通过命令行参数设置上报频率和测试频率设置功能

**运行方式：**
```bash
# 使用默认频率（100Hz）
python3 demo_position_report.py

# 测试频率设置功能（演示两种设置方式）
python3 demo_position_report.py --test-frequency

# 设置自定义频率
python3 demo_position_report.py --freq 50
```

#### 3. `demo_tactile_sensor_report.py`
OmniHand Dex UMI 触觉传感器周期上报示例。
- 演示如何接收和处理触觉传感器周期上报数据
- 触觉传感器周期上报（Pn6 = 0x06，频率由 Pn2.04 设置，默认 100Hz）
- 触觉传感器数据为原始传感器数据（单位：1g，最大值：255g）
- 周期上报是设备主动发送的数据，不需要发送请求
- 数据通过 ProcessMsg 回调函数自动接收和存储
- 支持通过命令行参数设置上报频率和测试频率设置功能

**运行方式：**
```bash
# 使用默认频率（100Hz）
python3 demo_tactile_sensor_report.py

# 测试频率设置功能（演示两种设置方式）
python3 demo_tactile_sensor_report.py --test-frequency

# 设置自定义频率
python3 demo_tactile_sensor_report.py --freq 50
```

#### 4. `demo_set_max_min_calibration.py`
OmniHand Dex UMI 最大最小位置校准示例。
- 演示如何设置位置传感器的最大和最小位置校准值
- 最小位置校准（Pn7，子寄存器 0x00）：将手指移动到最小位置（完全弯曲）后调用
- 最大位置校准（Pn7，子寄存器 0x01）：将手指移动到最大位置（完全伸直）后调用
- 校准操作会修改设备内部参数，请谨慎使用
- 校准后可能需要重新初始化设备才能生效

**运行方式：**
```bash
python3 demo_set_max_min_calibration.py
```

**注意事项：**
- 校准操作是写操作，会修改设备内部参数
- 请确保在安全的环境下进行校准
- 按照提示将手指移动到指定位置后再执行校准
- 建议在校准前先获取当前位置信息，确认设备状态正常


## 注意事项

1. **位置信息只读**：根据 UMI 协议，Pn3 (0x13) 是位置信息寄存器，**只读**，不支持位置设置。位置数据通过周期上报自动发送（默认 100Hz）。

2. **设备初始化**：所有示例都需要先初始化设备，如果初始化失败，请检查：
   - USB CANFD 设备是否正确连接
   - 设备驱动是否正确安装
   - 设备ID是否正确

3. **SocketCAN**：SocketCAN 示例仅适用于 Linux 系统，且需要先配置 CAN 接口。

4. **触觉传感器**：OmniHand Dex UMI 使用一维触觉传感器（1D sensors），与 OmniHand 2025 相同。

## 协议说明

OmniHand Dex UMI 使用以下协议寄存器：
- **Pn1 (0x01)**：厂家信息（只读）
- **Pn2 (0x02)**：设备信息（读写）
  - **Pn2.03**：位置周期上报频率（2字节，单位Hz，默认100，可写）
  - **Pn2.04**：触觉传感器周期上报频率（2字节，单位Hz，默认100，可写）
  - **Pn2.05**：ADC通道数量（1字节，只读）
  - **Pn2.06**：触觉传感器信息（可变长度，只读）
- **Pn3 (0x13)**：位置信息（**只读**，主动上报周期由Pn2.03设置，默认100Hz）
- **Pn6 (0x06)**：触觉传感器（只读，主动上报周期由Pn2.04设置，默认100Hz）
- **Pn7 (0x07)**：位置校准（只写）
  - 子寄存器 0x00：最小位置校准
  - 子寄存器 0x01：最大位置校准

## 接口说明

### 周期上报频率设置

OmniHand Dex UMI 提供了两种方式设置周期上报频率：

1. **独立函数设置**：
   ```python
   hand.set_position_report_frequency(100)  # 设置位置上报频率为100Hz
   hand.set_tactile_sensor_report_frequency(50)  # 设置触觉传感器上报频率为50Hz
   ```

2. **注册回调时设置**：
   ```python
   def position_callback(positions):
       print(f"Position data: {positions}")
   
   # 注册回调并同时设置频率为50Hz
   hand.set_position_report_callback(position_callback, frequency=50)
   
   # 只注册回调，使用当前频率（或默认100Hz）
   hand.set_position_report_callback(position_callback)
   ```

### 设备信息字段

`DeviceInfo` 对象包含以下 UMI 特定字段（通过 `get_device_info()` 获取）：
- `position_report_frequency` (Optional[int]): 位置上报频率（Pn2.03）
- `tactile_sensor_report_frequency` (Optional[int]): 触觉传感器上报频率（Pn2.04）
- `adc_channel_count` (Optional[int]): ADC通道数量（Pn2.05，只读）
- `tactile_sensor_info` (Optional[List[int]]): 触觉传感器信息（Pn2.06，只读）

详细信息请参考协议文档。
