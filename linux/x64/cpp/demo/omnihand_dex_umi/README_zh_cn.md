# OmniHand Dex UMI SDK C++ 示例

本目录包含 OmniHand Dex UMI SDK 的 C++ 使用示例，展示了不同通信方式的使用方法。

## 产品特点

- **10 自由度**：与 OmniHand 2025 相同的自由度
- **UMI 协议**：使用 UMI 协议（Pn1-Pn7 寄存器）
- **只读模式**：**不支持位置/速度/力矩控制**，仅支持读取位置和触觉传感器数据
- **周期上报**：支持周期性的位置和触觉传感器数据上报
- **仅支持 CAN 通信**：不支持 RS485

## 示例列表

所有示例都支持三种模式：
- `left` - 仅读取左手数据
- `right` - 仅读取右手数据
- `both` - 同时读取左右手数据

⚠️ **重要提示**：UMI 协议是只读的，所有示例仅用于读取数据，不支持控制操作。

### 1. `UMI_demo_canfd_id.cc`
**CANFD 通信（通过 canfd_id）**

使用 USB CANFD 适配器的设备索引创建和读取灵巧手数据。

```bash
# 读取左手数据
./demo_omnihand_dex_umi_canfd_id left

# 读取右手数据
./demo_omnihand_dex_umi_canfd_id right

# 同时读取左右手数据（使用同一适配器的不同通道）
./demo_omnihand_dex_umi_canfd_id both
```

**适用场景**：
- 只有一个 USB CANFD 适配器
- 知道适配器的索引（通常是 0）
- `both` 模式需要多通道适配器（例如 USBCANFD-200U 有 2 个通道）

### 2. `UMI_demo_canfd_serial.cc`
**CANFD 通信（通过 serial_number）**

使用 USB CANFD 适配器的序列号创建和读取灵巧手数据。

```bash
./demo_omnihand_dex_umi_canfd_serial left
./demo_omnihand_dex_umi_canfd_serial right
./demo_omnihand_dex_umi_canfd_serial both
```

**适用场景**：
- 有多个 USB CANFD 适配器
- 需要通过序列号精确指定设备
- 序列号支持部分匹配（例如 "201BFF2A" 可以匹配 "201BFF2AF01202D44690USBCANFD-200U"）

**注意**：代码中的序列号需要根据实际情况修改。

### 3. `UMI_demo_socketcan.cc` (仅 Linux)
**SocketCAN 通信**

使用 Linux SocketCAN 接口创建和读取灵巧手数据。

```bash
# 首先配置 CAN 接口
sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can0 up
sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
sudo ip link set can1 up

# 运行示例
./demo_omnihand_dex_umi_socketcan left    # 使用 can0
./demo_omnihand_dex_umi_socketcan right   # 使用 can0
./demo_omnihand_dex_umi_socketcan both    # 使用 can0 和 can1
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
cd cpp/demo/omnihand_dex_umi
mkdir build && cd build
cmake ..
make
```

## 运行

编译后的可执行文件位于 `build/bin/omnihand/demo/` 目录下：

```bash
# CANFD (by canfd_id)
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_id left
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_id right
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_id both

# CANFD (by serial_number)
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_serial left
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_serial right
./build/bin/omnihand/demo/demo_omnihand_dex_umi_canfd_serial both

# SocketCAN (Linux only)
./build/bin/omnihand/demo/demo_omnihand_dex_umi_socketcan left
./build/bin/omnihand/demo/demo_omnihand_dex_umi_socketcan right
./build/bin/omnihand/demo/demo_omnihand_dex_umi_socketcan both
```

## 通信方式对比

| 通信方式 | 优点 | 缺点 | 推荐场景 |
|---------|------|------|---------|
| **CANFD (canfd_id)** | 简单直接，零配置 | 需要知道适配器索引 | 单个适配器，简单场景 |
| **CANFD (serial_number)** | 精确指定设备，支持多适配器 | 需要知道序列号 | 多个适配器，需要精确控制 |
| **SocketCAN** | Linux 原生支持 | 需要配置驱动和接口 | Linux 环境，已有 SocketCAN |

## 注意事项

1. **只读模式**：⚠️ **UMI 协议不支持位置/速度/力矩控制**，所有示例仅用于读取数据
2. **设备 ID**：确保代码中的 `device_id` 与硬件中配置的设备 ID 一致
3. **通道选择**：
   - `left`/`right` 模式：使用第一个通道（channel_id=0）
   - `both` 模式：左手使用 channel_id=0，右手使用 channel_id=1（需要多通道适配器）
4. **序列号匹配**：使用序列号时，支持部分匹配，但需要确保唯一性
5. **权限问题**：
   - SocketCAN: 需要 root 权限配置接口
6. **周期上报**：UMI 支持周期性的位置和触觉传感器数据上报，可通过回调函数接收
7. **触觉传感器**：UMI 使用 1D 触觉传感器，使用 `GetTactileSensorData()` 方法读取
8. **UMI 协议**：使用 Pn1-Pn7 寄存器，支持配置上报频率等参数

## 更多信息

- [API 文档](../../../doc/zh_cn/API_CPP_O10_UMI.md)
- [English Documentation](README.md)
