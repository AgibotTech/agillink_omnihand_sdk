// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_3_ultra.h
 * @brief OmniHand 3 Ultra (O20) interface class - 20 DOF
 * @note This is the public interface for OmniHand 3 Ultra product
 */

#ifndef AGILINK_OMNIHAND_3_ULTRA_H
#define AGILINK_OMNIHAND_3_ULTRA_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_base.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

/**
 * @brief OmniHand 3 Ultra (O20) interface class - 20 DOF
 *
 * This class provides the public interface for OmniHand 3 Ultra product.
 * It supports 20 active degrees of freedom.
 *
 * TODO(O20): 运动学求解器未接入。SetAllActiveJointAngles / GetAllActiveJointAngles /
 * GetAllJointAngles / GetAllJointPos / SetHandGesture 当前为占位实现，请使用
 * SetAllJointMotorPosi / GetAllJointMotorPosi 等电机位置接口直接控制。
 */
class AGIBOT_EXPORT OmniHand3Ultra : public OmniHandBase {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 20;  // DoA
  static constexpr uint8_t kDefaultHandDeviceId = 9u;

  virtual ~OmniHand3Ultra() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByZlgcan(
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
   * @return A unique pointer to OmniHand3Ultra instance
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& tcp_host,
      uint16_t tcp_port,
      uint8_t canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   */
  static std::unique_ptr<OmniHand3Ultra> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  /**
   * @brief 天机 MARVIN 控制器 TJ SDK 末端 CAN/CANFD 透传（O20）
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByTJ(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& marvin_controller_ip);
#endif

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00)
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address by serial number
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address via SocketCAN
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface);
#endif

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number
   * TODO(O20): 待运动学接入后实现；当前为 no-op，请用 SetAllJointMotorPosi 控制
   */
  void SetHandGesture(int gesture_num = 1) override;

  // ============ O20 Extended API ============

  // Pn8 (0x08): Motor min limit (for calibration)
  // int16_t, unit: 0.1 degree, range: -1800 ~ 1800 (i.e. -180.0° ~ 180.0°), default: 0
  // e.g. 900 means 90.0°, -900 means -90.0°
  // Write returns the current value; read returns the current value.
  virtual int16_t SetMotorMinLimit(unsigned char joint_motor_index, int16_t min_limit) { (void)joint_motor_index; (void)min_limit; return 0; }
  virtual int16_t GetMotorMinLimit(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  virtual std::vector<int16_t> SetAllMotorMinLimit(const std::vector<int16_t>& vec_min_limit) { (void)vec_min_limit; return {}; }
  virtual std::vector<int16_t> GetAllMotorMinLimit() const { return {}; }

  // Pn7 (0x07): Motor max limit (for calibration)
  // int16_t, unit: 0.1 degree, range: -1800 ~ 1800 (i.e. -180.0° ~ 180.0°), default: 0
  virtual int16_t SetMotorMaxLimit(unsigned char joint_motor_index, int16_t max_limit) { (void)joint_motor_index; (void)max_limit; return 0; }
  virtual int16_t GetMotorMaxLimit(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  virtual std::vector<int16_t> SetAllMotorMaxLimit(const std::vector<int16_t>& vec_max_limit) { (void)vec_max_limit; return {}; }
  virtual std::vector<int16_t> GetAllMotorMaxLimit() const { return {}; }

  // Pn9 (0x09): Set motor current position as zero point
  // Returns true on success, false on failure.
  virtual bool SetJointMotorZeroPoint(unsigned char joint_motor_index) { (void)joint_motor_index; return false; }

  // Pn10 (0x0A): Save parameters to flash
  // Parameters set via Pn7~Pn9 are NOT persisted until this command is sent.
  // Returns true on success, false on failure.
  virtual bool SaveParameters() { return false; }

  // Pn11 (0x0B): Tactile sensor zero calibration
  // Returns true on success, false on failure.
  virtual bool CalibrateTactileSensor() { return false; }

  // Pn23 (0x17): Motor actual position (for calibration)
  // int16_t, unit: 0.1 degree, range: -1800 ~ 1800 (i.e. -180.0° ~ 180.0°), default: 0
  // Write sets target position, returns current actual position; read returns current actual position.
  virtual int16_t SetMotorActualPos(unsigned char joint_motor_index, int16_t pos) { (void)joint_motor_index; (void)pos; return 0; }
  virtual int16_t GetMotorActualPos(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  virtual std::vector<int16_t> SetAllMotorActualPos(const std::vector<int16_t>& vec_pos) { (void)vec_pos; return {}; }
  virtual std::vector<int16_t> GetAllMotorActualPos() const { return {}; }

  // Pn24 (0x18): Start auto calibration for all motors
  // Returns true on success, false on failure.
  virtual bool StartAutoCalibration() { return false; }

 protected:
  /**
   * @brief Initialize base class members
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note Product type is fixed to ProductType::OMNIHAND_3_ULTRA for this class
   */
  void Reset(uint8_t device_id, HandType hand_type);
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_ULTRA_H
