# SocketCAN User Guide

[简体中文](../zh_cn/SOCKETCAN_SETUP.md)

> **Linux only.** SocketCAN is a Linux kernel feature; it is **not** available on Windows.  
> If you use the **ZLG USBCANFD library**, you **do not** need this guide.

---

## 1. Download and install the SocketCAN driver from the vendor

Follow the **vendor PDF or package instructions** exactly (dependencies, build/install module, `insmod`/`modprobe`, USB checks).  

**Note:** Driver packages must match your kernel version. If the vendor provides an install script, prefer that. After this step, a CAN network interface should appear (see next section).

---

## 2. Check interfaces with `ip link show`

Run:

```bash
ip link show
```

Look for **`can0`**, **`can1`**, etc. (or `ip link show can0` for one interface).

- If interfaces exist, continue to **Section 3** to set OmniHand bit timings.  
- If **no** `can` interface appears, go back to **Section 1** (driver, USB enumeration) or the vendor troubleshooting guide.

Optional:

```bash
ls /sys/class/net | grep -E '^can'
```

---

## 3. Set the bus parameters of OmniHand

Typical OmniHand CANFD settings:

- **Arbitration:** 1 Mbps, **80%** sample point  
- **Data:** 5 Mbps, **75%** sample point  

The bundled helper `setup_socketcan.sh` (shipped next to `install.sh`)
enumerates every `canN` under `/sys/class/net/` and applies the three
`ip link` commands for each. Run once after driver install, or whenever
the interface is re-created:

```bash
sudo ./setup_socketcan.sh              # every canN on the system
sudo ./setup_socketcan.sh can0 can3    # only the listed interfaces
     ./setup_socketcan.sh --dry-run    # preview, no changes
```

Equivalent raw commands if you prefer the long form (single interface):

```bash
sudo ip link set can0 down
sudo ip link set can0 type can fd on bitrate 1000000 dbitrate 5000000 sample-point 0.8 dsample-point 0.75
sudo ip link set can0 up
```

Verify:

```bash
ip -details link show can0
```

---

## 4. First program

Prerequisite: `can0` (or your interface) is configured per Section 3 and **UP**.

**C++:**

```cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(demo_omnihand_2025_socketcan O10_demo_socketcan.cc)
  target_link_libraries(demo_omnihand_2025_socketcan PRIVATE ${OMNIHAND_TARGET})
endif()
```

```cpp
#include "omnihand/omnihand_2025.h"
#include <vector>
// ...
auto hand = OmniHand2025::createHandSocketCan(HandType::LEFT, 1, "can0");
if (!hand || !hand->Init()) { /* verify Sections 2–3 */ }
hand->SetAllActiveJointAngles(std::vector<double>(10, 0.0));  // O10, radians
```

**Python:**

```python
from omnihand import OmniHand2025, HandType
hand = OmniHand2025.create_hand_socketcan(
    hand_type=HandType.LEFT, hand_device_id=1, can_interface="can0")
if not hand.init():
    ...
hand.set_all_active_joint_angles([0.0] * 10)  # O10, radians
```

## 5. API documentation

**C++**

- [C++ API index](API_CPP.md)  
- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md)  
- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)  
- [OmniHand Dex UMI C++ API](API_CPP_O10_UMI.md)

**Python**

- [Python API index](API_PYTHON.md)  
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md)  
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)  
- [OmniHand Dex UMI Python API](API_PYTHON_UMI.md)

**ROS2 (Linux only)**

- [ROS2 API index](API_ROS2.md)  
- [OmniHand 2025 (O10) ROS2](API_ROS2_O10.md)  
- [OmniHand Pro 2025 (O12) ROS2](API_ROS2_O12.md)

See also [Quick Start](QUICK_START.md) and [Troubleshooting](TROUBLESHOOTING.md).

---

## 6. Unload Socketcan kernel modules

To **stop** using the SocketCAN kernel driver:

```bash
sudo rmmod usbcanfd
```

If you need to remove `can_dev` as well (depends on your system):

```bash
sudo rmmod can_dev
```

Harmless messages if a module is not loaded are normal. After removal, `can0` disappears; use Sections 1–3 again or the vendor script to restore SocketCAN.
