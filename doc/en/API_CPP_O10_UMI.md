# OmniHand Dex UMI (O10 UMI) C++ API

## Overview

**OmniHand Dex UMI (O10 UMI)** is a 10 DOF dexterous hand using the UMI protocol. This document describes the C++ API for controlling and interacting with OmniHand Dex UMI (O10 UMI) devices.

**Key Features:**
- 10 active degrees of freedom
- 1D tactile sensors (fingers, palm, dorsum)
- UMI protocol (Pn1-Pn7 registers)
- Periodic position and tactile sensor reports via callbacks
- Supports CAN (ZLG USB CANFD) communication only
- Supports SocketCAN (Linux only)
- **Read-only position information** (no position/velocity/torque control)

## Include Header

```cpp
#include "omnihand/omnihand_dex_umi.h"
```

## Enums

### EHandType

```cpp
enum class EHandType : unsigned char {
    eLeft    = 0,    // Left hand
    eRight   = 1,    // Right hand
    eUnknown = 10    // Unknown
};
```

### EFinger

```cpp
enum class EFinger : unsigned char {
    eThumb   = 0x01,    // Thumb
    eIndex   = 0x02,    // Index finger
    eMiddle  = 0x03,    // Middle finger
    eRing    = 0x04,    // Ring finger
    eLittle  = 0x05,    // Little (pinky) finger
    ePalm    = 0x06,    // Palm
    eDorsum  = 0x07,    // Dorsum (back of hand)
    eUnknown = 0xff     // Unknown
};
```

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
    
    // UMI-specific fields (optional, only populated for OmniHand Dex UMI)
    std::optional<uint16_t> position_report_frequency;        // Pn2.03: Position report frequency (Hz, default 100)
    std::optional<uint16_t> tactile_sensor_report_frequency; // Pn2.04: Tactile sensor report frequency (Hz, default 100)
    std::optional<unsigned char> adc_channel_count;          // Pn2.05: ADC channel count (read-only)
    std::optional<std::vector<unsigned char>> tactile_sensor_info; // Pn2.06: Tactile sensor information (read-only)
                                                                     // byte0: sensor count, byte1-N: point count per channel
    
    std::string toString() const;
};
```

### TactileSensorData

```cpp
struct TactileSensorData {
    EFinger sensor_id_;           // Sensor ID (finger/palm/dorsum)
    std::vector<uint8_t> data_;   // Sensor data (unit: 1g, max: 255g)
};
```

### Callback Types

```cpp
/**
 * @brief Callback function type for position periodic report
 * @param positions Position data vector (voltage values in mV)
 */
using PositionReportCallback = std::function<void(const std::vector<int16_t>& positions)>;

/**
 * @brief Callback function type for tactile sensor periodic report
 * @param sensor_data Tactile sensor data
 * @param sensor_id Sensor ID (sub-register address, 0x01~0x07)
 */
using TactileSensorReportCallback = std::function<void(const TactileSensorData& sensor_data, unsigned char sensor_id)>;
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
    EHandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHandDexUMI::createHandByZlgcan(
    EHandType::eLeft,
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
 * @param canfd_channel_id CAN channel index (default: 0)
 * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
 */
static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
    EHandType hand_type,
    unsigned char hand_device_id,
    const std::string& usbcanfd_serial_number,
    unsigned char canfd_channel_id = 0);
```

### HCAN USB CANFD

```cpp
/**
 * @brief Factory method - HCAN USB CANFD communication by device ID
 * @param hand_type Hand type (left/right)
 * @param hand_device_id Hand device ID
 * @param canfd_device_id HCAN device index
 * @param canfd_channel_id CAN channel index (default: 0)
 * @return A unique pointer to OmniHandDexUMI instance
 */
