// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNI_PICKER_3_H
#define AGILINK_OMNI_PICKER_3_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_base.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"

namespace agilink {
namespace omnihand {

class AGIBOT_EXPORT OmniPicker3 : public OmniHandBase {
 public:
  static constexpr unsigned char kDegreesOfActiveFreedom = 1;
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniPicker3() = default;

  // ============ Factory Methods ============
  static std::unique_ptr<OmniPicker3> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  static std::unique_ptr<OmniPicker3> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  static std::unique_ptr<OmniPicker3> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& tcp_host,
      uint16_t tcp_port,
      uint8_t canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

#ifdef __linux__
  static std::unique_ptr<OmniPicker3> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0");
#endif

  static std::unique_ptr<OmniPicker3> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  static std::unique_ptr<OmniPicker3> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  static std::unique_ptr<OmniPicker3> createHandByTJ(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& marvin_controller_ip);
#endif

  static DeviceInfo GetDeviceInfoFromBroadcast(
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif

  void SetHandGesture(int gesture_num = 1) override;

 protected:
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNI_PICKER_3, device_id, hand_type);
  }
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNI_PICKER_3_H
