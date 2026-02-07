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

Or specify path:
```
install.bat "D:\omnihand2025"
```

## Uninstall

Run as Administrator:
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

## C++ Usage

```cmake
set(OMNIHAND_ROOT "C:/Program Files/omnihand2025")
list(APPEND CMAKE_MODULE_PATH "${OMNIHAND_ROOT}/share/cmake/omnihand")
find_package(omnihand REQUIRED)
target_link_libraries(your_target omnihand)
```

Example C++ code (Recommended: ZLG USB CANFD - Zero configuration):

```cpp
#include "omnihand/omnihand_2025.h"  // For OmniHand 2025 (O10)
// #include "omnihand/omnihand_pro_2025.h"  // For OmniHand Pro 2025 (O12)

int main() {
    // OmniHand 2025 (O10, 10 DOF) - Recommended: ZLG USB CANFD
    auto hand = OmniHand2025::createHandByZlgcan(EHandType::eLeft, 1, 0, 0);

    if (!hand || !hand->Init()) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    }

    // Set all joint angles (recommended: solver automatically converts to motor positions)
    std::vector<double> angles(10, 0.0);
    hand->SetAllActiveJointAngles(angles);

    return 0;
}
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
```

For more examples, see [python/demo/](python/demo/) directory.

## License

Mulan PSL v2 - Copyright (c) 2025, Agibot Co., Ltd.
