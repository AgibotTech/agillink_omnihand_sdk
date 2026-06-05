# OmniHand Pro 2025 (O12) C++ API

## Overview

**OmniHand Pro 2025 (O12)** is a 12 DOF dexterous hand with 3D tactile sensors. This document describes the C++ API for controlling and interacting with OmniHand Pro 2025 devices.

**Key Features:**
- 12 active degrees of freedom
- 3D tactile sensors (fingers + palm, not dorsum)
- Motor position range: 0-2000
- Supports CAN (ZLG USB CANFD) communication only
- Supports SocketCAN (Linux only)
- Supports voltage command control
- Supports temperature and current report period settings

## Include Header

```cpp
#include "omnihand/omnihand_pro_2025.h"
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

### Finger

```cpp
enum class Finger : unsigned char {
    THUMB = 0x01,    // Thumb
    INDEX = 0x02,    // Index finger
    MIDDLE = 0x03,   // Middle finger
    RING = 0x04,     // Ring finger
    LITTLE = 0x05,   // Little (pinky) finger
    PALM = 0x06,     // Palm (not supported by O12)
    DORSUM = 0x07,   // Dorsum (not supported by O12)
    UNKNOWN = 0xff   // Unknown
};
```

**Note**: O12 supports fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM), not dorsum.

### ControlMode

```cpp
enum class ControlMode : unsigned char {
    POSITION         = 0,    // Position mode
    SERVO            = 1,    // Servo mode
    VELOCITY         = 2,    // Velocity mode
    TORQUE           = 3,    // Torque mode
    VOLTAGE          = 4,    // Voltage mode
    PROFILE_POSITION = 7,    // Profile-position mode
    UNKNOWN          = 10    // Unknown mode
};
```

**Note**: 
- **Position mode (0)**: Position control, used with `SetJointMotorPosi`.
- **Servo mode (1)**: Servo control mode. This mode requires position command frequency ≥ 50Hz. The motor adjusts its speed based on the difference between target and actual position.
- **Velocity mode (2)**: Velocity control, used with `SetJointMotorVelo`.
- **Torque mode (3)**: Torque control, used with `SetJointMotorTorque`.
- **Voltage mode (4)**: Voltage command control, used with `SetJointMotorVoltage` / `SetAllJointMotorVoltage`.
- **Mixed control**: Position+force is a dedicated mixed-control command via `MixControlByPT`, not `ControlMode::VOLTAGE`.

## Data Structures

### VendorInfo

```cpp
struct VendorInfo {
    std::string productModel;     // Product model
    std::string productSeqNum;    // Product serial number
    Version hardwareVersion;      // Hardware version
    Version softwareVersion;      // Software version
    int16_t voltage;              // Supply voltage (mV)
    unsigned char dof;            // Degrees of Freedom (12 for O12)

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

### TactileSensor3DData

```cpp
struct TactileSensor3DData {
    unsigned char online_state;          // 1=online, 0=offline
    unsigned int channel_value[6];       // Six native channel values
    unsigned short normal_force;         // Normal force (0-3000, unit: 0.1N)
    unsigned short tangent_force;       // Tangent force
    unsigned short tangent_force_angle; // Tangent force angle (0-359 degrees, fingertip up = 0°, clockwise)
    unsigned char capa_approach[4];      // Capacitive approach values (4 channels)
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
 * @return A unique pointer to OmniHandPro2025 instance
 * @note Device type (200U/100U/MINI) is automatically detected internally.
 * @note ✅ Recommended: Zero configuration, ready to use out of the box. No root privileges required.
 */
static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
    HandType hand_type,
    unsigned char hand_device_id = 1,
    unsigned char canfd_device_id = 0,
    unsigned char canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHandPro2025::createHandByZlgcan(
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
 * @return A unique pointer to OmniHandPro2025 instance, or nullptr if device not found
 */
static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
    HandType hand_type,
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
 * @param canfd_channel_id CAN channel index (default: 0). For dual-channel adapters (USBCANFD-200U): can0=0, can1=1. For single-channel adapters (USBCANFD-100U): always 0
 * @return A unique pointer to OmniHandPro2025 instance
 */
static std::unique_ptr<OmniHandPro2025> createHandByHcan(
    HandType hand_type,
    unsigned char hand_device_id,
    unsigned char canfd_device_id,
    unsigned char canfd_channel_id = 0);
```

**Example:**
```cpp
auto hand = OmniHandPro2025::createHandByHcan(
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
 * @return A unique pointer to OmniHandPro2025 instance, or nullptr if device not found
 */
static std::unique_ptr<OmniHandPro2025> createHandByHcan(
    HandType hand_type,
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
 * @return A unique pointer to OmniHandPro2025 instance
 * @warning ⚠️ Advanced Usage: Requires driver setup and root privileges.
 */
static std::unique_ptr<OmniHandPro2025> createHandSocketCan(
    HandType hand_type,
    unsigned char hand_device_id,
    const std::string& can_interface = "can0");
#endif
```

## Basic Information

```cpp
/**
 * @brief Get product type
 * @return ProductType::OMNIHAND_PRO_2025
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

## Joint Angle Control

## Motor Position Control

**Note**: OmniHand Pro 2025 (O12) motor position range is **0-2000** (different from O10 which is 0-4096).

```cpp
/**
 * @brief Sets the position of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param posi The motor position (range: 0-2000).
 */
void SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi);

