// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_3_lite_solver.h
 * @brief Simple hand-gesture solver for OmniHand 3 Lite (H3L, 4-DOF).
 *
 * Motors 1 and 4 are reversed between left and right hands.
 * Preset gesture positions are defined for the right hand; the solver
 * mirrors motors 0 and 3 when is_left_hand_ == true.
 */

#ifndef AGILINK_OMNIHAND_3_LITE_SOLVER_H
#define AGILINK_OMNIHAND_3_LITE_SOLVER_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "omnihand/export_symbols.h"
#include "omnihand/utils.h"

namespace agilink {
namespace omnihand {

enum class H3LActivateJoint {
  H3L_ACTIVATE_JOINT_THUMB_ROLL = 0,
  H3L_ACTIVATE_JOINT_THUMB_,
  H3L_ACTIVATE_JOINT_FORFINGER,
  H3L_ACTIVATE_OTHER,
  H3L_ACTIVATE_JOINT_COUNT
};
enum class OmniHand3LiteGesture : int {
  OMNI_HAND_3_LITE_GESTURE_ALL_ZERO = 0,
  OMNI_HAND_3_LITE_GESTURE_FIST,
  OMNI_HAND_3_LITE_GESTURE_OPEN,
};

class AGIBOT_EXPORT OmniHand3LiteSolver {
 public:
  static constexpr int16_t kActuatorInputMax = 4096;
  static constexpr int16_t kActuatorInputMin = 0;
  static constexpr uint8_t kDegreesOfActiveFreedom = 4;
  static constexpr std::size_t kActiveJointCount = static_cast<std::size_t>(H3LActivateJoint::H3L_ACTIVATE_JOINT_COUNT);
  struct JointMotorCalib {
    double rad_min;        // URDF lower limit (rad).
    double rad_max;        // URDF upper limit (rad).
    int16_t actuator_min;  // Actuator input that corresponds to rad_min.
    int16_t actuator_max;  // Actuator input that corresponds to rad_max.
  };
  explicit OmniHand3LiteSolver(bool is_left_hand);
  ~OmniHand3LiteSolver() = default;
  std::vector<int> SetHandGesture(OmniHand3LiteGesture gesture) const;
  static constexpr Int16Bound GetMotorPositionRange() {
    return {kActuatorInputMin, kActuatorInputMax};
  }
  double SingleActuatorInput2ActiveJointPos(std::size_t joint_index, int16_t actuator_input) const;
  int16_t SingleActiveJointPos2ActuatorInput(std::size_t joint_index, double rad) const;
  std::vector<double> ActuatorInput2ActiveJointPos(const std::vector<int16_t>& actuator_input) const;
  std::vector<int16_t> ActiveJointPos2ActuatorInput(const std::vector<double>& active_joint_pos) const;

 private:
  bool is_left_hand_;
  const JointMotorCalib* table_;  // Points to either kLeft or kRight.
  static const JointMotorCalib kRight[kActiveJointCount];
  static const JointMotorCalib kLeft[kActiveJointCount];
};
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_LITE_SOLVER_H