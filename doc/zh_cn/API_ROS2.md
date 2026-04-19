# OmniHand 2025 SDK ROS2 接口

> ⚠️ **仅限 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## 概述

OmniHand 2025 SDK 为两种产品型号提供 ROS2 接口：

- **OmniHand 2025 (O10)**：10 自由度灵巧手，配备 1D 触觉传感器
- **OmniHand Pro 2025 (O12)**：12 自由度灵巧手，配备 3D 触觉传感器

每个产品都有自己的 ROS2 节点和消息类型，提供产品特定的接口。

## 产品特定 ROS2 文档

- **[OmniHand 2025 (O10) ROS2 接口](API_ROS2_O10.md)** - 10 自由度，关节角话题与 set/get 关节角服务
- **[OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md)** - 12 自由度，关节角话题与 set/get 关节角服务

## 配置

ROS2 节点支持 YAML 配置文件，便于灵活管理参数。参数命名与 Python API 保持一致。

### 配置参数

参数按 `left_hand` 和 `right_hand` 命名空间组织。如果某只手的 `connection_type` 为空（或在 YAML 中注释掉整个命名空间），则跳过该手。

#### 每只手参数（`left_hand` / `right_hand`）

| 参数 | 类型 | 默认值 | 描述 |
|------|------|--------|------|
| `hand_device_id` | int | 1 | 手设备 ID (1-255) |
| `connection_type` | string | "" | 连接类型："zlgcan"、"hcan"、"rs485" 或 "usb"（rs485/usb 仅 O10）；为空则跳过 |
| `canfd_serial_number` | string | "" | CANFD 适配器序列号（重启/插拔后稳定；内部会先扫描再打开） |
| `canfd_device_id` | int | 0 | CANFD 适配器设备索引（不扫直接打开；重启/插拔后索引可能变） |
| `canfd_channel_id` | int | 0 | CAN 通道索引 (0 或 1) |
| `uart_port` | string | "" | 串口路径（仅 rs485/usb，仅 O10） |
| `baudrate` | int | 460800 | 波特率（仅 rs485/usb，仅 O10） |

**说明（ZLG CANFD 设备标识）**：
- **按序列号**（`canfd_serial_number`）：重启/插拔后不变；内部会先扫描（open/close 读信息）再打开使用。
- **按设备索引**（`canfd_device_id`）：不扫直接打开，设备只 open 一次；重启/插拔后索引可能变化。

### 使用示例

**默认配置** - 默认 YAML 配置启用单只左手。如需双手模式，请在 YAML 文件中取消注释 `right_hand` 部分。

**1. 使用 ros2 launch（推荐）：**

```bash
# 使用默认配置文件（单手，左手）
ros2 launch omnihand_node omnihand_2025_node.launch.py

# 使用指定配置文件（绝对路径）
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  config_file:=/path/to/your/omnihand_2025_node.yaml
```

**2. 使用 ros2 run：**

```bash
# 直接运行（使用代码中的默认参数，不推荐）
ros2 run omnihand_node omnihand_2025_node

# 使用配置文件（推荐）
ros2 run omnihand_node omnihand_2025_node --ros-args \
  --params-file $(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml
```

**3. YAML 配置文件示例：**

单手（CANFD）：
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_device_id: 0
      canfd_channel_id: 0
```

双手（CANFD）：
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "12345678"
      canfd_channel_id: 0

    right_hand:
      hand_device_id: 1
      connection_type: "zlgcan"
      canfd_serial_number: "87654321"
      canfd_channel_id: 1
```

USB 连接（仅 O10）：
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "usb"
      uart_port: "/dev/ttyACM0"
      baudrate: 460800
```

RS485 连接（仅 O10）：
```yaml
omnihand_2025_param_reader:
  ros__parameters:
    left_hand:
      hand_device_id: 1
      connection_type: "rs485"
      uart_port: "/dev/ttyUSB0"
      baudrate: 460800
```

**4. 连接类型说明：**

- **zlgcan** - ZLG USB CANFD（默认）
- **hcan** - HCAN USB CANFD
- **rs485** - RS485 串口通信（仅 O10）
- **usb** - USB 串口通信（仅 O10）

每个手可以独立选择连接类型。

**5. 设备识别方式：**

使用 ZLG USBCANFD 时：**200U** 有两个 CAN 通道（can0、can1），可分别接左右手；**100U / MINI** 仅单通道，`canfd_channel_id` 恒为 0，仅支持单手。

- **按序列号**（`canfd_serial_number`）：重启/插拔后不变；内部会先扫描再打开使用。
- **按设备索引**（`canfd_device_id`）：不扫直接打开，只 open 一次；重启/插拔后索引可能变化。
