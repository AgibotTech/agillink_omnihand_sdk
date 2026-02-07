# OmniHand 2025 SDK - Windows (x64)

[English Documentation](README.md) | [产品概述 & API 文档](../README_zh_cn.md)

📖 **[快速入门](../doc/zh_cn/QUICK_START.md)** | 🔧 **[故障排除](../doc/zh_cn/TROUBLESHOOTING.md)**

> **平台说明**：ROS2 接口与 SocketCAN **仅 Linux 支持**，Windows 不可用。

## 系统要求

### 硬件要求

支持以下通信接口：

- **CANFD (USB 适配器) - 推荐**：ZLG USBCANFD 系列（USBCANFD-100U-mini/USBCANFD-100U/USBCANFD-200U）
  - ✅ **零配置**：SDK 包含库文件，开箱即用
  - ✅ **无需管理员权限**：用户空间库
  - ✅ **简单 API**：`OmniHand2025.create_hand_by_zlgcan(...)` / `OmniHandPro2025.create_hand_by_zlgcan(...)` / `OmniHandDexUMI.create_hand_by_zlgcan(...)`
- **RS485（仅 OmniHand 2025）**：串口通信
- **USB（仅 OmniHand 2025）**：USB 串口通信

### 软件要求

- **操作系统**：Windows 10/11 (x64)
- **编译器**：MSVC 2019+ 或兼容版本
- **构建工具**：CMake 3.24 或更高版本（用于构建示例）
- **Python**：3.10 或更高版本（用于 Python SDK）

## 快速安装

以管理员身份运行：
```
install.bat
```

或指定路径：
```
install.bat "D:\omnihand2025"
```

## 卸载

以管理员身份运行：
```
uninstall.bat
```

## 目录结构

```
windows/
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
├── install.bat                  # 安装脚本
├── uninstall.bat                # 卸载脚本
├── README.md                    # [English Documentation](README.md)
└── README_zh_cn.md              # 本文档（中文）
```

## C++ 使用

```cmake
set(OMNIHAND_ROOT "C:/Program Files/omnihand2025")
list(APPEND CMAKE_MODULE_PATH "${OMNIHAND_ROOT}/share/cmake/omnihand")
find_package(omnihand REQUIRED)
target_link_libraries(your_target omnihand)
```

C++ 示例代码（推荐：ZLG USB CANFD - 零配置）：

```cpp
#include "omnihand/omnihand_2025.h"  // 用于 OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // 用于 OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - 推荐：ZLG USB CANFD
    auto hand = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand || !hand->Init()) {
        std::cerr << "初始化失败" << std::endl;
        return -1;
    }

    // 设置所有关节角度（推荐：求解器自动转换为电机位置）
    std::vector<double> angles(10, 0.0);
    hand->SetAllActiveJointAngles(angles);

    return 0;
}
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
```

更多示例，请参阅 [python/demo/](python/demo/) 目录。

## 许可证

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
