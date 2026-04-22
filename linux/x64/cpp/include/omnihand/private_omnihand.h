// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file private_omnihand.h
 * @brief Private base class for internal implementations - StreamCmd protocol interface
 * @note This is an internal class for internal software use. It implements all StreamCmd protocol
 *       functions directly, without inheriting from OmniHandBase.
 */

#ifndef AGILINK_PRIVATE_OMNIHAND_H
#define AGILINK_PRIVATE_OMNIHAND_H

#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

/**
 * @brief Product serial number structure (0xC2 response)
 * @note 19 bytes total:
 *       - supplier_code[3]: 3 bytes supplier code
 *       - material_code[6]: 6 bytes material code
 *       - date[6]: YYMMDD format (6 bytes ASCII)
 *       - serial[4]: 4 bytes serial number
 */
struct AGIBOT_EXPORT ProductSerialNumber {
  uint8_t supplier_code[3];   // Supplier code (3 bytes)
  uint8_t material_code[6];   // Material code (6 bytes)
  uint8_t date[6];            // Date YYMMDD (6 bytes ASCII)
  uint8_t serial[4];          // Serial number (4 bytes)
  
  ProductSerialNumber() {
    memset(supplier_code, 0, 3);
    memset(material_code, 0, 6);
    memset(date, 0, 6);
    memset(serial, 0, 4);
  }
  
  /**
   * @brief Convert to string representation (ASCII format)
   * @note Device returns 19-byte ASCII string like "AXXX89..."
   */
  std::string ToString() const {
    std::ostringstream oss;
    for (int i = 0; i < 3; i++) {
      if (supplier_code[i] >= 0x20 && supplier_code[i] < 0x7F) {
        oss << static_cast<char>(supplier_code[i]);
      }
    }
    for (int i = 0; i < 6; i++) {
      if (material_code[i] >= 0x20 && material_code[i] < 0x7F) {
        oss << static_cast<char>(material_code[i]);
      }
    }
    for (int i = 0; i < 6; i++) {
      if (date[i] >= 0x20 && date[i] < 0x7F) {
        oss << static_cast<char>(date[i]);
      }
    }
    for (int i = 0; i < 4; i++) {
      if (serial[i] >= 0x20 && serial[i] < 0x7F) {
        oss << static_cast<char>(serial[i]);
      }
    }
    return oss.str();
  }
};

/**
 * @brief Firmware version information structure (0xCD response)
 * @note 10 bytes total:
 *       - device_type: Device type (2=O10灵巧手, 1=O12, etc.)
 *       - product_status[2]: Product status (ASCII, e.g., "T1", "T2", "P1")
 *       - software_version: Software version (major, minor, patch)
 *       - hardware_version: Hardware version (major, minor, patch)
 *       - dof: Degrees of freedom
 */
struct AGIBOT_EXPORT FirmwareVersionInfo {
  uint8_t device_type;              // 设备类型 (2=O10灵巧手, 1=O12)
  char product_status[3];           // 产品状态（ASCII码，如"T1"、"T2"、"P1"等），包含结束符
  Version software_version;         // 软件版本 (major, minor, patch)
  Version hardware_version;         // 硬件版本 (major, minor, patch)
  uint8_t dof;                      // 自由度
  
  FirmwareVersionInfo() : device_type(0), dof(0) {
    product_status[0] = '\0';
    product_status[1] = '\0';
    product_status[2] = '\0';
    memset(&software_version, 0, sizeof(Version));
    memset(&hardware_version, 0, sizeof(Version));
  }
  
  /**
   * @brief Convert to string representation
   */
  std::string ToString() const {
    std::ostringstream oss;
    oss << "[Device Type: " << static_cast<unsigned int>(device_type)
        << "][Product Status: " << GetProductStatus()
        << "][Software Version: " << software_version.ToString()
        << "][Hardware Version: " << hardware_version.ToString()
        << "][DOF: " << static_cast<unsigned int>(dof) << "]";
    return oss.str();
  }

  ProductType GetProductType() const {
    switch (device_type) {
      case 2: return ProductType::OMNIHAND_2025;
      // case 1: return ProductType::OMNIHAND_2025;
      // case 4: return ProductType::OMNIHAND_3_LITE;
      default: return ProductType::UNKNOWN;
    }
  }

