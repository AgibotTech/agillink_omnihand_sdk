# AGILINK OmniHand SDK - Windows (x64) - Version 1.1.8

[中文文档](README_zh_cn.md) | [Overview & API Docs](../../README.md)

📖 **[Quick Start](../../doc/en/QUICK_START.md)** | 🔧 **[Troubleshooting](../../doc/en/TROUBLESHOOTING.md)**

> **Note**: ROS2 and SocketCAN are **Linux only**.

## Install

Run as Administrator:
```
install.bat
```

## Uninstall

```
uninstall.bat
```

## Directory Structure

```
x64/
├── cpp/
│   ├── share/cmake/omnihand/    # CMake config
│   ├── include/omnihand/        # Header files
│   ├── lib/                     # C++ libraries
│   ├── demo/                    # C++ demo source code (not installed)
│   ├── test/                    # C++ test source code (not installed)
│   └── bin/omnihand/
│       ├── demo/                # Demo executables
│       └── test/                # Test executables
├── python/
│   ├── *.whl                    # Python wheel
│   ├── demo/                    # Python demos (not installed)
│   └── test/                    # Python tests (not installed)
├── install.bat                  # Install script
├── uninstall.bat                # Uninstall script
├── README.md                    # This file (English)
└── README_zh_cn.md              # Chinese documentation
```

## Usage

- **[Quick Start Guide](../../doc/en/QUICK_START.md)** - Get started in 5 minutes
- Demo code: [cpp/demo/](cpp/demo/), [python/demo/](python/demo/)
- Test code: [cpp/test/](cpp/test/), [python/test/](python/test/)

For detailed API reference:
- [C++ API](../../doc/en/API_CPP.md)
- [Python API](../../doc/en/API_PYTHON.md)

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
