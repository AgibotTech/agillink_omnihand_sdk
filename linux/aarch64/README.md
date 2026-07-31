# AGILINK OmniHand SDK - Linux (aarch64) - Version 1.1.8

[中文文档](README_zh_cn.md) | [Overview & API Docs](../../README.md)

📖 **[Quick Start](../../doc/en/QUICK_START.md)** | 🔧 **[Troubleshooting](../../doc/en/TROUBLESHOOTING.md)**

## Install

```bash
./install.sh                  # Install SDK
sudo ./setup_udev.sh          # Configure USB permissions (first time only, then log out/in)
```

The ROS2 deliverable bundles the C++ SDK and device-vendor shared libraries
required by the ROS2 nodes. It does not include C++ headers, CMake development
files, or server executables. Before using the ROS2 nodes, run:

```bash
source ros2/setup.bash
```

When using a non-default installation prefix, also run:

```bash
export LD_LIBRARY_PATH="<prefix>/lib:${LD_LIBRARY_PATH:-}"
```

## Uninstall

```bash
./uninstall.sh
```

## Directory Structure

```
aarch64/
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
├── ros2/                        # ROS2 packages with bundled runtime libraries
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
