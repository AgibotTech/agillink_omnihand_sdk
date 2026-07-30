// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2

/**
 * @file omnihand_3_ultra_m.h
 * @brief OmniHand 3 Ultra (O20) interface class - 20 DOF
 * @note This is the public interface for OmniHand 3 Ultra product
 */

#ifndef AGILINK_OMNIHAND_3_ULTRA_M_H
#define AGILINK_OMNIHAND_3_ULTRA_M_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/i_control_mode.h"
#include "omnihand/i_omnihand_calibrator.h"
#include "omnihand/kinematics/omnihand_3_ultra_m/omnihand_3_ultra_m_solver.h"
#include "omnihand/omnihand.h"
#include "omnihand/proto.h"
#include "omnihand/utils.h"

namespace agilink {
namespace omnihand {

// ============================================
// Xense Visual-Tactile Sensor Interface
// ============================================
// Xense frame data structure
struct AGIBOT_EXPORT XenseFrame {
  // RGB image (HWC format, uint8)
  std::vector<uint8_t> rgb_image;
  uint32_t rgb_width;
  uint32_t rgb_height;

  // Depth image (HWC format, float32, meters)
  std::vector<float> depth_image;
  uint32_t depth_width;
  uint32_t depth_height;

  // Timestamp (nanoseconds)
  uint64_t timestamp;
};

// Xense sensor interface
class AGIBOT_EXPORT IXenseSensor {
 public:
  virtual ~IXenseSensor() = default;

  // Initialize Xense sensor
  virtual bool XenseInit(const std::string& serial_number = "") { return false; }

  // Start streaming
  virtual bool XenseStart() { return false; }

  // Stop streaming
  virtual bool XenseStop() { return false; }

  // Get current frame
  virtual bool XenseGetFrame(XenseFrame& frame) { return false; }

  // Calibrate Xense
  virtual bool XenseCalibrate() { return false; }

  // Export runtime config
  virtual bool XenseExportRuntimeConfig(const std::string& path) { return false; }

  // Check if sensor is initialized
  virtual bool XenseIsInitialized() const { return false; }
};

enum class H3UMErrorBit : uint16_t {
  H3U_M_ERR_ENCODER_COMM_TIMEOUT = 1 << 0,
  H3U_M_ERR_CALIBRATION          = 1 << 1,
  H3U_M_ERR_OVER_VOLTAGE         = 1 << 2,
  H3U_M_ERR_UNDER_VOLTAGE        = 1 << 3,
  H3U_M_ERR_OVER_TEMPERATURE     = 1 << 4,
  H3U_M_ERR_TORQUE_SATURATION    = 1 << 5,
  H3U_M_ERR_PARAM_CRC            = 1 << 6,
  H3U_M_ERR_HOMING               = 1 << 7,
  H3U_M_ERR_POSITION_FOLLOWING   = 1 << 8,
  H3U_M_ERR_VELOCITY_FOLLOWING   = 1 << 9,
  H3U_M_ERR_OVER_CURRENT         = 1 << 10,
  H3U_M_ERR_INNER_ENCODER_CRC    = 1 << 11,
  H3U_M_ERR_OUTER_ENCODER_CRC    = 1 << 12,
  H3U_M_ERR_ENCODER_MULTI_TURN   = 1 << 13,
  H3U_M_ERR_ANGLE_IDENTIFY_FAIL  = 1 << 14,
  H3U_M_ERR_RESERVED             = 1 << 15,
};

inline std::string H3UMErrorReportToString(const JointMotorErrorReport& report) {
  if (report.value_ == 0) return "0";
  static const char* names[] = {
      "encoder_comm_timeout",
      "calibration_error",
      "over_voltage",
      "under_voltage",
      "over_temperature",
      "torque_saturation",
      "param_crc_error",
      "homing_error",
      "position_following_error",
      "velocity_following_error",
      "over_current",
      "inner_encoder_crc_error",
      "outer_encoder_crc_error",
      "encoder_multi_turn_error",
      "angle_identify_fail",
      "bi15_reserved",
  };
  std::string s;
  for (int i = 0; i < 16; ++i) {
    if (report.value_ & (1 << i)) {
      if (!s.empty()) s += ",";
      s += names[i];
    }
  }
  return s;
}

/**
 * @brief OmniHand 3 Ultra (O20) interface class - 20 DOF
 *
 * This class provides the public interface for OmniHand 3 Ultra product.
 * It supports 20 active degrees of freedom and optional Xense visual-tactile sensor.
 *
 * Angle <-> motor position convention:
 *   - SetAllJointMotorPosi / GetAllJointMotorPosi int16_t values range [0, 4095]
 *     (12-bit, unrelated to calibration register Pn7/Pn8 0.1-degree encoding).
 *   - URDF active joint angle (rad) and tick are **per-joint independent linear mappings**,
 *     with different coefficients per joint that may be reversed;
 *     O20 does not need the polynomial correction used in O10.
 *   - For actual conversion and calibration table, see
 *       kinematics/omnihand_3_ultra_m/omnihand_3_ultra_m_solver.{h,cc}
 *     SetAll/GetAllActiveJointAngles internally forwards to OmniHand3UltraMSolver,
 *     so upper layers (including ROS2 nodes / Python bindings) only need to use rad units.
 *
 * TODO(O20): Kinematics solver (including passive joints) not yet integrated:
 *   SetHandGesture, and interfaces for accessing passive joints by single joint
 *   name/index are still placeholder implementations.
 */
class AGIBOT_EXPORT OmniHand3UltraM : public OmniHand, public IControlMode, public IOmniHandCalibrator, public IXenseSensor {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 20;  // DoA
  static constexpr uint8_t kDegreesOfPassiveFreedom = 0; // DoP
  static constexpr uint8_t kDefaultHandDeviceId = 9u;

