// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand.h
 * @brief OmniHand base class - common interface for all products
 * @note Users should use OmniHand2025, OmniHandPro2025, or OmniHandDexUMI instead
 */

#ifndef AGILINK_OMNIHAND_H
#define AGILINK_OMNIHAND_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"

namespace agilink {
namespace omnihand {

// Forward declarations
struct MixCtrl;
struct JointMotorErrorReport;

#define DEFAULT_DEVICE_ID 0x01

/**
 * @brief OmniHand base class - common interface for all products
 * @note This is the base class containing only methods common to all products.
 *       Users should use product-specific classes: OmniHand2025, OmniHandPro2025, or OmniHandDexUMI.
 *       However, this class must be exported (AGIBOT_EXPORT) to ensure methods like GetProductType()
 *       and Init() can be called through derived class pointers.
 */
class AGIBOT_EXPORT OmniHand {
 public:
  /**
   * @brief Gets product type.
   * @return Product type enum value， see @ref ProductType
   */
  ProductType GetProductType() const {
    return product_type_;
  }

  /**
   * @brief Checks if the hand is initialized.
   * @return true if initialized successfully, false otherwise
   */
  bool Init() const {
    return is_init_;
  }

  // ============ Basic Information Interface ============
  /**
   * @brief 0x01: Gets vendor information.
   * @return Vendor information structure containing product model, serial number, hardware version, software version, supply voltage, DOF, etc.
   */
  virtual VendorInfo GetVendorInfo() const = 0;
  
  /**
   * @brief 0x02: Gets device information.
   * @return Device information structure containing device ID and communication parameters
   * @note Serial port communication (RS485) does not support this interface. 
   *       RS485 implementation returns an empty DeviceInfo structure.
   */
  virtual DeviceInfo GetDeviceInfo() const = 0;

  // ============ Current Threshold ============
  /**
   * @brief 0x03: Sets the current threshold of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param current_threshold Current threshold value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetCurrentThreshold(unsigned char joint_motor_index, int16_t current_threshold) { (void)joint_motor_index; (void)current_threshold; }
  
  /**
   * @brief 0x03: Gets the current threshold of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current threshold value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual int16_t GetCurrentThreshold(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  
  /**
   * @brief 0x03: Sets current thresholds of all joint motors in batch.
   * @param current_thresholds Current threshold vector. Length depends on product type:
   *                           - OmniHand 2025 (O10): 10 values
   *                           - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetAllCurrentThreshold(const std::vector<int16_t>& current_thresholds) { (void)current_thresholds; }
  
  /**
   * @brief 0x03: Gets current thresholds of all joint motors in batch.
   * @return Current threshold vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual std::vector<int16_t> GetAllCurrentThreshold() const { return {}; }

  // ============ Joint Motor Position Control ============
  /**
   * @brief 0x13: Sets the position of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param posi Motor position. Range:
   *             - OmniHand 2025 (O10): 0-4096
   *             - OmniHand Pro 2025 (O12): 0-2000
   * @return Actual position from device response. -1 on failure or if protocol has no reply (e.g. USB no-reply).
   */
  virtual int16_t SetJointMotorPosi(unsigned char joint_motor_index, int16_t posi) { (void)joint_motor_index; (void)posi; return -1; }
  
  /**
   * @brief 0x13: Gets the position of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current position value. Range:
   *         - OmniHand 2025 (O10): 0-4096
   *         - OmniHand Pro 2025 (O12): 0-2000
   */
  virtual int16_t GetJointMotorPosi(unsigned char joint_motor_index) const = 0;
  
  /**
   * @brief 0x13: Sets positions of all joint motors in batch and returns the actual positions.
   * @param vec_posi Target position vector. Length and range depend on product type:
   *                 - OmniHand 2025 (O10): 10 values, each in range 0-4096
   *                 - OmniHand Pro 2025 (O12): 12 values, each in range 0-2000
   * @return Actual position vector from device response. Empty vector on failure.
   */
  virtual std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>& vec_posi) { (void)vec_posi; return {}; }
  
  /**
   * @brief 0x13: Gets positions of all joint motors in batch.
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
   * @return Actual joint angles from device (e.g. from SetAllJointMotorPosi response converted to angles). Empty on failure.
   */
  virtual std::vector<double> SetAllActiveJointAngles(const std::vector<double>& angles) { (void)angles; return {}; }
  
  /**
   * @brief Gets joint angles of all active joints in batch.
   * @return Joint angle vector (unit: radians). Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<double> GetAllActiveJointAngles() const { return {}; }
  
  /**
   * @brief Gets joint angles of all joints (including active and passive joints).
   * @return All joint angle vector (unit: radians)
   */
  virtual std::vector<double> GetAllJointAngles() const { return {}; }
  
  /**
   * @brief Calculates all joint angles (including active and passive joints) from active joint angles.
   * @param active_joint_angles Active joint angle vector (unit: radians). Length depends on product type:
   *                         - OmniHand 2025 (O10): 10 values
   *                         - OmniHand Pro 2025 (O12): 12 values
   * @return All joint angle vector (unit: radians), including active and passive joints
   * @note This function does not perform hardware communication, only kinematics calculation.
   */
  virtual std::vector<double> GetAllJointAngles(const std::vector<double>& active_joint_angles) const { (void)active_joint_angles; return {}; }

