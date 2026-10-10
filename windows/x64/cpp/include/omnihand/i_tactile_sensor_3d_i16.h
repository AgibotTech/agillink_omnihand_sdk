// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_I_TACTILE_SENSOR_3D_I16_H
#define AGILINK_OMNIHAND_I_TACTILE_SENSOR_3D_I16_H

#include <cstdint>
#include <map>
#include <vector>

#include "omnihand/i_tactile_sensor.h"

namespace agilink {
namespace omnihand {

inline const std::map<Finger, size_t> kH3UMTactilePointCounts = {
    {Finger::ALL_FINGERS, 102},
    {Finger::THUMB, 15},
    {Finger::INDEX, 15},
    {Finger::MIDDLE, 15},
    {Finger::RING, 15},
    {Finger::LITTLE, 15},
    {Finger::PALM, 27},
};

struct AGIBOT_EXPORT TactilePoint3DI16 {
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
};

struct AGIBOT_EXPORT TactileSensorData3DI16 {
  Finger sensor_id = Finger::UNKNOWN;
  std::vector<TactilePoint3DI16> points;
};

struct AGIBOT_EXPORT TactileSensorResultantForce3DI16 {
  Finger sensor_id = Finger::UNKNOWN;
  TactilePoint3DI16 force;
};

/**
 * @brief Interface for 3D tactile sensor data with signed 16-bit axes.
 */
class AGIBOT_EXPORT ITactileSensor3DI16 : public ITactileSensor {
 public:
  virtual TactileSensorData3DI16 GetTactileSensorDataRaw(Finger sensor) const = 0;

  virtual std::vector<TactileSensorData3DI16> GetAllTactileSensorDataRaw() const = 0;

  virtual TactileSensorResultantForce3DI16 GetTactileSensorResultantForce(Finger sensor) const = 0;

  virtual std::vector<TactileSensorResultantForce3DI16> GetAllTactileSensorResultantForces() const = 0;

 protected:
  ITactileSensor3DI16() = default;
  ~ITactileSensor3DI16() override = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_I_TACTILE_SENSOR_3D_I16_H
