// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_UTILS_H
#define AGILINK_OMNIHAND_UTILS_H

#include <functional>
#include <string>

namespace agilink {
namespace omnihand {

// Optional sink for ShowDataDetails traffic; when empty, SDK logs to stdout.
using DataDetailsLogCallback = std::function<void(const std::string& line)>;

struct UInt8Bound {
  uint8_t min_value;
  uint8_t max_value;
};

struct UInt16Bound {
  uint16_t min_value;
  uint16_t max_value;
};

struct Int8Bound {
  int8_t min_value;
  int8_t max_value;
};

struct Int16Bound {
  int16_t min_value;
  int16_t max_value;
};

struct FloatBound {
  float min_value;
  float max_value;
};

struct DoubleBound {
  double min_value;
  double max_value;
};

struct UInt8Range {
  uint8_t min_value;
  uint8_t max_value;
  uint8_t default_value;
};

struct UInt16Range {
  uint16_t min_value;
  uint16_t max_value;
  uint16_t default_value;
};

struct Int8Range {
  int8_t min_value;
  int8_t max_value;
  int8_t default_value;
};

struct Int16Range {
  int16_t min_value;
  int16_t max_value;
  int16_t default_value;
};

struct FloatRange {
  float min_value;
  float max_value;
  float default_value;
};

struct DoubleRange {
  double min_value;
  double max_value;
  double default_value;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_UTILS_H
