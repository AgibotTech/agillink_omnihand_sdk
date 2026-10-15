// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_I_TACTILE_SENSOR_1D_U16_H
#define AGILINK_OMNIHAND_I_TACTILE_SENSOR_1D_U16_H

#include <cstddef>
#include <vector>
#include "omnihand/i_tactile_sensor.h"

namespace agilink {
namespace omnihand {

struct AGIBOT_EXPORT TactileSensorDataU16 {
  Finger sensor_id_;
  std::vector<uint16_t> data_;
};

class AGIBOT_EXPORT ITactileSensor1DU16 : public ITactileSensor {
 public:
  virtual TactileSensorDataU16 GetTactileSensorData(Finger finger) const = 0;
  virtual TactileSensorDataU16 GetTactileSensorDataRaw(Finger finger) const = 0;
  virtual std::vector<TactileSensorDataU16> GetAllTactileSensorDataRaw() const = 0;

 protected:
  ITactileSensor1DU16() = default;
  ~ITactileSensor1DU16() override = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_I_TACTILE_SENSOR_1D_U16_H
