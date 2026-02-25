// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_base.h
 * @brief Internal base class for OmniHand 2025 (O10) and OmniHand Pro 2025 (O12) common interfaces
 * @note This is an internal class, not exported. Users should use OmniHand2025 or OmniHandPro2025.
 */

#ifndef AGILINK_OMNIHAND_BASE_H
#define AGILINK_OMNIHAND_BASE_H

#include <cstdint>
#include <vector>
#include "omnihand/omnihand.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

// Forward declarations
struct MixCtrl;
struct JointMotorErrorReport;

/**
 * @brief Internal base class for O10 and O12 common interfaces
 * @note This class is not exported. It provides common methods shared by OmniHand2025 and OmniHandPro2025.
 *       Uses virtual inheritance to avoid diamond inheritance problem.
 */
class AGIBOT_EXPORT OmniHandBase : public virtual OmniHand {
 public:
  // ============ Joint Motor Position Control ============
  /**
   * @brief Sets the position of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param posi Motor position. Range:
   *             - OmniHand 2025 (O10): 0-4096
   *             - OmniHand Pro 2025 (O12): 0-2000
   */
  virtual void SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi) = 0;
  
  /**
   * @brief Gets the position of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current position value. Range:
   *         - OmniHand 2025 (O10): 0-4096
   *         - OmniHand Pro 2025 (O12): 0-2000
   */
  virtual int16_t GetJointMotorPosi(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Sets positions of all joint motors in batch and returns the actual positions.
   * @param vec_posi Target position vector. Length and range depend on product type:
   *                 - OmniHand 2025 (O10): 10 values, each in range 0-4096
   *                 - OmniHand Pro 2025 (O12): 12 values, each in range 0-2000
   * @return Actual position vector from device response. Empty vector on failure.
   */
  virtual std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi) = 0;
  
  /**
   * @brief Gets positions of all joint motors in batch.
   * @return Current position vector. Length and range depend on product type:
   *         - OmniHand 2025 (O10): 10 values, each in range 0-4096
   *         - OmniHand Pro 2025 (O12): 12 values, each in range 0-2000
   */
  virtual std::vector<int16_t> GetAllJointMotorPosi() const = 0;

  // ============ Joint Angle Control ============
  // Note: SetActiveJointAngle and GetActiveJointAngle are not defined in base class.
  // They are conditionally defined in implementation classes via #if !DISABLE_FUNC.
  
  /**
   * @brief Sets joint angles of all active joints in batch.
   * @param angles Joint angle vector (unit: radians). Length depends on product type:
   *               - OmniHand 2025 (O10): 10 values
   *               - OmniHand Pro 2025 (O12): 12 values
   */
  virtual void SetAllActiveJointAngles(const std::vector<double>& angles) = 0;
  
  /**
   * @brief Gets joint angles of all active joints in batch.
   * @return Joint angle vector (unit: radians). Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<double> GetAllActiveJointAngles() const = 0;
  
  /**
   * @brief Gets joint angles of all joints (including active and passive joints).
   * @return All joint angle vector (unit: radians)
   */
  virtual std::vector<double> GetAllJointAngles() const = 0;
  
  /**
   * @brief Calculates all joint angles (including active and passive joints) from active joint angles.
   * @param active_joint_pos Active joint angle vector (unit: radians). Length depends on product type:
   *                         - OmniHand 2025 (O10): 10 values
   *                         - OmniHand Pro 2025 (O12): 12 values
   * @return All joint angle vector (unit: radians), including active and passive joints
   * @note This function does not perform hardware communication, only kinematics calculation.
   */
  virtual std::vector<double> GetAllJointPos(const std::vector<double>& active_joint_pos) const = 0;

  // ============ Velocity Control ============
  /**
   * @brief Sets the velocity of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param velo Target velocity value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo) = 0;
  
  /**
   * @brief Gets the velocity of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current velocity value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual int16_t GetJointMotorVelo(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Sets velocities of all joint motors in batch.
   * @param vec_velo Target velocity vector. Length depends on product type:
   *                 - OmniHand 2025 (O10): 10 values
   *                 - OmniHand Pro 2025 (O12): 12 values
   */
  virtual void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo) = 0;
  
  /**
   * @brief Gets velocities of all joint motors in batch.
   * @return Current velocity vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<int16_t> GetAllJointMotorVelo() const = 0;

  // ============ Torque Control ============
  // Note: SetJointMotorTorque, GetJointMotorTorque, SetAllJointMotorTorque, GetAllJointMotorTorque
  // are not defined in base class. They are conditionally defined in implementation classes via #if !DISABLE_FUNC.

  // ============ Control Mode ============
  /**
   * @brief Sets the control mode of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param mode Control mode enum value
   * @note SERVO mode (1) is available for servo control
   * @note Pure torque control (TORQUE) is not supported: Both O10 and O12 do not support pure torque mode, only mixed control modes
   */
  virtual void SetControlMode(unsigned char joint_motor_index, ControlMode mode) = 0;
  
  /**
   * @brief Gets the control mode of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current control mode
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual ControlMode GetControlMode(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Sets control modes of all joint motors in batch.
   * @param ctrl_modes Control mode vector. Length depends on product type:
   *                   - OmniHand 2025 (O10): 10 values
   *                   - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetAllControlMode(const std::vector<unsigned char>& ctrl_modes) = 0;
  
  /**
   * @brief Gets control modes of all joint motors in batch.
   * @return Control mode vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual std::vector<unsigned char> GetAllControlMode() const = 0;

  // ============ Current Threshold ============
  /**
   * @brief Sets the current threshold of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param current_threshold Current threshold value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold) = 0;
  
  /**
   * @brief Gets the current threshold of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current threshold value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual int16_t GetCurrentThreshold(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Sets current thresholds of all joint motors in batch.
   * @param current_thresholds Current threshold vector. Length depends on product type:
   *                           - OmniHand 2025 (O10): 10 values
   *                           - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds) = 0;
  
  /**
   * @brief Gets current thresholds of all joint motors in batch.
   * @return Current threshold vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual std::vector<int16_t> GetAllCurrentThreshold() const = 0;

  // ============ Mixed Control ============
  /**
   * @brief Mixed control for joint motors.
   * @param mix_ctrls Mixed control parameter vector
   * @note Pure torque control (TORQUE) is not supported: Both O10 and O12 do not support pure torque mode, only mixed control modes
   */
  virtual void MixCtrlJointMotor(const std::vector<MixCtrl>& mix_ctrls) = 0;

  // ============ Error Report ============
  /**
   * @brief Gets error report of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Error report structure
   */
  virtual JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Gets error reports of all joint motors.
   * @return Error report vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<JointMotorErrorReport> GetAllErrorReport() const = 0;
  
  /**
   * @brief Sets error report period of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param period Report period (unit: milliseconds)
   */
  virtual void SetErrorReportPeriod(unsigned char joint_motor_index, uint16_t period) = 0;
  
  /**
   * @brief Sets error report periods of all joint motors in batch.
   * @param vec_period Report period vector. Length depends on product type:
   *                   - OmniHand 2025 (O10): 10 values
   *                   - OmniHand Pro 2025 (O12): 12 values
   */
  virtual void SetAllErrorReportPeriod(std::vector<uint16_t> vec_period) = 0;

  // ============ Temperature Report ============
  /**
   * @brief Gets temperature report of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current temperature value (unit: Celsius)
   */
  virtual uint16_t GetTemperatureReport(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Gets temperature reports of all joint motors in batch.
   * @return Temperature value vector (unit: Celsius). Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<uint16_t> GetAllTemperatureReport() const = 0;
  
  /**
   * @brief Sets temperature report period of a single joint motor (O12 only).
   * @param joint_motor_index Joint motor index (1-12)
   * @param period Report period (unit: milliseconds)
   * @note OmniHand 2025 (O10) does not support this interface.
   */
  virtual void SetTemperReportPeriod(unsigned char joint_motor_index, uint16_t period) = 0;
  
  /**
   * @brief Sets temperature report periods of all joint motors in batch (O12 only).
   * @param vec_period Report period vector, length 12
   * @note OmniHand 2025 (O10) does not support this interface.
   */
  virtual void SetAllTemperReportPeriod(std::vector<uint16_t> vec_period) = 0;

  // ============ Current Report ============
  /**
   * @brief Gets current report of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current value
   */
  virtual int16_t GetCurrentReport(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief Gets current reports of all joint motors in batch.
   * @return Current value vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<uint16_t> GetAllCurrentReport() const = 0;
  
  /**
   * @brief Sets current report period of a single joint motor (O12 only).
   * @param joint_motor_index Joint motor index (1-12)
   * @param period Report period (unit: milliseconds)
   * @note OmniHand 2025 (O10) does not support this interface.
   */
  virtual void SetCurrentReportPeriod(unsigned char joint_motor_index, uint16_t period) = 0;
  
  /**
   * @brief Sets current report periods of all joint motors in batch (O12 only).
   * @param vec_period Report period vector, length 12
   * @note OmniHand 2025 (O10) does not support this interface.
   */
  virtual void SetAllCurrentReportPeriod(std::vector<uint16_t> vec_period) = 0;

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number (implementation-specific, see derived classes for details)
   * @note Default implementation is provided in OmniHand2025 and OmniHandPro2025
   */
  virtual void SetHandGesture(int gesture_num = 1) {}

 protected:
  /**
   * @brief Constructor - protected to prevent direct instantiation
   * @note Users should use product-specific classes: OmniHand2025 or OmniHandPro2025
   */
  OmniHandBase() = default;

  /**
   * @brief Destructor - protected to prevent direct instantiation
   * @note Users should use product-specific classes: OmniHand2025 or OmniHandPro2025
   */
  virtual ~OmniHandBase() = default;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_BASE_H
