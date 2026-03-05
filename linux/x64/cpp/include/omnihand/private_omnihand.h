// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

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
  uint8_t supplier_code[3];   // 供应商暗码 (3 bytes)
  uint8_t material_code[6];   // 物料暗码 (6 bytes)
  uint8_t date[6];            // 日期 YYMMDD (6 bytes ASCII)
  uint8_t serial[4];          // 流水号 (4 bytes)
  
  ProductSerialNumber() {
    memset(supplier_code, 0, 3);
    memset(material_code, 0, 6);
    memset(date, 0, 6);
    memset(serial, 0, 4);
  }
  
  /**
   * @brief Convert to string representation
   */
  std::string ToString() const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 3; i++) {
      oss << std::setw(2) << static_cast<unsigned>(supplier_code[i]);
    }
    oss << "-";
    for (int i = 0; i < 6; i++) {
      oss << std::setw(2) << static_cast<unsigned>(material_code[i]);
    }
    oss << "-";
    for (int i = 0; i < 6; i++) {
      oss << static_cast<char>(date[i]);
    }
    oss << "-";
    for (int i = 0; i < 4; i++) {
      oss << std::setw(2) << static_cast<unsigned>(serial[i]);
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
   * @brief Get product status as string
   */
  std::string GetProductStatus() const {
    return std::string(product_status, 2);
  }
};

/**
 * @brief Private base class for internal implementations - StreamCmd protocol interface
 * @note This class is for internal software use. It provides all StreamCmd protocol functions
 *       directly, implementing the complete stream protocol command set.
 * @note Inherits from OmniHandBase to get all non-private interfaces (SetJointMotorPosi, etc.)
 *       and adds StreamCmd protocol functions for complete protocol implementation.
 * @note Tactile sensor interfaces (GetTactileSensorData, etc.) are declared here but should only
 *       be implemented by products that support them (e.g., O10). O4 does not support tactile sensors.
 */
class AGIBOT_EXPORT PrivateOmniHand {
 public:
  virtual ~PrivateOmniHand() = default;

  // ============ StreamCmd Protocol Functions ============
  // 0x01: 设备使能或失能
  /**
   * @brief Set power state (enable/disable/calibration mode)
   * @param state 0=disable, 1=enable, 2=calibration mode
   * @return true on success, false on failure
   */
  virtual bool SetPowerState(uint8_t state) = 0;

  // 0x02: 查询当前设备使能状态
  /**
   * @brief Get current power state
   * @return 0=disable, 1=enable, 2=calibration mode
   */
  virtual uint8_t GetPowerState() const = 0;

  // 0x03: 设置当前位置为轴中间位置
  /**
   * @brief Set current position as axis middle position (homing)
   * @param axis_index 0=all axes, 1-12=axis index
   * @return true on success, false on failure
   */
  virtual bool SetAxisHoming(uint8_t axis_index) = 0;

  // 0x04: 设置ID
  /**
   * @brief Set device ID
   * @param id Device ID, range [1, 0x7FF)
   * @return true on success, false on failure
   */
  virtual bool SetId(uint16_t id) = 0;

  // 0x05: 保存参数
  /**
   * @brief Save parameters (blocking operation)
   * @return true on success, false on failure
   */
  virtual bool SaveParam() = 0;

  // 0x06: 设置单个轴位置
  /**
   * @brief Set single axis position
   * @param axis_index Axis index (1-12)
   * @param position Target position (0-4096)
   * @return Current position (0-4096)
   */
  virtual uint16_t SetSingleAxisPos(uint8_t axis_index, uint16_t position) = 0;

  // 0x07: 读取单个轴位置
  /**
   * @brief Get single axis position
   * @param axis_index Axis index (1-12)
   * @return Current position (0-4096)
   */
  virtual uint16_t GetSingleAxisPos(uint8_t axis_index) const = 0;

  // 0x08: 设置全部轴位置
  /**
   * @brief Set all axes positions
   * @param positions Vector of target positions (20 bytes, 10 axes * 2 bytes each, range 0-4096)
   * @return Response data (60 bytes: positions, velocities, torques, fault states)
   */
  virtual std::vector<uint8_t> SetAllAxisPos(const std::vector<uint16_t>& positions) = 0;

