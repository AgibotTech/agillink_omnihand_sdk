# OmniHand 2025 SDK - Linux (x64)

[English Documentation](README.md) | [产品概述 & API 文档](../../README_zh_cn.md)

📖 **[快速入门](../../doc/zh_cn/QUICK_START.md)** | 🔧 **[故障排除](../../doc/zh_cn/TROUBLESHOOTING.md)**

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
./install.sh                  # 安装 SDK
sudo ./setup_udev.sh          # 配置 USB 权限（首次需要，然后注销重新登录）
```

## 卸载

```bash
./uninstall.sh
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

## 使用方法

- **[快速入门指南](../../doc/zh_cn/QUICK_START.md)** - 5 分钟上手
- 示例代码：[cpp/demo/](cpp/demo/)、[python/demo/](python/demo/)
- 测试代码：[cpp/test/](cpp/test/)、[python/test/](python/test/)

详细 API 参考：
- [C++ API](../../doc/zh_cn/API_CPP.md)
- [Python API](../../doc/zh_cn/API_PYTHON.md)
- [ROS2 API](../../doc/zh_cn/API_ROS2.md)（仅限 Linux）

## 许可证

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