  // ============ Velocity Control ============
  /**
   * @brief 0x12: Sets the velocity of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @param velo Target velocity value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual void SetJointMotorVelo(unsigned char joint_motor_index, int16_t velo) { (void)joint_motor_index; (void)velo; }
  
  /**
   * @brief 0x12: Gets the velocity of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current velocity value
   * @note Serial port communication (RS485) does not support this interface.
   */
  virtual int16_t GetJointMotorVelo(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  
  /**
   * @brief 0x12: Sets velocities of all joint motors in batch.
   * @param vec_velo Target velocity vector. Length depends on product type:
   *                 - OmniHand 2025 (O10): 10 values
   *                 - OmniHand Pro 2025 (O12): 12 values
   */
  virtual void SetAllJointMotorVelo(const std::vector<int16_t>& vec_velo) { (void)vec_velo; }
  
  /**
   * @brief 0x12: Gets velocities of all joint motors in batch.
   * @return Current velocity vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<int16_t> GetAllJointMotorVelo() const { return {}; }

  // ============ Torque Control ============
  // Note: SetJointMotorTorque, GetJointMotorTorque, SetAllJointMotorTorque, GetAllJointMotorTorque
  // are not defined in base class. They are conditionally defined in implementation classes via #if !DISABLE_FUNC.

  // ============ Control Mode ============
  // Note: SetControlMode, GetControlMode, SetAllControlMode, GetAllControlMode
  // are not defined in base class. They are conditionally defined via interface IControlMode.
  
  // ============ Mixed Control ============
  /**
   * @brief 0x14: Mixed control for joint motors.
   * @param mix_ctrls Mixed control parameter vector.
   */
  virtual void MixCtrlJointMotor(const std::vector<MixCtrl>& mix_ctrls) { (void)mix_ctrls; }

  // ============ Error Report ============
  /**
   * @brief 0x20: Gets error report of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Error report structure
   */
  virtual JointMotorErrorReport GetErrorReport(unsigned char joint_motor_index) const { (void)joint_motor_index; return {}; }
  
  /**
   * @brief 0x20: Gets error reports of all joint motors.
   * @return Error report vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<JointMotorErrorReport> GetAllErrorReport() const { return {}; }
  
  // ============ Temperature Report ============
  /**
   * @brief 0x21: Gets temperature report of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current temperature value (unit: Celsius)
   */
  virtual int16_t GetTemperatureReport(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  
  /**
   * @brief 0x21: Gets temperature reports of all joint motors in batch.
   * @return Temperature value vector (unit: Celsius). Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<int16_t> GetAllTemperatureReport() const { return {}; }
  
  // ============ Current Report ============
  /**
   * @brief 0x22: Gets current report of a single joint motor.
   * @param joint_motor_index Joint motor index (O10: 1-10, O12: 1-12)
   * @return Current value
   */
  virtual int16_t GetCurrentReport(unsigned char joint_motor_index) const { (void)joint_motor_index; return 0; }
  
  /**
   * @brief 0x22: Gets current reports of all joint motors in batch.
   * @return Current value vector. Length depends on product type:
   *         - OmniHand 2025 (O10): 10 values
   *         - OmniHand Pro 2025 (O12): 12 values
   */
  virtual std::vector<int16_t> GetAllCurrentReport() const { return {}; }
  
  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number (implementation-specific, see derived classes for details)
   * @note Default implementation is provided in OmniHand2025 and OmniHandPro2025
   */
  virtual void SetHandGesture(int gesture_num = 1) {
    (void)gesture_num;  // Suppress unused parameter warning
  }

  /**
   * @brief Returns the motor position array corresponding to a gesture, without sending to device.
   * @param gesture_num Gesture number (implementation-specific)
   * @return Motor position vector (0-4095 range). Empty if gesture_num is invalid or unsupported.
   */
  virtual std::vector<int16_t> GetHandGesture(int gesture_num) {
    (void)gesture_num;
    return {};
  }

  // ============ Joint Naming Interface ============
  /**
   * @brief Gets the ordered joint names for this hand product.
   * @return Vector of joint names, whose length and order must match the product's
   *         motor index ordering used by SetAllJointMotorPosi / GetAllJointMotorPosi.
   *         Default returns an empty vector, meaning the product has not declared
   *         a ROS-friendly joint naming (e.g. kinematics / URDF not yet integrated).
   * @note Consumers (e.g. ROS2 node) can treat an empty result as "not provided" and
   *       fall back to their own naming scheme if needed.
   */
  virtual std::vector<std::string> GetJointNames() const { return {}; }

  // ============ Debug Interface ============
  /**
   * @brief Shows send/receive data details.
   * @param show Whether to show data details (true=show, false=hide)
   */
  virtual void ShowDataDetails(bool show) const = 0;

  // ============ Request Interval Control ============
  /**
   * @brief Set request interval to control CAN bus communication rate
   * @param milliseconds Minimum interval between requests in milliseconds (range: 0-100ms, default: 0ms = no throttling)
   * @note 0 = no limit (no throttling, matches CanBusDeviceBase / SerialDevice default),
   *       1-100ms = throttled requests
   * @note This helps prevent CAN bus congestion and device timeout issues.
   *       Higher interval = more stable but slower response.
   *       Lower interval = faster but may cause timeouts if device is busy.
   * @note Only applies to CAN communication. RS485 communication is not affected.
   * @note Serial port communication (RS485) does not support this interface.
   *       RS485 implementation does nothing (no-op).
   */
  virtual void SetRequestInterval(int milliseconds) = 0;

  /**
   * @brief Get current request interval setting
   * @return Request interval in milliseconds (0 = no limit)
   * @note Serial port communication (RS485) always returns 0 (not applicable).
   */
  virtual int GetRequestInterval() const = 0;

  // ============ Frame Reception Timeout Control ============
  /**
   * @brief Set frame reception timeout
   * @param milliseconds Timeout for receiving a single frame in milliseconds (range: 10-1000ms, default: 50ms)
   * @note This timeout applies to both single-frame and multi-frame reception
   * @note For multi-frame reception, this timeout applies to each frame individually
   * @note Only applies to CAN communication. RS485 communication is not affected.
   * @note Serial port communication (RS485) does not support this interface.
   *       RS485 implementation does nothing (no-op).
   */
  virtual void SetFrameRecvTimeout(int milliseconds) = 0;

  /**
   * @brief Get current frame reception timeout setting
   * @return Frame reception timeout in milliseconds
   * @note Serial port communication (RS485) always returns 0 (not applicable).
   */
  virtual int GetFrameRecvTimeout() const = 0;

  // ============ Frame Send Timeout Control (CAN only) ============
  /**
   * @brief Set frame send timeout (ZLG device tx_timeout)
   * @param milliseconds Timeout for send in milliseconds (0 = driver default, typically no limit; 1-10000ms recommended, e.g. 100ms)
   * @note Only applies to ZLG CAN FD. Reduces send blocking and request timeout when buffer is busy. RS485/USB do nothing.
   */
  virtual void SetFrameSendTimeout(int milliseconds) = 0;

  /**
   * @brief Get current frame send timeout setting
   * @return Frame send timeout in milliseconds (0 = not set / not applicable)
   */
  virtual int GetFrameSendTimeout() const = 0;

  // ============ Firmware Update (OTA) ============
  /**
   * @brief Updates firmware via OTA (Over-The-Air) upgrade
   * @param file_name Path to the firmware binary file
   * @param progress_callback Optional callback for progress (current_packet, total_packets, status)
   * @note Default implementation reports AGILINK_OTA_NOT_SUPPORTED. Products that support OTA override this.
   */
  virtual void UpdateFirmware(const std::string& /* file_name */, OtaProgressCallback progress_callback = nullptr) {
    if (progress_callback) {
      progress_callback(static_cast<int>(OtaErrorCode::AGILINK_OTA_NOT_SUPPORTED), 0, OtaProgressStatus::AGILINK_OTA_ERROR);
    }
  }

  /**
   * @brief Exits (aborts) an ongoing OTA upgrade process
   * @param code Abnormal-exit code sent to device (default 0)
   * @return true if the device acknowledged the exit, false on timeout or unsupported
   * @note Default implementation returns false (not supported). Products that support OTA override this.
   */
  virtual bool ExitOtaUpgrade(uint32_t /* code */ = 0) { return false; }

 protected:
  /**
   * @brief Constructor - protected to prevent direct instantiation
   * @note Users should use product-specific classes: OmniHand2025, OmniHandPro2025, or OmniHandDexUMI
   */
  OmniHand() = default;

 public:
  /**
   * @brief Sets device ID.
   * @param device_id Device ID
   * @note Serial port communication (RS485) does not support this interface.
   *       RS485 implementation does nothing (device ID is fixed at construction time).
   * @warning Changing device ID without proper documentation may result in device inaccessibility.
   */
  virtual void SetDeviceId(unsigned char device_id) = 0;
  /**
   * @brief Destructor - public for pybind11 compatibility
   * @note Users should NOT instantiate this class directly - use product-specific classes instead.
   *       Constructor is protected to prevent direct instantiation.
   */
  virtual ~OmniHand() = default;

  /**
   * @brief Initialize base class members
   * @param product_type Product type: ProductType::OMNIHAND_2025 (O10) or ProductType::OMNIHAND_PRO_2025 (O12)
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   */
  void Reset(ProductType product_type, unsigned char device_id, HandType hand_type) {
    product_type_ = product_type;
    device_id_ = device_id;
    is_left_hand_ = (hand_type == HandType::LEFT);
  }
  
  inline uint8_t StdMilliampToRegister(int16_t ma) {
    int val = static_cast<int>(ma) * 255 / 1000;
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    return static_cast<uint8_t>(val);
  }

  ProductType product_type_{ProductType::UNKNOWN};
  unsigned char device_id_{DEFAULT_DEVICE_ID};
  bool is_left_hand_{true};
  bool is_init_{false};
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_H
