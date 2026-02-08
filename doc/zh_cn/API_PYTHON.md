# OmniHand 2025 SDK Python API

## 概述

OmniHand 2025 SDK 为三种不同的产品提供了**产品特定的接口**：

- **OmniHand 2025 (O10)**: 10 自由度灵巧手，配备 1D 触觉传感器
- **OmniHand Pro 2025 (O12)**: 12 自由度灵巧手，配备 3D 触觉传感器
- **OmniHand Dex UMI (O10 UMI)**: 10 自由度灵巧手，支持 UMI 协议

每个产品都有自己的接口类（`OmniHand2025`、`OmniHandPro2025`、`OmniHandDexUMI`），具有产品特定的工厂方法和 API。与使用 `ProductType` 的统一接口相比，这种设计提供了更好的类型安全性和更清晰的 API 组织。

## 产品特定 API 文档

- **[OmniHand 2025 (O10) Python API](API_PYTHON_O10.md)** - 10 自由度，1D 触觉传感器，支持 CAN 和 RS485
- **[OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)** - 12 自由度，3D 触觉传感器，仅支持 CAN
- **[OmniHand Dex UMI (O10 UMI) Python API](API_PYTHON_UMI.md)** - 10 自由度，UMI 协议，周期上报，仅支持 CAN

## 快速开始示例

### OmniHand 2025 (O10)

```python
from omnihand import OmniHand2025, EHandType

# 创建手部实例
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand.init():
    print("初始化失败")
    exit(1)

# 设置关节角度（单位：弧度，O10 有 10 个关节）
angles = [0.0] * 10  # 所有关节归零
hand.set_all_active_joint_angles(angles)
```

### OmniHand Pro 2025 (O12)

```python
from omnihand import OmniHandPro2025, EHandType

# 创建手部实例
hand = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand.init():
    print("初始化失败")
    exit(1)

# 设置关节角度（单位：弧度，O12 有 12 个关节）
angles = [0.0] * 12  # 所有关节归零
hand.set_all_active_joint_angles(angles)
```

### OmniHand Dex UMI

```python
from omnihand import OmniHandDexUMI, EHandType

# 创建手部实例
hand = OmniHandDexUMI.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand.init():
    print("初始化失败")
    exit(1)

# 注册位置上报回调
def position_callback(positions):
    print(f"位置上报: {len(positions)} 个值")

hand.set_position_report_callback(position_callback, frequency=100)  # 100 Hz
```

## 从统一接口迁移

如果您之前使用的是旧的统一 `OmniHand.create_hand_by_zlgcan(ProductType, ...)` 接口：

**旧代码：**
```python
from omnihand import OmniHand, ProductType, EHandType

hand = OmniHand.create_hand_by_zlgcan(
    ProductType.OMNIHAND_2025,
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)
```

**新代码：**
```python
from omnihand import OmniHand2025, EHandType

hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)
```

产品类型现在由您使用的类确定，提供了更好的类型安全性和编译时检查。

## 相关文档

- [SocketCAN 设置指南](SOCKETCAN_SETUP.md) - Linux SocketCAN 配置说明
- [OmniHand 2025 (O10) 运动学求解器 Python API](API_KINEMATICS_PYTHON_O10.md) - 运动学计算
- [OmniHand Pro 2025 (O12) 运动学求解器 Python API](API_KINEMATICS_PYTHON_O12.md) - 运动学计算