  // 0x09: 读取全部轴位置数据
  /**
   * @brief Get all axes position data
   * @return Position data (20 bytes) + fault states (12 bytes)
   */
  virtual std::vector<uint8_t> GetAllAxisPos() const = 0;

  // 0x0A: 读取所有轴电流信息
  /**
   * @brief Get all axes current information
   * @return Current data (20 bytes, unit: 0.01A)
   */
  virtual std::vector<uint16_t> GetAllAxisCurrent() const = 0;

  // 0x0B: 读取所有轴速度信息
  /**
   * @brief Get all axes velocity information
   * @return Velocity data (20 bytes)
   */
  virtual std::vector<uint16_t> GetAllAxisVelocity() const = 0;

  // 0x0C: 读取全部轴温度数据
  /**
   * @brief Get all axes temperature data
   * @return Temperature data (10 bytes, unit: degrees Celsius, int8 per axis)
   */
  virtual std::vector<int8_t> GetAllAxisTemp() const = 0;

  // 0x0D: 读取错误码
  /**
   * @brief Get error code
   * @return Error code (uint16): 0x0000=no error, 1-10=motor stall, 21-30=over temp, etc.
   */
  virtual uint16_t GetErrorCode() const = 0;

  // 0x0E: 清除错误
  /**
   * @brief Clear error
   * @return true on success, false on failure
   */
  virtual bool ClearError() = 0;

  // 0x0F: 播放内置动作
  /**
   * @brief Play built-in action
   * @param action_id Action ID
   * @return true on success, false on failure
   */
  virtual bool PlayAction(uint8_t action_id) = 0;

  // 0x10: 读取10个轴的位置量程
  /**
   * @brief Get position range for 10 axes
   * @return Position range data (20 bytes, unit: 0.1 degrees)
   */
  virtual std::vector<uint16_t> GetAllAxisPosRange() const = 0;

  // 0x11: 读取单个指尖传感器的数据
  /**
   * @brief Get single fingertip sensor data
   * @param sensor_index Sensor index
   * @return Sensor data (17 bytes for finger, 26 bytes for palm/back)
   */
  virtual std::vector<uint8_t> GetFingertipSensor(uint8_t sensor_index) const = 0;

  // 0x12: 读取所有指尖传感器的数据1
  /**
   * @brief Get all fingertip sensor data 1 (thumb, index, middle finger)
   * @return Sensor data (48 bytes, 4x4 points per sensor, uint8 per point)
   */
  virtual std::vector<uint8_t> GetAllFingertipSensorA() const = 0;

  // 0x13: 读取所有指尖传感器的数据2
  /**
   * @brief Get all fingertip sensor data 2 (ring, little finger)
   * @return Sensor data (32 bytes, 4x4 points per sensor, uint8 per point)
   */
  virtual std::vector<uint8_t> GetAllFingertipSensorB() const = 0;

  // 0x14: 读取所有指尖传感器的数据3
  /**
   * @brief Get all fingertip sensor data 3 (palm, back of hand)
   * @return Sensor data (50 bytes, 5x5 points per sensor, uint8 per point)
   */
  virtual std::vector<uint8_t> GetAllFingertipSensorC() const = 0;

  /**
   * @brief Get all tactile sensor data by querying 0x12, 0x13, 0x14 and assembling into vector&lt;TactileSensorData&gt;
   * @return Vector of TactileSensorData (7 elements: thumb..dorsum), empty on failure
   */
  virtual std::vector<TactileSensorData> GetAllTactileSensorData() const = 0;

  // 0x15: 运行模式
  /**
   * @brief Set run mode
   * @param motor_id Motor ID
   * @param mode Run mode
   * @return true on success, false on failure
   */
  virtual bool SetRunMode(uint8_t motor_id, uint8_t mode) = 0;

  // 0x16: 设置单个轴位置（标定）
  /**
   * @brief Set single axis position (calibration)
   * @param axis_index Axis index (1-10)
   * @param position Target position (0-4096)
   * @return Current position (0-4096)
   */
  virtual uint16_t SetSingleActualAxisPos(uint8_t axis_index, uint16_t position) = 0;

