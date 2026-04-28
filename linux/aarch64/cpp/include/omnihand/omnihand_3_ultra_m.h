// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

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
#include "omnihand/i_omnihand_calibrator.h"
#include "omnihand/kinematics/omnihand_3_ultra_m/omnihand_3_ultra_m_solver.h"
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
 * 角度 <-> 电机位置约定：
 *   - SetAllJointMotorPosi / GetAllJointMotorPosi 中 int16_t 值域为 [0, 4095]
 *     （12-bit，与标定寄存器 Pn7/Pn8 的 0.1° 编码无关）。
 *   - URDF 主动关节角 (rad) 与 tick 是 **按关节独立的线性映射**，不同关节系数不同、
 *     可能反向；O20 不需要 O10 那种多项式修正。
 *   - 实际换算与标定表见
 *       kinematics/omnihand_3_ultra_m/omnihand_3_ultra_m_solver.{h,cc}
 *     SetAll/GetAllActiveJointAngles 内部即转发给 OmniHand3UltraMSolver，
 *     上层（含 ROS2 节点 / Python 绑定）只需用 rad 单位即可。
 *
 * TODO(O20): 运动学求解器（含被动关节）未接入：SetHandGesture、按单关节名/索引
 *   访问被动关节的接口仍为占位实现。
 */
class AGIBOT_EXPORT OmniHand3UltraM : public OmniHandBase, public IOmniHandCalibrator {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 20;  // DoA
  static constexpr uint8_t kDefaultHandDeviceId = 9u;

  virtual ~OmniHand3UltraM() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByZlgcan(
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
   * @return A unique pointer to OmniHand3UltraM instance
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByZlgCanTcp(
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
  static std::unique_ptr<OmniHand3UltraM> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  /**
   * @brief 天机 MARVIN 控制器 TJ SDK 末端 CAN/CANFD 透传（O20）
   */
  static std::unique_ptr<OmniHand3UltraM> createHandByTJ(
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

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number
   * TODO(O20): 待手势库接入后实现；当前为 no-op。上层请用 SetAllActiveJointAngles(rad)
   * 下发整手关节目标，或 SetAllJointMotorPosi(0-4095 ticks) 走原始 actuator 通道。
   */
  void SetHandGesture(int gesture_num = 1) override;

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
  virtual bool ClearAllErrorReport() { return false; }
  virtual bool ClearErrorReport(unsigned char joint_motor_index) { (void)joint_motor_index; return false; }

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

  /**
   * @brief Per-joint linear rad <-> motor-tick solver for OmniHand 3 Ultra.
   *        Bound to kLeft / kRight inside Reset() based on is_left_hand_.
   *        Shared by every transport implementation.
   */
  std::unique_ptr<h3um::OmniHand3UltraMSolver> joint_motor_solver_;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_ULTRA_M_H