/**
 * @brief Gets the position of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current position value (range: 0-2000).
 */
int16_t GetJointMotorPosi(unsigned char joint_motor_index) const;

/**
 * @brief Sets the positions of all joint motors in batch and returns the actual positions.
 * @param vec_posi A vector of target positions. Must have 12 values, each in range 0-2000.
 * @return A vector of actual positions from device response. Empty vector on failure.
 */
std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi);

/**
 * @brief Gets the positions of all joint motors in batch.
 * @return A vector of the current positions. Returns 12 values, each in range 0-2000.
 */
std::vector<int16_t> GetAllJointMotorPosi() const;
```

## Joint Angle Control

### Joint Angle I/O Order (Right Hand)

| Index | Joint Name (URDF)       | Min Angle (rad)      | Max Angle (rad)     | Min Angle (°) | Max Angle (°) | Velocity Limit (rad/s) |
| ----- | ----------------------- | -------------------- | ------------------- | ------------ | ------------- | ---------------------- |
| 1     | `R_thumb_roll_joint`    | 0.0                  | 0.9424777960769379  | 0.0          | 54.0          | 2.38                   |
| 2     | `R_thumb_abad_joint`    | -1.387536755335492   | 0.0                 | -79.5        | 0.0           | 2.33                   |
| 3     | `R_thumb_mcp_joint`     | -0.8272860654453121  | 0.0                 | -47.4        | 0.0           | 1.35                   |
| 4     | `R_thumb_pip_joint`      | -1.2915436464758039  | 0.0                 | -74.0        | 0.0           | 1.87                   |
| 5     | `R_index_abad_joint`     | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 6     | `R_index_mcp_joint`      | 0.0                  | 1.3526301702956054  | 0.0          | 77.5          | 2.22                   |
| 7     | `R_index_pip_joint`      | 0.0                  | 1.530653753999027   | 0.0          | 87.7          | 2.49                   |
| 8     | `R_middle_abad_joint`    | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 9     | `R_middle_mcp_joint`     | 0.0                  | 1.3578661580515883  | 0.0          | 77.8          | 2.22                   |
| 10    | `R_middle_pip_joint`     | 0.0                  | 1.8151424220741028  | 0.0          | 104.0         | 2.16                   |
| 11    | `R_ring_mcp_joint`       | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |
| 12    | `R_pinky_mcp_joint`      | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |

### Joint Angle I/O Order (Left Hand)

| Index | Joint Name (URDF)       | Min Angle (rad)      | Max Angle (rad)     | Min Angle (°) | Max Angle (°) | Velocity Limit (rad/s) |
| ----- | ----------------------- | -------------------- | ------------------- | ------------ | ------------- | ---------------------- |
| 1     | `L_thumb_roll_joint`    | -0.9424777960769379  | 0.0                 | -54.0        | 0.0           | 2.38                   |
| 2     | `L_thumb_abad_joint`    | 0.0                  | 1.387536755335492   | 0.0          | 79.5          | 2.33                   |
| 3     | `L_thumb_mcp_joint`     | -0.8272860654453121  | 0.0                 | -47.4        | 0.0           | 1.35                   |
| 4     | `L_thumb_pip_joint`     | -1.2915436464758039  | 0.0                 | -74.0        | 0.0           | 1.87                   |
| 5     | `L_index_abad_joint`    | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 6     | `L_index_mcp_joint`      | 0.0                  | 1.3526301702956054  | 0.0          | 77.5          | 2.22                   |
| 7     | `L_index_pip_joint`      | 0.0                  | 1.530653753999027   | 0.0          | 87.7          | 2.49                   |
| 8     | `L_middle_abad_joint`   | -0.2617993877991494  | 0.2617993877991494  | -15.0        | 15.0          | 2.16                   |
| 9     | `L_middle_mcp_joint`     | 0.0                  | 1.3578661580515883  | 0.0          | 77.8          | 2.22                   |
| 10    | `L_middle_pip_joint`    | 0.0                  | 1.8151424220741028  | 0.0          | 104.0         | 2.16                   |
| 11    | `L_ring_mcp_joint`       | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |
| 12    | `L_pinky_mcp_joint`      | 0.0                  | 1.53588974175501    | 0.0          | 88.0          | 2.54                   |

**Note**: The left hand is generated via xacro mirroring, with the same joint limits as the right hand, only the joint name prefix changes from `R_` to `L_`.

```cpp
/**
 * @brief Sets the angles of all active joints.
 * @param angles A vector of joint angles (in radians). Must have 12 values.
 * @note The order follows the table above (indices 1-12).
 */
