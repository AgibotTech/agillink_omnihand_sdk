// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_I_OMNIHAND_CALIBRATOR_H
#define AGILINK_I_OMNIHAND_CALIBRATOR_H

#include <cstdint>
#include <vector>
#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {

struct AGIBOT_EXPORT AxisLimitPos {
  std::vector<int16_t> min_limits;
  std::vector<int16_t> max_limits;

  bool empty() const {
    return min_limits.empty() || max_limits.empty() || min_limits.size() != max_limits.size();
  }

  static AxisLimitPos DecodeLittleEndian(const uint8_t* data, size_t byte_len) {
    AxisLimitPos out;
    if (!data || byte_len < 4 || (byte_len % 4) != 0) {
      return out;
    }
    const size_t half = byte_len / 2;
    const size_t n_axes = half / 2;
    out.min_limits.resize(n_axes);
    out.max_limits.resize(n_axes);
    for (size_t i = 0; i < n_axes; ++i) {
      out.min_limits[i] = static_cast<int16_t>(
          static_cast<uint16_t>(data[2 * i]) | (static_cast<uint16_t>(data[2 * i + 1]) << 8));
      out.max_limits[i] = static_cast<int16_t>(
          static_cast<uint16_t>(data[half + 2 * i]) |
          (static_cast<uint16_t>(data[half + 2 * i + 1]) << 8));
    }
    return out;
  }
};

class AGIBOT_EXPORT IOmniHandCalibrator {
 public:
  virtual ~IOmniHandCalibrator() = default;

  // Set single axis actual (calibration) position.
  // @param axis_index  1-based joint index
  // @param position    target position
  // @return current actual position after set
  virtual int16_t SetSingleActualAxisPos(uint8_t axis_index, int16_t position) = 0;

  // Get single axis actual (calibration) position.
  // @param axis_index  1-based joint index
  // @return current actual position
  virtual int16_t GetSingleActualAxisPos(uint8_t axis_index) const = 0;

  // Set all axes actual (calibration) positions.
  // @param positions  target positions; length = DOF
  // @return actual positions after set; empty on failure
  virtual std::vector<int16_t> SetAllActualAxisPos(const std::vector<int16_t>& positions) = 0;

  // Get all axes actual (calibration) positions.
  // @return actual positions; length = DOF; empty on failure
  virtual std::vector<int16_t> GetAllActualAxisPos() const = 0;

  // Set axis homing (zero reference) position.
  // @param axis_index  0=all axes, 1-N=single axis
  // @param pos         target zero position; ignored when axis_index==0
  // @return true on success
  virtual bool SetAxisHoming(uint8_t axis_index, int16_t pos) = 0;

  // Set single axis minimum position limit.
  // @param axis_index  1-based joint index
  // @param min_pos     minimum position
  // @return true on success
  virtual bool SetAxisMinPos(uint8_t axis_index, int16_t min_pos) = 0;

  // Set single axis maximum position limit.
  // @param axis_index  1-based joint index
  // @param max_pos     maximum position
  // @return true on success
  virtual bool SetAxisMaxPos(uint8_t axis_index, int16_t max_pos) = 0;

  // Get all axes position limits (min and max).
  // @return AxisLimitPos with min_limits and max_limits; empty() on failure
  virtual AxisLimitPos GetAxisLimitPos() const = 0;

  // Clear all axis position limits.
  // @return true on success
  virtual bool ClearAllLimitPos() = 0;

  // Save parameters to flash. Unsaved parameters are lost on power-off.
  // @return true on success
  virtual bool SaveParam() = 0;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_I_OMNIHAND_CALIBRATOR_H
