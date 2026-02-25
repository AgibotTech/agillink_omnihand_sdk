// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file omnihand_2025.h
 * @brief OmniHand 2025 (O10) interface class - 10 DOF
 * @note This is the public interface for OmniHand 2025 product
 */

#ifndef AGILINK_OMNIHAND_2025_H
#define AGILINK_OMNIHAND_2025_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_base.h"
#include "omnihand/omnihand_sensor_base.h"
#include "omnihand/proto.h"
#include "omnihand/ota_types.h"
#include "omnihand/kinematics/omnihand_2025/omnihand_2025_solver.h"

namespace agilink {
namespace omnihand {

class OmniHand2025CanImpl;
class OmniHand2025RsImpl;

/**
 * @brief OmniHand 2025 (O10) interface class - 10 DOF
 * 
 * This class provides the public interface for OmniHand 2025 product.
 * It includes all methods supported by O10, including 1D tactile sensors.
 * 
 * @note Uses virtual inheritance from OmniHandSensorBase to avoid diamond inheritance problem.
 */
class AGIBOT_EXPORT OmniHand2025 : public OmniHandBase, public virtual OmniHandSensorBase {
 public:
  // Constants
  static constexpr unsigned char kDegreesOfActiveFreedom = 10;  // O10 has 10 active degrees of freedom (DoA)
  static constexpr unsigned char kDegreesOfPassiveFreedom = 6;  // O10 has 6 passive degrees of freedom (DoP)

  virtual ~OmniHand2025() = default;

  // ============ Factory Methods ============
  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by canfd_device_id
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @param frame_format Extended (command in CAN ID) or Standard (command in D0, same as USB/RS485). Default Extended.
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgcan(
      HandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0,
      CanFrameFormat frame_format = CanFrameFormat::Extended);

  /**
   * @brief Factory method - CAN communication (ZLG USB CANFD) by serial number
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @param frame_format Extended or Standard. Default Extended.
   * @return A unique pointer to OmniHand2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHand2025> createHandByZlgcan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& usbcanfd_serial_number,
      unsigned char canfd_channel_id = 0,
      CanFrameFormat frame_format = CanFrameFormat::Extended);

  /**
   * @brief Factory method - RS485 communication (OmniHand 2025 only)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (e.g., "/dev/ttyUSB0")
   * @param baudrate Baud rate (default 460800)
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByRs485(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

  /**
   * @brief Factory method - USB communication (OmniHand 2025 only)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param uart_port Serial port path (e.g., "/dev/ttyUSB0")
   * @param baudrate Baud rate (default 460800)
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByUsb(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& uart_port,
      int32_t baudrate = 460800);

#ifdef __linux__
  /**
   * @brief Factory method - SocketCAN communication (Linux native CAN interface)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @param frame_format Extended or Standard. Default Extended.
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandSocketCan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& can_interface = "can0",
      CanFrameFormat frame_format = CanFrameFormat::Extended);
#endif

  /**
   * @brief Factory method - HCAN USB CANFD communication (by canfd_device_id)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param canfd_device_id HCAN device index
   * @param canfd_channel_id CAN channel index (default 0)
   * @param frame_format Extended or Standard. Default Extended.
   * @return A unique pointer to OmniHand2025 instance
   */
  static std::unique_ptr<OmniHand2025> createHandByHcan(
      HandType hand_type,
      unsigned char hand_device_id,
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0,
      CanFrameFormat frame_format = CanFrameFormat::Extended);

  /**
   * @brief Factory method - HCAN USB CANFD communication (by serial number)
   * @param hand_type Hand type (left/right)
   * @param hand_device_id Hand device ID
   * @param hcan_serial_number HCAN device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   * @param frame_format Extended or Standard. Default Extended.
   * @return A unique pointer to OmniHand2025 instance, or nullptr if device not found
   */
  static std::unique_ptr<OmniHand2025> createHandByHcan(
      HandType hand_type,
      unsigned char hand_device_id,
      const std::string& hcan_serial_number,
      unsigned char canfd_channel_id = 0,
      CanFrameFormat frame_format = CanFrameFormat::Extended);

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00)
   * @param canfd_device_id USB CANFD adapter device index
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   * @note This function sends a broadcast request to discover devices on the CAN bus
   * @note Only works with CAN communication, not supported for RS485
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      unsigned char canfd_device_id,
      unsigned char canfd_channel_id = 0);

