// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_2025_mock.h
 * @brief Mock implementation of OmniHand2025 for testing and debugging (O10 only)
 * 
 * This is a standalone mock class that provides the same factory methods as OmniHand2025,
 * but returns mock implementations that do not require hardware.
 * 
 * Usage:
 *   Replace `OmniHand2025::createHandByZlgcan(...)` with `OmniHand2025Mock::createHandByZlgcan(...)`
 *   when BUILD_MOCK_SDK is defined.
 */

#ifndef AGILINK_OMNIHAND_2025_MOCK_H
#define AGILINK_OMNIHAND_2025_MOCK_H

#include <cstdint>
#include <memory>
#include <string>
#include "omnihand/omnihand_2025.h"

namespace agilink {
namespace omnihand {

// Forward declaration
class OmniHand2025MockImpl;

/**
 * @brief Mock factory class for OmniHand2025 - provides all factory methods matching OmniHand2025
 * 
 * This class provides the same factory methods as OmniHand2025, but returns mock implementations
 * that do not require hardware. All method signatures match exactly with OmniHand2025.
 */
class AGIBOT_EXPORT OmniHand2025Mock {
 public:
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index (ignored in mock)
   * @param canfd_channel_id CAN channel index (ignored in mock)
   * @return A unique pointer to OmniHand2025 mock instance
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - RS485 communication
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (ignored in mock)
   * @param baudrate Baud rate (ignored in mock)
   * @return A unique pointer to OmniHand2025 mock instance
   */
  static std::unique_ptr<OmniHand2025> createHandByRs485(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

  /**
   * @brief Factory method - USB communication
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (ignored in mock)
   * @param baudrate Baud rate (ignored in mock)
   * @return A unique pointer to OmniHand2025 mock instance
   */
  static std::unique_ptr<OmniHand2025> createHandByUsb(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (ignored in mock)
   * @return A unique pointer to OmniHand2025 mock instance
   */
  static std::unique_ptr<OmniHand2025> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id HCAN device index (ignored in mock)
   * @param canfd_channel_id CAN channel index (ignored in mock)
   * @return A unique pointer to OmniHand2025 mock instance
   */
  static std::unique_ptr<OmniHand2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  static std::unique_ptr<OmniHand2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address (mock implementation)
   * @param canfd_device_id USB CANFD adapter device index (ignored in mock)
   * @param canfd_channel_id CAN channel index (ignored in mock)
   * @return Mock DeviceInfo structure
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address by serial number (mock implementation)
   * @param usbcanfd_serial_number USB CANFD device serial number (ignored in mock)
   * @param canfd_channel_id CAN channel index (ignored in mock)
   * @return Mock DeviceInfo structure
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address via SocketCAN (mock implementation)
   * @param can_interface CAN interface name (ignored in mock)
   * @return Mock DeviceInfo structure
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_2025_MOCK_H
