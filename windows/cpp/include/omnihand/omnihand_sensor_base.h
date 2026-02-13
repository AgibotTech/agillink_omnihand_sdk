// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_sensor_base.h
 * @brief Internal base class for OmniHand 2025 (O10) and OmniHand Dex UMI common sensor interfaces
 * @note This is an internal class, not exported. Users should use OmniHand2025 or OmniHandDexUMI.
 */

#pragma once

#include <cstddef>
#include <vector>
#include "omnihand/omnihand.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

/**
 * @brief Internal base class for O10 and UMI common sensor interfaces
 * @note This class is not exported. It provides common methods shared by OmniHand2025 and OmniHandDexUMI.
 *       Uses virtual inheritance to avoid diamond inheritance problem.
 */
class AGIBOT_EXPORT OmniHandSensorBase : public virtual OmniHand {
 public:
  // ============ Sensor Utilities ============
  /**
   * @brief Get sensor data length for a specific finger
   * @param eFinger Finger enum value
   * @return Sensor data length in bytes
   */
  static size_t GetSensorDataLength(Finger eFinger);

  /**
   * @brief Get sensor order vector
   * @return Reference to sensor order vector
   */
  static const std::vector<Finger>& GetSensorOrder();

  // ============ Common Tactile Sensor Interface (O10 and UMI) ============
  /**
   * @brief Gets all 1D tactile sensor raw data from all sensors at once.
   * @return Vector of TactileSensorData structures
   * @note This returns full resolution data, unlike GetTactileSensorData() which returns downsampled data.
   * @note Supported by OmniHand 2025 (O10) and OmniHand Dex UMI.
   */
  virtual std::vector<TactileSensorData> GetAllTactileSensorDataRaw() const = 0;

  /**
   * @brief Gets 1D tactile sensor raw data for a single sensor.
   * @param eFinger Finger/palm enum value
   * @return TactileSensorData structure containing full resolution data
   * @note Supported by OmniHand 2025 (O10) and OmniHand Dex UMI.
   */
  virtual TactileSensorData GetTactileSensorDataRaw(Finger eFinger) const = 0;

 protected:
  /**
   * @brief Constructor - protected to prevent direct instantiation
   * @note Users should use product-specific classes: OmniHand2025 or OmniHandDexUMI
   */
  OmniHandSensorBase() = default;

  /**
   * @brief Destructor - protected to prevent direct instantiation
   * @note Users should use product-specific classes: OmniHand2025 or OmniHandDexUMI
   */
  virtual ~OmniHandSensorBase() = default;
};

}  // namespace omnihand
}  // namespace agilink
