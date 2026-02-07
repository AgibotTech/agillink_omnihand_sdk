# OmniHand 2025 SDK - Linux (x64)

[中文文档](README_zh_cn.md) | [Overview & API Docs](../../README.md)

📖 **[Quick Start](../../doc/en/QUICK_START.md)** | 🔧 **[Troubleshooting](../../doc/en/TROUBLESHOOTING.md)**

## System Requirements

### Hardware Requirements

Supports the following communication interfaces:

- **CANFD (USB adapter) - Recommended**: ZLG USBCANFD series (USBCANFD-100U-mini/USBCANFD-100U/USBCANFD-200U)
  - ✅ **Zero configuration**: SDK includes libraries, ready to use out of the box
  - ✅ **No root privileges required**: User-space library
  - ✅ **Simple API**: `OmniHand2025.create_hand_by_zlgcan(...)` / `OmniHandPro2025.create_hand_by_zlgcan(...)` / `OmniHandDexUMI.create_hand_by_zlgcan(...)`
- **SocketCAN (Linux native) - Advanced**: Any CAN interface supported by Linux SocketCAN (e.g., can0, can1)
  - ⚠️ **Requires driver setup**: For USB CANFD devices, you need to compile and load kernel modules
  - ⚠️ **Requires root privileges**: For loading drivers and configuring interfaces
  - 💡 **Use case**: Suitable if you already have SocketCAN environment (onboard CAN, other SocketCAN devices)
- **RS485 (OmniHand 2025 only)**: Serial port communication
- **USB (OmniHand 2025 only)**: USB serial communication

### Software Requirements

- **Operating System**: Ubuntu 22.04 (x86_64/aarch64) or compatible Linux distributions
- **Compiler**: gcc 11.4+ or compatible
- **Build Tool**: CMake 3.24 or higher (for building examples)
- **Python**: 3.10 or higher (for Python SDK)
- **ROS2** (optional): Humble or compatible distribution (for ROS2 support)

## Quick Install

```bash
./install.sh                  # Install SDK
sudo ./setup_udev.sh          # Configure USB permissions (first time only, then log out/in)
```

## Uninstall

```bash
./uninstall.sh
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
├── ros2/                        # ROS2 packages
│   ├── humble/          # ROS2 distribution
│   └── setup.bash               # Auto-detect ROS distribution
├── install.sh                   # Install script
├── uninstall.sh                 # Uninstall script
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
- [ROS2 API](../../doc/en/API_ROS2.md) (Linux only)

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