static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
    EHandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHandDexUMI::createHandByHcan(
    EHandType::eLeft,
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
 * @param canfd_channel_id CAN channel index (default: 0)
 * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
 */
static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
    EHandType hand_type,
    unsigned char hand_device_id,
    const std::string& hcan_serial_number,
    unsigned char canfd_channel_id = 0);
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
    EHandType hand_type,
    unsigned char hand_device_id,
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
 * @return DeviceInfo structure containing device ID, communication parameters, and UMI-specific fields:
 *         - position_report_frequency (Pn2.03): Position report frequency in Hz (default: 100)
 *         - tactile_sensor_report_frequency (Pn2.04): Tactile sensor report frequency in Hz (default: 100)
 *         - adc_channel_count (Pn2.05): ADC channel count (read-only)
 *         - tactile_sensor_info (Pn2.06): Tactile sensor information (read-only)
 */
DeviceInfo GetDeviceInfo() const;

/**
 * @brief Sets the device ID.
 * @param hand_device_id The device ID.
 */
void SetDeviceId(unsigned char hand_device_id);
```

## Position Calibration

**Note**: UMI protocol supports position calibration via Pn7 register. This is a write-only operation.

```cpp
/**
 * @brief Set minimum position calibration (UMI Protocol Pn7, sub-register 0x00).
 * @note This is a write-only operation for position calibration.
 *       The device should be in minimum position when calling this function.
 */
void SetMinPositionCalibration();

/**
 * @brief Set maximum position calibration (UMI Protocol Pn7, sub-register 0x01).
 * @note This is a write-only operation for position calibration.
 *       The device should be in maximum position when calling this function.
 */
void SetMaxPositionCalibration();
```

## Periodic Report Frequency Settings

```cpp
/**
 * @brief Set position report frequency (UMI Protocol Pn2.03)
 * @param frequency Report frequency in Hz (default: 100)
 * @note Setting frequency to 0 will disable periodic reports
 */
void SetPositionReportFrequency(uint16_t frequency);

/**
 * @brief Set tactile sensor report frequency (UMI Protocol Pn2.04)
 * @param frequency Report frequency in Hz (default: 100)
 * @note Setting frequency to 0 will disable periodic reports
 */
void SetTactileSensorReportFrequency(uint16_t frequency);
```

## Periodic Report Callbacks

**Note**: UMI protocol supports periodic reports via callbacks. The callbacks are executed in the `RecvFrame` thread, so they should be thread-safe.

```cpp
/**
 * @brief Register callback function for position periodic report (UMI Protocol Pn3, Pn2.03 sets frequency)
 * @param callback Callback function to be called when position data is received
 * @param frequency Optional frequency in Hz (if provided, sets Pn2.03 before registering callback, default: 100)
 * @note The callback will be called in the RecvFrame thread, so it should be thread-safe
 * @note If callback is nullptr, the callback will be unregistered
 * @note Position data is in voltage values (mV)
 */
void SetPositionReportCallback(PositionReportCallback callback, std::optional<uint16_t> frequency = std::nullopt);

/**
 * @brief Register callback function for tactile sensor periodic report (UMI Protocol Pn6, Pn2.04 sets frequency)
 * @param callback Callback function to be called when tactile sensor data is received
 * @param frequency Optional frequency in Hz (if provided, sets Pn2.04 before registering callback, default: 100)
 * @note The callback will be called in the RecvFrame thread, so it should be thread-safe
 * @note If callback is nullptr, the callback will be unregistered
 * @note sensor_id in callback is the sub-register address (0x01~0x07)
 */
void SetTactileSensorReportCallback(TactileSensorReportCallback callback, std::optional<uint16_t> frequency = std::nullopt);
```

## Tactile Sensor Data

OmniHand Dex UMI (O10 UMI) uses **1D tactile sensors** similar to OmniHand 2025 (O10):
- **Data unit**: 1g
- **Max value**: 255g
- **Sensor locations**: Fingers, Palm, Dorsum
- **Protocol**: UMI Protocol Pn6 (read-only, sub-register 0x01~0x07)

```cpp
/**
 * @brief Gets all 1D tactile sensor raw data from all sensors at once.
 * @return Vector of TactileSensorData structures
 * @note This returns full resolution data.
 * @note Uses UMI Protocol Pn6.
 */
