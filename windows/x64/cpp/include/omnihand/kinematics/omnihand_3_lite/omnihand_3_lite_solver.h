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

#include <cstdint>
#include <vector>

#include "omnihand/export_symbols.h"
#include "omnihand/utils.h"

namespace agilink {
namespace omnihand {

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

  explicit OmniHand3LiteSolver(bool is_left_hand);

  std::vector<int> SetHandGesture(OmniHand3LiteGesture gesture) const;

  static Int16Bound GetMotorPositionRange() {
    return {kActuatorInputMin, kActuatorInputMax};
  }

 private:
  bool is_left_hand_;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_LITE_SOLVER_H