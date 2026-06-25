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
#include "i_tactile_sensor_1d_u16.h"
#include "omnihand/export_symbols.h"
#include "omnihand/kinematics/omnihand_3_lite/omnihand_3_lite_solver.h"
#include "omnihand/omnihand.h"
#include "omnihand/private_omnihand.h"
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
 * @note Currently does not support angle-based control (SetAllActiveJointAngles) as full kinematics solver is not available.
 *       Use motor position control (SetJointMotorPosi, SetAllJointMotorPosi) instead.
 *       Gesture control is available via SetHandGesture() with OmniHand3LiteGesture enum.
 */
class AGIBOT_EXPORT OmniHand3Lite : public OmniHand, public PrivateOmniHand, public ITactileSensor1DU16 {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 4;  // DoA
  static constexpr uint8_t kDegreesOfPassiveFreedom = 5; // DoP
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
   * @brief Factory method - TJ MARVIN controller TJ SDK end-effector CAN/CANFD passthrough (O4, same semantics as O10)
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
  /**
   * @brief Get joint names for O4 (OmniHand 3 Lite)
   * @return Vector of joint names
   */
  std::vector<std::string> GetJointNames() const override {
    const std::string p = hand_type_ == HandType::LEFT ? "L_" : "R_";
    // because of doesn't have URDF, so we hardcode the joint names
    return {
      p + "joint1",  // 1
      p + "joint2",  // 2
      p + "joint3",  // 3
      p + "joint4",  // 4
    };
  }

  static uint8_t GetNumOfJointMotors() {
    return kDegreesOfActiveFreedom;
  }

  static uint8_t GetDoA() {
    return kDegreesOfActiveFreedom;
  }

  static uint8_t GetDoP() {
    return kDegreesOfPassiveFreedom;
  }

  static Int16Bound GetMinMaxMotorPosition(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return OmniHand3LiteSolver::GetMotorPositionRange();
  }

  static Int16Bound GetMinMaxActualMotorPosition(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return kActualMotorPositionBound;
  }

  static Int16Range GetMinMaxDefaultMixCtrlVelocity(uint8_t joint_motor_index) {
    if (joint_motor_index <= 0 || joint_motor_index > kDegreesOfActiveFreedom) return {0, 0, 0};
    return kMixCtrlVelocityRange;
  }

  static Int16Range GetMinMaxDefaultMixCtrlTorque(uint8_t joint_motor_index) {
    if (joint_motor_index <= 0 || joint_motor_index > kDegreesOfActiveFreedom) return {0, 0, 0};
    return kMixCtrlTorqueRange;
  }

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture (typed API).
   */
  void SetHandGesture(OmniHand3LiteGesture gesture);

  std::vector<int16_t> GetHandGesture(OmniHand3LiteGesture gesture);

  void SetHandGesture(int gesture_num) override;

  std::vector<int16_t> GetHandGesture(int gesture_num) override;

 protected:
  /**
   * @brief Initialize base class members and kinematics solver
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note Product type is fixed to ProductType::OMNIHAND_3_LITE for this class
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNIHAND_3_LITE, device_id, hand_type);
    solver_ = std::make_unique<OmniHand3LiteSolver>(hand_type_ == HandType::LEFT);
  }

  void OnHandTypeChanged() override {
    solver_ = std::make_unique<OmniHand3LiteSolver>(hand_type_ == HandType::LEFT);
  }

  std::unique_ptr<OmniHand3LiteSolver> solver_;
  static constexpr Int16Bound kActualMotorPositionBound = {0, 4095};
  static constexpr Int16Range kMixCtrlVelocityRange = {0, 23767, 8000}; // unit: rpm
  static constexpr Int16Range kMixCtrlTorqueRange = {0, 1000, 300}; // unit: mA
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_LITE_H
