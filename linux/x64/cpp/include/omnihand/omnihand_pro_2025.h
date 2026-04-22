// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_pro_2025.h
 * @brief OmniHand Pro 2025 (O12) interface class - 12 DOF
 * @note This is the public interface for OmniHand Pro 2025 product
 */

#ifndef AGILINK_OMNIHAND_2025_PRO_H
#define AGILINK_OMNIHAND_2025_PRO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_base.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"
#include "omnihand/kinematics/omnihand_pro_2025/omnihand_pro_2025_solver.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniHand Pro 2025 (O12) interface class - 12 DOF
 * 
 * This class provides the public interface for OmniHand Pro 2025 product.
 * It includes all methods supported by O12, including 3D tactile sensors and report period settings.
 */
class AGIBOT_EXPORT OmniHandPro2025 : public OmniHandBase {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 12;   // DoA
  static constexpr unsigned char kDegreesOfPassiveFreedom = 7;   // DoP
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniHandPro2025() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandPro2025 instance
   */
  static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id = kDefaultHandDeviceId,
      uint8_t canfd_device_id = 0,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandPro2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHandPro2025> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (e.g. WiFi/Ethernet adapter as TCP server)
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param host TCP server IP or hostname (e.g. "192.168.0.178")
   * @param port TCP server port (e.g. 8000)
   * @param canfd_channel_id Logical channel (default 0)
   * @return A unique pointer to OmniHandPro2025 instance
   */
  static std::unique_ptr<OmniHandPro2025> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& host,
      uint16_t port,
      uint8_t canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return A unique pointer to OmniHandPro2025 instance
   */
  static std::unique_ptr<OmniHandPro2025> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id = kDefaultHandDeviceId,
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
   * @return A unique pointer to OmniHandPro2025 instance
   */
  static std::unique_ptr<OmniHandPro2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id = kDefaultHandDeviceId,
      uint8_t canfd_device_id = 0,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param hcan_serial_number HCAN device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHandPro2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHandPro2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  /**
   * @brief 天机 MARVIN 控制器 TJ SDK 末端 CAN/CANFD 透传（O12）
   */
  static std::unique_ptr<OmniHandPro2025> createHandByTJ(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& marvin_controller_ip);
#endif

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00)
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   * @note This function sends a broadcast request to discover devices on the CAN bus
   * @note Only works with CAN communication
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00) by serial number
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if device not found or request failed
   * @note This function sends a broadcast request to discover devices on the CAN bus
   * @note Only works with CAN communication
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address (device_id = 0x00) via SocketCAN
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   * @note This function sends a broadcast request to discover devices on the CAN bus
   * @note Only works with CAN communication (Linux only)
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface);
#endif

  // ============ O12-Specific Interfaces ============
  /**
   * @brief Gets 3D tactile sensor data for the specified finger (O12 only).
   * @param eFinger Finger enum value (O12 supports fingers only, not palm/dorsum)
   * @return 3D tactile sensor data structure
   */
  virtual TactileSensor3DData GetTactileSensor3DData(Finger eFinger) const = 0;

  /**
   * @brief Sets error report period of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @param period Report period (unit: milliseconds)
   */
  virtual void SetErrorReportPeriod(unsigned char joint_motor_index, uint16_t period) = 0;

  /**
   * @brief Sets error report periods of all joint motors in batch.
   * @param vec_period Report period vector, length 12
   */
  virtual void SetAllErrorReportPeriod(std::vector<uint16_t> vec_period) = 0;

  /**
   * @brief Sets temperature report period of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @param period Report period (unit: milliseconds)
   */
  virtual void SetTemperReportPeriod(unsigned char joint_motor_index, uint16_t period) = 0;

  /**
   * @brief Sets temperature report periods of all joint motors in batch.
   * @param vec_period Report period vector, length 12
   */
  virtual void SetAllTemperReportPeriod(std::vector<uint16_t> vec_period) = 0;

  /**
   * @brief Sets current report period of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @param period Report period (unit: milliseconds)
   */
  virtual void SetCurrentReportPeriod(unsigned char joint_motor_index, uint16_t period) = 0;

  /**
   * @brief Sets current report periods of all joint motors in batch.
   * @param vec_period Report period vector, length 12
   */
  virtual void SetAllCurrentReportPeriod(std::vector<uint16_t> vec_period) = 0;

  // ============ Joint Naming ============
  /**
   * @brief Returns the 12 active joint names of O12 in motor-index order.
   * @note Order MUST match the ActiveJointID enum used by the kinematics solver
   *       and SetAllJointMotorPosi / GetAllJointMotorPosi. Names are prefixed
   *       with "L_"/"R_" to match the URDF in
   *       `omnihand_sdk/assets/o12_hand_description`.
   */
  std::vector<std::string> GetJointNames() const override {
    const std::string p = is_left_hand_ ? "L_" : "R_";
    return {
        p + "thumb_roll_joint",
        p + "thumb_abad_joint",
        p + "thumb_mcp_joint",
        p + "thumb_pip_joint",
        p + "index_abad_joint",
        p + "index_mcp_joint",
        p + "index_pip_joint",
        p + "middle_abad_joint",
        p + "middle_mcp_joint",
        p + "middle_pip_joint",
        p + "ring_mcp_joint",
        p + "pinky_mcp_joint",
    };
  }

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number (ignored for O12, only one FIST type supported)
   */
  void SetHandGesture(int gesture_num = 1) override;

 protected:
  /**
   * @brief Initialize base class members and kinematics solver
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note This method automatically initializes the kinematics solver after calling base class Reset()
   * @note Product type is fixed to ProductType::OMNIHAND_PRO_2025 for this class
   */
  void Reset(unsigned char device_id, HandType hand_type);

  /**
   * @brief Kinematics solver for OmniHand Pro 2025 (O12)
   */
  std::unique_ptr<o12::OmniHandPro2025Solver> kinematics_solver_;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_2025_PRO_H
