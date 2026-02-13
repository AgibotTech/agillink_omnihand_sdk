# Quick Start Guide

[中文](../zh_cn/QUICK_START.md)

This guide helps you get started with the OmniHand 2025 SDK in 5 minutes.

## Requirements

### Hardware

| Interface | Supported Models | Notes |
|-----------|------------------|-------|
| **ZLG USBCANFD** (Recommended) | All | 100U-mini, 100U, 200U |
| **HCAN** | All | Similar to ZLG |
| **SocketCAN** (Linux) | All | See [SocketCAN Setup](SOCKETCAN_SETUP.md) |
| **USB** | O10 only | Direct connection |
| **RS485** | O10 only | Serial port |

### Software

| | Linux | Windows |
|---|-------|---------|
| **OS** | Ubuntu 22.04+ | Windows 10/11 |
| **Python** | 3.10+ | 3.10+ |
| **C++ Compiler** | gcc 11.4+ | MSVC 2019+ |
| **CMake** | 3.24+ | 3.24+ |
| **ROS2** (optional) | Humble | ❌ Not supported |

## Step 1: Hardware Connection

### Recommended: ZLG USBCANFD Adapter

1. Connect ZLG USBCANFD adapter to your computer via USB
2. Connect the CANFD cable from the adapter to your OmniHand
3. Power on the OmniHand (24V power supply)

```
[Computer] --USB--> [ZLG USBCANFD] --CANFD--> [OmniHand]
                                          |
                                    [24V Power]
```

### Alternative: USB Direct (OmniHand 2025 O10 only)

Connect the OmniHand directly to your computer via USB cable.

## Step 2: Install SDK

### Linux

```bash
cd release/linux/x64
./install.sh

# Configure USB permissions (first time only)
sudo ./setup_udev.sh
# Then log out and log back in
```

### Windows

Run as Administrator:
```cmd
cd release\windows
install.bat
```

## Step 3: Verify Installation

### Python

```bash
python3 -c "from omnihand import OmniHand2025; print('SDK installed successfully!')"
```

### C++

```bash
# Run test (Linux)
./cpp/bin/omnihand/test/test_omnihand_2025 --gtest_filter=*CreateHand*
```

## Step 4: First Program

### Python Example

```python
from omnihand import OmniHand2025, HandType

# Create hand instance (ZLG USBCANFD)
hand = OmniHand2025.create_hand_by_zlgcan(
    hand_type=HandType.LEFT,    # or HandType.RIGHT
    hand_device_id=1,            # Hand ID (usually 1)
    canfd_device_id=0,           # CANFD adapter index
    canfd_channel_id=0           # Channel (0 or 1)
)

# Initialize
if not hand.init():
    print("Failed to initialize hand")
    exit(1)

print("Hand initialized successfully!")

# Get device info
info = hand.get_device_info()
print(f"Device ID: {info.hand_device_id}")

# Read current joint angles (radians)
angles = hand.get_all_active_joint_angles()
print(f"Current angles: {angles}")

# Move to a position (all fingers open, 10 joints in radians)
target_angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
hand.set_all_active_joint_angles(target_angles)
print("Moved to target position")
```

### C++ Example

**CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.14)
project(my_omnihand_app)

# Find omnihand package
list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/omnihand")  # Linux
# list(APPEND CMAKE_MODULE_PATH "C:/Program Files/omnihand2025/share/cmake/omnihand")  # Windows
find_package(omnihand REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE omnihand)
```

**main.cpp:**

```cpp
#include "omnihand/omnihand_2025.h"
using namespace agilink::omnihand;
#include <iostream>

int main() {
    // createHandByZlgcan(hand_type, hand_device_id, canfd_device_id, canfd_channel_id)
    //   hand_type: eLeft or eRight
    //   hand_device_id: Hand CAN ID (1-254, usually 1)
    //   canfd_device_id: ZLG adapter index (0 = first adapter)
    //   canfd_channel_id: Channel (0 or 1 for dual-channel adapters)
    auto hand = OmniHand2025::createHandByZlgcan(HandType::LEFT, 1, 0, 0);
    
    if (!hand || !hand->Init()) {
        std::cerr << "Failed to initialize hand" << std::endl;
        return -1;
    }
    
    std::cout << "Hand initialized successfully!" << std::endl;
    
    // Read current joint angles (radians)
    auto angles = hand->GetAllActiveJointAngles();
    std::cout << "Current angles: ";
    for (auto a : angles) std::cout << a << " ";
    std::cout << std::endl;
    
    // Move to target position (10 joints in radians)
    std::vector<double> target_angles(10, 0.0);
    hand->SetAllActiveJointAngles(target_angles);
    
    return 0;
}
```

### ROS2 Example (Linux Only)

```bash
# Source ROS2 and OmniHand
source /opt/ros/humble/setup.bash
source ros2/setup.bash

# Run node (defaults to both hands)
ros2 run omnihand_node omnihand_2025_node

# Or single hand
ros2 run omnihand_node omnihand_2025_node --ros-args -p enable_both_hands:=false -p hand_type:=left
```

Control via ROS2 topics:

```bash
# List available topics
ros2 topic list | grep omnihand

# Set joint angles (10 values in radians)
ros2 topic pub /omnihand/omnihand_2025/left/motor_angle_cmd omnihand_2025_node_msgs/msg/MotorAngle "{angles: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}"

# Read joint angles
ros2 topic echo /omnihand/omnihand_2025/left/motor_angle
```

## Step 5: Explore Demos

| Language | Path |
|----------|------|
| Python | `python/demo/omnihand_2025/`, `python/demo/omnihand_pro_2025/` |
| C++ | `cpp/demo/omnihand_2025/`, `cpp/demo/omnihand_pro_2025/` |
| ROS2 | `ros2/` (Linux only) |

## Next Steps

- **API Documentation**
  - [C++ API](API_CPP.md)
  - [Python API](API_PYTHON.md)
  - [ROS2 API](API_ROS2.md) (Linux only)
- [Troubleshooting](TROUBLESHOOTING.md) - Common issues and solutions
