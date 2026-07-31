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
#include <utility>
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
struct AGIBOT_EXPORT Op1CanfdCtrlFrame {
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
struct AGIBOT_EXPORT Op1CanfdStateFrame {
  uint8_t fault_code;  ///< Fault code, see @ref Op1FaultCode.
  uint8_t state;       ///< Motion state, see @ref Op1State.
  uint8_t pos;         ///< Current position feedback.
  uint8_t vel;         ///< Current velocity feedback.
  uint8_t force;       ///< Current force feedback.
};

struct AGIBOT_EXPORT Op1USBRange {
  float pos_min, pos_max;
  float vel_min, vel_max;
  float tor_min, tor_max;
  float kp_min, kp_max;
  float kd_min, kd_max;
};

struct AGIBOT_EXPORT Op1USBCtrlFrame {
  float pos;
  float vel;
  float tor;
  float kp;
  float kd;
};

#pragma pack(pop)

static_assert(sizeof(Op1CanfdCtrlFrame) == 5);
static_assert(sizeof(Op1CanfdStateFrame) == 5);

struct AGIBOT_EXPORT Op1CanfdFrameRange {
  UInt8Bound pos;  ///< Position byte range. pos=0 is fully closed; pos=max is fully open.
  UInt8Range vel;  ///< Velocity byte range.
  UInt8Range tor;  ///< Force/torque byte range.
  UInt8Range acc;  ///< Acceleration byte range. 0 is rejected by firmware; use 255 for max.
  UInt8Range dec;  ///< Deceleration byte range. 0 is rejected by firmware; use 255 for max.
};

/**
 * @brief OP1 fault codes reported in Op1CanfdStateFrame::fault_code.
 */
enum class AGIBOT_EXPORT Op1FaultCode : uint8_t {
  NO_FAULT = 0x00,       ///< No fault.
  OVER_TEMPTURE = 0x01,  ///< Over-temperature fault.
  OVER_SPEED = 0x02,     ///< Over-speed fault.
  INIT_ERROR = 0x03,     ///< Initialization fault.
  OVER_LIMIT = 0x04      ///< Position or motion limit exceeded.
};

/**
 * @brief OP1 motion states reported in Op1CanfdStateFrame::state.
 */
enum class AGIBOT_EXPORT Op1State : uint8_t {
  ARRIVERED = 0x00,  ///< Target has been reached.
  MOVING = 0x01,     ///< Motor is moving toward the target.
  BLOCKED = 0x02,    ///< Motion is blocked by contact or obstruction.
  FALLED = 0x03      ///< Motion failed or dropped out of normal tracking.
};

enum class AGIBOT_EXPORT Op1MotorRequestState : uint8_t {
  MOTOR_STATE_IDLE = 0,         ///< Motor disabled
  MOTOR_STATE_RUN = 1,          ///< Motor enabled
  MOTOR_STATE_CALIBRATION = 2,  ///< Motor calibration in progress
  MOTOR_STATE_BRAKE = 3,        ///< Motor brake
  MOTOR_STATE_PLAY_TONE = 5,    ///< Play tone
};

/**
 * @brief Converts an OP1 motor state to a text label.
 */
AGIBOT_EXPORT inline std::string ToString(Op1MotorRequestState state) {
  switch (state) {
    case Op1MotorRequestState::MOTOR_STATE_IDLE:
      return "IDLE";
    case Op1MotorRequestState::MOTOR_STATE_RUN:
      return "RUN";
    case Op1MotorRequestState::MOTOR_STATE_CALIBRATION:
      return "CALIBRATION";
    case Op1MotorRequestState::MOTOR_STATE_BRAKE:
      return "BRAKE";
    case Op1MotorRequestState::MOTOR_STATE_PLAY_TONE:
      return "PLAY_TONE";
    default:
      return "UNKNOWN";
  }
}

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
AGIBOT_EXPORT std::string ToString(const Op1CanfdStateFrame& frame);

/**
 * @brief Streams an OP1 fault code using ToString(Op1FaultCode).
 */
AGIBOT_EXPORT std::ostream& operator<<(std::ostream& os, Op1FaultCode fault_code);

/**
 * @brief Streams an OP1 motion state using ToString(Op1State).
 */
AGIBOT_EXPORT std::ostream& operator<<(std::ostream& os, Op1State state);

/**
 * @brief Streams an OP1 state frame using ToString(const Op1CanfdStateFrame&).
 */
AGIBOT_EXPORT std::ostream& operator<<(std::ostream& os, const Op1CanfdStateFrame& frame);

struct AGIBOT_EXPORT Op1MotorInfo {
  uint32_t error_code{0};     ///< Motor error code (0 = no error)
  uint32_t ctrl_mode{0};      ///< Control mode
  float current_limit{0};     ///< Current limit (A)
  float velocity_limit{0};    ///< Velocity limit (rad/s)
  float pos_gain{0};          ///< Position gain
  float vel_gain{0};          ///< Velocity gain
  uint8_t calib_valid{0};     ///< Motor calibration valid (0 = invalid, 1 = valid)
  uint8_t enable_on_boot{0};  ///< Enable motor on boot (0 = disabled, 1 = enabled)
};

struct AGIBOT_EXPORT Op1DeviceInfo {
  uint8_t device_type{0};     ///< Device type
  uint8_t can_node_id{0};     ///< CAN bus node ID
  uint8_t debug_flag{0};      ///< Debug flag
  uint32_t serial_number{0};  ///< Serial number
  Version fw_version;         ///< Firmware version
  uint32_t fw_hash{0};        ///< Firmware hash
  Op1MotorInfo motor;         ///< Motor information
};

/**
 * @brief Formats OP1 motor info into a readable string.
 */
AGIBOT_EXPORT std::string ToString(const Op1MotorInfo& motor);

/**
 * @brief Formats OP1 device info into a readable string.
 */
AGIBOT_EXPORT std::string ToString(const Op1DeviceInfo& info);

/**
 * @brief Formats OP1 CANFD frame byte ranges into a readable string.
 */
AGIBOT_EXPORT std::string ToString(const Op1CanfdFrameRange& range);

/**
 * @brief OmniPicker 2025 predefined gestures for SetHandGesture.
 */
enum class OmniPicker2025Gesture : int {
  OMNIPICKER_2025_GESTURE_ZERO = 0,
  OMNIPICKER_2025_GESTURE_HALF_OPEN,
  OMNIPICKER_2025_GESTURE_FULL_OPEN,
};

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
  static constexpr uint8_t kDegreesOfActiveFreedom = 1;
  static constexpr uint8_t kDefaultHandDeviceId = 1u;

