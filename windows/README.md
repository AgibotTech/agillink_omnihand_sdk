# OmniHand 2025 SDK v1.0.0 - Windows

Supported architecture: x64

**Platform note**: This is the Windows release. The **ROS2 interface and SocketCAN** are not supported on Windows; they are available only on Linux. The API documentation in this package (e.g. `doc/en/`, `doc/zh_cn/`) excludes ROS2-related topics for Windows.

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
  - ✅ **No administrator privileges required**: User-space library
  - ✅ **Simple API**: `OmniHand2025.create_hand_by_zlgcan(...)` / `OmniHandPro2025.create_hand_by_zlgcan(...)` / `OmniHandDexUMI.create_hand_by_zlgcan(...)`
- **RS485 (OmniHand 2025 only)**: Serial port communication
- **Not supported on Windows**: ROS2 interface and SocketCAN are Linux-only; the bundled API documentation excludes ROS2.

### Software Requirements

- **Operating System**: Windows 10/11 (x64)
- **Compiler**: MSVC 2019+ or compatible
- **Build Tool**: CMake 3.24 or higher (for building examples)
- **Python**: 3.10 or higher (for Python SDK)

## Quick Install

Run as Administrator:
``````
install.bat
``````

Or specify path:
``````
install.bat `"D:\omnihand2025`"
``````

## Uninstall

Run as Administrator:
``````
uninstall.bat
``````

## Directory Structure

``````
windows/
├── cpp/
│   ├── share/
│   │   └── cmake/
│   │   └── omnihand/        # CMake config
│   ├── include/omnihand/         # Header files
│   ├── lib/                      # C++ libraries
│   ├── demo/                      # C++ demo source code (not installed)
│   │   ├── omnihand_2025/
│   │   ├── omnihand_pro_2025/
│   │   └── omnihand_dex_umi/
│   ├── test/                      # C++ test source code (not installed)
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
├── doc/                          # Documentation
├── install.bat                   # Install script
├── uninstall.bat                 # Uninstall script
├── README.md                     # This file (English)
└── README_zh_cn.md               # [中文文档](README_zh_cn.md)
``````

## C++ Usage

``````cmake
set(OMNIHAND_ROOT "C:/Program Files/omnihand2025")
list(APPEND CMAKE_MODULE_PATH `"`${OMNIHAND_ROOT}/share/cmake/omnihand`")
find_package(omnihand REQUIRED)
target_link_libraries(your_target omnihand)  # unified library supports OmniHand 2025 (O10), OmniHand Pro 2025 (O12), and OmniHand Dex UMI (O10 UMI)
``````

Example C++ code (Recommended: ZLG USB CANFD - Zero configuration):

``````cpp
#include "omnihand/omnihand_2025.h"  // For OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // For OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - Recommended: ZLG USB CANFD
    //   hand_type    = EHandType::eLeft      // left or right hand
    //   device_id    = 1                     // device ID written in the hand firmware
    //   canfd_id     = 0                     // USB CANFD adapter index
    //   channel_id   = 0                     // CAN channel index on that adapter
    auto hand = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand || !hand->Init()) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    }

    // Set all joint angles (recommended: solver automatically converts to motor positions)
    std::vector<double> angles(10, 0.0);  // 10 joints, all set to 0 radians
    hand->SetAllActiveJointAngles(angles);

    return 0;
}
``````

For more demos, see [cpp/demo/](cpp/demo/) directory with product-specific subdirectories.

For detailed C++ API description, see [doc/en/API_CPP.md](doc/en/API_CPP.md) - Index page with links to product-specific APIs:
- [OmniHand 2025 (O10) C++ API](doc/en/API_CPP_O10.md)
- [OmniHand Pro 2025 (O12) C++ API](doc/en/API_CPP_O12.md)
- [OmniHand Dex UMI (O10 UMI) C++ API](doc/en/API_CPP_O10_UMI.md)

## Python Usage

``````python
# Recommended: ZLG USB CANFD (Zero configuration)
from omnihand import OmniHand2025, OmniHandPro2025, EHandType

# OmniHand 2025 (10 DOF)
hand_o10 = OmniHand2025.create_hand_by_zlgcan(
    hand_type=EHandType.RIGHT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)

if not hand_o10.init():
    print("Failed to initialize")
    exit(1)

# OmniHand Pro 2025 (12 DOF)
hand_o12 = OmniHandPro2025.create_hand_by_zlgcan(
    hand_type=EHandType.LEFT,
    hand_device_id=1,
    canfd_device_id=0,
    canfd_channel_id=0
)
``````

For more examples, see [python/demo/README.md](python/demo/README.md).

For detailed Python API description, see [doc/en/API_PYTHON.md](doc/en/API_PYTHON.md) - Index page with links to product-specific APIs:
- [OmniHand 2025 (O10) Python API](doc/en/API_PYTHON_O10.md)
- [OmniHand Pro 2025 (O12) Python API](doc/en/API_PYTHON_O12.md)
- [OmniHand Dex UMI (O10 UMI) Python API](doc/en/API_PYTHON_UMI.md)

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
