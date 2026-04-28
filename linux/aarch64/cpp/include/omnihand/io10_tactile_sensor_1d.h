// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_IO10_TACTILE_SENSOR_1D_H
#define AGILINK_OMNIHAND_IO10_TACTILE_SENSOR_1D_H

#include <cstddef>
#include <map>
#include <vector>
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

static const std::map<Finger, size_t> kSensorDataLengths = {
  {Finger::THUMB, 16},
  {Finger::INDEX, 18},
  {Finger::MIDDLE, 18},
  {Finger::RING, 18},
  {Finger::LITTLE, 18},
  {Finger::PALM, 78},
  {Finger::DORSUM, 102},
};

struct AGIBOT_EXPORT TactileSensorData {
  Finger sensor_id_;          // Sensor ID (finger/palm/dorsum)
  std::vector<uint8_t> data_; // Data unit: 1g, Max value: 255g, Sampling frequency: 10Hz
};

/**
 * @brief Interface for 1D tactile sensor data based on OmniHand2025(O10)
 */
class AGIBOT_EXPORT IO10TactileSensor1D {
 public:
  /**
   * @brief Get sensor data length for a specific finger
   * @param finger Finger enum value
   * @return Sensor data length in bytes
   */
  virtual size_t GetSensorDataLength(Finger finger) const = 0;
  /**
   * @brief Get sensor order vector
   * @return Reference to sensor order vector
   */
  virtual const std::vector<Finger>& GetSensorOrder() const = 0;

  /**
   * @brief Gets 1D tactile sensor raw data for a single sensor.
   * @param finger Finger/palm enum value
   * @return TactileSensorData structure containing full resolution data
   */
  virtual TactileSensorData GetTactileSensorDataRaw(Finger finger) const = 0;

  /**
   * @brief Gets all 1D tactile sensor raw data from all sensors at once.
   * @return Vector of TactileSensorData structures
   */
  virtual std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const = 0;

 protected:
  /**
   * @note Protected to prevent direct instantiation
   */
  IO10TactileSensor1D() = default;

  /**
   * @note Protected to prevent direct instantiation
   */ 
  virtual ~IO10TactileSensor1D() = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_IO10_TACTILE_SENSOR_1D_H