  virtual ~OmniPicker2025() = default;

  /**
   * @brief Sends one OP1 control frame and waits synchronously for the state response.
   * @param frame Raw 5-byte OP1 command payload.
   * @return Device state frame when a valid response is received; std::nullopt on timeout,
   *         transport failure, or invalid response length.
   */
  virtual std::optional<Op1CanfdStateFrame> SendFrameSyncByCanfd(const Op1CanfdCtrlFrame& frame) {
    ThrowUnsupported("SendFrameSyncByCanfd");
  };

  /**
   * @brief Sends one CANFD FD broadcast frame (CAN ID=0, 64 bytes) to up to 8 OP1 nodes simultaneously.
   *
   * The 64-byte frame is partitioned into 8 fixed slots of 8 bytes each:
   *   slot offset = (node_id - 1) * 8
   * Each node on the bus reads only its own slot; slots never overlap.
   *
   * @warning Node IDs not present in @p commands have their slot left as all-zeros,
   *          which the firmware interprets as a valid command (pos=0, vel=0, force=0).
   *          To avoid unintentional motion on other nodes, pass an entry for every
   *          active node on the bus, or use SendFrameSyncByCanfd for single-node control.
   *
   * The frame is fire-and-forget — no per-node response is collected.
   *
   * @param commands Node-ID-to-frame pairs. Node IDs outside [1, 8] are silently ignored.
   * @return true if the frame was dispatched to the CAN driver; false on device error.
   */
  virtual bool SendBroadcastFrameByCanfd(const std::vector<std::pair<uint8_t, Op1CanfdCtrlFrame>>& commands) {
    ThrowUnsupported("SendBroadcastFrameByCanfd");
  };

