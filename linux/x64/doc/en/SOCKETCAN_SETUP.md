# SocketCAN Setup Guide

**Applicable scenarios:**
- You already have a SocketCAN environment on Linux (onboard CAN, PCIe CAN, or other SocketCAN-compatible devices), or
- You need to map ZLG USBCANFD-200U and similar USB devices to `can0` for unified SocketCAN access.

For most users who only use ZLG USB-to-CANFD devices, **we recommend using the ZLG library method directly**, and you don't need the steps in this file.

---

## 1. Overview

- SocketCAN abstracts CAN devices as network interfaces (e.g., `can0`, `can1`) in the kernel.
- The SDK uses product-specific factory methods:
  - C++: `OmniHand2025::createHandSocketCan(...)`, `OmniHandPro2025::createHandSocketCan(...)`, `OmniHandDexUMI::createHandSocketCan(...)`
  - Python: `OmniHand2025.create_hand_socketcan(...)`, `OmniHandPro2025.create_hand_socketcan(...)`, `OmniHandDexUMI.create_hand_socketcan(...)`
  
  to communicate directly using these interface names.

**Note:**
- When using SocketCAN, the underlying driver is a kernel driver, no longer using the ZLG userspace library;
- The same USB device **cannot be used simultaneously** by the ZLG library and SocketCAN driver; they are mutually exclusive.

---

## 2. Check if SocketCAN Interface Already Exists

```bash
ip link show | grep -E "can[0-9]"
ls /sys/class/net | grep -E "^can"
```

If you already see interfaces like `can0`, `can1`, and can use `candump can0` to send/receive messages, you can skip to "Section 4: Using SocketCAN in the SDK".

---

## 3. Enable SocketCAN for ZLG USBCANFD Devices (Optional, Advanced)

If you are using ZLG USBCANFD-200U or similar USB devices and want to access them via SocketCAN, you can use the kernel driver example included in the repository (located at `cpp/third_party/zlgcan/socketcan`).

### 3.1 Install Build Dependencies

```bash
sudo apt-get update
sudo apt-get install -y gcc-12 build-essential linux-headers-$(uname -r) can-utils
```

### 3.2 Compile Kernel Module

```bash
cd cpp/third_party/zlgcan/socketcan
make clean
make
```

The generated `usbcanfd.ko` is the SocketCAN driver module.

### 3.3 Load Driver and Create `can0`/`can1`

It is recommended to use the vendor script:

```bash
cd cpp/third_party/zlgcan/socketcan
sudo bash driver_load.sh
```

The script will:
- Unload old `usbcanfd` / `can_dev` drivers (if any);
- Load `can_dev` and the newly compiled `usbcanfd.ko`;
- Create and configure `can0`, `can1` interfaces for the device (default bitrate 500 kbps, data bitrate 2 Mbps);
- Set interfaces to `UP` state.

After that, you can use:

```bash
ip link show can0
candump can0
```

to verify that SocketCAN is working properly.

> If you want to use different bitrates, refer to the examples in `driver_load.sh` or `readme.txt` and adjust the `ip link set can0 type can ...` parameters accordingly.

---

## 4. Using SocketCAN in the SDK

### 4.1 C++: `createHandSocketCan`

Prerequisites:
- SDK is installed (e.g., installed to `/usr/local`);
- `can0` has been properly configured via SocketCAN and is `up`.

Example for OmniHand 2025 (O10):

```cpp
#include "omnihand/omnihand_2025.h"

int main() {
    // SocketCAN example: using can0 interface for OmniHand 2025 (O10)
    // hand_type    : EHandType::eLeft / EHandType::eRight
    // device_id    : Device ID configured in hand firmware (usually 1)
    // can0         : Linux SocketCAN interface name
    auto hand = OmniHand2025::createHandSocketCan(
        EHandType::eLeft,
        1,
        "can0"
    );

    if (!hand || !hand->Init()) {
        std::cerr << "[Error]: Failed to initialize SocketCAN device (can0)." << std::endl;
        std::cerr << "Please check:" << std::endl;
        std::cerr << "  1. ip link show can0" << std::endl;
        std::cerr << "  2. sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on" << std::endl;
        std::cerr << "  3. sudo ip link set can0 up" << std::endl;
        return -1;
    }

    // Read vendor information
    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // Set motor positions for all joints (range: 0-4096 for O10)
    std::vector<int16_t> positions{500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094};
    hand->SetAllJointMotorPosi(positions);

    return 0;
}
```

