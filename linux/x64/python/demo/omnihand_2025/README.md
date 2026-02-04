# OmniHand 2025 (O10) Python 示例

本目录包含 OmniHand 2025 SDK 的 Python 使用示例，展示了各种功能的使用方法。

## 示例列表

### 综合示例（推荐）

以下示例覆盖多个场景，类似于 C++ demo，展示了完整的功能流程：

#### 1. `demo_canfd_id.py`
**CANFD 通信（通过 canfd_id）**

使用 USB CANFD 适配器设备索引创建和控制灵巧手。

**运行方式：**
```bash
# 控制左手
python3 demo_canfd_id.py left

# 控制右手
python3 demo_canfd_id.py right

# 同时控制左右手（使用同一适配器的不同通道）
python3 demo_canfd_id.py both
```

**功能覆盖：**
- 获取设备信息（厂家信息、设备信息）
- 读取传感器数据（触觉、温度、电流、错误报告、速度）
- 关节角度控制
- 支持 left/right/both 三种模式

**适用场景：**
- 单个 USB CANFD 适配器
- 知道适配器索引（通常为 0）
- `both` 模式需要多通道适配器（如 USBCANFD-200U 有 2 个通道）

#### 2. `demo_canfd_serial.py`
**CANFD 通信（通过 serial_number）**

使用 USB CANFD 适配器序列号创建和控制灵巧手。

**运行方式：**
```bash
python3 demo_canfd_serial.py left
python3 demo_canfd_serial.py right
python3 demo_canfd_serial.py both
```

**功能覆盖：**
- 获取设备信息（厂家信息、设备信息）
- 读取传感器数据（触觉、温度、电流、错误报告、速度）
- 关节角度控制
- 支持 left/right/both 三种模式

**适用场景：**
- 多个 USB CANFD 适配器
- 需要通过序列号精确指定设备
- 序列号支持部分匹配（例如 "201BFF2A" 可以匹配 "201BFF2AF01202D44690USBCANFD-200U"）

**注意**：代码中的序列号需要根据实际情况修改。

#### 3. `demo_socketcan.py`（仅 Linux）
**SocketCAN 通信综合示例**

使用 Linux SocketCAN 接口创建和控制灵巧手。

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
- 获取设备信息（厂家信息、设备信息）
- 读取传感器数据（触觉、温度、电流、错误报告、速度）
- 关节角度控制
- 支持 left/right/both 三种模式

**适用场景：**
- 已有 SocketCAN 环境（板载 CAN、其他 SocketCAN 设备）
- 对于 USB CANFD 设备，推荐使用 ZLG 库方式（无需驱动配置）

### 基础功能示例

#### 1. `demo_get_hardware_info.py`
获取设备硬件信息
- 厂家信息：产品型号、序列号、硬件版本、软件版本、供电电压、自由度（10 DOF）
- 设备信息：设备ID、通讯参数配置
- 最简单的入门示例，适合快速上手

**运行方式：**
```bash
python3 demo_get_hardware_info.py
```

#### 2. `demo_set_angle.py`
设置关节角度
- 演示如何使用 `set_all_active_joint_angles()` 设置所有主动关节角度（10 个关节）
- 展示完整的动作序列：reset → FIST → reset
- 适合快速测试关节角度控制功能

**运行方式：**
```bash
python3 demo_set_angle.py
```

### 特殊功能示例

#### 3. `demo_set_motion.py`
手势控制
- 演示预置手势控制功能
- 支持多种手势（握拳、OK、点赞等）

**运行方式：**
```bash
python3 demo_set_motion.py
```

### 多设备场景示例

#### 4. `demo_set_motor_via_multicans.py`
多适配器控制示例
- 演示如何使用多个 CANFD 适配器同时控制多个灵巧手

#### 5. `demo_set_motor_via_multichannels.py`
多通道控制示例
- 演示如何使用单个多通道适配器的不同通道控制多个灵巧手

#### 6. `demo_set_motor_via_multisocketcans.py`
多 SocketCAN 控制示例
- 演示如何使用多个 SocketCAN 接口同时控制多个灵巧手

## 使用说明

### 设备连接

所有示例默认使用以下参数：
- 设备ID：1
- CANFD ID：0（第一个适配器）
- 通道ID：0（第一个通道）

如需修改，请在代码中调整 `create_hand_by_zlgcan()` 的参数。

### 通信方式

示例支持以下通信方式：
- **CANFD（推荐）**：使用 ZLG USB CANFD 适配器，零配置
- **SocketCAN（仅 Linux）**：需要先配置 CAN 接口

### 注意事项

1. **设备初始化**：所有示例都需要先初始化设备，如果初始化失败，请检查：
   - USB CANFD 设备是否正确连接
   - 设备驱动是否正确安装
   - 设备ID是否正确

2. **SocketCAN**：SocketCAN 示例仅适用于 Linux 系统，且需要先配置 CAN 接口。

3. **触觉传感器**：OmniHand 2025 使用一维触觉传感器（1D sensors），支持读取各手指、手心、手背的传感器数据。

4. **频率控制**：SDK 内部已实现请求频率控制（默认 10 Hz），无需手动添加延迟。

## 更多信息

- [API 文档](../../doc/en/API_PYTHON_O10.md)
- C++ 示例：`../../../cpp/demo/omnihand_2025/`
- 测试代码：`../../../cpp/test/`