void SetAllActiveJointAngles(const std::vector<double>& angles);

/**
 * @brief Gets the angles of all active joints.
 * @return A vector of joint angles (in radians). Returns 12 values.
 * @note The order follows the table above (indices 1-12).
 */
std::vector<double> GetAllActiveJointAngles() const;

/**
 * @brief Gets the angles of all joints (both active and passive).
 * @return A vector of joint angles (in radians). Returns 19 values (12 active + 7 passive).
 * @note The first 12 values are active joints (order follows the table above), followed by 7 passive joints.
 */
std::vector<double> GetAllJointAngles() const;

/**
 * @brief Computes all joint angles (including passive) from active joint angles.
 * @param active_joint_angles A vector of active joint angles (in radians). Must have 12 values.
 * @return A vector of all joint angles (in radians), including both active and passive joints.
 * @note This function does not communicate with hardware; it only performs kinematics calculations.
 */
std::vector<double> GetAllJointAngles(const std::vector<double>& active_joint_angles) const;
```

## Velocity Control

```cpp
/**
 * @brief Sets the velocity of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param velo The target velocity value (recommended: 200-2500).
 */
void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo);

/**
 * @brief Gets the velocity of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current velocity value.
 */
int16_t GetJointMotorVelo(unsigned char joint_motor_index) const;

/**
 * @brief Sets the velocities of all joint motors in batch.
 * @param vec_velo A vector of target velocities. Must have 12 values (recommended: 200-2500).
 */
void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo);

/**
 * @brief Gets the velocities of all joint motors in batch.
 * @return A vector of the current velocities. Returns 12 values.
 */
std::vector<int16_t> GetAllJointMotorVelo() const;
```

## Voltage Control

Set the corresponding joint motors to `ControlMode::VOLTAGE` before sending voltage commands.

```cpp
/**
 * @brief Sets the voltage command of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param voltage The voltage command. The implementation clamps it to the supported range.
 */
void SetJointMotorVoltage(unsigned char joint_motor_index, int16_t voltage);

/**
 * @brief Sets voltage commands of all joint motors in batch.
 * @param vec_voltage A vector of voltage commands. Must have 12 values.
 */
void SetAllJointMotorVoltage(const std::vector<int16_t>& vec_voltage) const;

/**
 * @brief Gets the voltage command of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current voltage command.
 */
int16_t GetJointMotorVoltage(unsigned char joint_motor_index) const;

/**
 * @brief Gets voltage commands of all joint motors in batch.
 * @return A vector of voltage commands. Returns 12 values when the request succeeds.
 */
