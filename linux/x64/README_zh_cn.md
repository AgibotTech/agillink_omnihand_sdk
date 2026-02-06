# OmniHand 2025 SDK - Linux (x64)

[English Documentation](README.md) | [产品概述 & API 文档](../../README_zh_cn.md)

## 系统要求

### 硬件要求

支持以下通信接口：

- **CANFD (USB 适配器) - 推荐**：ZLG USBCANFD 系列（USBCANFD-100U-mini/USBCANFD-100U/USBCANFD-200U）
  - ✅ **零配置**：SDK 包含库文件，开箱即用
  - ✅ **无需 root 权限**：用户空间库
  - ✅ **简单 API**：`OmniHand2025.create_hand_by_zlgcan(...)` / `OmniHandPro2025.create_hand_by_zlgcan(...)` / `OmniHandDexUMI.create_hand_by_zlgcan(...)`
- **SocketCAN (Linux 原生) - 高级**：Linux SocketCAN 支持的任何 CAN 接口（例如 can0, can1）
  - ⚠️ **需要驱动配置**：对于 USB CANFD 设备，需要编译和加载内核模块
  - ⚠️ **需要 root 权限**：用于加载驱动和配置接口
  - 💡 **使用场景**：适合已有 SocketCAN 环境（板载 CAN、其他 SocketCAN 设备）
- **RS485（仅 OmniHand 2025）**：串口通信
- **USB（仅 OmniHand 2025）**：USB 串口通信

### 软件要求

- **操作系统**：Ubuntu 22.04 (x86_64/aarch64) 或兼容的 Linux 发行版
- **编译器**：gcc 11.4+ 或兼容版本
- **构建工具**：CMake 3.24 或更高版本（用于构建示例）
- **Python**：3.10 或更高版本（用于 Python SDK）
- **ROS2**（可选）：Humble 或兼容发行版（用于 ROS2 支持）

## 快速安装

```bash
# 自动检测架构并安装
./install.sh

# 或指定安装路径
./install.sh /opt/omnihand
```

## 卸载

```bash
# 卸载（使用默认路径 /usr/local）
./uninstall.sh

# 或指定安装路径
./uninstall.sh /opt/omnihand
```

## 目录结构

```
x64/
├── cpp/
│   ├── share/cmake/omnihand/    # CMake 配置
│   ├── include/omnihand/        # 头文件
│   ├── lib/                     # C++ 库
│   ├── demo/                    # C++ 示例源码（不安装）
│   ├── test/                    # C++ 测试源码（不安装）
│   └── bin/omnihand/
│       ├── demo/                # 示例可执行文件
│       └── test/                # 测试可执行文件
├── python/
│   ├── *.whl                    # Python wheel
│   ├── demo/                    # Python 示例（不安装）
│   └── test/                    # Python 测试（不安装）
├── ros2/                        # ROS2 包
│   ├── humble/          # ROS2 发行版
│   └── setup.bash               # 自动检测 ROS 发行版
├── install.sh                   # 安装脚本
├── uninstall.sh                 # 卸载脚本
├── README.md                    # [English Documentation](README.md)
└── README_zh_cn.md              # 本文档（中文）
```

## ROS2 使用

ROS2 包不会自动安装，需要手动 source：

```bash
# 首先 source 系统 ROS2
source /opt/ros/humble/setup.bash

# 然后 source omnihand
source ros2/setup.bash

# 运行节点
ros2 run omnihand_node omnihand_2025_node  # 或 omnihand_pro_2025_node 用于 OmniHand Pro 2025

# 检查和管理 ROS2 节点
python3 ros2/humble/share/omnihand_node/scripts/check_ros2_nodes.py          # 列出活动节点
python3 ros2/humble/share/omnihand_node/scripts/check_ros2_nodes.py --kill  # 终止 omnihand 节点
python3 ros2/humble/share/omnihand_node/scripts/check_ros2_nodes.py --kill-all  # 终止所有 ROS2 节点
```

## C++ 使用

```cmake
# 库安装到 /usr/local/lib，头文件安装到 /usr/local/include
list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/omnihand")
find_package(omnihand REQUIRED)

add_executable(my_app main.cc)
target_link_libraries(my_app PRIVATE omnihand)
```

C++ 示例代码（推荐：ZLG USB CANFD - 零配置）：

```cpp
#include "omnihand/omnihand_2025.h"  // 用于 OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // 用于 OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - 推荐：ZLG USB CANFD
    auto hand_o10 = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand_o10 || !hand_o10->Init()) {
        std::cerr << "初始化失败" << std::endl;
        return -1;
    }

    // 设置所有关节角度（推荐：求解器自动转换为电机位置）
    std::vector<double> angles_o10(10, 0.0);
    hand_o10->SetAllActiveJointAngles(angles_o10);
    return 0;
}

// 高级：SocketCAN（仅 Linux，需要驱动配置）
// auto hand = OmniHand2025::createHandSocketCan(EHandType::eLeft, 1, "can0");
```

更多示例，请参阅 [cpp/demo/](cpp/demo/) 目录。

## Python 使用

```python
from omnihand import OmniHand2025, OmniHandPro2025, EHandType

# OmniHand 2025 (10 DOF) - 推荐：ZLG USB CANFD
hand_o10 = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.RIGHT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand_o10.init():
    print("初始化失败")
    exit(1)

# 高级：SocketCAN（仅 Linux，需要驱动配置）
# hand = OmniHand2025.create_hand_socketcan(EHandType.LEFT, 1, "can0")
```

更多示例，请参阅 [python/demo/](python/demo/) 目录。

## 许可证

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
