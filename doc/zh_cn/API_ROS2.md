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

#### 第一个手参数

| 参数 | 类型 | 默认值 | 描述 |
|------|------|--------|------|
| `hand_type` | string | "left" | 手类型："left" 或 "right" |
| `hand_device_id` | int | 1 | 手设备 ID (1-255) |
| `connection_type` | string | "zlg_can" | 连接类型："zlg_can"、"hcan" 或 "rs485" |
| `canfd_serial_number` | string | "" | CANFD 适配器序列号（重启/插拔后稳定；内部会先扫描再打开） |
| `canfd_device_id` | int | 0 | CANFD 适配器设备索引（不扫直接打开，只 open 一次；重启/插拔后索引可能变） |
| `canfd_channel_id` | int | 0 | CAN 通道索引 (0 或 1) |
| `uart_port` | string | "/dev/ttyUSB0" | 串口路径（仅 rs485） |
| `baudrate` | int | 460800 | 波特率（仅 rs485） |

#### 第二个手参数（可选）

如果配置了第二个手参数，会自动启动第二个手。

| 参数 | 类型 | 默认值 | 描述 |
|------|------|--------|------|
| `second_hand_type` | string | "" | 第二只手类型："left" 或 "right" |
| `second_hand_device_id` | int | 1 | 第二只手设备 ID |
| `second_connection_type` | string | "zlg_can" | 第二只手连接类型 |
| `second_canfd_serial_number` | string | "" | 第二只手 CANFD 适配器序列号 |
| `second_canfd_device_id` | int | 0 | 第二只手 CANFD 适配器设备索引 |
| `second_canfd_channel_id` | int | 1 | 第二只手 CAN 通道索引 |
| `second_uart_port` | string | "/dev/ttyUSB1" | 第二只手串口路径（仅 rs485） |
| `second_baudrate` | int | 460800 | 第二只手波特率（仅 rs485） |

**说明（ZLG CANFD 设备标识）**：
- **按序列号**（`canfd_serial_number`）：重启/插拔后不变；内部会先扫描（open/close 读信息）再打开使用。
- **按设备索引**（`canfd_device_id`）：不扫直接打开，设备只 open 一次；重启/插拔后索引可能变化。

### 使用示例

**配置方式** - 支持通过配置文件或 launch 参数配置一个或两个手。如果配置了第二个手参数，则启动两个手；否则只启动第一个手。

**1. 使用 ros2 launch（推荐）：**

```bash
# 使用默认配置文件（单手，左手）
# 默认加载 config/omnihand_2025_node.yaml（单手配置）
ros2 launch omnihand_node omnihand_2025_node.launch.py

# 使用指定配置文件（相对路径，相对于包的 share 目录）
ros2 launch omnihand_node omnihand_2025_node.launch.py config_file:=config/omnihand_2025_node.yaml

# 使用指定配置文件（绝对路径）
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  config_file:=$(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml

# 通过参数覆盖配置（按序列号）
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  hand_type:=left \
  canfd_serial_number:="12345678" \
  canfd_channel_id:=0

# 通过参数覆盖配置（按设备索引）
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  hand_type:=left \
  canfd_device_id:=0 \
  canfd_channel_id:=0

# 配置两个手（使用序列号）
ros2 launch omnihand_node omnihand_2025_node.launch.py \
  hand_type:=left \
  canfd_serial_number:="12345678" \
  canfd_channel_id:=0 \
  second_hand_type:=right \
  second_canfd_serial_number:="87654321" \
  second_canfd_channel_id:=1
```

**2. 使用 ros2 run：**

```bash
# 直接运行（使用代码中的默认参数，不推荐）
# 默认参数定义在 node/src/omnihand_2025/main.cpp 中
# 建议使用配置文件或 launch 文件
ros2 run omnihand_node omnihand_2025_node

# 使用配置文件（推荐）
ros2 run omnihand_node omnihand_2025_node --ros-args \
  --params-file $(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_node.yaml

# 通过参数配置（按序列号）
ros2 run omnihand_node omnihand_2025_node --ros-args \
  -p hand_type:=left \
  -p connection_type:=zlg_can \
  -p canfd_serial_number:="12345678" \
  -p canfd_channel_id:=0
```

**3. YAML 配置文件示例（单手）：**

```yaml
# omnihand_2025_node.yaml - 单手配置
# SDK 中路径：ros2/humble/share/omnihand_node/config/omnihand_2025_node.yaml
omnihand_2025_param_reader:
  ros__parameters:
    # 第一个手配置
    hand_type: "left"                  # "left" or "right"
    hand_device_id: 1                  # Hand device ID (1-255)
    connection_type: "zlg_can"         # "zlg_can", "hcan", or "rs485"
    
    # 二选一：序列号 或 设备索引
    canfd_serial_number: "12345678"    # 按序列号（稳定；内部会先扫描再打开）
    # canfd_device_id: 0                # 按设备索引（不扫直接打开；重启/插拔后可能变）
    canfd_channel_id: 0                # CAN channel index (0 or 1)
    
    # 如果使用 rs485，取消注释并设置：
    # uart_port: "/dev/ttyUSB0"        # Serial port path
    # baudrate: 460800                 # Baud rate
```

**4. YAML 配置文件示例（双手）：**

```yaml
# omnihand_2025_node.yaml - 双手配置
# SDK 中路径：ros2/humble/share/omnihand_node/config/omnihand_2025_node.yaml
omnihand_2025_param_reader:
  ros__parameters:
    # 第一个手配置
    hand_type: "left"
    hand_device_id: 1
    connection_type: "zlg_can"
    canfd_serial_number: "12345678"    # 按序列号
    canfd_channel_id: 0
    
    # 第二个手配置（如果配置了，会自动启动第二个手）
    second_hand_type: "right"
    second_hand_device_id: 1
    second_connection_type: "zlg_can"
    second_canfd_serial_number: "87654321"  # 按序列号
    # second_canfd_device_id: 0         # 按设备索引
    second_canfd_channel_id: 1
```

**5. 连接类型说明：**

- **zlg_can** - ZLG USB CANFD（默认）
- **hcan** - HCAN USB CANFD
- **rs485** - RS485 串口通信

每个手可以独立选择连接类型。

**6. 设备识别方式：**

使用 ZLG USBCANFD 时：**200U** 有两个 CAN 通道（can0、can1），可分别接左右手；**100U / MINI** 仅单通道，`canfd_channel_id` 恒为 0，仅支持单手。

- **按序列号**（`canfd_serial_number`）：重启/插拔后不变；内部会先扫描再打开使用。
- **按设备索引**（`canfd_device_id`）：不扫直接打开，只 open 一次；重启/插拔后索引可能变化。
