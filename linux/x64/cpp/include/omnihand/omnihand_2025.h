// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_2025.h
 * @brief OmniHand 2025 (O10) interface class - 10 DOF
 * @note This is the public interface for OmniHand 2025 product
 */

#ifndef AGILINK_OMNIHAND_2025_H
#define AGILINK_OMNIHAND_2025_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/omnihand.h"
#include "omnihand/private_omnihand.h"
#include "omnihand/i_o10_tactile_sensor_1d.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"
#include "omnihand/kinematics/omnihand_2025/omnihand_2025_solver.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniHand 2025 (O10) interface class - 10 DOF
 * 
 * This class provides the public interface for OmniHand 2025 product.
 * It includes all methods supported by O10, including 1D tactile sensors.
 * 
 * @note Inherits from IO10TactileSensor1D for 1D tactile sensor interface.
 */
class AGIBOT_EXPORT OmniHand2025 : public OmniHand, public PrivateOmniHand, public IO10TactileSensor1D{
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 10;  // DoA
  static constexpr unsigned char kDegreesOfPassiveFreedom = 6;  // DoP
  /** @brief Default hand device ID on the bus for O10 (protocol value 1). Pass explicitly to factories. */
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniHand2025() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgcan(
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
   * @return A unique pointer to OmniHand2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgcan(
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
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& host,
      uint16_t port,
      uint8_t canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

  /**
   * @brief Factory method - RS485 communication (OmniHand 2025 only)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (e.g., "/dev/ttyUSB0")
   * @param baudrate Baud rate (default 460800)
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByRs485(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

  /**
   * @brief Factory method - USB communication (OmniHand 2025 only)
   * @note Pass `kDefaultHandDeviceId` explicitly; `uart_port` has no default.
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (e.g., "/dev/ttyACM0" or "COM3")
   * @param baudrate Baud rate (default 460800)
   * @return A unique pointer to OmniHand2025 instance
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
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return A unique pointer to OmniHand2025 instance
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
   * @param canfd_device_id HCAN device index
   * @param canfd_channel_id CAN channel index (default 0)
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByHcan(
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
   * @return A unique pointer to OmniHand2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHand2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  /**
   * @brief Factory method - TJ MARVIN controller TJ SDK end-effector CAN/CANFD passthrough (O10, parallel to USB-CAN/SocketCAN)
   * @param hand_type Hand type: LEFT -> TJ left arm end-effector channel (OnSetChDataA), RIGHT -> right arm (OnSetChDataB). Consistent with the convention that left/right hand is mounted on the same-side robotic arm
   * @param hand_device_id Hand device ID on the OmniHand bus
   * @param marvin_controller_ip Robotic arm controller IP (UDP, consistent with TJ SDK OnLinkTo)
   * @note TJ end-effector uses CAN/CANFD passthrough (set_ch=1); COM passthrough does not use this interface
   */
  static std::unique_ptr<OmniHand2025> createHandByTJ(
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
   * @note Only works with CAN communication, not supported for RS485
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
   * @note Only works with CAN communication, not supported for RS485
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
   * @note Only works with CAN communication, not supported for RS485
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif

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

  /**
   * @brief Gets tactile sensor data for the specified process, such as downsampled data.
   * @param finger Finger/palm enum value
   * @return Tactile sensor data vector
   */
  virtual std::vector<uint8_t> GetTactileSensorData(Finger finger) const = 0;

  /**
   * @brief Gets all 1D tactile sensor data for the specified process, such as downsampled data.
   * @return Vector of tactile sensor data vector
   */
  virtual std::vector<TactileSensorData> GetAllTactileSensorData() const = 0;

  // ============ Joint Naming ============
  /**
   * @brief Returns the 10 active joint names of O10 in motor-index order.
   * @note Order MUST match the OmnihandActiveJoint enum used by the kinematics
   *       solver and SetAllJointMotorPosi / GetAllJointMotorPosi. Names are
   *       prefixed with "L_"/"R_" to match the URDF in
   *       `omnihand_sdk/assets/omnihand_description`.
   */
  std::vector<std::string> GetJointNames() const override {
    const std::string p = is_left_hand_ ? "L_" : "R_";
    return {
      p + "thumb_roll_joint",
      p + "thumb_abad_joint",
      p + "thumb_mcp_joint",
      p + "index_abad_joint",
      p + "index_pip_joint",
      p + "middle_pip_joint",
      p + "ring_abad_joint",
      p + "ring_pip_joint",
      p + "pinky_abad_joint",
      p + "pinky_pip_joint",
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
    return o10::OmniHand2025Solver::GetMotorPositionRange();
  }

  static FloatBound GetMinMaxJointAngle(uint8_t joint_motor_index, HandType hand_type) {
    return o10::OmniHand2025Solver::GetJointAngleRange(joint_motor_index - 1, hand_type == HandType::LEFT);
  }

  static Int16Bound GetMinMaxActualMotorPosition(uint8_t joint_motor_index) {
    if (joint_motor_index <= 0 || joint_motor_index > kDegreesOfActiveFreedom) return {0, 0};
    if (joint_motor_index == o10::ActiveJointIndexAbAd + 1 ||
        joint_motor_index == o10::ActiveJointRingAbAd + 1 ||
        joint_motor_index == o10::ActiveJointPinkyAbAd + 1) {
      return kActualSideMotorPositionBound;
    }
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
  void SetHandGesture(o10::OmniHand2025Gesture gesture);

  std::vector<int16_t> GetHandGesture(o10::OmniHand2025Gesture gesture);

  /**
   * @brief Same gestures as o10::OmniHand2025Gesture: 0 = PAPER … 16 = CLASPING;
   *        17 = RESET (all active joints zero). Default 1 = FIST1.
   */
  void SetHandGesture(int gesture_num = 1) override;

  /**
   * @brief Returns motor positions for a gesture without sending to device.
   */
  std::vector<int16_t> GetHandGesture(int gesture_num) override;

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
  static constexpr Int16Bound kActualMotorPositionBound = {0, 4095};
  static constexpr Int16Bound kActualSideMotorPositionBound = {0, 1023}; // 4, 7 ,9
  static constexpr Int16Range kMixCtrlVelocityRange = {0, 23767, 8000}; // unit: rpm
  static constexpr Int16Range kMixCtrlTorqueRange = {0, 1000, 300}; // unit: mA
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_2025_H
