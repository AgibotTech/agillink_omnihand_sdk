// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#ifndef AGILINK_OMNI_PICKER_2025_H
#define AGILINK_OMNI_PICKER_2025_H
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/i_o10_tactile_sensor_1d.h"
#include "omnihand/omnihand.h"
#include "omnihand/ota_types.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

#pragma pack(push, 1)

/**
 * @brief Raw OP1 control payload sent in one CAN/CANFD request.
 *
 * OP1 uses a compact 5-byte command payload. Each field is encoded as one
 * unsigned command byte and passed through to the gripper firmware.
 */
struct Op1ControlFrame {
  uint8_t pos_cmd;    ///< Position command. 0x00 means closed/minimum position.
  uint8_t vel_cmd;    ///< Motor velocity command. 0x00 means zero velocity.
  uint8_t force_cmd;  ///< Grip force command, mapped to motor torque by firmware.
  uint8_t acc_cmd;    ///< Positive acceleration command.
  uint8_t dec_cmd;    ///< Deceleration command.
};

/**
 * @brief Raw OP1 state payload returned by the device.
 *
 * The response mirrors the firmware report format: fault and motion state are
 * followed by current position, velocity, and force feedback bytes.
 */
struct Op1StateFrame {
  uint8_t fault_code;  ///< Fault code, see @ref Op1FaultCode.
  uint8_t state;       ///< Motion state, see @ref Op1State.
  uint8_t pos;         ///< Current position feedback.
  uint8_t vel;         ///< Current velocity feedback.
  uint8_t force;       ///< Current force feedback.
};
#pragma pack(pop)

static_assert(sizeof(Op1ControlFrame) == 5);
static_assert(sizeof(Op1StateFrame) == 5);

/**
 * @brief OP1 fault codes reported in Op1StateFrame::fault_code.
 */
enum class Op1FaultCode : uint8_t {
  NO_FAULT = 0x00,       ///< No fault.
  OVER_TEMPTURE = 0x01,  ///< Over-temperature fault.
  OVER_SPEED = 0x02,     ///< Over-speed fault.
  INIT_ERROR = 0x03,     ///< Initialization fault.
  OVER_LIMIT = 0x04      ///< Position or motion limit exceeded.
};

/**
 * @brief OP1 motion states reported in Op1StateFrame::state.
 */
enum class Op1State : uint8_t {
  ARRIVERED = 0x00,  ///< Target has been reached.
  MOVING = 0x01,     ///< Motor is moving toward the target.
  BLOCKED = 0x02,    ///< Motion is blocked by contact or obstruction.
  FALLED = 0x03      ///< Motion failed or dropped out of normal tracking.
};

/**
 * @brief Converts an OP1 fault code to a stable text label.
 * @param fault_code Fault code enum value.
 * @return Text label such as "NO_FAULT" or "UNKNOWN".
 */
AGIBOT_EXPORT std::string ToString(Op1FaultCode fault_code);

/**
 * @brief Converts an OP1 motion state to a stable text label.
 * @param state Motion state enum value.
 * @return Text label such as "MOVING" or "UNKNOWN".
 */
AGIBOT_EXPORT std::string ToString(Op1State state);

/**
 * @brief Formats an OP1 state frame for logs and demos.
 * @param frame Raw OP1 state frame.
 * @return Single-line string containing decoded fault/state labels and raw byte values.
 */
AGIBOT_EXPORT std::string ToString(const Op1StateFrame& frame);

/**
 * @brief Streams an OP1 fault code using ToString(Op1FaultCode).
 */
AGIBOT_EXPORT std::ostream& operator<<(std::ostream& os, Op1FaultCode fault_code);

/**
 * @brief Streams an OP1 motion state using ToString(Op1State).
 */
AGIBOT_EXPORT std::ostream& operator<<(std::ostream& os, Op1State state);

/**
 * @brief Streams an OP1 state frame using ToString(const Op1StateFrame&).
 */
AGIBOT_EXPORT std::ostream& operator<<(std::ostream& os, const Op1StateFrame& frame);

