# OmniHand 2025 SDK v1.0.0 - x64

本软件包适用于 x64 架构。

## 产品概述

OmniHand 2025 SDK 支持三种产品型号：

**OmniHand 2025 灵动款 (O10)**：紧凑型高自由度交互灵巧手，具有 `10 个主动 + 6 个被动自由度`。重量仅 500g，采用 CANFD 通信接口，配备 `400+ 触觉点，0.1N 阵列分辨率，最大指尖力 5N`。适用于各种人形机器人和机械臂。其紧凑轻量化的设计和丰富的触觉交互能力，使其在交互服务、研究教育、轻量作业等领域具有重要价值。

![](doc/pic/hand_o10.jpg)

**OmniHand Pro 2025 专业款 (O12)**：12 自由度专业灵巧手，具有精确操作和灵活控制能力。配备触觉传感器和多种控制模式（位置控制、力矩控制、混合控制），适用于研究教育、娱乐商业演出、展览引导、工业场景等多种应用。

![](doc/pic/hand_o12.jpg)

**OmniHand Dex UMI (O10 UMI)**：使用 UMI 协议的只读灵巧手，支持周期性的位置和触觉传感器数据上报。

## 灵巧手电机索引

**OmniHand 2025 灵动款 (O10)**：具有 10 个自由度，索引从 1 到 10。对应的控制电机如下图所示：

![](doc/pic/hand_o10_motors.jpg)

**OmniHand Pro 2025 专业款 (O12)**：具有 12 个自由度，索引从 1 到 12。对应的控制电机如下图所示：

![](doc/pic/hand_o12_motors.jpg)

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
│   ├── share/
│   │   └── cmake/
│   │       └── omnihand/        # CMake 配置
│   ├── include/omnihand/        # 头文件
│   ├── lib/                      # C++ 库
│   ├── demo/                      # C++ 示例源码（不安装）
│   │   ├── omnihand_2025/
│   │   ├── omnihand_pro_2025/
│   │   └── omnihand_dex_umi/
│   ├── test/                      # C++ 测试源码（不安装）
│   │   ├── test_omnihand_2025.cc
│   │   ├── test_omnihand_pro_2025.cc
│   │   └── test_omnihand_dex_umi.cc
│   └── bin/omnihand/
│       ├── demo/                  # 示例可执行文件
│       └── test/                   # 测试可执行文件
├── python/
│   ├── *.whl                     # Python wheel
│   ├── demo/                     # Python 示例（不安装）
│   │   ├── omnihand_2025/
│   │   ├── omnihand_pro_2025/
│   │   └── omnihand_dex_umi/
│   └── test/                      # Python 测试（不安装）
├── ros2/                         # ROS2 包（如果可用）
│   ├── humble/                   # ROS2 发行版
│   └── setup.bash                # 自动检测 ROS 发行版
├── doc/                          # 文档
├── install.sh                    # 安装脚本
├── uninstall.sh                  # 卸载脚本
├── README.md                     # [English Documentation](README.md)
└── README_zh_cn.md               # 本文档（中文）
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

详细的 ROS2 API 说明，请参阅 [doc/zh_cn/API_ROS2.md](doc/zh_cn/API_ROS2.md) - 包含产品特定 ROS2 接口链接的索引页：
- [OmniHand 2025 (O10) ROS2 接口](doc/zh_cn/API_ROS2_O10.md)
- [OmniHand Pro 2025 (O12) ROS2 接口](doc/zh_cn/API_ROS2_O12.md)

## C++ 使用

```cmake
# 库安装到 /usr/local/lib，头文件安装到 /usr/local/include
list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/omnihand")
find_package(omnihand REQUIRED)

add_executable(my_app main.cc)
target_link_libraries(my_app PRIVATE omnihand)  # 统一库支持 OmniHand 2025 (O10)、OmniHand Pro 2025 (O12) 和 OmniHand Dex UMI (O10 UMI)
```

C++ 示例代码（推荐：ZLG USB CANFD - 零配置）：

```cpp
#include "omnihand/omnihand_2025.h"  // 用于 OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // 用于 OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - 推荐：ZLG USB CANFD
    //   hand_type    = EHandType::eLeft      // 左手或右手
    //   device_id    = 1                     // 写入灵巧手固件的设备 ID
    //   canfd_id     = 0                     // USB CANFD 适配器索引
    //   channel_id   = 0                     // 该适配器上的 CAN 通道索引
    auto hand_o10 = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand_o10 || !hand_o10->Init()) {
        std::cerr << "初始化失败" << std::endl;
        return -1;
    }

    // 设置所有关节角度（推荐：求解器自动转换为电机位置）
    std::vector<double> angles_o10(10, 0.0);  // 10 个关节，全部设置为 0 弧度
    hand_o10->SetAllActiveJointAngles(angles_o10);
    return 0;
}

// 高级：SocketCAN（仅 Linux，需要驱动配置）
// #ifdef __linux__
// auto hand = OmniHand2025::createHandSocketCan(EHandType::eLeft, 1, "can0");
// #endif
```

更多示例，请参阅 [cpp/demo/](cpp/demo/) 目录，包含产品特定的子目录。

详细的 C++ API 说明，请参阅 [doc/zh_cn/API_CPP.md](doc/zh_cn/API_CPP.md) - 包含产品特定 API 链接的索引页：
- [OmniHand 2025 (O10) C++ API](doc/zh_cn/API_CPP_O10.md)
- [OmniHand Pro 2025 (O12) C++ API](doc/zh_cn/API_CPP_O12.md)
- [OmniHand Dex UMI (O10 UMI) C++ API](doc/zh_cn/API_CPP_O10_UMI.md)

## Python 使用

```python
# 推荐：ZLG USB CANFD（零配置）
from omnihand import OmniHand2025, OmniHandPro2025, EHandType

# OmniHand 2025 (10 DOF)
hand_o10 = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.RIGHT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand_o10.init():
    print("初始化失败")
    exit(1)

# OmniHand Pro 2025 (12 DOF)
hand_o12 = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

# 高级：SocketCAN（仅 Linux，需要驱动配置）
# hand = OmniHand2025.create_hand_socketcan(EHandType.LEFT, 1, "can0")
```

更多示例，请参阅 [python/demo/README.md](python/demo/README.md)。

详细的 Python API 说明，请参阅 [doc/zh_cn/API_PYTHON.md](doc/zh_cn/API_PYTHON.md) - 包含产品特定 API 链接的索引页：
- [OmniHand 2025 (O10) Python API](doc/zh_cn/API_PYTHON_O10.md)
- [OmniHand Pro 2025 (O12) Python API](doc/zh_cn/API_PYTHON_O12.md)
- [OmniHand Dex UMI (O10 UMI) Python API](doc/zh_cn/API_PYTHON_UMI.md)

## 许可证

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
