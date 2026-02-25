// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_3_ultra.h
 * @brief OmniHand 3 Ultra (O20) interface class - 20 DOF
 * @note This is the public interface for OmniHand 3 Ultra product
 */

#ifndef AGILINK_OMNIHAND_3_ULTRA_H
#define AGILINK_OMNIHAND_3_ULTRA_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_base.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

class OmniHand3UltraCanImpl;

/**
 * @brief OmniHand 3 Ultra (O20) interface class - 20 DOF
 *
 * This class provides the public interface for OmniHand 3 Ultra product.
 * It supports 20 active degrees of freedom.
 *
 * TODO(O20): 运动学求解器未接入。SetAllActiveJointAngles / GetAllActiveJointAngles /
 * GetAllJointAngles / GetAllJointPos / SetHandGesture 当前为占位实现，请使用
 * SetAllJointMotorPosi / GetAllJointMotorPosi 等电机位置接口直接控制。
 */
class AGIBOT_EXPORT OmniHand3Ultra : public OmniHandBase {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 20;  // O20 has 20 active degrees of freedom (DoA)

  virtual ~OmniHand3Ultra() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByZlgcan(
      HandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByZlgcan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& usbcanfd_serial_number,
      unsigned char canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   */
  static std::unique_ptr<OmniHand3Ultra> createHandSocketCan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByHcan(
      HandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   */
  static std::unique_ptr<OmniHand3Ultra> createHandByHcan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& hcan_serial_number,
      unsigned char canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00)
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address by serial number
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      unsigned char canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address via SocketCAN
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface);
#endif

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number
   * TODO(O20): 待运动学接入后实现；当前为 no-op，请用 SetAllJointMotorPosi 控制
   */
  void SetHandGesture(int gesture_num = 1) override;

 protected:
  /**
   * @brief Initialize base class members
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note Product type is fixed to ProductType::OMNIHAND_3_ULTRA for this class
   */
  void Reset(unsigned char device_id, HandType hand_type);
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_3_ULTRA_H
