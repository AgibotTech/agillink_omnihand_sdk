# OmniHand 2025 SDK - Linux (x64)

[中文文档](README_zh_cn.md) | [Overview & API Docs](../../README.md)

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

## C++ Usage

```cmake
# Libraries installed to /usr/local/lib, headers to /usr/local/include
list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/omnihand")
find_package(omnihand REQUIRED)

add_executable(my_app main.cc)
target_link_libraries(my_app PRIVATE omnihand)
```

Example C++ code (Recommended: ZLG USB CANFD - Zero configuration):

```cpp
#include "omnihand/omnihand_2025.h"  // For OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // For OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - Recommended: ZLG USB CANFD
    auto hand_o10 = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand_o10 || !hand_o10->Init()) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    }

    // Set all joint angles (recommended: solver automatically converts to motor positions)
    std::vector<double> angles_o10(10, 0.0);
    hand_o10->SetAllActiveJointAngles(angles_o10);
    return 0;
}

// Advanced: SocketCAN (Linux only, requires driver setup)
// auto hand = OmniHand2025::createHandSocketCan(EHandType::eLeft, 1, "can0");
```

For more demos, see [cpp/demo/](cpp/demo/) directory.

## Python Usage

```python
from omnihand import OmniHand2025, OmniHandPro2025, EHandType

# OmniHand 2025 (10 DOF) - Recommended: ZLG USB CANFD
hand_o10 = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.RIGHT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand_o10.init():
    print("Failed to initialize")
    exit(1)

# Advanced: SocketCAN (Linux only, requires driver setup)
# hand = OmniHand2025.create_hand_socketcan(EHandType.LEFT, 1, "can0")
```

For more examples, see [python/demo/](python/demo/) directory.

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
