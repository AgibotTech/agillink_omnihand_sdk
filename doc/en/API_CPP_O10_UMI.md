# OmniHand Dex UMI (O10 UMI) C++ API

## Overview

**OmniHand Dex UMI (O10 UMI)** is a 10 DOF dexterous hand using the UMI protocol. This document describes the C++ API for controlling and interacting with OmniHand Dex UMI (O10 UMI) devices.

**Key Features:**
- 10 active degrees of freedom
- 1D tactile sensors (fingers and palm, no dorsum)
- UMI protocol (Pn1-Pn8 registers)
- Active position query (no periodic reports)
- Supports CAN (ZLG USB CANFD) communication only
- Supports SocketCAN (Linux only)
- **Read-only position information** (no position/velocity/torque control)

## Include Header

```cpp
#include "omnihand/omnihand_dex_umi.h"
#include <cstdint>  // std::uint8_t for factory parameters (also pulled in by the header)
```

**Note:** Factory methods use **`std::uint8_t`** for `hand_device_id` and CAN adapter indices (`canfd_device_id`, `canfd_channel_id`), consistent with `omnihand_dex_umi.h`.

## Enums

### HandType

```cpp
enum class HandType : unsigned char {
    LEFT = 0,      // Left hand
    RIGHT = 1,     // Right hand
    UNKNOWN = 255  // Unknown hand type
};
```

### Finger

```cpp
enum class Finger : unsigned char {
    THUMB = 0x01,    // Thumb
    INDEX = 0x02,    // Index finger
    MIDDLE = 0x03,   // Middle finger
    RING = 0x04,     // Ring finger
    LITTLE = 0x05,   // Little (pinky) finger
    PALM = 0x06,     // Palm
    DORSUM = 0x07,   // Dorsum (not supported by UMI)
    UNKNOWN = 0xff   // Unknown
};
```

**Note**
- UMI supports finger and palm sensors (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM), but does not support dorsum (DORSUM) sensor.
- **UMI is a read-only device**: OmniHand Dex UMI (O10 UMI) does not support position, velocity, or torque control, so these control mode enums are not available for UMI devices.

## Data Structures

### VendorInfo

