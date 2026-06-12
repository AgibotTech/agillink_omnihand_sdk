// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_3_ultra_m_solver.h
 * @brief Per-joint linear rad <-> motor-tick solver for O20 (h3u_m).
 *
 * Naming: aligned with the URDF repo `h3u_m_description`, hence the `_m`
 * suffix on the directory / class. Hardware-wise this is OmniHand3UltraM
 * (20 active DOF).
 *
 */

#ifndef AGILINK_OMNIHAND_3_ULTRA_M_SOLVER_H
#define AGILINK_OMNIHAND_3_ULTRA_M_SOLVER_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {
namespace h3um {

/**
 * @brief O20 (h3u_m) active-joint enum, in motor-index / GetJointNames() order.
 * @note Joint order must match OmniHand3UltraM::GetJointNames() exactly:
 *     [0..3]   pinky  (abad, mcp, pip, dip)
 *     [4..7]   ring
 *     [8..11]  middle
 *     [12..15] index
 *     [16..19] thumb
 */
enum class H3UMActiveJoint {
  H3U_M_ACTIVE_JOINT_PINKY_ABAD = 0,
  H3U_M_ACTIVE_JOINT_PINKY_MCP,
  H3U_M_ACTIVE_JOINT_PINKY_PIP,
  H3U_M_ACTIVE_JOINT_PINKY_DIP,
  H3U_M_ACTIVE_JOINT_RING_ABAD,
  H3U_M_ACTIVE_JOINT_RING_MCP,
  H3U_M_ACTIVE_JOINT_RING_PIP,
  H3U_M_ACTIVE_JOINT_RING_DIP,
  H3U_M_ACTIVE_JOINT_MIDDLE_ABAD,
  H3U_M_ACTIVE_JOINT_MIDDLE_MCP,
  H3U_M_ACTIVE_JOINT_MIDDLE_PIP,
  H3U_M_ACTIVE_JOINT_MIDDLE_DIP,
  H3U_M_ACTIVE_JOINT_INDEX_ABAD,
  H3U_M_ACTIVE_JOINT_INDEX_MCP,
  H3U_M_ACTIVE_JOINT_INDEX_PIP,
  H3U_M_ACTIVE_JOINT_INDEX_DIP,
  H3U_M_ACTIVE_JOINT_THUMB_ABAD,
  H3U_M_ACTIVE_JOINT_THUMB_MCP,
  H3U_M_ACTIVE_JOINT_THUMB_PIP,
  H3U_M_ACTIVE_JOINT_THUMB_DIP,

  H3U_M_ACTIVE_JOINT_COUNT  ///< Total active-joint count = 20.
};

enum class H3UMGesture : int {
  H3U_M_GESTURE_ALL_ZERO = 0,
};

/**
 * @class OmniHand3UltraMSolver
 * @brief Per-joint linear rad <-> actuator-input solver for OmniHand 3 Ultra
 *        (h3u_m, O20).
 *
 * Naming follows the O10 / O12 solvers:
 *   - "ActiveJointPos" : joint position in rad (vector<double>), per-joint
 *                        ROS convention (sensor_msgs::JointState::position).
 *   - "ActuatorInput"  : raw motor command, vector<int16_t> in [0, 4095].
 *                        (O10 uses int and 0-4096; O12 uses int and 0-2000.
 *                        For O20 we keep int16_t to match the SDK's
 *                        Set/GetAllJointMotorPosi protocol type.)
 *
 * Calibration data (rad_min, rad_max, actuator_min, actuator_max) lives in the
 * kRight / kLeft tables in the .cc file. rad bounds come from the URDF;
 * actuator bounds default to (kActuatorInputMin, kActuatorInputMax) and must be
 * back-filled after physical calibration.
 *
 * Per-joint linear mapping:
 *     actuator = (rad      - rad_min)      / (rad_max      - rad_min)
 *                * (actuator_max - actuator_min) + actuator_min
 *     rad      = (actuator - actuator_min) / (actuator_max - actuator_min)
 *                * (rad_max      - rad_min)      + rad_min
 *
 * If a joint's hardware direction is opposite to the URDF, simply write the
 * row's (actuator_min, actuator_max) as (4095, 0) -- no separate sign flag.
 */
class AGIBOT_EXPORT OmniHand3UltraMSolver {
 public:
  /// Number of active joints (= H3UMActiveJoint::H3U_M_ACTIVE_JOINT_COUNT = 20).
  static constexpr std::size_t kActiveJointCount =
      static_cast<std::size_t>(H3UMActiveJoint::H3U_M_ACTIVE_JOINT_COUNT);
  /// Actuator-input value range (matches int16_t domain of
  /// SetAllJointMotorPosi / GetAllJointMotorPosi).
  static constexpr int16_t kActuatorInputMin = 0;
  static constexpr int16_t kActuatorInputMax = 4096;

  /// Per-joint linear calibration entry.
  struct JointMotorCalib {
    double rad_min;          // URDF lower limit (rad).
    double rad_max;          // URDF upper limit (rad).
    int16_t actuator_min;    // Actuator input that corresponds to rad_min.
    int16_t actuator_max;    // Actuator input that corresponds to rad_max.
  };

  /**
   * @brief Constructor.
   * @param is_left_hand true=left hand (uses kLeft), false=right hand (uses kRight).
   */
  explicit OmniHand3UltraMSolver(bool is_left_hand);
  ~OmniHand3UltraMSolver() = default;

  static std::pair<int16_t, int16_t> GetMotorPositionRange() {
    return {kActuatorInputMin, kActuatorInputMax};
  }

  /**
   * @brief Active-joint position (rad) -> actuator input (0..4095).
   *        Input length must be 20.
   * @return Actuator inputs of length 20; empty vector if input length is wrong.
   * @note Inputs are clamped per joint to [rad_min, rad_max] and outputs to [0, 4095].
   */
  std::vector<int16_t> ActiveJointPos2ActuatorInput(const std::vector<double>& active_joint_pos) const;

  /**
   * @brief Actuator input (0..4095) -> active-joint position (rad).
   *        Input length must be 20.
   * @return Active-joint positions in rad, length 20; empty vector if input length is wrong.
   */
  std::vector<double> ActuatorInput2ActiveJointPos(const std::vector<int16_t>& actuator_input) const;

  /**
   * @brief Single-joint conversion by index (for joint-name / index access APIs).
   * @param joint_index Range [0, 20); out-of-range indices are clamped.
   */
  int16_t SingleActiveJointPos2ActuatorInput(std::size_t joint_index, double rad) const;
  double SingleActuatorInput2ActiveJointPos(std::size_t joint_index, int16_t actuator_input) const;

  /// Direct access to the calibration table currently in use (length = kActiveJointCount).
  const JointMotorCalib* CalibTable() const { return table_; }
  bool IsLeftHand() const { return is_left_hand_; }


  std::vector<int16_t> SetHandGesture(H3UMGesture gesture) const;

 private:
  bool is_left_hand_;
  const JointMotorCalib* table_;  // Points to either kLeft or kRight.

  static const JointMotorCalib kRight[kActiveJointCount];
  static const JointMotorCalib kLeft[kActiveJointCount];
};

}  // namespace h3um
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_ULTRA_M_SOLVER_H
