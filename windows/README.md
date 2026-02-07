# OmniHand 2025 SDK - Windows (x64)

[中文文档](README_zh_cn.md) | [Overview & API Docs](../README.md)

📖 **[Quick Start](../doc/en/QUICK_START.md)** | 🔧 **[Troubleshooting](../doc/en/TROUBLESHOOTING.md)**

> **Platform note**: ROS2 interface and SocketCAN are **Linux only** and not available on Windows.

## System Requirements

### Hardware Requirements

Supports the following communication interfaces:

- **CANFD (USB adapter) - Recommended**: ZLG USBCANFD series (USBCANFD-100U-mini/USBCANFD-100U/USBCANFD-200U)
  - ✅ **Zero configuration**: SDK includes libraries, ready to use out of the box
  - ✅ **No administrator privileges required**: User-space library
  - ✅ **Simple API**: `OmniHand2025.create_hand_by_zlgcan(...)` / `OmniHandPro2025.create_hand_by_zlgcan(...)` / `OmniHandDexUMI.create_hand_by_zlgcan(...)`
- **RS485 (OmniHand 2025 only)**: Serial port communication
- **USB (OmniHand 2025 only)**: USB serial communication

### Software Requirements

- **Operating System**: Windows 10/11 (x64)
- **Compiler**: MSVC 2019+ or compatible
- **Build Tool**: CMake 3.24 or higher (for building examples)
- **Python**: 3.10 or higher (for Python SDK)

## Quick Install

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
windows/
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

- **[Quick Start Guide](../doc/en/QUICK_START.md)** - Get started in 5 minutes
- Demo code: [cpp/demo/](cpp/demo/), [python/demo/](python/demo/)
- Test code: [cpp/test/](cpp/test/), [python/test/](python/test/)

For detailed API reference:
- [C++ API](../doc/en/API_CPP.md)
- [Python API](../doc/en/API_PYTHON.md)

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