```cpp
struct VendorInfo {
    std::string productModel;     // Product model
    std::string productSeqNum;    // Product serial number
    Version hardwareVersion;      // Hardware version
    Version softwareVersion;      // Software version
    int16_t voltage;              // Supply voltage (mV)
    unsigned char dof;            // Degrees of Freedom (10 for UMI)

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

### TactileSensorData

```cpp
struct TactileSensorData {
    Finger sensor_id_;           // Sensor ID (finger/palm, UMI has no dorsum)
    std::vector<uint8_t> data_;   // Sensor data (unit: 1g, max: 255g)
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
 * @param canfd_channel_id CAN channel index (default: 0, USBCANFD-200U has 2 channels: 0 and 1)
 * @return A unique pointer to OmniHandDexUMI instance
 * @note Device type (200U/100U/MINI) is automatically detected internally.
 * @note ✅ Recommended: Zero configuration, ready to use out of the box. No root privileges required.
 */
static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
    HandType hand_type,
    uint8_t hand_device_id = 1,
    uint8_t canfd_device_id = 0,
    uint8_t canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHandDexUMI::createHandByZlgcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### Factory Method by Serial Number

```cpp
/**
 * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
 * @param hand_type Hand type (left/right)
 * @param hand_device_id Hand device ID
 * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
 * @param canfd_channel_id CAN channel index (default: 0). For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0
 * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
 */
static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& usbcanfd_serial_number,
    uint8_t canfd_channel_id = 0);
```

### HCAN USB CANFD

```cpp
/**
 * @brief Factory method - HCAN USB CANFD communication by device ID
 * @param hand_type Hand type (left/right)
 * @param hand_device_id Hand device ID
 * @param canfd_device_id HCAN device index
 * @param canfd_channel_id CAN channel index (default: 0). For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0
 * @return A unique pointer to OmniHandDexUMI instance
 */
static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
    HandType hand_type,
    uint8_t hand_device_id,
    uint8_t canfd_device_id,
    uint8_t canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHandDexUMI::createHandByHcan(
    HandType::LEFT,
    1,      // hand_device_id
    0,      // canfd_device_id
    0       // canfd_channel_id
);
```

### Factory Method by HCAN Serial Number

```cpp
/**
 * @brief Factory method - HCAN USB CANFD communication by serial number
 * @param hand_type Hand type (left/right)
 * @param hand_device_id Hand device ID
 * @param hcan_serial_number HCAN device serial number (supports partial matching)
 * @param canfd_channel_id CAN channel index (default: 0). For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0
 * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
 */
static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& hcan_serial_number,
    uint8_t canfd_channel_id = 0);
```

### Advanced: SocketCAN (Linux Only)

```cpp
#ifdef __linux__
/**
 * @brief Factory method - SocketCAN communication (Linux native CAN interface)
 * @param hand_type Hand type (left/right)
 * @param hand_device_id Device ID
 * @param can_interface CAN interface name (e.g., "can0", "can1")
 * @return A unique pointer to OmniHandDexUMI instance
 * @warning ⚠️ Advanced Usage: Requires driver setup and root privileges.
 */
static std::unique_ptr<OmniHandDexUMI> createHandSocketCan(
    HandType hand_type,
    uint8_t hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

## Basic Information

```cpp
/**
 * @brief Get product type
 * @return ProductType::OMNIHAND_DEX_UMI
 */
ProductType GetProductType() const;

/**
 * @brief Check initialization status
 * @return true if initialized successfully, false otherwise
 */
bool Init() const;
```

## Device Information

```cpp
/**
 * @brief Gets vendor information.
 * @return VendorInfo structure containing product model, serial number, hardware version, software version, etc.
 */
VendorInfo GetVendorInfo() const;

/**
 * @brief Gets device information.
 * @return DeviceInfo structure containing device ID and communication parameters.
 */
DeviceInfo GetDeviceInfo() const;

/**
 * @brief Sets the device ID.
 * @param hand_device_id The device ID.
 */
void SetDeviceId(unsigned char hand_device_id);
```

## Position Query

**Note**: UMI protocol supports active position query via Pn3 (0x13) register.

```cpp
/**
 * @brief Get single joint motor position (UMI Protocol Pn3=0x13, sub-register 0x01-0x0A)
 * @param joint_motor_index Joint motor index (1-10)
 * @return Joint position value (0-4096), -1 if error
 */
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;

/**
 * @brief Get all joint motor positions (UMI Protocol Pn3=0x13, sub-register 0x00)
 * @return Vector of all joint positions (10 values, range 0-4096), empty if error
 */
std::vector<int16_t> GetAllJointMotorPosi() const;
```

## Joint Angles

```cpp
/**
 * @brief Get 10 active joint angles
 * 
 * Converts motor positions (0-4096) to angles via calibration parameters.
 * @return 10 active joint angles in rad
 */
std::vector<double> GetAllActiveJointAngles() const;
```

## Position Calibration

**Note**: UMI protocol supports position calibration via Pn7/Pn8 register. This is a write-only operation.

```cpp
/**
 * @brief Set minimum position calibration for all 10 joints (UMI Protocol Pn8=0x08, sub-register 0x00).
 * @note This is a write-only operation for position calibration.
 *       The device should be in minimum position when calling this function.
 */
void SetMinPositionCalibration();

/**
 * @brief Set minimum position calibration for a single joint (UMI Protocol Pn8=0x08, sub-register 0x01-0x0A).
 * @param joint_index Joint index (1-10, where 1 is the first joint)
 * @note This is a write-only operation for position calibration.
 */
void SetMinPositionCalibration(unsigned char joint_index);

/**
 * @brief Set maximum position calibration for all 10 joints (UMI Protocol Pn7=0x07, sub-register 0x00).
 * @note This is a write-only operation for position calibration.
 *       The device should be in maximum position when calling this function.
 */
void SetMaxPositionCalibration();

/**
 * @brief Set maximum position calibration for a single joint (UMI Protocol Pn7=0x07, sub-register 0x01-0x0A).
 * @param joint_index Joint index (1-10, where 1 is the first joint)
 * @note This is a write-only operation for position calibration.
 */
void SetMaxPositionCalibration(unsigned char joint_index);
```


## Tactile Sensor Data

OmniHand Dex UMI (O10 UMI) uses **1D tactile sensors** similar to OmniHand 2025 (O10):
- **Data unit**: 1g
- **Max value**: 255g
- **Sensor locations**: Fingers (Thumb, Index, Middle, Ring, Little), Palm (Note: UMI has no Dorsum sensor)
- **Protocol**: UMI Protocol Pn6 (read-only)
  - **Pn6.00**: Read all sensor data (6 sensors)
  - **Pn6.01~Pn6.06**: Read individual sensor data (sensor 1-6)

```cpp
/**
 * @brief Gets all 1D tactile sensor raw data from all sensors at once.
 * @return Vector of TactileSensorData structures
 * @note This returns full resolution data.
 * @note Uses UMI Protocol Pn6.
 * @note UMI devices have 6 sensors: Thumb, Index, Middle, Ring, Little, Palm (no dorsum sensor)
 */
std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const;

/**
 * @brief Gets 1D tactile sensor raw data for a single sensor.
 * @param eFinger Finger/palm enum value
 * @return TactileSensorData structure containing full resolution data
 * @note Uses UMI Protocol Pn6.
 * @note UMI does not support DORSUM (dorsum sensor)
 */
TactileSensorData GetTactileSensorDataRaw(Finger eFinger) const;

/**
 * @brief Get sensor data length for a specific finger (static method)
 * @param eFinger Finger enum value
 * @return Sensor data length in bytes
 * @note For UMI: Returns 0 for DORSUM (UMI does not have dorsum sensor)
 */
static size_t GetSensorDataLength(Finger eFinger);

/**
 * @brief Get sensor order vector (static method)
 * @return Reference to sensor order vector
 * @note For UMI: The returned vector includes DORSUM, but UMI devices do not have dorsum sensor.
 *       When using GetAllTactileSensorDataRaw(), only sensors available on UMI (Thumb, Index, Middle, Ring, Little, Palm) are returned.
 */
static const std::vector<Finger>& GetSensorOrder();
```

## Debugging Features

```cpp
/**
 * @brief Toggles the display of raw send/receive data details.
 * @param show Whether to show the data details.
 */
void ShowDataDetails(bool show, DataDetailsLogCallback log_callback = nullptr) const;
// When log_callback is set, each line is delivered to the callback; otherwise stdout (see omnihand/utils.h).
```

## Important Notes

1. **No Position/Velocity/Torque Control**: OmniHand Dex UMI (O10 UMI) is a **read-only** device. It does not support position, velocity, or torque control. It only provides position information via active query.

2. **Active Position Query**: UMI protocol supports actively querying joint positions using `GetJointMotorPosi()` or `GetAllJointMotorPosi()`. Position values range from 0-4096.

3. **Position Calibration**: Position calibration (min/max) is a write-only operation. The device should be in the appropriate position when calling calibration functions.

## Complete Example

```cpp
#include "omnihand/omnihand_dex_umi.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Create hand instance
    auto hand = OmniHandDexUMI::createHandByZlgcan(
        HandType::LEFT,
        1,      // hand_device_id
        0,      // canfd_device_id
        0       // canfd_channel_id
    );