std::vector<int16_t> GetAllJointMotorVoltage() const;
```

## Tactile Sensor Data

OmniHand Pro 2025 (O12) uses **3D tactile sensors** with the following characteristics:
- **Sensor locations**: Fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM)
- **Not supported**: Dorsum sensor
- **Data structure**: TactileSensor3DData with normal force, tangent force, tangent force angle, etc.

```cpp
/**
 * @brief Gets 3D tactile sensor data for the specified finger (O12 only).
 * @param eFinger Finger/palm enum value (O12 supports fingers + palm, not dorsum)
 * @return TactileSensor3DData structure containing:
 *         - online_state: Sensor online status (1=online, 0=offline)
 *         - channel_value[6]: Six native channel values
 *         - normal_force: Normal force (0-3000, unit: 0.1N)
 *         - tangent_force: Tangent force
 *         - tangent_force_angle: Tangent force angle (0-359 degrees, fingertip up = 0°, clockwise)
 *         - capa_approach[4]: Capacitive approach values
 * @note O12 supports fingers + palm (THUMB, INDEX, MIDDLE, RING, LITTLE, PALM), not dorsum.
 */
TactileSensor3DData GetTactileSensor3DData(Finger eFinger) const;
```

## Control Mode

O12 supports switching motor control modes via `SetControlMode`. The commonly used command modes are:

| Mode Enum | Value | Description |
|---|---|---|
| `POSITION` | 0 | Position control (default) |
| `SERVO` | 1 | Servo control mode (requires position command frequency ≥ 50Hz) |
| `VELOCITY` | 2 | Velocity control mode |
| `TORQUE` | 3 | Torque control mode |
| `VOLTAGE` | 4 | Voltage command control |

**Note**: Position + force mixed control uses the dedicated `MixControlByPT` API. `MixControlByPV` / `MixControlByPVT` are not supported on O12.

```cpp
/**
 * @brief Sets the control mode of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param mode The control mode enum value.
 * @note Set `ControlMode::VOLTAGE` before using the voltage command APIs.
 */
void SetControlMode(unsigned char joint_motor_index, ControlMode mode);

/**
 * @brief Gets the control mode of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current control mode.
 */
ControlMode GetControlMode(unsigned char joint_motor_index) const;

/**
 * @brief Sets the control modes of all joint motors in batch.
 * @param ctrl_modes A vector of control modes. Must have 12 values.
 */
void SetAllControlMode(const std::vector<unsigned char>& ctrl_modes);

/**
 * @brief Gets the control modes of all joint motors in batch.
 * @return A vector of control modes. Returns 12 values.
 */
std::vector<unsigned char> GetAllControlMode() const;
```

## Current Threshold Control

```cpp
/**
 * @brief Sets the current threshold of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param current_threshold The current threshold value.
 */
void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold);

/**
 * @brief Gets the current threshold of a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current threshold value.
 */
int16_t GetCurrentThreshold(unsigned char joint_motor_index) const;

/**
 * @brief Sets the current thresholds of all joint motors in batch.
 * @param current_thresholds A vector of current thresholds. Must have 12 values.
 */
void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds);

/**
 * @brief Gets the current thresholds of all joint motors in batch.
 * @return A vector of current thresholds. Returns 12 values.
 */
std::vector<int16_t> GetAllCurrentThreshold() const;
```

## Mixed Control

**Note**: O12 supports **`MixControlByPT` (position + force) only**. `MixControlByPV` and `MixControlByPVT` are declared on the base class but return an empty vector and do not send CAN frames.

`tgt_torque_` is in **0.01 N** (correlated with tactile sensor normal force). Array index `i` maps to mix-control joint id `i` (0-based). Echo `joint_index_` is 1-based.

```cpp
std::vector<MixCtrl> MixControlByPT(const std::vector<int16_t>& positions,
                                    const std::vector<int16_t>& torques);  // supported
std::vector<MixCtrl> MixControlByPV(const std::vector<int16_t>& velocities,
                                    const std::vector<int16_t>& torques);   // not supported
std::vector<MixCtrl> MixControlByPVT(const std::vector<int16_t>& positions,
                                     const std::vector<int16_t>& velocities,
                                     const std::vector<int16_t>& torques);  // not supported
```

## Error Handling

```cpp
/**
 * @brief Gets the error report for a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The error report structure.
 */
JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const;

/**
 * @brief Gets the error reports for all joint motors.
 * @return A vector of error reports. Returns 12 values.
 */
std::vector<JointMotorErrorReport> GetAllErrorReport() const;

/**
 * @brief Sets the error report period for a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param period The reporting period in milliseconds.
 */
void SetErrorReportPeriod(unsigned char joint_motor_index, uint16_t period);