  /**
   * @brief Get product status as string
   */
  std::string GetProductStatus() const {
    return std::string(product_status, 2);
  }
};

/**
 * @brief Response of SetAllAxisPos (0x08): positions, velocities, currents after set.
 * @note 60-byte protocol: [0:20] 10×position (uint16 LE), [20:40] 10×velocity (uint16 LE), [40:50] 10×current (1 byte/axis).
 *       O10: all three filled (10 elements each). O4: positions.size()=4, velocities/currents empty or 4 if device returns them.
 */
struct AGIBOT_EXPORT SetAllAxisPosResponse {
  std::vector<uint16_t> positions;   ///< Position per axis (O10: 10, O4: 4)
  std::vector<int16_t> velocities; ///< Velocity per axis (same length as positions when present)
  std::vector<int8_t> currents;   ///< Current per axis, 1 byte in protocol zero-extended to uint16 (same length when present)
  std::vector<int8_t> error_codes;  ///< Error code per axis, 1 byte per axis (same length when present)
  bool empty() const { return positions.empty(); }

  std::string ToString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < positions.size(); i++) {
      oss << "[Joint:" << i + 1 << "][Position:" << static_cast<unsigned int>(positions[i])
          << "][Velocity:" << static_cast<int>(velocities[i])
          << "][Current:" << static_cast<int>(currents[i])
          << "][Error Code:" << static_cast<int>(error_codes[i]) << "]\n";
    }
    return oss.str();
  } 
};

/**
 * @brief Position limits for all axes from GET_AXIS_LIMIT_POS (0x30).
 * @note Wire format: first block = N×uint16 LE min (0–4095), second block = N×uint16 LE max.
 *       O10: N=10 → 40 bytes total; O4: N=4 → 16 bytes.
 */
struct AGIBOT_EXPORT AxisLimitPos {
  std::vector<uint16_t> min_limits;
  std::vector<uint16_t> max_limits;

  bool empty() const {
    return min_limits.empty() || max_limits.empty() || min_limits.size() != max_limits.size();
  }

  /** Parse raw payload: [0, half) = mins, [half, byte_len) = maxs, each uint16 little-endian. */
  static AxisLimitPos DecodeLittleEndian(const uint8_t* data, size_t byte_len) {
    AxisLimitPos out;
    if (!data || byte_len < 4 || (byte_len % 4) != 0) {
      return out;
    }
    const size_t half = byte_len / 2;
    const size_t n_axes = half / 2;
    out.min_limits.resize(n_axes);
    out.max_limits.resize(n_axes);
    for (size_t i = 0; i < n_axes; ++i) {
      out.min_limits[i] = static_cast<uint16_t>(
          static_cast<uint16_t>(data[2 * i]) | (static_cast<uint16_t>(data[2 * i + 1]) << 8));
      out.max_limits[i] = static_cast<uint16_t>(
          static_cast<uint16_t>(data[half + 2 * i]) |
          (static_cast<uint16_t>(data[half + 2 * i + 1]) << 8));
    }
    return out;
  }
};

class AGIBOT_EXPORT PrivateOmniHand {
 public:
  virtual ~PrivateOmniHand() = default;

  // ============ StreamCmd Protocol Functions ============
  /**
   * @brief 0x01: Set motor power state (enable/disable/calibration mode)
   * @param state 0=disable, 1=enable, 2=calibration mode
   * @return true on success, false on failure
   */
  virtual bool SetPowerState(uint8_t state) = 0;

  /**
   * @brief 0x02: Get current motor power state
   * @return 0=disable, 1=enable, 2=calibration mode
   */
  virtual uint8_t GetPowerState() const = 0;

  /**
   * @brief 0x03: Set axis homing (zero reference) position
   * @param axis_index 0=all axes (pos ignored), 1-12=axis index
   * @param pos target middle/zero position, range [-4095, 4095]; ignored when axis_index==0
   * @return true on success, false on failure
   */
  virtual bool SetAxisHoming(uint8_t axis_index, int16_t pos) = 0;

