// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file private_omnihand_3_lite.h
 * @brief Private interface class for OmniHand 3 Lite S (O4) - internal software use
 * @note This is the private interface for internal software. Public users should use OmniHand3Lite.
 */

#ifndef AGILINK_PRIVATE_OMNIHAND_3_LITE_H
#define AGILINK_PRIVATE_OMNIHAND_3_LITE_H

#include <cstdint>
#include <memory>
#include <string>
#include "omnihand/export_symbols.h"
#include "omnihand/private_omnihand.h"
#include "omnihand/omnihand_3_lite.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

/**
 * @brief Private interface class for OmniHand 3 Lite S (O4) - internal software use
 * 
 * This class provides the private interface for internal software use.
 * 
 * @note Public users should use OmniHand3Lite instead.
 * @note O4 does not have tactile sensors.
 * @note This class also inherits from OmniHand3Lite to allow conversion to public interface.
 */
class AGIBOT_EXPORT PrivateOmniHand3Lite : public OmniHand3Lite, public PrivateOmniHand {
 public:
  virtual ~PrivateOmniHand3Lite() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to PrivateOmniHand3Lite instance
   */
  static std::unique_ptr<PrivateOmniHand3Lite> createHandByZlgcan(
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
   * @return A unique pointer to PrivateOmniHand3Lite instance, or nullptr if device not found
   */
  static std::unique_ptr<PrivateOmniHand3Lite> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (WiFi/网口转 CANFD)
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   */
  static std::unique_ptr<PrivateOmniHand3Lite> createHandByZlgCanTcp(
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
   * @return A unique pointer to PrivateOmniHand3Lite instance
   */
  static std::unique_ptr<PrivateOmniHand3Lite> createHandSocketCan(
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
   * @return A unique pointer to PrivateOmniHand3Lite instance
   */
  static std::unique_ptr<PrivateOmniHand3Lite> createHandByHcan(
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
   * @return A unique pointer to PrivateOmniHand3Lite instance, or nullptr if device not found
   */
  static std::unique_ptr<PrivateOmniHand3Lite> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

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

#endif  // AGILINK_PRIVATE_OMNIHAND_3_LITE_H
