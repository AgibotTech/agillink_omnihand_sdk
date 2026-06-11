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
#include "omnihand/i_control_mode.h"
#include "omnihand/kinematics/omnihand_pro_2025/omnihand_pro_2025_solver.h"
#include "omnihand/omnihand.h"
#include "omnihand/ota_types.h"
#include "omnihand/proto.h"
#include "omnihand/utils.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniHand Pro 2025 (O12) interface class - 12 DOF
 *
 * This class provides the public interface for OmniHand Pro 2025 product.
 * It includes all methods supported by O12, including 3D tactile sensors and report period settings.
 */
class AGIBOT_EXPORT OmniHandPro2025 : public OmniHand, public IControlMode {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 12;  // DoA
  static constexpr unsigned char kDegreesOfPassiveFreedom = 7;  // DoP
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
   * @brief Factory method - TJ MARVIN controller TJ SDK end-effector CAN/CANFD passthrough (O12)
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

  // ============ Voltage Control ============
  /**
   * @brief Sets the voltage command of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @param voltage Voltage command, clamped by implementation to the supported range
   */
  virtual void SetJointMotorVoltage(unsigned char joint_motor_index, int16_t voltage) = 0;

  /**
   * @brief Sets voltage commands of all joint motors in batch.
   * @param vec_voltage Voltage command vector, length 12
   */
  virtual void SetAllJointMotorVoltage(const std::vector<int16_t>& vec_voltage) = 0;

  /**
   * @brief Gets the voltage command of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Current voltage command
   * @note O12 firmware versions up to and including 1.2.15 do not support voltage readback.
   */
  virtual int16_t GetJointMotorVoltage(unsigned char joint_motor_index) const = 0;

  /**
   * @brief Gets voltage commands of all joint motors in batch.
   * @return Voltage command vector, length 12 when the request succeeds
   * @note O12 firmware versions up to and including 1.2.15 do not support voltage readback.
   */
  virtual std::vector<int16_t> GetAllJointMotorVoltage() const = 0;

  // ============ Torque Control ============
  /**
   * @brief Sets the torque of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @param torque Torque value
   */
  virtual void SetJointMotorTorque(unsigned char joint_motor_index, int16_t torque) = 0;

  /**
   * @brief Gets the torque of a single joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Current torque value
   */
  virtual int16_t GetJointMotorTorque(unsigned char joint_motor_index) const = 0;

  /**
   * @brief Sets torques of all joint motors in batch.
   * @param vec_torque Torque vector, length 12
   */
  virtual void SetAllJointMotorTorque(const std::vector<int16_t>& vec_torque) = 0;

  /**
   * @brief Gets torques of all joint motors in batch.
   * @return Torque vector, length 12
   */
  virtual std::vector<int16_t> GetAllJointMotorTorque() const = 0;

  // ============ Position Mode Fine-Tune ============
  /**
   * @brief Enables or disables position mode fine-tune for all joint motors.
   * @param enable true to enable, false to disable
   * @note Only effective when all motors are in position control mode
   */
  virtual void SetPositionFineTuneMode(bool enable) = 0;

  /**
   * @brief Gets the current position mode fine-tune state.
   * @return true if fine-tune is enabled, false if disabled
   */
  virtual bool GetPositionFineTuneMode() const = 0;

  // ============ Joint Naming ============
  /**
   * @brief Returns names for physical motors 1–12 (O12handProActuator order).
   * @note Order MUST match SetAllJointMotorPosi / GetAllJointMotorPosi and
   *       o12::O12handProActuator in omnihand_pro_2025_solver.h. Motors 1–2 are
   *       index coupled tendons (not thumb); thumb roll/abad are motors 5–6.
   */
  std::vector<std::string> GetJointNames() const override {
    const std::string p = is_left_hand_ ? "L_" : "R_";
    return {
        p + "index_abad_joint",   // 1 ActuatorIndex1 (near thumb)
        p + "index_mcp_joint",    // 2 ActuatorIndex2
        p + "middle_abad_joint",  // 3 ActuatorMiddle1
        p + "middle_mcp_joint",   // 4 ActuatorMiddle2
        p + "thumb_abad_joint",   // 5 ActuatorThumbABAD
        p + "thumb_roll_joint",   // 6 ActuatorThumbRoll
        p + "index_pip_joint",    // 7 ActuatorIndex3
        p + "middle_pip_joint",   // 8 ActuatorMiddle3
        p + "ring_mcp_joint",     // 9 ActuatorRing
        p + "pinky_mcp_joint",    // 10 ActuatorPinky
        p + "thumb_pip_joint",    // 11 ActuatorThumbPIP
        p + "thumb_mcp_joint",    // 12 ActuatorThumbMCP
    };
  }

