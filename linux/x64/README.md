# OmniHand 2025 SDK v1.0.0 - x64

This package is for x64 architecture.

## Overview

The OmniHand 2025 SDK supports three product models:

**OmniHand 2025 灵动款 (O10)**: A compact, high-DOF interactive dexterous hand featuring `10 active + 6 passive degrees of freedom`. Weighing only 500g, it utilizes CANFD communication interfaces and is equipped with `400+ tactile points and 0.1N array resolution, with maximum fingertip force of 5N`. It's suitable for various humanoid robots and robotic arms. Its compact, lightweight design and rich tactile interaction capabilities make it valuable for interactive services, research, education, and light-duty operations.

![](doc/pic/hand_o10.jpg)

**OmniHand Pro 2025 专业款 (O12)**: A 12-degree-of-freedom professional dexterous hand featuring precise operation and flexible control capabilities. It is equipped with tactile sensors and multiple control modes (position control, torque control, hybrid control), making it suitable for a wide range of applications including research and education, entertainment and commercial performances, exhibition guidance, and industrial scenarios.

![](doc/pic/hand_o12.jpg)

**OmniHand Dex UMI (O10 UMI)**: A read-only dexterous hand using UMI protocol, supporting periodic position and tactile sensor data reporting.

## Dexterous Hand Motor Index

**OmniHand 2025 灵动款 (O10)**: Has 10 degrees of freedom, indexed from 1 to 10. The corresponding control motors are shown in the following image:

![](doc/pic/hand_o10_motors.jpg)

**OmniHand Pro 2025 专业款 (O12)**: Has 12 degrees of freedom, indexed from 1 to 12. The corresponding control motors are shown in the following image:

![](doc/pic/hand_o12_motors.jpg)

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

### Software Requirements

- **Operating System**: Ubuntu 22.04 (x86_64/aarch64) or compatible Linux distributions
- **Compiler**: gcc 11.4+ or compatible
- **Build Tool**: CMake 3.24 or higher (for building examples)
- **Python**: 3.10 or higher (for Python SDK)
- **ROS2** (optional): Humble or compatible distribution (for ROS2 support)

## Quick Install

```bash
# Auto-detect architecture and install
./install.sh

# Or specify install path
./install.sh /opt/omnihand
```

## Uninstall

```bash
# Uninstall (using default path /usr/local)
./uninstall.sh

# Or specify install path
./uninstall.sh /opt/omnihand
```

## Directory Structure

```
x64/
├── cpp/
│   ├── share/
│   │   └── cmake/
│   │       └── omnihand/        # CMake config
│   ├── include/omnihand/        # Header files
│   ├── lib/                      # C++ libraries
│   ├── demo/                      # C++ demo source code (not installed)
│   │   ├── omnihand_2025/
│   │   ├── omnihand_pro_2025/
│   │   └── omnihand_dex_umi/
│   ├── test/                      # C++ test source code (not installed)
│   │   ├── test_omnihand_2025.cc
│   │   ├── test_omnihand_pro_2025.cc
│   │   └── test_omnihand_dex_umi.cc
│   └── bin/omnihand/
│       ├── demo/                  # Demo executables
│       └── test/                   # Test executables
├── python/
│   ├── *.whl                     # Python wheel
│   ├── demo/                     # Python demos (not installed)
│   │   ├── omnihand_2025/
│   │   ├── omnihand_pro_2025/
│   │   └── omnihand_dex_umi/
│   └── test/                      # Python tests (not installed)
├── ros2/                         # ROS2 packages (if available)
│   ├── humble/                   # ROS2 distribution
│   └── setup.bash                # Auto-detect ROS distribution
├── doc/                          # Documentation
├── install.sh                    # Install script
├── uninstall.sh                  # Uninstall script
├── README.md                     # This file (English)
└── README_zh_cn.md               # Chinese documentation
```

## ROS2 Usage

ROS2 packages are not auto-installed, source manually:

