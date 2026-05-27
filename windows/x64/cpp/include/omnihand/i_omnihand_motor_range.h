// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_I_OMNIHAND_MOTOR_RANGE_H
#define AGILINK_OMNIHAND_I_OMNIHAND_MOTOR_RANGE_H

#include <cstdint>
#include <utility>
#include <vector>
#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {

/**
 * @brief Motor position range interface (nominal and calibrated limits).
 */
class AGIBOT_EXPORT IOmniHandMotorRange {
 public:
  virtual ~IOmniHandMotorRange() = default;

  /**
   * @brief Gets nominal min/max motor position ranges per joint.
   * @return Per-joint (min, max) in motor ticks.
   */
  virtual std::vector<std::pair<int16_t, int16_t>> GetAllMaxMinMotorPos() const = 0;

  /**
   * @brief Gets actual/usable min/max motor position ranges after calibration mapping.
   * @return Per-joint (min, max) in motor ticks.
   */
  virtual std::vector<std::pair<int16_t, int16_t>> GetAllMaxMinActualMotorPos() const = 0;

 protected:
  IOmniHandMotorRange() = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_I_OMNIHAND_MOTOR_RANGE_H
