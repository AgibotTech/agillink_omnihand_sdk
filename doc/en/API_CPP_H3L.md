# OmniHand 3 Lite (H3L) C++ API

## Overview

**OmniHand 3 Lite (H3L)** is a 4 DOF lightweight dexterous hand. This document describes the C++ API for controlling and interacting with OmniHand 3 Lite devices.

**Key Features:**
- 4 active degrees of freedom
- Motor position range: 0-4096 (int16 motor ticks)
- Supports CAN (ZLG USB CANFD / HCAN) communication
- Supports SocketCAN (Linux only)
- Supports ZLG CAN TCP (Linux x64 / Windows only)
- **No tactile sensors**
- **No kinematics solver**: angle-based control is unavailable (`SetAllActiveJointAngles` is a stub). Use motor position control (`SetAllJointMotorPosi` / `SetAllJointMotorPosi`) instead.

## Include Header

```cpp
#include "omnihand/omnihand_3_lite.h"
#include <cstdint>
using namespace agilink::omnihand;
```

## Enums

### HandType

```cpp
enum class HandType : unsigned char {
    LEFT = 0,      // Left hand
    RIGHT = 1,     // Right hand
    UNKNOWN = 255  // Unknown hand type
};
```

### ControlMode

```cpp
namespace agilink {
namespace omnihand {
enum class ControlMode : unsigned char {
    POSITION                  = 0,    // Position mode (default)
    SERVO                     = 1,    // Servo mode (H3L does not support SetControlMode switching)
    VELOCITY                  = 2,    // Velocity mode (H3L does not support SetControlMode switching)
    TORQUE                    = 3,    // Torque mode (H3L not supported)
    POSITION_TORQUE           = 4,    // Position-Torque mode (Mixed control: position + current)
    VELOCITY_TORQUE           = 5,    // Velocity-Torque mode (H3L not yet available)
    POSITION_VELOCITY_TORQUE  = 6,    // Position-Velocity-Torque mode (H3L not yet available)
    UNKNOWN                   = 10    // Unknown mode
};
}  // namespace omnihand
}  // namespace agilink
```

**H3L available modes**:
- `POSITION` (0): Default position control
- `POSITION_TORQUE` (4): Position + current mixed control
- Other modes are not available or not yet open

## Data Structures

### VendorInfo

```cpp
struct VendorInfo {
    std::string productModel;     // Product model
    std::string productSeqNum;    // Product serial number
    Version hardwareVersion;      // Hardware version
    Version softwareVersion;      // Software version
    int16_t voltage;              // Supply voltage (mV)
    unsigned char dof;            // Degrees of Freedom (4 for H3L)

    std::string toString() const;
};
```

### DeviceInfo

```cpp
struct DeviceInfo {
    unsigned char hand_device_id; // Hand device ID
    CommuParams commu_params;     // Communication parameters
    std::string toString() const;
};
```

## Factory Methods

### Recommended: ZLG USB CANFD (Zero Configuration)

```cpp
/**
 * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
 * @param hand_type Hand type (left/right)
 * @param hand_device_id Device ID (default: 1)
 * @param canfd_device_id USB CANFD adapter device index (default: 0)
 * @param canfd_channel_id CAN channel index (default: 0). Dual-channel (200U): can0=0, can1=1; single-channel: always 0
 * @return A unique pointer to OmniHand3Lite instance
 * @note ✅ Recommended: Zero configuration, ready to use out of the box. No root privileges required.
 */
static std::unique_ptr<OmniHand3Lite> createHandByZlgcan(
    HandType hand_type,
    uint8_t hand_device_id = 1,
    uint8_t canfd_device_id = 0,
    uint8_t canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHand3Lite::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### Factory Method by Serial Number

```cpp
static std::unique_ptr<OmniHand3Lite> createHandByZlgcan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& usbcanfd_serial_number,
    uint8_t canfd_channel_id = 0);
```

### HCAN USB CANFD

```cpp
static std::unique_ptr<OmniHand3Lite> createHandByHcan(
    HandType hand_type,
    uint8_t hand_device_id,
    uint8_t canfd_device_id,
    uint8_t canfd_channel_id = 0);

static std::unique_ptr<OmniHand3Lite> createHandByHcan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& hcan_serial_number,
    uint8_t canfd_channel_id = 0);
```

### SocketCAN (Linux only)

```cpp
#ifdef __linux__
static std::unique_ptr<OmniHand3Lite> createHandSocketCan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

