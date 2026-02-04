# OmniHand Pro 2025 SDK C++ 示例

本目录包含 OmniHand Pro 2025 SDK 的 C++ 使用示例，展示了不同通信方式的使用方法。

## 产品特点

- **12 自由度**：比 OmniHand 2025 多 2 个自由度
- **3D 触觉传感器**：支持读取 3D 触觉传感器数据
- **仅支持 CAN 通信**：不支持 RS485
- **多种控制模式**：支持位置、速度、力矩及混合控制

## 示例列表

所有示例都支持三种控制模式：
- `left` - 仅控制左手
- `right` - 仅控制右手
- `both` - 同时控制左右手

### 1. `O12_demo_canfd_id.cc`
**CANFD 通信（通过 canfd_id）**

使用 USB CANFD 适配器的设备索引创建和控制灵巧手。

```bash
# 控制左手
./demo_omnihand_pro_2025_canfd_id left

# 控制右手
./demo_omnihand_pro_2025_canfd_id right

# 同时控制左右手（使用同一适配器的不同通道）
./demo_omnihand_pro_2025_canfd_id both
```

**适用场景**：
- 只有一个 USB CANFD 适配器
- 知道适配器的索引（通常是 0）
- `both` 模式需要多通道适配器（例如 USBCANFD-200U 有 2 个通道）

### 2. `O12_demo_canfd_serial.cc`
**CANFD 通信（通过 serial_number）**

使用 USB CANFD 适配器的序列号创建和控制灵巧手。

```bash
./demo_omnihand_pro_2025_canfd_serial left
./demo_omnihand_pro_2025_canfd_serial right
./demo_omnihand_pro_2025_canfd_serial both
```

**适用场景**：
- 有多个 USB CANFD 适配器
- 需要通过序列号精确指定设备
- 序列号支持部分匹配（例如 "201BFF2A" 可以匹配 "201BFF2AF01202D44690USBCANFD-200U"）

**注意**：代码中的序列号需要根据实际情况修改。

### 3. `O12_demo_socketcan.cc` (仅 Linux)
**SocketCAN 通信**

使用 Linux SocketCAN 接口创建和控制灵巧手。

```bash
# 首先配置 CAN 接口
sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can0 up
sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can1 up

# 运行示例
./demo_omnihand_pro_2025_socketcan left    # 使用 can0
./demo_omnihand_pro_2025_socketcan right   # 使用 can0
./demo_omnihand_pro_2025_socketcan both    # 使用 can0 和 can1
```

**适用场景**：
- 已有 SocketCAN 环境（板载 CAN、其他 SocketCAN 设备）
- 对于 USB CANFD 设备，推荐使用 ZLG 库方式（无需配置驱动）

## 编译

### 方式 1：作为 SDK 的一部分编译

```bash
cd /path/to/Omnihand-2025-SDK
mkdir build && cd build
cmake .. -DBUILD_CPP_DEMO=ON
make
```

### 方式 2：独立编译（需要先安装 SDK）

```bash
cd cpp/demo/omnihand_pro_2025
mkdir build && cd build
cmake ..
make
```

## 运行

编译后的可执行文件位于 `build/bin/omnihand/demo/` 目录下：

```bash
# CANFD (by canfd_id)
./build/bin/omnihand/demo/demo_omnihand_pro_2025_canfd_id left
./build/bin/omnihand/demo/demo_omnihand_pro_2025_canfd_id right
./build/bin/omnihand/demo/demo_omnihand_pro_2025_canfd_id both

# CANFD (by serial_number)
./build/bin/omnihand/demo/demo_omnihand_pro_2025_canfd_serial left
./build/bin/omnihand/demo/demo_omnihand_pro_2025_canfd_serial right
./build/bin/omnihand/demo/demo_omnihand_pro_2025_canfd_serial both

# SocketCAN (Linux only)
./build/bin/omnihand/demo/demo_omnihand_pro_2025_socketcan left
./build/bin/omnihand/demo/demo_omnihand_pro_2025_socketcan right
./build/bin/omnihand/demo/demo_omnihand_pro_2025_socketcan both
```

## 通信方式对比

| 通信方式 | 优点 | 缺点 | 推荐场景 |
|---------|------|------|---------|
| **CANFD (canfd_id)** | 简单直接，零配置 | 需要知道适配器索引 | 单个适配器，简单场景 |
| **CANFD (serial_number)** | 精确指定设备，支持多适配器 | 需要知道序列号 | 多个适配器，需要精确控制 |
| **SocketCAN** | Linux 原生支持 | 需要配置驱动和接口 | Linux 环境，已有 SocketCAN |

## 注意事项

1. **设备 ID**：确保代码中的 `device_id` 与硬件中配置的设备 ID 一致
2. **通道选择**：
   - `left`/`right` 模式：使用第一个通道（channel_id=0）
   - `both` 模式：左手使用 channel_id=0，右手使用 channel_id=1（需要多通道适配器）
3. **序列号匹配**：使用序列号时，支持部分匹配，但需要确保唯一性
4. **权限问题**：
   - SocketCAN: 需要 root 权限配置接口
5. **频率控制**：SDK 内部已实现请求频率控制（默认 33 Hz），无需手动添加延迟
6. **3D 触觉传感器**：O12 支持读取 3D 触觉传感器数据，使用 `GetTactileSensor3DData()` 方法
7. **关节数量**：O12 有 12 个关节，比 O10 多 2 个

## 更多信息

- [API 文档](../../../doc/zh_cn/API_CPP_O12.md)
- [English Documentation](README.md)
