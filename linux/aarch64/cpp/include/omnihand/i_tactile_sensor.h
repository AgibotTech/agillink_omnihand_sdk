// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_I_TACTILE_SENSOR_H
#define AGILINK_OMNIHAND_I_TACTILE_SENSOR_H

#include <cstddef>
#include <string>
#include <vector>

#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

/**
 * @brief Common interface for tactile sensor metadata.
 *
 * Tactile data access is intentionally defined by the derived interfaces,
 * because different products use different raw data structures.
 */
class AGIBOT_EXPORT ITactileSensor {
 public:
  virtual size_t GetNumOfTactileSensors() const = 0;

  virtual size_t GetNumOfTactilePoints(Finger finger) const = 0;

  virtual bool InitTactilePointsMap() = 0;

  virtual size_t GetLenOfTactileDatum(Finger finger) const = 0;

  virtual size_t GetNumOfRepliedTactileFrames(Finger finger) const = 0;

  virtual std::string GetSNOfTactileSensor(Finger finger) const = 0;

  /**
   * @deprecated Use GetNumOfTactilePoints() instead.
   * @brief Get the number of tactile points for a specific sensor.
   * @param finger Finger enum value
   * @return Number of tactile points
   */
  virtual size_t GetSensorDataLength(Finger finger) const = 0;

  /**
   * @brief Get sensor order vector.
   * @return Reference to sensor order vector
   */
  virtual const std::vector<Finger>& GetSensorOrder() const = 0;

 protected:
  ITactileSensor() = default;
  virtual ~ITactileSensor() = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_I_TACTILE_SENSOR_H
