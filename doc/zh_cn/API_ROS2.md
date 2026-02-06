# OmniHand 2025 SDK ROS2 接口

> ⚠️ **仅限 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## 概述

OmniHand 2025 SDK 为两种产品型号提供 ROS2 接口：

- **OmniHand 2025 (O10)**：10 自由度灵巧手，配备 1D 触觉传感器
- **OmniHand Pro 2025 (O12)**：12 自由度灵巧手，配备 3D 触觉传感器

每个产品都有自己的 ROS2 节点和消息类型，提供产品特定的接口。

## 产品特定 ROS2 文档

- **[OmniHand 2025 (O10) ROS2 接口](API_ROS2_O10.md)** - 10 自由度，1D 触觉传感器，支持混合控制
- **[OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md)** - 12 自由度，3D 触觉传感器，不支持混合控制

## 配置

ROS2 节点支持 YAML 配置文件，便于灵活管理参数。参数命名与 Python API 保持一致。

### 配置参数

| 参数 | 类型 | 默认值 | 描述 |
|------|------|--------|------|
| `hand_type` | string | "left" | 手类型："left" 或 "right" |
| `hand_device_id` | int | 1 | 手设备 ID (1-255) |
| `canfd_device_id` | int | 0 | USB CANFD 适配器设备索引 |
| `canfd_channel_id` | int | 0 | CAN 通道索引 (0 或 1) |
| `enable_both_hands` | bool | true | 启用双手模式 |
| `second_hand_type` | string | "right" | 第二只手类型（双手模式） |
| `second_hand_device_id` | int | 1 | 第二只手设备 ID |
| `second_canfd_device_id` | int | 0 | 第二只手 CANFD 适配器设备索引 |
| `second_canfd_channel_id` | int | 1 | 第二只手通道索引 |

### 使用示例

**默认：双手模式** - `ros2 run` 和 `ros2 launch` 默认都是双手模式（左手通道0，右手通道1）。

**1. 使用 ros2 run（默认双手）：**

```bash
# 默认：双手（左手通道0，右手通道1）
ros2 run omnihand_node omnihand_2025_node

# 仅单手
ros2 run omnihand_node omnihand_2025_node --ros-args -p enable_both_hands:=false -p hand_type:=left
```

**2. 使用 ros2 launch（默认双手）：**

```bash
# 默认：双手（左手通道0，右手通道1）
ros2 launch omnihand_node omnihand_2025.launch.py

# 单左手
ros2 launch omnihand_node omnihand_2025.launch.py enable_both_hands:=false hand_type:=left canfd_channel_id:=0

# 单右手
ros2 launch omnihand_node omnihand_2025.launch.py enable_both_hands:=false hand_type:=right canfd_channel_id:=1

# 自定义双手配置
ros2 launch omnihand_node omnihand_2025.launch.py canfd_channel_id:=0 second_canfd_channel_id:=1
```

**3. 使用 YAML 配置文件：**

```bash
# 使用 release 包中的预配置文件 (ros2/config/)
ros2 run omnihand_node omnihand_2025_node --ros-args --params-file ros2/config/omnihand_2025_left.yaml

# 使用已安装 ROS2 包中的预配置文件
ros2 run omnihand_node omnihand_2025_node --ros-args \
    --params-file $(ros2 pkg prefix omnihand_node)/share/omnihand_node/config/omnihand_2025_left.yaml
```

**4. YAML 配置文件示例：**

```yaml
# omnihand_2025_left.yaml
omnihand_2025_param_reader:
  ros__parameters:
    hand_type: "left"
    hand_device_id: 1
    canfd_device_id: 0
    canfd_channel_id: 0
    enable_both_hands: false
```

**5. 双手配置：**

```yaml
# omnihand_2025_both.yaml
omnihand_2025_param_reader:
  ros__parameters:
    hand_type: "left"
    hand_device_id: 1
    canfd_device_id: 0
    canfd_channel_id: 0
    enable_both_hands: true
    second_hand_type: "right"
    second_hand_device_id: 1
    second_canfd_device_id: 0
    second_canfd_channel_id: 1
```

### 预配置 YAML 文件

SDK 在 `ros2/config/` 目录中包含预配置的 YAML 文件：

- `omnihand_2025_left.yaml` - 仅左手
- `omnihand_2025_right.yaml` - 仅右手
- `omnihand_2025_both.yaml` - 双手
- `omnihand_pro_2025_left.yaml` - 仅左手 (O12)
- `omnihand_pro_2025_right.yaml` - 仅右手 (O12)
- `omnihand_pro_2025_both.yaml` - 双手 (O12)

## 快速参考

### O10 话题
- 话题前缀：`/omnihand/omnihand_2025/`
- 消息命名空间：`omnihand_2025_node_msgs`
- 自由度：10
- 电机位置范围：0-4096
- 支持混合控制：是

### O12 话题
- 话题前缀：`/omnihand/omnihand_pro_2025/`
- 消息命名空间：`omnihand_pro_2025_node_msgs`
- 自由度：12
- 电机位置范围：0-2000
- 支持混合控制：否

详细的话题列表和消息定义，请参考上面的产品特定文档。