  virtual void SendMitFrameAsync(const Op1USBCtrlFrame& frame) {
    ThrowUnsupported("SendFrameSyncByCanfd");
  };

  /**
   * @brief Returns the CANFD frame byte encoding ranges for position, velocity, and torque.
   */
  virtual Op1CanfdFrameRange GetCanfdFrameRange() {
    ThrowUnsupported("GetCanfdFrameRange");
  }

  virtual Op1USBRange GetMitFrameRange() {
    ThrowUnsupported("GetMitFrameRange");
  };

  virtual void UpdateFirmwareViaFlash(const std::string&, OtaProgressCallback = nullptr) {
    ThrowUnsupported("UpdateFirmwareViaFlash");
  }

  // ============ Calibration (Fibre protocol over USB) ============

  /**
   * @brief Trigger motor calibration (phase resistance, inductance, encoder offset).
   * @note Enters STATE_CALIBRATION on the firmware. The motor runs the calibration
   *       procedure automatically, saves config to flash, and reboots the device on success.
   *       After reboot, the USB and CAN connections will be lost.
   * @return true if the calibration request was sent successfully.
   */
  virtual bool StartMotorCalibration() {
    ThrowUnsupported("StartMotorCalibration");
  };

  /**
   * @brief Reboot the device via Fibre USB protocol.
   * @note After reboot, the USB and CAN connections will be lost. The caller must
   *       re-establish connections after the device re-enumerates.
   * @return true if the reboot request was sent successfully.
   */
  virtual bool Reboot() {
    ThrowUnsupported("Reboot");
  };

  /**
   * @brief Set CAN bus node ID (disables motor, writes new ID, saves config).
   * @note The motor must be re-powered for the new CAN ID to take effect.
   * @param node_id New CAN bus node ID
   * @return true if the operation was sent successfully.
   */
  virtual bool SetCanNodeId(uint8_t node_id) {
    ThrowUnsupported("SetCanNodeId");
  };

  /**
   * @brief Get CAN bus node ID via USB Fibre protocol.
   * @return CAN bus node ID, or 0 if failed.
   */
  virtual uint8_t GetCanNodeId() {
    ThrowUnsupported("GetCanNodeId");
  };

  /**
   * @brief Show device information via USB Fibre protocol (device type, serial number, firmware version, CAN node ID).
   * @note This is equivalent to typing "ref0" in the REF-CLI tool.
   */
  virtual Op1DeviceInfo ShowDeviceInfo() const = 0;

  // ============ Factory Methods ============
  static std::unique_ptr<OmniPicker2025> createHandByUSB();
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


  /**
   * @brief Sets the gripper to a predefined gesture (typed API).
   */
  virtual float SetHandGesture(OmniPicker2025Gesture gesture) = 0;

  /**
   * @brief Returns gesture target motor position by typed gesture.
   */
  virtual float GetHandGesture(OmniPicker2025Gesture gesture) = 0;

  void SetHandGesture(int gesture_num = 1) override;

  std::vector<int16_t> GetHandGesture(int gesture_num) override;

  /**
   * @brief Returns the number of joint motors.
   * @return Number of joint motors (12)
   */
  static constexpr uint8_t GetNumOfJointMotors() {
    return kDegreesOfActiveFreedom;
  }

  /**
   * @brief Returns the degrees of active freedom (DoA).
   * @return Degrees of active freedom (12)
   */
  static constexpr uint8_t GetDoA() {
    return kDegreesOfActiveFreedom;
  }

 protected:
  /**
   * @brief Initialize common base-class metadata for OP1.
   * @param device_id Hand device ID.
   * @param hand_type Hand type (left/right).
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNI_PICKER_2025, device_id, hand_type);
  }

  /**
   * @brief Move the gripper to a position expressed as a normalized ratio [0.0, 1.0].
   * @param ratio 0.0 = fully closed, 1.0 = fully open.
   * @return ratio of position in feaaback.
   */
  virtual float SetPositionRatio(float ratio) {
    ThrowUnsupported("SetPositionRatio");
  }

};
}  // namespace omnihand
}  // namespace agilink

#endif  //! AGILINK_OMNI_PICKER_2025_H