  /**
   * @brief 0x04: Set device ID
   * @param id Device ID, range [1, 0x7FF)
   * @return true on success, false on failure
   */
  virtual bool SetId(uint16_t id) = 0;

  /**
   * @brief 0x05: Save persistent parameters
   * @return true on success, false on failure
   */
  virtual bool SaveParam() = 0;

  /**
   * @brief 0x06: Set single axis target position
   * @param axis_index Axis index (1-12)
   * @param position Target position (0-4096)
   * @return Current position (0-4096)
   */
  virtual uint16_t SetSingleAxisPos(uint8_t axis_index, uint16_t position) = 0;

  /**
   * @brief 0x07: Get single axis position
   * @param axis_index Axis index (1-12)
   * @return Current position (0-4096)
   */
  virtual uint16_t GetSingleAxisPos(uint8_t axis_index) const = 0;

  /**
   * @brief 0x08: Set all axes target positions
   * @param positions Target positions; length = axis count (O10: 10, O4: 4), each 0-4096
   * @return Positions, velocities, currents after set (for o10_hmi compatibility); empty on failure
   */
  virtual SetAllAxisPosResponse SetAllAxisPos(const std::vector<uint16_t>& positions) = 0;

  /**
   * @brief 0x09: Get all axes positions
   * @return Position list; length = axis count (O10: 10, O4: 4), each 0-4096
   */
  virtual std::vector<uint16_t> GetAllAxisPos() const = 0;

  /**
   * @brief 0x0A: Get all axes current information
   * @return Current per axis (0.01A units, int16, signed)
   */
  virtual std::vector<int16_t> GetAllAxisCurrent() const = 0;

  /**
   * @brief 0x0B: Get all axes velocity information (CAN: 0x0B stream; USB may use CVP)
   * @return Velocity per axis (int16, signed)
   */
  virtual std::vector<int16_t> GetAllAxisVelocity() const = 0;

  /**
   * @brief 0x0C: Get all axes temperature data
   * @return Temperature data (10 bytes, unit: degrees Celsius, int8 per axis)
   */
  virtual std::vector<int8_t> GetAllAxisTemp() const = 0;

  /**
   * @brief 0x0D: Get aggregated error code
   * @return Error code (uint16): 0x0000=no error, 1-10=motor stall, 21-30=over temp, etc.
   */
  virtual uint16_t GetErrorCode() const = 0;

  /**
   * @brief 0x0E: Clear device error state
   * @return true on success, false on failure
   */
  virtual bool ClearError() = 0;

  /**
   * @brief 0x0F: Play built-in action sequence
   * @param action_id Action ID
   * @return true on success, false on failure
   */
  virtual bool PlayAction(uint8_t action_id) = 0;

  /**
   * @brief 0x10: Get position range of all active axes
   * @return Position range data (20 bytes, unit: 0.1 degrees)
   */
  virtual std::vector<uint16_t> GetAllAxisPosRange() const = 0;

  /**
   * @brief 0x11: Get one fingertip sensor frame
   * @param sensor_index Sensor index
   * @return Sensor data (17 bytes for finger, 26 bytes for palm/back)
   */
  virtual std::vector<uint8_t> GetFingertipSensor(uint8_t sensor_index) const = 0;

  /**
   * @brief 0x12: Get fingertip sensor group A data
   * @return Sensor data (48 bytes, 4x4 points per sensor, uint8 per point)
   */
  virtual std::vector<uint8_t> GetAllFingertipSensorA() const = 0;

  /**
   * @brief 0x13: Get fingertip sensor group B data
   * @return Sensor data (32 bytes, 4x4 points per sensor, uint8 per point)
   */
  virtual std::vector<uint8_t> GetAllFingertipSensorB() const = 0;

  /**
   * @brief 0x14: Get fingertip sensor group C data
   * @return Sensor data (50 bytes, 5x5 points per sensor, uint8 per point)
   */
  virtual std::vector<uint8_t> GetAllFingertipSensorC() const = 0;

  /**
   * @brief 0x15: Set motor run mode
   * @param motor_id Motor ID
   * @param mode Run mode
   * @return true on success, false on failure
   */
  virtual bool SetRunMode(uint8_t motor_id, uint8_t mode) = 0;