/**
 * @brief OmniPicker 2025 (OP1) public interface.
 *
 * OmniPicker 2025 is a 1-DOF gripper that exposes a compact raw command/feedback
 * frame API through CAN/CANFD transports.
 */
class AGIBOT_EXPORT OmniPicker2025 : public OmniHand {
 private:
  [[noreturn]] static void ThrowUnsupported(const char* api_name) {
    throw std::logic_error(std::string("OmniPicker2025 does not support ") + api_name);
  }

 public:
  virtual ~OmniPicker2025() = default;

  /**
   * @brief Sends one OP1 control frame and waits synchronously for the state response.
   * @param frame Raw 5-byte OP1 command payload.
   * @return Device state frame when a valid response is received; std::nullopt on timeout,
   *         transport failure, or invalid response length.
   */
  virtual std::optional<Op1StateFrame> SendFrameSync(const Op1ControlFrame& frame) = 0;

  // ============ Unsupported OmniHand Interfaces ============
  VendorInfo GetVendorInfo() const override {
    ThrowUnsupported("GetVendorInfo");
  }

  void SetCurrentThreshold(unsigned char, int16_t) override {
    ThrowUnsupported("SetCurrentThreshold");
  }

  int16_t GetCurrentThreshold(unsigned char) const override {
    ThrowUnsupported("GetCurrentThreshold");
  }

  void SetAllCurrentThreshold(const std::vector<int16_t>&) override {
    ThrowUnsupported("SetAllCurrentThreshold");
  }

  std::vector<int16_t> GetAllCurrentThreshold() const override {
    ThrowUnsupported("GetAllCurrentThreshold");
  }

  int16_t SetJointMotorPosi(unsigned char, int16_t) override {
    ThrowUnsupported("SetJointMotorPosi");
  }

  int16_t GetJointMotorPosi(unsigned char) const override {
    ThrowUnsupported("GetJointMotorPosi");
  }

  std::vector<int16_t> SetAllJointMotorPosi(const std::vector<int16_t>&) override {
    ThrowUnsupported("SetAllJointMotorPosi");
  }

  std::vector<int16_t> GetAllJointMotorPosi() const override {
    ThrowUnsupported("GetAllJointMotorPosi");
  }

  std::vector<double> SetAllActiveJointAngles(const std::vector<double>&) override {
    ThrowUnsupported("SetAllActiveJointAngles");
  }

  std::vector<double> GetAllActiveJointAngles() const override {
    ThrowUnsupported("GetAllActiveJointAngles");
  }

  std::vector<double> GetAllJointAngles() const override {
    ThrowUnsupported("GetAllJointAngles");
  }

  std::vector<double> GetAllJointAngles(const std::vector<double>&) const override {
    ThrowUnsupported("GetAllJointAngles");
  }

  void SetJointMotorVelo(unsigned char, int16_t) override {
    ThrowUnsupported("SetJointMotorVelo");
  }

  int16_t GetJointMotorVelo(unsigned char) const override {
    ThrowUnsupported("GetJointMotorVelo");
  }

  void SetAllJointMotorVelo(const std::vector<int16_t>&) override {
    ThrowUnsupported("SetAllJointMotorVelo");
  }

  std::vector<int16_t> GetAllJointMotorVelo() const override {
    ThrowUnsupported("GetAllJointMotorVelo");
  }

  std::vector<MixCtrl> MixControlByPT(const std::vector<int16_t>&,
                                      const std::vector<int16_t>&) override {
    ThrowUnsupported("MixControlByPT");
  }

  std::vector<MixCtrl> MixControlByPV(const std::vector<int16_t>&,
                                      const std::vector<int16_t>&) override {
    ThrowUnsupported("MixControlByPV");
  }

  std::vector<MixCtrl> MixControlByPVT(const std::vector<int16_t>&,
                                       const std::vector<int16_t>&,
                                       const std::vector<int16_t>&) override {
    ThrowUnsupported("MixControlByPVT");
  }