Example for OmniHand Pro 2025 (O12):

```cpp
#include "omnihand/omnihand_pro_2025.h"

int main() {
    // SocketCAN example: using can0 interface for OmniHand Pro 2025 (O12)
    auto hand = OmniHandPro2025::createHandSocketCan(
        EHandType::eLeft,
        1,
        "can0"
    );

    if (!hand || !hand->Init()) {
        std::cerr << "[Error]: Failed to initialize SocketCAN device (can0)." << std::endl;
        return -1;
    }

    // Set motor positions for all joints (range: 0-2000 for O12)
    std::vector<int16_t> positions{500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500};
    hand->SetAllJointMotorPosi(positions);

    return 0;
}
```

### 4.2 Python: `create_hand_socketcan`

Same prerequisites: `can0` must be configured.

Example for OmniHand 2025 (O10):

```python
from omnihand import OmniHand2025, EHandType

def main():
    # SocketCAN example for OmniHand 2025 (O10)
    hand = OmniHand2025.create_hand_socketcan(
        hand_type=EHandType.LEFT,
        device_id=1,       # Device ID in hand firmware
        can_interface="can0"
    )

    if not hand.init():
        print("[Error]: Failed to initialize SocketCAN device (can0).")
        print("Please check:")
        print("  1. ip link show can0")
        print("  2. sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on")
        print("  3. sudo ip link set can0 up")
        return

    vendor = hand.get_vendor_info()
    print(vendor)

    # Set motor positions (range: 0-4096 for O10)
    positions = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
    hand.set_all_joint_positions(positions)

if __name__ == "__main__":
    main()
```

Example for OmniHand Pro 2025 (O12):

```python
from omnihand import OmniHandPro2025, EHandType

def main():
    # SocketCAN example for OmniHand Pro 2025 (O12)
    hand = OmniHandPro2025.create_hand_socketcan(
        hand_type=EHandType.LEFT,
        device_id=1,
        can_interface="can0"
    )

    if not hand.init():
        print("[Error]: Failed to initialize SocketCAN device (can0).")
        return

    # Set motor positions (range: 0-2000 for O12)
    positions = [500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500]
    hand.set_all_joint_positions(positions)

if __name__ == "__main__":
    main()
```

> For more complete Python examples, refer to `python/demo/omnihand_2025/demo_socketcan.py` or `python/demo/omnihand_pro_2025/demo_socketcan.py`.

---

## 5. Switching Between ZLG Library and SocketCAN

For the same ZLG USB device:
- **When using ZLG library method (recommended)**: Do not load `usbcanfd.ko`, use product-specific factory methods like `OmniHand2025::createHandByZlgcan(..., canfd_device_id, canfd_channel_id)` / `OmniHand2025.create_hand_by_zlgcan(..., canfd_device_id, canfd_channel_id)` directly;
- **When using SocketCAN method**: First ensure the kernel driver is loaded and `can0` is created, then use product-specific factory methods like `OmniHand2025::createHandSocketCan` / `OmniHand2025.create_hand_socketcan`.

Switching approach:
- If `usbcanfd.ko` is currently loaded and you want to return to ZLG library method:

```bash
sudo rmmod usbcanfd
sudo rmmod can_dev   # Ignore if no error
```

Then rerun examples or your own programs that use `canfd_device_id` / `canfd_channel_id`.

---

## 6. Quick Configuration Commands

For onboard CAN or other SocketCAN devices, configure the interface:

```bash
# Configure CAN interface
sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on

# Bring interface up
sudo ip link set can0 up

# Verify interface status
ip link show can0

# Test with candump (optional)
candump can0
```

For USB CANFD devices using the kernel driver, use the vendor script as described in Section 3.3.