  /**
   * @brief 0x16: Set single axis calibration position
   * @param axis_index Axis index (1-10)
   * @param position Target position (0-4096)
   * @return Current position (0-4096)
   */
  virtual uint16_t SetSingleActualAxisPos(uint8_t axis_index, uint16_t position) = 0;

  /**
   * @brief 0x17: Get single axis calibration position
   * @param axis_index Axis index (1-10)
   * @return Current position (0-4096)
   */
  virtual uint16_t GetSingleActualAxisPos(uint8_t axis_index) const = 0;

  /**
   * @brief 0x18: Set all axes calibration positions
   * @param positions Target positions; length = axis count (O10: 10, O4: 4), each 0-4096
   * @return Actual position list after set; length = axis count (same as input), empty on failure
   */
  virtual std::vector<uint16_t> SetAllActualAxisPos(const std::vector<uint16_t>& positions) = 0;

  /**
   * @brief 0x19: Get all axes calibration positions
   * @return Position list; length = axis count (O10: 10, O4: 4), each 0-4096
   */
  virtual std::vector<uint16_t> GetAllActualAxisPos() const = 0;

  /**
   * @brief 0x1A: Get all axes load data
   * @return Load data (20 bytes, voltage duty cycle, unit: 0.1%)
   */
  virtual std::vector<uint16_t> GetAllLoadData() const = 0;

  /**
   * @brief 0x1B: Set single axis minimum position limit
   * @param axis_index Axis index
   * @param min_pos Minimum position (0-4096)
   * @return true on success, false on failure
   */
  virtual bool SetAxisMinPos(uint8_t axis_index, uint16_t min_pos) = 0;

  /**
   * @brief 0x1C: Set single axis maximum position limit
   * @param axis_index Axis index
   * @param max_pos Maximum position (0-4096)
   * @return true on success, false on failure
   */
  virtual bool SetAxisMaxPos(uint8_t axis_index, uint16_t max_pos) = 0;

  /**
   * @brief 0x1D: Clear all axis position limits
   * @return true on success, false on failure
   */
  virtual bool ClearAllLimitPos() = 0;

  /**
   * @brief 0x20: Set run speed of all motors
   * @param speeds Vector of speeds (20 bytes, 10 axes * 2 bytes each, int16, range -4096~4096)
   * @return true on success, false on failure
   */
  virtual bool SetAllRunSpeed(const std::vector<int16_t>& speeds) = 0;

  /**
   * @brief 0x21: Set motor overload torque threshold
   * @param motor_id Motor ID
   * @param torque Overload torque (0-1000, unit: 0.1%, represents 0%-100.0% of max torque)
   * @return true on success, false on failure
   */
  virtual bool SetOverloadTorque(uint8_t motor_id, uint16_t torque) = 0;

  /**
   * @brief 0x22: Set motor overload protection time
   * @param motor_id Motor ID
   * @param time Overload time (unit: 0.01s)
   * @return true on success, false on failure
   */
  virtual bool SetOverloadProtectionTime(uint8_t motor_id, uint16_t time) = 0;

  /**
   * @brief 0x23: Set motor protected torque threshold
   * @param motor_id Motor ID
   * @param torque Protected torque (0-100, unit: 1%, represents 0%-100% of max torque)
   * @return true on success, false on failure
   */
  virtual bool SetProtectedTorque(uint8_t motor_id, uint8_t torque) = 0;

  /**
   * @brief 0x24: Set minimum starting torque
   * @param axis_index Axis index
   * @param min_force Minimum starting force (uint8, unit: 0.1%, percentage of stall torque)
   * @return true on success, false on failure
   */
  virtual bool SetMinTorque(uint8_t axis_index, uint8_t min_force) = 0;

  /**
   * @brief 0x25: Set axis protective current threshold
   * @param axis_index Axis index
   * @param current Protective current (uint16, unit: 10mA, max 3250mA)
   * @return true on success, false on failure
   */
  virtual bool SetProtectiveCurrent(uint8_t axis_index, uint16_t current) = 0;

