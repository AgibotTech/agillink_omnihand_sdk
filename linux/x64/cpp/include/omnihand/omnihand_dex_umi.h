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

/**
 * @brief OmniHand Dex UMI interface class - 10 DOF, UMI Protocol
 * 
 * This class provides the public interface for OmniHand Dex UMI product.
 * UMI uses a different protocol (Pn1-Pn7) and supports periodic reports via callbacks.
 * Note: UMI does not support position/velocity/torque control (read-only position information).
 */
class AGIBOT_EXPORT OmniHandDexUMI : public virtual OmniHandSensorBase {
 public:
  virtual ~OmniHandDexUMI() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
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
   * @return A unique pointer to OmniHandDexUMI instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHandDexUMI> createHandByHcan(
      EHandType hand_type,
      unsigned char hand_device_id,
      const std::string& hcan_serial_number,
      unsigned char canfd_channel_id = 0);

  // ============ Sensor Utilities (from OmniHandSensorBase) ============
  // GetSensorDataLength and GetSensorOrder are inherited from OmniHandSensorBase

  // ============ UMI-Specific Interfaces ============
  /**
   * @brief Set minimum position calibration (UMI Protocol Pn7, sub-register 0x00).
   * @note This is a write-only operation for position calibration.
   */
  virtual void SetMinPositionCalibration() = 0;

  /**
   * @brief Set maximum position calibration (UMI Protocol Pn7, sub-register 0x01).
   * @note This is a write-only operation for position calibration.
   */
  virtual void SetMaxPositionCalibration() = 0;

  /**
   * @brief Set position report frequency (UMI Protocol Pn2.03)
   * @param frequency Report frequency in Hz (default: 100)
   * @note Setting frequency to 0 will disable periodic reports
   */
  virtual void SetPositionReportFrequency(uint16_t frequency) = 0;

  /**
   * @brief Set tactile sensor report frequency (UMI Protocol Pn2.04)
   * @param frequency Report frequency in Hz (default: 100)
   * @note Setting frequency to 0 will disable periodic reports
   */
  virtual void SetTactileSensorReportFrequency(uint16_t frequency) = 0;

  /**
   * @brief Register callback function for position periodic report (UMI Protocol Pn3, Pn2.03 sets frequency)
   * @param callback Callback function to be called when position data is received
   * @param frequency Optional frequency in Hz (if provided, sets Pn2.03 before registering callback, default: 100)
   * @note The callback will be called in the RecvFrame thread, so it should be thread-safe
   * @note If callback is nullptr, the callback will be unregistered
   */
  virtual void SetPositionReportCallback(PositionReportCallback callback, std::optional<uint16_t> frequency = std::nullopt) = 0;

  /**
   * @brief Register callback function for tactile sensor periodic report (UMI Protocol Pn6, Pn2.04 sets frequency)
   * @param callback Callback function to be called when tactile sensor data is received
   * @param frequency Optional frequency in Hz (if provided, sets Pn2.04 before registering callback, default: 100)
   * @note The callback will be called in the RecvFrame thread, so it should be thread-safe
   * @note If callback is nullptr, the callback will be unregistered
   */
  virtual void SetTactileSensorReportCallback(TactileSensorReportCallback callback, std::optional<uint16_t> frequency = std::nullopt) = 0;

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
