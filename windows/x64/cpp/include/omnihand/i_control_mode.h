// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNIHAND_I_CONTROL_MODE_H
#define AGILINK_OMNIHAND_I_CONTROL_MODE_H

#include <cstdint>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

/**
 * @brief 0x10: Control mode interface for OmniHand
 */
class AGIBOT_EXPORT IControlMode {
 public:
  /**
   * @brief Sets the control mode of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param mode Control mode enum value
   * @note SERVO mode (1) is available for servo control
   * @note Pure torque control (TORQUE) is not supported: Both O10 and O12 do not support pure torque mode, only mixed control modes
   */
  virtual void SetControlMode(unsigned char joint_motor_index, ControlMode mode) { (void)joint_motor_index; (void)mode; }
  
  /**
   * @brief Gets the control mode of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current control mode
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual ControlMode GetControlMode(unsigned char joint_motor_index) const { (void)joint_motor_index; return ControlMode::POSITION; }
  
  /**
   * @brief Sets control modes of all joint motors in batch.
   * @param ctrl_modes Control mode vector. Length depends on product type:
   *                   - OmniHand 2025 (O10): 10 values
   *                   - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetAllControlMode(const std::vector<unsigned char>& ctrl_modes) { (void)ctrl_modes; }
  
  /**
   * @brief Gets control modes of all joint motors in batch.
   * @return Control mode vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual std::vector<unsigned char> GetAllControlMode() const { return {}; }

 protected:
  /**
   * @brief Protected constructor to prevent direct instantiation.
   */
  IControlMode() = default;

  /**
   * @brief Protected destructor to prevent direct deletion.
   */
  virtual ~IControlMode() = default;
}; // class IControlMode

} // namespace omnihand
} // namespace agilink

#endif  // AGILINK_OMNIHAND_I_CONTROL_MODE_H