  /**
   * @brief 0x26: Get all motor IDs
   * @return Motor ID data (10 bytes, int8 per axis)
   */
  virtual std::vector<int8_t> GetAllElectricMotorId() const = 0;

  /**
   * @brief 0x27: Get all tactile sensor IDs
   * @return Sensor ID data (7 bytes, int8 per sensor)
   */
  virtual std::vector<int8_t> GetAllSensorId() const = 0;

  /**
   * @brief 0x28: Set upload interval for all-axis CVP/PLOT data
   * @param interval_ms Interval in milliseconds (2 bytes)
   * @return true on success, false on failure
   */
  virtual bool SetAllAxisCvpUploadInterval(uint16_t interval_ms) = 0;

  /**
   * @brief 0x29: Get all-axis CVP/PLOT data
   * @return PLOT data (60 bytes: 10 axes * 6 bytes each, each axis: 2 bytes position, 2 bytes velocity, 2 bytes current)
   */
  virtual std::vector<uint8_t> GetAllAxisCvp() const = 0;

  /**
   * @brief 0x30: Get all axis position limits
   * @return Decoded limits; empty() on failure or timeout.
   */
  virtual AxisLimitPos GetAxisLimitPos() const = 0;

  /**
   * @brief 0x31: Set left-hand or right-hand configuration
   * @param hand_type 0=right hand (default), 1=left hand
   * @return true on success, false on failure
   */
  virtual bool SetRightOrLeft(uint8_t hand_type) = 0;

  /**
   * @brief 0x32: Set all axes position, speed, and torque targets
   * @param positions Vector of positions (20 bytes, 10 axes * 2 bytes each, range 0-4096)
   * @param speeds Vector of speeds (20 bytes, 10 axes * 2 bytes each, int16, range -4095~4096)
   * @param torques Vector of torques (10 bytes, 1 byte per axis, range 0-255)
   * @return Response data (60 bytes: positions, velocities, torques, fault states)
   */
  virtual SetAllAxisPosResponse SetPosSpeedTorqueData(const std::vector<uint16_t>& positions,
                                                      const std::vector<int16_t>& speeds,
                                                      const std::vector<uint8_t>& torques) = 0;

  /**
   * @brief 0x33: Get finger tactile force summary data
   * @return Tactile force data (35 bytes: 1 byte sensor online status + 4 bytes fingertip pressure per finger)
   *         Order: thumb, index, middle, ring, little, palm, back
   */
  virtual std::vector<uint8_t> GetFingerTactileForce() const = 0;

  /**
   * @brief 0x34: Set over-temperature threshold
   * @param threshold Temperature threshold
   * @return true on success, false on failure
   */
  virtual bool SetTemperatureThreshold(uint8_t threshold) = 0;

  /**
   * @brief 0x80: Set control source
   * @param source 0=robot body control (default), 1=HMI operator control
   * @return Original data (echo back)
   */
  virtual uint8_t SetControlSource(uint8_t source) = 0;

  /**
   * @brief 0x81: Get current control source
   * @return Control source: 0=robot body control, 1=HMI operator control
   */
  virtual uint8_t GetControlSource() const = 0;

  /**
   * @brief 0xC1: Set product serial number
   * @param serial_number Serial number (19 bytes: 3 bytes supplier code + 6 bytes material code + 6 bytes date YYMMDD + 4 bytes serial)
   * @return true on success, false on failure
   */
  virtual bool SetProductSerialNumber(const std::vector<uint8_t>& serial_number) = 0;

  /**
   * @brief 0xC2: Get product serial number
   * @return ProductSerialNumber structure containing supplier code, material code, date, and serial number
   */
  virtual ProductSerialNumber GetProductSerialNumber() const = 0;

  /**
   * @brief 0xCD: Get device model and firmware/hardware version info
   * @return FirmwareVersionInfo structure containing device type, product status, software/hardware version, and DOF
   */
  virtual FirmwareVersionInfo GetFwVersion() const = 0;

 protected:
  /**
   * @brief Constructor - protected to prevent direct instantiation
   * @note Users should use PrivateOmniHand2025 or PrivateOmniHand3Lite
   */
  PrivateOmniHand() = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_PRIVATE_OMNIHAND_H
