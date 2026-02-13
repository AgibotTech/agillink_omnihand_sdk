# 快速入门指南

[English](../en/QUICK_START.md)

本指南帮助您在 5 分钟内开始使用 OmniHand 2025 SDK。

## 环境要求

### 硬件

| 接口 | 支持型号 | 说明 |
|------|----------|------|
| **ZLG USBCANFD**（推荐） | 全部 | 100U-mini, 100U, 200U |
| **HCAN** | 全部 | 用法与 ZLG 类似 |
| **SocketCAN**（Linux） | 全部 | 参见 [SocketCAN 配置](SOCKETCAN_SETUP.md) |
| **USB** | 仅 O10 | USB 直连 |
| **RS485** | 仅 O10 | 串口 |

### 软件

| | Linux | Windows |
|---|-------|---------|
| **操作系统** | Ubuntu 22.04+ | Windows 10/11 |
| **Python** | 3.10+ | 3.10+ |
| **C++ 编译器** | gcc 11.4+ | MSVC 2019+ |
| **CMake** | 3.24+ | 3.24+ |
| **ROS2**（可选） | Humble | ❌ 不支持 |

## 步骤 1：硬件连接

### 推荐：ZLG USBCANFD 适配器

1. 通过 USB 将 ZLG USBCANFD 适配器连接到电脑
2. 用 CANFD 线缆连接适配器和 OmniHand
3. 给 OmniHand 上电（24V 电源）

```
[电脑] --USB--> [ZLG USBCANFD] --CANFD--> [OmniHand]
                                      |
                                [24V 电源]
```

### 备选：USB 直连（仅限 OmniHand 2025 O10）

通过 USB 线直接连接 OmniHand 到电脑。

## 步骤 2：安装 SDK

### Linux

```bash
cd release/linux/x64
./install.sh

# 配置 USB 权限（仅首次需要）
sudo ./setup_udev.sh
# 然后注销并重新登录
```

### Windows

以管理员身份运行：
```cmd
cd release\windows
install.bat
```

## 步骤 3：验证安装

### Python

```bash
python3 -c "from omnihand import OmniHand2025; print('SDK 安装成功！')"
```

### C++

```bash
# 运行测试（Linux）
./cpp/bin/omnihand/test/test_omnihand_2025 --gtest_filter=*CreateHand*
```

## 步骤 4：第一个程序

### Python 示例

```python
from omnihand import OmniHand2025, HandType

# 创建灵巧手实例（ZLG USBCANFD）
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,    # 或 HandType.RIGHT
    hand_device_id=1,            # 灵巧手 ID（通常为 1）
    canfd_device_id=0,           # CANFD 适配器索引
    canfd_channel_id=0           # 通道（0 或 1）
)

# 初始化
if not hand.init():
    print("初始化失败")
    exit(1)

print("灵巧手初始化成功！")

# 获取设备信息
info = hand.get_device_info()
print(f"设备 ID: {info.hand_device_id}")

# 读取当前关节角度（弧度）
angles = hand.get_all_active_joint_angles()
print(f"当前角度: {angles}")

# 移动到目标位置（10 个关节，单位弧度）
target_angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
hand.set_all_active_joint_angles(target_angles)
print("已移动到目标位置")
```

### C++ 示例

**CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.14)
project(my_omnihand_app)

# 查找 omnihand 包
list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/omnihand")  # Linux
# list(APPEND CMAKE_MODULE_PATH "C:/Program Files/omnihand2025/share/cmake/omnihand")  # Windows
find_package(omnihand REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE omnihand)
```

**main.cpp:**

```cpp
#include "omnihand/omnihand_2025.h"
using namespace agilink::omnihand;
#include <iostream>

int main() {
    // createHandByZlgcan(hand_type, hand_device_id, canfd_device_id, canfd_channel_id)
    //   hand_type: eLeft 或 eRight
    //   hand_device_id: 灵巧手 CAN ID（1-254，通常为 1）
    //   canfd_device_id: ZLG 适配器索引（0 = 第一个适配器）
    //   canfd_channel_id: 通道（双通道适配器为 0 或 1）
    auto hand = OmniHand2025::createHandByZlgcan(HandType::LEFT, 1, 0, 0);
    
    if (!hand || !hand->Init()) {
        std::cerr << "初始化失败" << std::endl;
        return -1;
    }
    
    std::cout << "灵巧手初始化成功！" << std::endl;
    
    // 读取当前关节角度（弧度）
    auto angles = hand->GetAllActiveJointAngles();
    std::cout << "当前角度: ";
    for (auto a : angles) std::cout << a << " ";
    std::cout << std::endl;
    
    // 移动到目标位置（10 个关节，单位弧度）
    std::vector<double> target_angles(10, 0.0);
    hand->SetAllActiveJointAngles(target_angles);
    
    return 0;
}
```

### ROS2 示例（仅限 Linux）

```bash
# Source ROS2 和 OmniHand
source /opt/ros/humble/setup.bash
source ros2/setup.bash

# 运行节点（默认双手）
ros2 run omnihand_node omnihand_2025_node

# 或单手
ros2 run omnihand_node omnihand_2025_node --ros-args -p enable_both_hands:=false -p hand_type:=left
```

通过 ROS2 话题控制：

```bash
# 列出可用话题
ros2 topic list | grep omnihand

# 设置关节角度（10 个值，单位弧度）
ros2 topic pub /omnihand/omnihand_2025/left/motor_angle_cmd omnihand_2025_node_msgs/msg/MotorAngle "{angles: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}"

# 读取关节角度
ros2 topic echo /omnihand/omnihand_2025/left/motor_angle
```

## 步骤 5：探索示例

| 语言 | 路径 |
|------|------|
| Python | `python/demo/omnihand_2025/`, `python/demo/omnihand_pro_2025/` |
| C++ | `cpp/demo/omnihand_2025/`, `cpp/demo/omnihand_pro_2025/` |
| ROS2 | `ros2/`（仅限 Linux） |

## 下一步

- **API 文档**
  - [C++ API](API_CPP.md)
  - [Python API](API_PYTHON.md)
  - [ROS2 API](API_ROS2.md)（仅限 Linux）
- [故障排除](TROUBLESHOOTING.md) - 常见问题和解决方案