  // ============ Motor Range ============
  /**
   * @brief Returns the number of joint motors.
   * @return Number of joint motors (12)
   */
  static uint8_t GetNumOfJointMotors() {
    return kDegreesOfActiveFreedom;
  }

  /**
   * @brief Returns the degrees of active freedom (DoA).
   * @return Degrees of active freedom (12)
   */
  static uint8_t GetDoA() {
    return kDegreesOfActiveFreedom;
  }

  /**
   * @brief Returns the degrees of passive freedom (DoP).
   * @return Degrees of passive freedom (7)
   */
  static uint8_t GetDoP() {
    return kDegreesOfPassiveFreedom;
  }

  /**
   * @brief Returns the min/max motor position range (unit: motor ticks) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Min/max motor position range
   */
  static Int16Bound GetMinMaxMotorPosition(uint8_t joint_motor_index) {
    if (joint_motor_index <= 0 || joint_motor_index > kDegreesOfActiveFreedom) return {0, 0};
    auto [mn, mx] = o12::OmniHandPro2025Solver::GetMotorPositionRange(joint_motor_index - 1);
    return {static_cast<int16_t>(mn), static_cast<int16_t>(mx)};
  }

  /**
   * @brief Returns the min/max motor angle range (unit: radians) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @param hand_type Hand type (left/right)
   * @return Min/max motor angle range
   */
  static FloatBound GetMinMaxJointAngle(uint8_t joint_motor_index, HandType hand_type) {
    if (joint_motor_index <= 0 || joint_motor_index > kDegreesOfActiveFreedom) return {0.0f, 0.0f};
    auto [mn, mx] = o12::OmniHandPro2025Solver::GetJointAngleRange(joint_motor_index - 1, hand_type == HandType::LEFT);
    return {static_cast<float>(mn), static_cast<float>(mx)};
  }

  /**
   * @brief Returns the min/max actual motor position range (unit: motor ticks) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Min/max actual motor position range
   */
  static Int16Bound GetMinMaxActualMotorPosition(uint8_t joint_motor_index) {
    return GetMinMaxMotorPosition(joint_motor_index);
  }

  /**
   * @brief Returns the min/max default motor velocity range (unit: tick/s) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Min/max default motor velocity range
   */
  static Int16Range GetMinMaxDefaultMotorVelocity(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return kMotorVelocityRange;
  }

  /**
   * @brief Returns the min/max default motor torque range (unit: mA) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Min/max default motor torque range
   */
  static Int16Range GetMinMaxDefaultMotorTorque(uint8_t joint_motor_index) {
    return (joint_motor_index <= kServoMotorCount) ? kServoMotorTorqueCurrentRange
                                                   : kNormalMotorTorqueCurrentRange;
  }

  /**
   * @brief Returns the min/max default mixed control torque range (unit: 0.01N) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Min/max default mixed control torque range
   */
  static Int16Range GetMinMaxDefaultMixCtrlTorque(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return kMixCtrlTorqueRange;
  }

  /**
   * @brief Returns the min/max default motor voltage range (unit: ‰) for a joint motor.
   * @param joint_motor_index Joint motor index (1-12)
   * @return Min/max default motor voltage range
   */
  static Int16Range GetMinMaxDefaultMotorVoltage(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return kMotorVoltageRange;
  }

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number (ignored for O12, only one FIST type supported)
   */
  void SetHandGesture(o12::OmniHandPro2025Gesture gesture);

  void SetHandGesture(int gesture_num) override;

  std::vector<int16_t> GetHandGesture(o12::OmniHandPro2025Gesture gesture);

  std::vector<int16_t> GetHandGesture(int gesture_num) override;

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
  static constexpr uint8_t kServoMotorCount = 10; // 1 - 10
  static constexpr Int16Range kMotorVelocityRange = {200, 2500, 2000}; // unit: tick/s (2000 tick is full)
  static constexpr Int16Range kServoMotorTorqueCurrentRange = {1, 350, 350}; // 1 - 10, unit: mA
  static constexpr Int16Range kNormalMotorTorqueCurrentRange = {1, 350, 320}; // 11 - 12, unit: mA
  static constexpr Int16Range kMixCtrlTorqueRange = {100, 2000, 100}; // unit: 0.01N
  static constexpr Int16Range kMotorVoltageRange = {-1000, 1000, 0}; // unit: ‰
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_2025_PRO_H