/**
 * @brief Sets the error report periods for all joint motors.
 * @param vec_period A vector of reporting periods. Must have 12 values.
 */
void SetAllErrorReportPeriod(std::vector<uint16_t> vec_period);
```

## Temperature Monitoring

```cpp
/**
 * @brief Gets the temperature report for a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current temperature value in degrees Celsius.
 */
int16_t GetTemperatureReport(unsigned char joint_motor_index) const;

/**
 * @brief Gets the temperature reports for all joint motors.
 * @return A vector of temperature values. Returns 12 values.
 */
std::vector<int16_t> GetAllTemperatureReport() const;

/**
 * @brief Sets the temperature report period for a single joint motor (O12 only).
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param period The reporting period in milliseconds.
 */
void SetTemperReportPeriod(unsigned char joint_motor_index, uint16_t period);

/**
 * @brief Sets the temperature report periods for all joint motors (O12 only).
 * @param vec_period A vector of reporting periods. Must have 12 values.
 */
void SetAllTemperReportPeriod(std::vector<uint16_t> vec_period);
```

## Current Monitoring

```cpp
/**
 * @brief Gets the current report for a single joint motor.
 * @param joint_motor_index The index of the joint motor (1-12).
 * @return The current value.
 */
int16_t GetCurrentReport(unsigned char joint_motor_index) const;

/**
 * @brief Gets the current reports for all joint motors.
 * @return A vector of current values. Returns 12 values.
 */
std::vector<int16_t> GetAllCurrentReport() const;

/**
 * @brief Sets the current report period for a single joint motor (O12 only).
 * @param joint_motor_index The index of the joint motor (1-12).
 * @param period The reporting period in milliseconds.
 */
void SetCurrentReportPeriod(unsigned char joint_motor_index, uint16_t period);

/**
 * @brief Sets the current report periods for all joint motors (O12 only).
 * @param vec_period A vector of reporting periods. Must have 12 values.
 */
void SetAllCurrentReportPeriod(std::vector<uint16_t> vec_period);
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

## Complete Example

```cpp
#include "omnihand/omnihand_pro_2025.h"
#include <iostream>

int main() {
    // Create hand instance
    auto hand = OmniHandPro2025::createHandByZlgcan(
        HandType::LEFT,
        1,      // hand_device_id
        0,      // canfd_device_id
        0       // canfd_channel_id
    );

    if (!hand || !hand->Init()) {
        std::cerr << "Failed to initialize OmniHand Pro 2025" << std::endl;
        return -1;
    }

    // Get vendor information
    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // Set joint angles
    std::vector<double> angles{0.3, -0.5, -0.3, -0.5, 0.0, 0.6, 0.7, 0.0, 0.6, 0.7, 0.7, 0.7};  // 12 joint angles (in radians)
    hand->SetAllActiveJointAngles(angles);
    std::cout << "Set joint angles: ";
    for (size_t i = 0; i < angles.size(); ++i) {
        std::cout << angles[i];
        if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << " (rad)" << std::endl;

    // Get 3D tactile sensor data
    auto thumb_data = hand->GetTactileSensor3DData(Finger::THUMB);
    std::cout << "Thumb normal force: " << thumb_data.normal_force << " (0.1N)" << std::endl;

    return 0;
}
```

## Demo Files

The SDK release package includes ready-to-compile C++ demo source code:

| Demo | Path |
|------|------|
| CAN FD (connect by ID) | [O12_demo_canfd_id.cc](../../../cpp/demo/omnihand_pro_2025/O12_demo_canfd_id.cc) |
| CAN FD (connect by serial) | [O12_demo_canfd_serial.cc](../../../cpp/demo/omnihand_pro_2025/O12_demo_canfd_serial.cc) |
| SocketCAN | [O12_demo_socketcan.cc](../../../cpp/demo/omnihand_pro_2025/O12_demo_socketcan.cc) |

See [CMakeLists.txt](../../../cpp/demo/omnihand_pro_2025/CMakeLists.txt) for build instructions.

## Related Documentation

- [OmniHand Pro 2025 (O12) Kinematics Solver C++ API](API_KINEMATICS_CPP_O12.md) - For kinematics calculations
- [SocketCAN Setup Guide](SOCKETCAN_SETUP.md) - For Linux SocketCAN configuration