  /**
   * @brief Get device information from broadcast address (hand_device_id = 0x00) by serial number
   * @param usbcanfd_serial_number USB CANFD device serial number (supports partial matching)
   * @param canfd_channel_id CAN channel index (default 0)
   *        - Dual-channel (USBCANFD-200U): can0=0, can1=1
   *        - Single-channel (USBCANFD-100U): always 0
   * @return DeviceInfo structure, or empty DeviceInfo if device not found or request failed
   * @note This function sends a broadcast request to discover devices on the CAN bus
   * @note Only works with CAN communication, not supported for RS485
   */
  static DeviceInfo GetDeviceInfoFromBroadcast(
      const std::string& usbcanfd_serial_number,
      unsigned char canfd_channel_id = 0);

#ifdef __linux__
  /**
   * @brief Get device information from broadcast address (device_id = 0x00) via SocketCAN
   * @param can_interface CAN interface name (e.g., "can0", "can1")
   * @return DeviceInfo structure, or empty DeviceInfo if request failed
   * @note This function sends a broadcast request to discover devices on the CAN bus
   * @note Only works with CAN communication, not supported for RS485
   */
  static DeviceInfo GetDeviceInfoFromBroadcastSocketCan(
      const std::string& can_interface = "can0");
#endif


  // ============ Sensor Utilities (from OmniHandSensorBase) ============
  // GetSensorDataLength and GetSensorOrder are inherited from OmniHandSensorBase

  // ============ O10-Specific Tactile Sensor Interface ============
  /**
   * @brief Gets tactile sensor data for the specified part (O10 only).
   * @param eFinger Finger/palm enum value
   * @return Tactile sensor data vector
   * @note Data unit: 1g, Max value: 255g, Sampling frequency: 10Hz
   */
  virtual std::vector<uint8_t> GetTactileSensorData(Finger eFinger) const = 0;

  // GetAllTactileSensorDataRaw and GetTactileSensorDataRaw are inherited from OmniHandSensorBase

  // ============ Gesture Control ============
  /**
   * @brief Sets the hand to a predefined gesture.
   * @param gesture_num Gesture number: 1 = FIST1, 2 = FIST2 (default: 1)
   */
  void SetHandGesture(int gesture_num = 1) override;

  // ============ Firmware Update (OTA) ============
  /**
   * @brief Updates firmware via OTA (Over-The-Air) upgrade
   * @param file_name Path to the firmware binary file
   * @param progress_callback Optional callback function to receive progress updates
   *                          - current_packet: Meaning depends on status (see OtaProgressStatus)
   *                          - total_packets: Total number of packets
   *                          - status: Progress status (see OtaProgressStatus)
   * @note This function is supported for:
   *       - CAN communication (ZLG CANFD, HCAN, SocketCAN) - all platforms
   *       - USB communication (Windows only, Ubuntu does not support USB CDC OTA)
   * @note RS485 communication does not support OTA upgrade
   * @note Do not power off or restart the device during the update process
   * @warning This is a blocking operation that may take several minutes depending on firmware size
   * @note If progress_callback is nullptr (default), progress will be printed to stdout/stderr
   */
  virtual void UpdateFirmware(const std::string& file_name, OtaProgressCallback progress_callback = nullptr);

 protected:
  /**
   * @brief Initialize base class members and kinematics solver
   * @param device_id Device ID
   * @param hand_type Hand type (left/right)
   * @note This method automatically initializes the kinematics solver after calling base class Reset()
   * @note Product type is fixed to ProductType::OMNIHAND_2025 for this class
   */
  void Reset(unsigned char device_id, HandType hand_type) {
    OmniHand::Reset(ProductType::OMNIHAND_2025, device_id, hand_type);
    // Automatically initialize kinematics solver
    kinematics_solver_ = std::make_unique<o10::OmniHand2025Solver>(is_left_hand_);
  }

  /**
   * @brief Kinematics solver for OmniHand 2025 (O10)
   */
  std::unique_ptr<o10::OmniHand2025Solver> kinematics_solver_;
};

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_2025_H