  virtual ~OmniHand3UltraM() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id CANFD device ID
   * @param canfd_channel_id CANFD channel ID
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0,
      const std::string& xense_serial_number = "");

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param usbcanfd_serial_number USB CANFD serial number
   * @param canfd_channel_id CANFD channel ID
   * @param xense_serial_number Xense sensor serial number (empty = no Xense)
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0,
      const std::string& xense_serial_number = "");

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (WiFi/Ethernet to CANFD, this machine acts as TCP client)
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param tcp_host TCP server IP or hostname (e.g. "192.168.0.178")
   * @param tcp_port TCP server port (e.g. 8000)
   * @param canfd_channel_id CAN channel index (0 or 1, default 0)
   * @param xense_serial_number Xense sensor serial number (empty = no Xense)
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& tcp_host,
      uint16_t tcp_port,
      uint8_t canfd_channel_id = 0,
      const std::string& xense_serial_number = "");
#endif

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g. "can0")
   * @param xense_serial_number Xense sensor serial number (empty = no Xense)
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0",
      const std::string& xense_serial_number = "");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id CANFD device ID
   * @param canfd_channel_id CANFD channel ID
   * @param xense_serial_number Xense sensor serial number (empty = no Xense)
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0,
      const std::string& xense_serial_number = "");

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param hcan_serial_number HCAN serial number
   * @param canfd_channel_id CANFD channel ID
   * @param xense_serial_number Xense sensor serial number (empty = no Xense)
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0,
      const std::string& xense_serial_number = "");

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

  // ============ Joint Naming ============
  /**
   * @brief Returns the 20 joint names of O20 in motor-index order.
   * @note Order MUST match SetAllJointMotorPosi / GetAllJointMotorPosi index layout.
   *       This is the single source of truth for "motor index <-> ROS joint name"
   *       binding; ROS2 node code should call this instead of hard-coding the list.
   */
  std::vector<std::string> GetJointNames() const override {
    return {
        "pinky_abad_joint",  "pinky_mcp_joint",  "pinky_pip_joint",  "pinky_dip_joint",
        "ring_abad_joint",   "ring_mcp_joint",   "ring_pip_joint",   "ring_dip_joint",
        "middle_abad_joint", "middle_mcp_joint", "middle_pip_joint", "middle_dip_joint",
        "index_abad_joint",  "index_mcp_joint",  "index_pip_joint",  "index_dip_joint",
        "thumb_abad_joint",  "thumb_mcp_joint",  "thumb_pip_joint",  "thumb_dip_joint",
    };
  }

  static constexpr uint8_t GetNumOfJointMotors() {
    return kDegreesOfActiveFreedom;
  }

  static constexpr uint8_t GetDoA() {
    return kDegreesOfActiveFreedom;
  }

  static constexpr uint8_t GetDoP() {
    return kDegreesOfPassiveFreedom;
  }

  static constexpr Int16Bound GetMinMaxMotorPosition(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return h3um::OmniHand3UltraMSolver::GetMotorPositionRange();
  }

  static constexpr Int16Bound GetMinMaxActualMotorPosition(uint8_t joint_motor_index) {
    (void)joint_motor_index;
    return kActualMotorPositionBound;
  }

  // ============ Gesture Control ============
  void SetHandGesture(h3um::H3UMGesture gesture);

  std::vector<int16_t> GetHandGesture(h3um::H3UMGesture gesture);

  void SetHandGesture(int gesture_num = 1) override;

  std::vector<int16_t> GetHandGesture(int gesture_num) override;

  // ============ O20 Extended API ============

  // ---- IOmniHandCalibrator interface ----

  // Pn23 (0x17): Single axis actual (calibration) position
  // int16_t, unit: 0.1 degree, range: -1800 ~ 1800 (i.e. -180.0° ~ 180.0°), default: 0
  int16_t SetSingleActualAxisPos(uint8_t axis_index, int16_t position) override { (void)axis_index; (void)position; return 0; }
  int16_t GetSingleActualAxisPos(uint8_t axis_index) const override { (void)axis_index; return 0; }

  // Pn23 (0x17): All axes actual (calibration) positions
  std::vector<int16_t> SetAllActualAxisPos(const std::vector<int16_t>& positions) override { (void)positions; return {}; }
  std::vector<int16_t> GetAllActualAxisPos() const override { return {}; }

  // Pn9 (0x09): Set axis homing (zero reference) position
  // axis_index: 0=all axes (pos ignored), 1-20=single axis
  bool SetAxisHoming(uint8_t axis_index, int16_t pos) override { (void)axis_index; (void)pos; return false; }

  // Pn8 (0x08): Set single axis minimum position limit
  // int16_t, unit: 0.1 degree, range: -1800 ~ 1800, default: 0
  bool SetAxisMinPos(uint8_t axis_index, int16_t min_pos) override { (void)axis_index; (void)min_pos; return false; }

  // Pn7 (0x07): Set single axis maximum position limit
  // int16_t, unit: 0.1 degree, range: -1800 ~ 1800, default: 0
  bool SetAxisMaxPos(uint8_t axis_index, int16_t max_pos) override { (void)axis_index; (void)max_pos; return false; }

  // Get all axes position limits (Pn7 & Pn8)
  AxisLimitPos GetAxisLimitPos() const override { return {}; }

  // Clear all axis position limits (not supported on O20, always returns false)
  bool ClearAllLimitPos() override { return false; }

  // Pn10 (0x0A): Save parameters to flash
  // Parameters set via Pn7~Pn9 are NOT persisted until this command is sent.
  bool SaveParam() override { return false; }

  // ---- O20-specific extensions (not in IOmniHandCalibrator) ----

  // Pn11 (0x0B): Tactile sensor zero calibration
  virtual bool CalibrateTactileSensor() { return false; }

  // Pn24 (0x18): Start auto calibration for all motors
  virtual bool StartAutoCalibration() { return false; }

  // Clear error report
  virtual void ClearAllErrorReport() {}
  virtual void ClearErrorReport(unsigned char joint_motor_index) { (void)joint_motor_index; }

 protected:
  /**
   * @brief Initialize base class members and per-joint rad <-> tick solver.
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note Product type is fixed to ProductType::OMNIHAND_3_ULTRA_M for this class.
   *       Solver is built here so derived impls (CAN / Serial / ...) can use
   *       joint_motor_solver_ directly without re-implementing init logic.
   */
  void Reset(uint8_t device_id, HandType hand_type);
  void OnHandTypeChanged() override;

  /**
   * @brief Per-joint linear rad <-> motor-tick solver for OmniHand 3 Ultra.
   *        Bound to kLeft / kRight inside Reset() based on hand_type_.
   *        Shared by every transport implementation.
   */
  std::unique_ptr<h3um::OmniHand3UltraMSolver> joint_motor_solver_;
  static constexpr Int16Bound kActualMotorPositionBound = {-1800, 1800};
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_ULTRA_M_H
