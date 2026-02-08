// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_dex_umi.h
 * @brief OmniHand Dex UMI interface class - 10 DOF, UMI Protocol
 * @note This is the public interface for OmniHand Dex UMI product
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand.h"
#include "omnihand/omnihand_sensor_base.h"
#include "omnihand/proto.h"

// Forward declarations
class OmniHandDexUmiCanImpl;

/**
 * @brief OmniHand Dex UMI interface class - 10 DOF, UMI Protocol
 * 
 * This class provides the public interface for OmniHand Dex UMI product.
 * UMI uses a different protocol (Pn1-Pn7) and supports periodic reports via callbacks.
 * Note: UMI does not support position/velocity/torque control (read-only position information).
 */
class AGIBOT_EXPORT OmniHandDexUMI : public virtual OmniHandSensorBase {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 10;  // O10 UMI has 10 active degrees of freedom (DoA)

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
      EHandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

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
      EHandType hand_type,
      unsigned char hand_device_id,
      const std::string& usbcanfd_serial_number,
      unsigned char canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return A unique pointer to OmniHandDexUMI instance
   */
  static std::unique_ptr<OmniHandDexUMI> createHandSocketCan(
      EHandType hand_type,
      unsigned char hand_device_id,
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
      EHandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

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
      EHandType hand_type,
      unsigned char hand_device_id,
      const std::string& hcan_serial_number,
      unsigned char canfd_channel_id = 0);

  // ============ Sensor Utilities (from OmniHandSensorBase) ============
  // GetSensorDataLength and GetSensorOrder are inherited from OmniHandSensorBase

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
  virtual void SetMaxPositionCalibration(unsigned char joint_index) = 0;

  // GetAllTactileSensorDataRaw and GetTactileSensorDataRaw are inherited from OmniHandSensorBase

 protected:
  /**
   * @brief Initialize base class members
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note Product type is fixed to ProductType::OMNIHAND_DEX_UMI for this class
   */
  void Reset(unsigned char device_id, EHandType hand_type) {
    OmniHand::Reset(ProductType::OMNIHAND_DEX_UMI, device_id, hand_type);
  }
};