std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const;

/**
 * @brief Gets 1D tactile sensor raw data for a single sensor.
 * @param eFinger Finger/palm enum value
 * @return TactileSensorData structure containing full resolution data
 * @note Uses UMI Protocol Pn6.
 */
TactileSensorData GetTactileSensorDataRaw(EFinger eFinger) const;

/**
 * @brief Get sensor data length for a specific finger (static method)
 * @param eFinger Finger enum value
 * @return Sensor data length in bytes
 */
static size_t GetSensorDataLength(EFinger eFinger);

/**
 * @brief Get sensor order vector (static method)
 * @return Reference to sensor order vector
 */
static const std::vector<EFinger>& GetSensorOrder();
```

## Sensor Utilities

```cpp
/**
 * @brief Get sensor data length for a specific finger (static method)
 * @param eFinger Finger enum value
 * @return Sensor data length in bytes
 */
static size_t GetSensorDataLength(EFinger eFinger);

/**
 * @brief Get sensor order vector (static method)
 * @return Reference to sensor order vector
 */
static const std::vector<EFinger>& GetSensorOrder();
```

## Debugging Features

```cpp
/**
 * @brief Toggles the display of raw send/receive data details.
 * @param show Whether to show the data details.
 */
void ShowDataDetails(bool show) const;
```

## Important Notes

1. **No Position/Velocity/Torque Control**: OmniHand Dex UMI (O10 UMI) is a **read-only** device. It does not support position, velocity, or torque control. It only provides position information via periodic reports.

2. **Periodic Reports**: UMI protocol supports periodic reports for position and tactile sensor data. Use callbacks to receive this data asynchronously.

3. **Thread Safety**: Callback functions are executed in the `RecvFrame` thread. Ensure your callbacks are thread-safe.

4. **Position Calibration**: Position calibration (min/max) is a write-only operation. The device should be in the appropriate position when calling calibration functions.

## Complete Example

```cpp
#include "omnihand/omnihand_dex_umi.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Create hand instance
    auto hand = OmniHandDexUMI::createHandByZlgcan(
        EHandType::eLeft,
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

    // Get device information (includes UMI-specific fields)
    auto device_info = hand->GetDeviceInfo();
    std::cout << device_info.toString() << std::endl;

    // Register position report callback
    hand->SetPositionReportCallback(
        [](const std::vector<int16_t>& positions) {
            std::cout << "Position report: " << positions.size() << " values" << std::endl;
        },
        100  // 100 Hz frequency
    );

    // Register tactile sensor report callback
    hand->SetTactileSensorReportCallback(
        [](const TactileSensorData& sensor_data, unsigned char sensor_id) {
            std::cout << "Tactile sensor report: sensor_id=" << static_cast<int>(sensor_id)
                      << ", data_size=" << sensor_data.data_.size() << std::endl;
        },
        100  // 100 Hz frequency
    );

    // Keep running to receive periodic reports
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Unregister callbacks
    hand->SetPositionReportCallback(nullptr);
    hand->SetTactileSensorReportCallback(nullptr);

    return 0;
}
```

## UMI Protocol Register Reference

- **Pn1**: Vendor information (read-only)
- **Pn2**: Device information (read-only)
  - **Pn2.03**: Position report frequency (read-write, 2 bytes, Hz, default 100)
  - **Pn2.04**: Tactile sensor report frequency (read-write, 2 bytes, Hz, default 100)
  - **Pn2.05**: ADC channel count (read-only, 1 byte)
  - **Pn2.06**: Tactile sensor information (read-only, variable length)
- **Pn3**: Position information (read-only, periodic report)
- **Pn6**: Tactile sensor data (read-only, periodic report, sub-register 0x01~0x07)
- **Pn7**: Position calibration (write-only)
  - **Pn7.00**: Minimum position calibration
  - **Pn7.01**: Maximum position calibration

## Related Documentation

- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
