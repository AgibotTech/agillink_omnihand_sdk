// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_3_lite.h
 * @brief OmniHand 3 Lite S (O4) interface class - 4 DOF
 * @note This is the public interface for OmniHand 3 Lite S product
 */

#ifndef AGILINK_OMNIHAND_3_LITE_H
#define AGILINK_OMNIHAND_3_LITE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_base.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniHand 3 Lite S (O4) interface class - 4 DOF
 * 
 * This class provides the public interface for OmniHand 3 Lite S product.
 * 
 * @note O4 does not have tactile sensors. Tactile sensor methods return empty data.
 * @note Currently does not support angle-based control (SetAllActiveJointAngles) as kinematics solver is not available.
 *       Use motor position control (SetJointMotorPosi, SetAllJointMotorPosi) instead.
 */
class AGIBOT_EXPORT OmniHand3Lite : public OmniHandBase {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 4;  // DoA
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniHand3Lite() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHand3Lite instance
   */
  static std::unique_ptr<OmniHand3Lite> createHandByZlgcan(
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
   * @return A unique pointer to OmniHand3Lite instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHand3Lite> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (WiFi/Ethernet to CANFD, this machine acts as TCP client)
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param tcp_host TCP server IP or hostname (e.g. "192.168.0.178")
   * @param tcp_port TCP server port (e.g. 8000)
   * @param canfd_channel_id CAN channel index (0 or 1, default 0)
   * @return A unique pointer to OmniHand3Lite instance
   */
  static std::unique_ptr<OmniHand3Lite> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& tcp_host,
      uint16_t tcp_port,
      uint8_t canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return A unique pointer to OmniHand3Lite instance
   */
  static std::unique_ptr<OmniHand3Lite> createHandSocketCan(
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
   * @return A unique pointer to OmniHand3Lite instance
   */
  static std::unique_ptr<OmniHand3Lite> createHandByHcan(
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
   * @return A unique pointer to OmniHand3Lite instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHand3Lite> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  /**
   * @brief 天机 MARVIN 控制器 TJ SDK 末端 CAN/CANFD 透传（O4，与 O10 语义一致）
   */
  static std::unique_ptr<OmniHand3Lite> createHandByTJ(
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
   * @note Only works with CAN communication
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif

  // ============ Sensor Utilities ============
  // Note: O4 (OmniHand3Lite) does not support tactile sensors

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number (not supported without kinematics solver)
   * @note This function is not supported for O4 as kinematics solver is not available.
   *       Use SetAllJointMotorPosi() to set motor positions directly instead.
   */
  void SetHandGesture(int gesture_num = 1) override;

 protected:
  /**
   * @brief Initialize base class members
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note This method initializes the base class without kinematics solver
   * @note Product type is fixed to ProductType::OMNIHAND_3_LITE for this class
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNIHAND_3_LITE, device_id, hand_type);
    // Note: No kinematics solver for O4
  }
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_LITE_H