  JointMotorErrorReport GetErrorReport(unsigned char) const override {
    ThrowUnsupported("GetErrorReport");
  }

  std::vector<JointMotorErrorReport> GetAllErrorReport() const override {
    ThrowUnsupported("GetAllErrorReport");
  }

  int16_t GetTemperatureReport(unsigned char) const override {
    ThrowUnsupported("GetTemperatureReport");
  }

  std::vector<int16_t> GetAllTemperatureReport() const override {
    ThrowUnsupported("GetAllTemperatureReport");
  }

  int16_t GetCurrentReport(unsigned char) const override {
    ThrowUnsupported("GetCurrentReport");
  }

  std::vector<int16_t> GetAllCurrentReport() const override {
    ThrowUnsupported("GetAllCurrentReport");
  }

  void SetHandGesture(int = 1) override {
    ThrowUnsupported("SetHandGesture");
  }

  std::vector<int16_t> GetHandGesture(int) override {
    ThrowUnsupported("GetHandGesture");
  }

  std::vector<std::string> GetJointNames() const override {
    ThrowUnsupported("GetJointNames");
  }

  void UpdateFirmware(const std::string&, OtaProgressCallback = nullptr) override {
    ThrowUnsupported("UpdateFirmware");
  }

  bool ExitOtaUpgrade(uint32_t = 0) override {
    ThrowUnsupported("ExitOtaUpgrade");
  }

  // ============ Factory Methods ============

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniPicker2025 instance
   */
  static std::unique_ptr<OmniPicker2025> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniPicker2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniPicker2025> createHandByZlgcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#if OMNIHAND_ZLG_TCP_SUPPORTED
  /**
   * @brief Factory method - ZLG CANFD over TCP (WiFi/Ethernet to CANFD, this machine acts as TCP client)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param tcp_host TCP server IP or hostname (e.g. "192.168.0.178")
   * @param tcp_port TCP server port (e.g. 8000)
   * @param canfd_channel_id CAN channel index (0 or 1, default 0)
   * @return A unique pointer to OmniPicker2025 instance
   * @note Only available on Windows and Linux x64 (not supported on Linux aarch64/arm64)
   */
  static std::unique_ptr<OmniPicker2025> createHandByZlgCanTcp(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& tcp_host,
      uint16_t tcp_port,
      uint8_t canfd_channel_id = 0);
#endif  // OMNIHAND_ZLG_TCP_SUPPORTED

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (default "can0")
   * @return A unique pointer to OmniPicker2025 instance
   */
  static std::unique_ptr<OmniPicker2025> createHandSocketCan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& can_interface = "can0");
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id HCAN device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniPicker2025 instance
   */
  static std::unique_ptr<OmniPicker2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param hcan_serial_number HCAN device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return A unique pointer to OmniPicker2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniPicker2025> createHandByHcan(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& hcan_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef OMNIHAND_TJ_MARVIN_SDK
  /**
   * @brief Factory method - TJ MARVIN controller TJ SDK end-effector CAN/CANFD passthrough
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param marvin_controller_ip Robotic arm controller IP
   * @return A unique pointer to OmniPicker2025 instance
   */
  static std::unique_ptr<OmniPicker2025> createHandByTJ(
      HandType hand_type,
      uint8_t hand_device_id,
      const std::string& marvin_controller_ip);
#endif
  //   static std::unique_ptr<OmniPicker2025> createHandByTtl
  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00)
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      uint8_t canfd_device_id,
      uint8_t canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00) by serial number
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if device not found or request failed
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      uint8_t canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address (device_id = 0x00) via SocketCAN
   * @param can_interface CAN interface name (default "can0")
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif

 protected:
  /**
   * @brief Initialize common base-class metadata for OP1.
   * @param device_id Hand device ID.
   * @param hand_type Hand type (left/right).
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNI_PICKER_2025, device_id, hand_type);
  }
};
}  // namespace omnihand
}  // namespace agilink

#endif  //! AGILINK_OMNI_PICKER_2025_H