```bash
# First source system ROS2
source /opt/ros/humble/setup.bash

# Then source omnihand
source ros2/setup.bash

# Run node
ros2 run omnihand_node omnihand_2025_node  # or omnihand_pro_2025_node for OmniHand Pro 2025

# Check and manage ROS2 nodes
python3 ros2/humble/share/omnihand_node/scripts/check_ros2_nodes.py          # List active nodes
python3 ros2/humble/share/omnihand_node/scripts/check_ros2_nodes.py --kill  # Kill omnihand nodes
python3 ros2/humble/share/omnihand_node/scripts/check_ros2_nodes.py --kill-all  # Kill all ROS2 nodes
```

For detailed ROS2 API description, see [doc/en/API_ROS2.md](doc/en/API_ROS2.md) - Index page with links to product-specific ROS2 interfaces:
- [OmniHand 2025 (O10) ROS2 Interface](doc/en/API_ROS2_O10.md)
- [OmniHand Pro 2025 (O12) ROS2 Interface](doc/en/API_ROS2_O12.md)

## C++ Usage

```cmake
# Libraries installed to /usr/local/lib, headers to /usr/local/include
list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/omnihand")
find_package(omnihand REQUIRED)

add_executable(my_app main.cc)
target_link_libraries(my_app PRIVATE omnihand)  # unified library supports OmniHand 2025 (O10), OmniHand Pro 2025 (O12), and OmniHand Dex UMI (O10 UMI)
```

Example C++ code (Recommended: ZLG USB CANFD - Zero configuration):

```cpp
#include "omnihand/omnihand_2025.h"  // For OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // For OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - Recommended: ZLG USB CANFD
    //   hand_type    = EHandType::eLeft      // left or right hand
    //   device_id    = 1                     // device ID written in the hand firmware
    //   canfd_id     = 0                     // USB CANFD adapter index
    //   channel_id   = 0                     // CAN channel index on that adapter
    auto hand_o10 = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand_o10 || !hand_o10->Init()) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    }

    // Set all joint angles (recommended: solver automatically converts to motor positions)
    std::vector<double> angles_o10(10, 0.0);  // 10 joints, all set to 0 radians
    hand_o10->SetAllActiveJointAngles(angles_o10);
    return 0;
}

// Advanced: SocketCAN (Linux only, requires driver setup)
// #ifdef __linux__
// auto hand = OmniHand2025::createHandSocketCan(EHandType::eLeft, 1, "can0");
// #endif
```

For more demos, see [cpp/demo/](cpp/demo/) directory with product-specific subdirectories.

For detailed C++ API description, see [doc/en/API_CPP.md](doc/en/API_CPP.md) - Index page with links to product-specific APIs:
- [OmniHand 2025 (O10) C++ API](doc/en/API_CPP_O10.md)
- [OmniHand Pro 2025 (O12) C++ API](doc/en/API_CPP_O12.md)
- [OmniHand Dex UMI (O10 UMI) C++ API](doc/en/API_CPP_O10_UMI.md)

## Python Usage

```python
# Recommended: ZLG USB CANFD (Zero configuration)
from omnihand import OmniHand2025, OmniHandPro2025, EHandType

# OmniHand 2025 (10 DOF)
hand_o10 = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.RIGHT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

if not hand_o10.init():
    print("Failed to initialize")
    exit(1)

# OmniHand Pro 2025 (12 DOF)
hand_o12 = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    device_id=1,
    canfd_id=0,
    channel_id=0
)

# Advanced: SocketCAN (Linux only, requires driver setup)
# hand = OmniHand2025.create_hand_socketcan(EHandType.LEFT, 1, "can0")
```

For more examples, see [python/demo/README.md](python/demo/README.md).

For detailed Python API description, see [doc/en/API_PYTHON.md](doc/en/API_PYTHON.md) - Index page with links to product-specific APIs:
- [OmniHand 2025 (O10) Python API](doc/en/API_PYTHON_O10.md)
- [OmniHand Pro 2025 (O12) Python API](doc/en/API_PYTHON_O12.md)
- [OmniHand Dex UMI (O10 UMI) Python API](doc/en/API_PYTHON_UMI.md)

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