    if (!hand || !hand->Init()) {
        std::cerr << "Failed to initialize OmniHand Dex UMI (O10 UMI)" << std::endl;
        return -1;
    }

    // Get vendor information
    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // Get device information
    auto device_info = hand->GetDeviceInfo();
    std::cout << device_info.toString() << std::endl;

    // Query joint positions (active query)
    auto positions = hand->GetAllJointMotorPosi();
    std::cout << "All joint positions (" << positions.size() << " values): ";
    for (size_t i = 0; i < positions.size(); ++i) {
        std::cout << positions[i];
        if (i < positions.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    // Get tactile sensor data
    auto tactile_data = hand->GetAllTactileSensorDataRaw();
    std::cout << "Tactile sensors: " << tactile_data.size() << " sensors" << std::endl;

    return 0;
}
```

## UMI Protocol Register Reference

- **Pn1**: Vendor information (read-only)
- **Pn2**: Device information (read-only)
- **Pn3**: Position information (read-only, active query)
  - **Pn3.00**: Read all joint positions (10 values)
  - **Pn3.01~Pn3.0A**: Read individual joint position (joint 1-10)
- **Pn6**: Tactile sensor data (read-only)
  - **Pn6.00**: Read all sensor data (6 sensors: Thumb, Index, Middle, Ring, Little, Palm, UMI has no Dorsum)
  - **Pn6.01~Pn6.06**: Read individual sensor data (sensor 1-6)
- **Pn7**: Maximum position calibration (write-only)
  - **Pn7.00**: Set all joints max position at once
  - **Pn7.01~Pn7.0A**: Set individual joint max position (joint 1-10)
- **Pn8**: Minimum position calibration (write-only)
  - **Pn8.00**: Set all joints min position at once
  - **Pn8.01~Pn8.0A**: Set individual joint min position (joint 1-10)

## Demo Files

The SDK release package includes ready-to-compile C++ demo source code:

| Demo | Path |
|------|------|
| CAN FD (connect by ID) | [UMI_demo_canfd_id.cc](../../../cpp/demo/omnihand_dex_umi/UMI_demo_canfd_id.cc) |
| CAN FD (connect by serial) | [UMI_demo_canfd_serial.cc](../../../cpp/demo/omnihand_dex_umi/UMI_demo_canfd_serial.cc) |
| SocketCAN | [UMI_demo_socketcan.cc](../../../cpp/demo/omnihand_dex_umi/UMI_demo_socketcan.cc) |

See [CMakeLists.txt](../../../cpp/demo/omnihand_dex_umi/CMakeLists.txt) for build instructions.

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
