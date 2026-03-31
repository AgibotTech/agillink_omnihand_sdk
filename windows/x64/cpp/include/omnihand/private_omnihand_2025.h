// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file private_omnihand_2025.h
 * @brief Private interface class for OmniHand 2025 (O10) - internal software use
 * @note This is the private interface for internal software. Public users should use OmniHand2025.
 */

#ifndef AGILINK_PRIVATE_OMNIHAND_2025_H
#define AGILINK_PRIVATE_OMNIHAND_2025_H

#include <cstdint>
#include <memory>
#include <string>
#include "omnihand/export_symbols.h"
#include "omnihand/private_omnihand.h"
#include "omnihand/omnihand_2025.h"
#include "omnihand/io10_tactile_sensor_1d.h"
#include "omnihand/proto.h"
#include "omnihand/kinematics/omnihand_2025/omnihand_2025_solver.h"

namespace agilink {
namespace omnihand {

/**
 * @brief Private interface class for OmniHand 2025 (O10) - internal software use
 * 
 * This class provides the private interface for internal software use.
 * It includes factory methods for all communication types (CAN, USB, RS485).
 * 
 * @note Public users should use OmniHand2025 instead.
 * @note O10 supports tactile sensors, so this class inherits from IO10TactileSensor1D.
 * @note This class also inherits from OmniHand2025 to allow conversion to public interface.
 */
class AGIBOT_EXPORT PrivateOmniHand2025 : public OmniHand2025, public PrivateOmniHand {
 public:
  virtual ~PrivateOmniHand2025() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to PrivateOmniHand2025 instance
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByZlgcan(
      HandType hand_type,
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
   * @return A unique pointer to PrivateOmniHand2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByZlgcan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& usbcanfd_serial_number,
      unsigned char canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (e.g. WiFi/Ethernet adapter as TCP server)
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param host TCP server IP or hostname (e.g. "192.168.0.178")
   * @param port TCP server port (e.g. 8000)
   * @param canfd_channel_id Logical channel (default 0)
   * @return A unique pointer to PrivateOmniHand2025 instance
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByZlgCanTcp(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& host,
      uint16_t port,
      unsigned char canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

  /**
   * @brief Factory method - RS485 communication (OmniHand 2025 only)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (e.g., "/dev/ttyUSB0")
   * @param baudrate Baud rate (default 460800)
   * @return A unique pointer to PrivateOmniHand2025 instance
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByRs485(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

  /**
   * @brief Factory method - USB communication (OmniHand 2025 only)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (e.g., "/dev/ttyUSB0")
   * @param baudrate Baud rate (default 460800)
   * @return A unique pointer to PrivateOmniHand2025 instance
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByUsb(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return A unique pointer to PrivateOmniHand2025 instance
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandSocketCan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id HCAN device index
   * @param canfd_channel_id CAN channel index (default 0)
   * @return A unique pointer to PrivateOmniHand2025 instance
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByHcan(
      HandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param hcan_serial_number HCAN device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   * @return A unique pointer to PrivateOmniHand2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<PrivateOmniHand2025> createHandByHcan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& hcan_serial_number,
      unsigned char canfd_channel_id = 0);

 protected:
  /**
   * @brief Initialize base class members and kinematics solver
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note This method automatically initializes the kinematics solver after calling base class Reset()
   * @note Product type is fixed to ProductType::OMNIHAND_2025 for this class
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNIHAND_2025, device_id, hand_type);
    // Automatically initialize kinematics solver
    kinematics_solver_ = std::make_unique<o10::OmniHand2025Solver>(is_left_hand_);
  }

  /**
   * @brief Kinematics solver for OmniHand 2025 (O10)
   */
  std::unique_ptr<o10::OmniHand2025Solver> kinematics_solver_;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_PRIVATE_OMNIHAND_2025_H