  // 0x17: 读取单个轴位置（标定）
  /**
   * @brief Get single axis position (calibration)
   * @param axis_index Axis index (1-10)
   * @return Current position (0-4096)
   */
  virtual uint16_t GetSingleActualAxisPos(uint8_t axis_index) const = 0;

  // 0x18: 设置全部轴位置（标定）
  /**
   * @brief Set all axes positions (calibration), 0x18
   * @param positions Target positions (O10: 10 axes, O4: 4 axes), each 0-4096
   * @return Actual position list after set (same length), empty on failure
   */
  virtual std::vector<uint16_t> SetAllActualAxisPos(const std::vector<uint16_t>& positions) = 0;

  // 0x19: 读取全部轴位置数据（标定）
  /**
   * @brief Get all axes position data (calibration)
   * @return Position data (20 bytes, range 0-4096)
   */
  virtual std::vector<uint16_t> GetAllActualAxisPos() const = 0;

  // 0x1A: 读取全部轴负载数据
  /**
   * @brief Get all axes load data
   * @return Load data (20 bytes, voltage duty cycle, unit: 0.1%)
   */
  virtual std::vector<uint16_t> GetAllLoadData() const = 0;

  // 0x1B: 设置单个轴最小位置值
  /**
   * @brief Set single axis minimum position value
   * @param axis_index Axis index
   * @param min_pos Minimum position (0-4096)
   * @return true on success, false on failure
   */
  virtual bool SetAxisMinPos(uint8_t axis_index, uint16_t min_pos) = 0;

  // 0x1C: 设置单个轴最大位置值
  /**
   * @brief Set single axis maximum position value
   * @param axis_index Axis index
   * @param max_pos Maximum position (0-4096)
   * @return true on success, false on failure
   */
  virtual bool SetAxisMaxPos(uint8_t axis_index, uint16_t max_pos) = 0;

  // 0x1D: 清除所有轴限位位置值
  /**
   * @brief Clear all axes limit position values
   * @return true on success, false on failure
   */
  virtual bool ClearAllLimitPos() = 0;

  // 0x20: 设置电机运行速度
  /**
   * @brief Set motor run speed for all axes
   * @param speeds Vector of speeds (20 bytes, 10 axes * 2 bytes each, int16, range -4096~4096)
   * @return true on success, false on failure
   */
  virtual bool SetAllRunSpeed(const std::vector<int16_t>& speeds) = 0;

  // 0x21: 设置电机过载扭矩
  /**
   * @brief Set motor overload torque
   * @param motor_id Motor ID
   * @param torque Overload torque (0-1000, unit: 0.1%, represents 0%-100.0% of max torque)
   * @return true on success, false on failure
   */
  virtual bool SetOverloadTorque(uint8_t motor_id, uint16_t torque) = 0;

  // 0x22: 设置电机过载时间
  /**
   * @brief Set motor overload protection time
   * @param motor_id Motor ID
   * @param time Overload time (unit: 0.01s)
   * @return true on success, false on failure
   */
  virtual bool SetOverloadProtectionTime(uint8_t motor_id, uint16_t time) = 0;

  // 0x23: 设置保护扭矩
  /**
   * @brief Set protected torque
   * @param motor_id Motor ID
   * @param torque Protected torque (0-100, unit: 1%, represents 0%-100% of max torque)
   * @return true on success, false on failure
   */
  virtual bool SetProtectedTorque(uint8_t motor_id, uint8_t torque) = 0;

  // 0x24: 设置最小启动力
  /**
   * @brief Set minimum starting force
   * @param axis_index Axis index
   * @param min_force Minimum starting force (uint8, unit: 0.1%, percentage of stall torque)
   * @return true on success, false on failure
   */
  virtual bool SetMinTorque(uint8_t axis_index, uint8_t min_force) = 0;

  // 0x25: 设置保护电流
  /**
   * @brief Set protective current
   * @param axis_index Axis index
   * @param current Protective current (uint16, unit: 10mA, max 3250mA)
   * @return true on success, false on failure
   */
  virtual bool SetProtectiveCurrent(uint8_t axis_index, uint16_t current) = 0;

