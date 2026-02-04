# OmniHand 2025 SDK ROS2 接口

## 概述

OmniHand 2025 SDK 为两种产品型号提供 ROS2 接口：

- **OmniHand 2025 (O10)**：10 自由度灵巧手，配备 1D 触觉传感器
- **OmniHand Pro 2025 (O12)**：12 自由度灵巧手，配备 3D 触觉传感器

每个产品都有自己的 ROS2 节点和消息类型，提供产品特定的接口。

## 产品特定 ROS2 文档

- **[OmniHand 2025 (O10) ROS2 接口](API_ROS2_O10.md)** - 10 自由度，1D 触觉传感器，支持混合控制
- **[OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md)** - 12 自由度，3D 触觉传感器，不支持混合控制

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
