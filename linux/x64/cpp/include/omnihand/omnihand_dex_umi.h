// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_dex_umi.h
 * @brief OmniHand Dex UMI interface class - 10 DOF, UMI Protocol
 * @note This is the public interface for OmniHand Dex UMI product
 */

#ifndef AGILINK_OMNIHAND_DEX_UMI_H
#define AGILINK_OMNIHAND_DEX_UMI_H

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand.h"
#include "omnihand/i_o10_tactile_sensor_1d.h"
#include "omnihand/i_omnihand_motor_range.h"
#include "omnihand/ota_types.h"
#include "omnihand/proto.h"
#include "omnihand/kinematics/omnihand_2025/omnihand_2025_solver.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniHand Dex UMI interface class - 10 DOF, UMI Protocol
 * 
 * This class provides the public interface for OmniHand Dex UMI product.
 * UMI uses a different protocol (Pn1-Pn8) and supports active position query.
 * Note: UMI does not support position/velocity/torque control (read-only position information).
 */
class AGIBOT_EXPORT OmniHandDexUMI : public OmniHand, public IO10TactileSensor1D, public IOmniHandMotorRange {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 10;  // DoA
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniHandDexUMI() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandDexUMI instance
   */
  static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHandDexUMI> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return A unique pointer to OmniHandDexUMI instance
   */
  static std::unique_ptr<OmniHandDexUMI> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id HCAN device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandDexUMI instance
   */
  static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param hcan_serial_number HCAN device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);


  std::vector<std::string> GetJointNames() const override {
    return {
      "thumb_roll_joint",
      "thumb_abad_joint",
      "thumb_mcp_joint",
      "index_abad_joint",
      "index_pip_joint",
      "middle_pip_joint",
      "ring_abad_joint",
      "ring_pip_joint",
      "pinky_abad_joint",
      "pinky_pip_joint",
    };
  }

  // Motor range tables: provisional, same as O10 until UMI limits are confirmed.
  std::vector<std::pair<int16_t, int16_t>> GetAllMaxMinMotorPos() const override {
    static const std::vector<std::pair<int16_t, int16_t>> kAllMaxMinMotorPos = {
      {0, 4095},  // 1: thumb_roll_joint
      {0, 4095},  // 2: thumb_abad_joint
      {0, 4095},  // 3: thumb_mcp_joint
      {0, 4095},  // 4: index_abad_joint
      {0, 4095},  // 5: index_pip_joint
      {0, 4095},  // 6: middle_pip_joint
      {0, 4095},  // 7: ring_abad_joint
      {0, 4095},  // 8: ring_pip_joint
      {0, 4095},  // 9: pinky_abad_joint
      {0, 4095},  // 10: pinky_pip_joint
    };
    return kAllMaxMinMotorPos;
  }

  std::vector<std::pair<int16_t, int16_t>> GetAllMaxMinActualMotorPos() const override {
    static const std::vector<std::pair<int16_t, int16_t>> kAllMaxMinActualMotorPos = {
      {0, 4095},  // 1: thumb_roll_joint
      {0, 4095},  // 2: thumb_abad_joint
      {0, 4095},  // 3: thumb_mcp_joint
      {0, 1024},  // 4: index_abad_joint
      {0, 4095},  // 5: index_pip_joint
      {0, 4095},  // 6: middle_pip_joint
      {0, 1024},  // 7: ring_abad_joint
      {0, 4095},  // 8: ring_pip_joint
      {0, 1024},  // 9: pinky_abad_joint
      {0, 4095},  // 10: pinky_pip_joint
    };
    return kAllMaxMinActualMotorPos;
  }

  /**
   * @brief Get sensor data length for a specific finger
   * @param finger Finger enum value
   * @return Sensor data length in bytes
   */
   virtual size_t GetSensorDataLength(Finger finger) const override;
   /**
    * @brief Get sensor order vector
    * @return Reference to sensor order vector
    */
   virtual const std::vector<Finger>& GetSensorOrder() const override;

  // ============ Position Query (UMI Protocol Pn3=0x13) ============
  /**
   * @brief Get single joint motor position (UMI Protocol Pn3=0x13, sub-register 0x01-0x0A)
   * @param joint_motor_index Joint motor index (1-10)
   * @return Joint position value (0-4096), -1 if error
   */
  virtual int16_t GetJointMotorPosi(unsigned char joint_motor_index) const = 0;

  /**
   * @brief Get all joint motor positions (UMI Protocol Pn3=0x13, sub-register 0x00)
   * @return Vector of all joint positions (10 values, range 0-4096), empty if error
   */
  virtual std::vector<int16_t> GetAllJointMotorPosi() const = 0;

  // ============ UMI-Specific Interfaces ============
  /**
   * @brief Set minimum position calibration for all 10 joints (UMI Protocol Pn8=0x08, sub-register 0x00).
   * @note This is a write-only operation for position calibration.
   */
  virtual void SetMinPositionCalibration() = 0;

  /**
   * @brief Set minimum position calibration for a single joint (UMI Protocol Pn8=0x08, sub-register 0x01-0x0A).
   * @param joint_index Joint index (1-10, where 1 is the first joint)
   * @note This is a write-only operation for position calibration.
   */
  virtual void SetMinPositionCalibration(unsigned char joint_index) = 0;

  /**
   * @brief Set maximum position calibration for all 10 joints (UMI Protocol Pn7=0x07, sub-register 0x00).
   * @note This is a write-only operation for position calibration.
   */
  virtual void SetMaxPositionCalibration() = 0;

  /**
   * @brief Set maximum position calibration for a single joint (UMI Protocol Pn7=0x07, sub-register 0x01-0x0A).
   * @param joint_index Joint index (1-10, where 1 is the first joint)
   * @note This is a write-only operation for position calibration.
   */
  virtual   void SetMaxPositionCalibration(unsigned char joint_index) = 0;

  // GetAllTactileSensorDataRaw and GetTactileSensorDataRaw are inherited from IO10TactileSensor1D

 protected:
  /**
   * @brief Kinematics solver for OmniHand 2025 (O10)
   */
  std::unique_ptr<o10::OmniHand2025Solver> kinematics_solver_;

  /**
   * @brief Initialize base class members
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note Product type is fixed to ProductType::OMNIHAND_DEX_UMI for this class
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNIHAND_DEX_UMI, device_id, hand_type);
    kinematics_solver_ = std::make_unique<o10::OmniHand2025Solver>(is_left_hand_);
  }
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_DEX_UMI_H