  // 0x26: 读取所有电机ID
  /**
   * @brief Get all motor IDs
   * @return Motor ID data (10 bytes, int8 per axis)
   */
  virtual std::vector<int8_t> GetAllElectricMotorId() const = 0;

  // 0x27: 读取所有传感器ID
  /**
   * @brief Get all sensor IDs
   * @return Sensor ID data (7 bytes, int8 per sensor)
   */
  virtual std::vector<int8_t> GetAllSensorId() const = 0;

  // 0x28: 设置PLOT数据上报时间间隔
  /**
   * @brief Set PLOT data upload interval
   * @param interval_ms Interval in milliseconds (2 bytes)
   * @return true on success, false on failure
   */
  virtual bool SetAllAxisCvpUploadInterval(uint16_t interval_ms) = 0;

  // 0x29: 获取所有轴的PLOT数据
  /**
   * @brief Get all axes PLOT data
   * @return PLOT data (60 bytes: 10 axes * 6 bytes each, each axis: 2 bytes position, 2 bytes velocity, 2 bytes current)
   */
  virtual std::vector<uint8_t> GetAllAxisCvp() const = 0;

  // 0x30: 读取所有轴限位值
  /**
   * @brief Get all axes limit values
   * @return Limit data (48 bytes: 20 bytes min limits, 20 bytes max limits)
   */
  virtual std::vector<uint8_t> GetAxisLimitPos() const = 0;

  // 0x31: 设置左手/右手
  /**
   * @brief Set left/right hand
   * @param hand_type 0=right hand (default), 1=left hand
   * @return true on success, false on failure
   */
  virtual bool SetRightOrLeft(uint8_t hand_type) = 0;

  // 0x32: 设置全部轴位置（基础款定制）
  /**
   * @brief Set all axes position with speed and torque (basic model custom)
   * @param positions Vector of positions (20 bytes, 10 axes * 2 bytes each, range 0-4096)
   * @param speeds Vector of speeds (20 bytes, 10 axes * 2 bytes each, int16, range -4095~4096)
   * @param torques Vector of torques (10 bytes, 1 byte per axis, range 0-255)
   * @return Response data (60 bytes: positions, velocities, torques, fault states)
   */
  virtual std::vector<uint8_t> SetPosSpeedCurData(const std::vector<uint16_t>& positions,
                                                   const std::vector<int16_t>& speeds,
                                                   const std::vector<uint8_t>& torques) = 0;

  // 0x33: 读取手指触觉力
  /**
   * @brief Get finger tactile force
   * @return Tactile force data (35 bytes: 1 byte sensor online status + 4 bytes fingertip pressure per finger)
   *         Order: thumb, index, middle, ring, little, palm, back
   */
  virtual std::vector<uint8_t> GetFingerTactileForce() const = 0;

  // 0x34: 设置温度阈值
  /**
   * @brief Set temperature threshold
   * @param threshold Temperature threshold
   * @return true on success, false on failure
   */
  virtual bool SetTemperatureThreshold(uint8_t threshold) = 0;

  // 0x80: 控制源
  /**
   * @brief Set control source
   * @param source 0=robot body control (default), 1=HMI operator control
   * @return Original data (echo back)
   */
  virtual uint8_t SetControlSource(uint8_t source) = 0;

  // 0x81: 控制源查询
  /**
   * @brief Get control source
   * @return Control source: 0=robot body control, 1=HMI operator control
   */
  virtual uint8_t GetControlSource() const = 0;

  // 0xC1: 设置产品系列号
  /**
   * @brief Set product serial number
   * @param serial_number Serial number (19 bytes: 3 bytes supplier code + 6 bytes material code + 6 bytes date YYMMDD + 4 bytes serial)
   * @return true on success, false on failure
   */
  virtual bool SetProductSerialNumber(const std::vector<uint8_t>& serial_number) = 0;

  // 0xC2: 读取产品系列号
  /**
   * @brief Get product serial number
   * @return ProductSerialNumber structure containing supplier code, material code, date, and serial number
   */
  virtual ProductSerialNumber GetProductSerialNumber() const = 0;

  // 0xCD: 查询设备型号以及软硬件版本
  /**
   * @brief Get device model and software/hardware version
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