### ZLG CAN TCP (partial platform support)

```cpp
#if OMNIHAND_ZLG_TCP_SUPPORTED
static std::unique_ptr<OmniHand3Lite> createHandByZlgCanTcp(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& tcp_host,
    uint16_t tcp_port,
    uint8_t canfd_channel_id = 0);
#endif
```

## Core API

### Initialization

```cpp
bool Init();
```

All factory methods return an instance that requires `Init()` to complete initialization. Returns `false` on failure.

### Gesture Control

```cpp
enum class OmniHand3LiteGesture : int {
  OMNI_HAND_3_LITE_GESTURE_ALL_ZERO = 0,  // All-zero position
  OMNI_HAND_3_LITE_GESTURE_FIST,          // Fist
  OMNI_HAND_3_LITE_GESTURE_OPEN,          // Open hand
};

void SetHandGesture(OmniHand3LiteGesture gesture);
```

Preset gesture positions are defined for the right hand. The solver automatically mirrors motors 1 and 4 for the left hand.

### Motor Position Control

```cpp
int16_t SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi);
std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi);
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;
std::vector<int16_t> GetAllJointMotorPosi() const;
```

### Joint Angle Control (unavailable)

```cpp
// These methods are stubs on H3L. Use motor position control instead.
std::vector<double> SetAllActiveJointAngles(const std::vector<double>& angles);
std::vector<double> GetAllActiveJointAngles() const;
```

### Velocity Control

```cpp
void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo);
int16_t GetJointMotorVelo(unsigned char joint_motor_index) const;
void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo);
std::vector<int16_t> GetAllJointMotorVelo() const;
```

### Current Threshold Control

```cpp
void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold);
int16_t GetCurrentThreshold(unsigned char joint_motor_index) const;
void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds);
std::vector<int16_t> GetAllCurrentThreshold() const;
```

### Mixed Control

```cpp
void MixCtrlJointMotor(const std::vector<MixCtrl>& mix_ctrls);
```
- `ctrl_mode_`: only `POSITION_TORQUE` is available
- `tgt_posi_`: target position (0-4095 encoder raw value)
- `tgt_torque_`: target current (unit: mA)

### Error Handling

```cpp
JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const;
std::vector<JointMotorErrorReport> GetAllErrorReport() const;
```

### Temperature Monitoring

```cpp
int16_t GetTemperatureReport(unsigned char joint_motor_index) const;
std::vector<int16_t> GetAllTemperatureReport() const;
```

### Current Monitoring

```cpp
int16_t GetCurrentReport(unsigned char joint_motor_index) const;
std::vector<int16_t> GetAllCurrentReport() const;
```

### Vendor & Device Information

```cpp
VendorInfo GetVendorInfo() const;
DeviceInfo GetDeviceInfo() const;
void SetDeviceId(unsigned char device_id);
```

### Debugging

```cpp
void ShowDataDetails(bool show) const;
```

## Complete Example

```cpp
#include "omnihand/omnihand_3_lite.h"
#include <iostream>

int main() {
    auto hand = OmniHand3Lite::createHandByZlgcan(
        HandType::LEFT, 1, 0, 0);

    if (!hand || !hand->Init()) {
        std::cerr << "Failed to initialize OmniHand 3 Lite" << std::endl;
        return -1;
    }

    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // Set motor positions (4 joints, 0-4095)
    std::vector<int16_t> positions = {2048, 2048, 2048, 2048};
    auto echo = hand->SetAllJointMotorPosi(positions);

    auto temps = hand->GetAllTemperatureReport();
    return 0;
}
```

## Demo Files

| Demo | Path |
|------|------|
| CAN FD (connect by ID) | [H3L_demo_canfd_id.cc](../../../cpp/demo/omnihand_3_lite/H3L_demo_canfd_id.cc) |
| CAN FD (connect by serial) | [H3L_demo_canfd_serial.cc](../../../cpp/demo/omnihand_3_lite/H3L_demo_canfd_serial.cc) |

See [CMakeLists.txt](../../../cpp/demo/omnihand_3_lite/CMakeLists.txt) for build instructions.

## Related Documentation

- [OmniHand 3 Lite (H3L) Python API](API_PYTHON_H3L.md)
- [OmniHand 3 Lite (H3L) ROS2 Interface](API_ROS2_H3L.md)
- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md)
