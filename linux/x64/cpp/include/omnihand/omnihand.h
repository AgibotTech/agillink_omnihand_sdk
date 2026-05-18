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
   * @brief Gets vendor information.
   * @return Vendor information structure containing product model, serial number, hardware version, software version, supply voltage, DOF, etc.
   */
  virtual VendorInfo GetVendorInfo() const = 0;
  
  /**
   * @brief Gets device information.
   * @return Device information structure containing device ID and communication parameters
   * @note Serial port communication (RS485) does not support this interface. 
   *       RS485 implementation returns an empty DeviceInfo structure.
   */
  virtual DeviceInfo GetDeviceInfo() const = 0;

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

  ProductType product_type_{ProductType::UNKNOWN};
  unsigned char device_id_{DEFAULT_DEVICE_ID};
  bool is_left_hand_{true};
  bool is_init_{false};
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_H
