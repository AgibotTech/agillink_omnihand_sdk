# AGILINK OmniHand SDK - Linux (x64) - 版本 1.1.0

[English Documentation](README.md) | [产品概述 & API 文档](../../README_zh_cn.md)

📖 **[快速入门](../../doc/zh_cn/QUICK_START.md)** | 🔧 **[故障排除](../../doc/zh_cn/TROUBLESHOOTING.md)**

## 安装

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
