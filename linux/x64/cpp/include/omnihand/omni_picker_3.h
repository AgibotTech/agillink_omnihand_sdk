// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNI_PICKER_3_H
#define AGILINK_OMNI_PICKER_3_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand.h"
#include "omnihand/i_tactile_sensor_1d_u16.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniPicker 3 predefined gestures for SetHandGesture.
 */
enum class OmniPicker3Gesture : int {
  OMNIPICKER_3_GESTURE_ZERO = 0,
  OMNIPICKER_3_GESTURE_HALF_OPEN,
  OMNIPICKER_3_GESTURE_FULL_OPEN,
};

/**
 * @brief OmniPicker 3 interface class - 1 DOF gripper
 *
 * This class provides the public interface for OmniPicker 3 product.
 * It supports 1 active degree of freedom for gripping operations.
 */
class AGIBOT_EXPORT OmniPicker3 : public OmniHand, public ITactileSensor1DU16 {
 public:
  static constexpr unsigned char kDegreesOfActiveFreedom = 1;
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniPicker3() = default;

  // ============ Factory Methods ============

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniPicker3 instance
   */
  static std::unique_ptr<OmniPicker3> createHandByZlgcan(
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
   * @return A unique pointer to OmniPicker3 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniPicker3> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (WiFi/Ethernet to CANFD, this machine acts as TCP client)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param tcp_host TCP server IP or hostname (e.g. "192.168.0.178")
   * @param tcp_port TCP server port (e.g. 8000)
   * @param canfd_channel_id CAN channel index (0 or 1, default 0)
   * @return A unique pointer to OmniPicker3 instance
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   */
  static std::unique_ptr<OmniPicker3> createHandByZlgCanTcp(
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
   * @param can_interface CAN interface name (default "can0")
   * @return A unique pointer to OmniPicker3 instance
   */
  static std::unique_ptr<OmniPicker3> createHandSocketCan(
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
   * @return A unique pointer to OmniPicker3 instance
   */
  static std::unique_ptr<OmniPicker3> createHandByHcan(
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
   * @return A unique pointer to OmniPicker3 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniPicker3> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);


  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00)
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
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
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address (device_id = 0x00) via SocketCAN
   * @param can_interface CAN interface name (default "can0")
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif

  std::vector<std::string> GetJointNames() const override {
    return {
      "joint1",
    };
  }

    /**
   * @brief Returns the number of joint motors.
   * @return Number of joint motors (12)
   */
  static constexpr uint8_t GetNumOfJointMotors() {
    return kDegreesOfActiveFreedom;
  }

  /**
   * @brief Returns the degrees of active freedom (DoA).
   * @return Degrees of active freedom (12)
   */
  static constexpr uint8_t GetDoA() {
    return kDegreesOfActiveFreedom;
  }

  static constexpr Int16Bound GetMinMaxMotorPosition() {
    return kMotorPositionBound;
  }

  static constexpr Int16Range GetMinMaxMixCtrlTorque() {
    return kMixCtrlTorqueRange;
  }

  /**
   * @brief Sets the gripper to a predefined gesture (typed API).
   */
  void SetHandGesture(OmniPicker3Gesture gesture);

  /**
   * @brief Returns gesture target motor position by typed gesture.
   */
  std::vector<int16_t> GetHandGesture(OmniPicker3Gesture gesture);

  /**
   * @brief Sets the gripper to a predefined gesture by numeric ID.
   * @param gesture_num Gesture number (default: 1 = HALF_OPEN)
   */
  void SetHandGesture(int gesture_num = 1) override;

  std::vector<int16_t> GetHandGesture(int gesture_num) override;

 protected:
  /**
   * @brief Initialize base class members
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNI_PICKER_3, device_id, hand_type);
  }

  static constexpr Int16Bound kMotorPositionBound = {0, 4095};
  static constexpr Int16Range kMixCtrlTorqueRange = {0, 6500, 3000}; // unit: mA
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNI_PICKER_3_H
