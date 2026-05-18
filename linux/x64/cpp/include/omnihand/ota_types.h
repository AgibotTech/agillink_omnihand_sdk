// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file ota_types.h
 * @brief OTA (Over-The-Air) upgrade type definitions
 * @note This header defines all OTA-related types including status, error codes, and callbacks
 */

#ifndef AGILINK_OTA_TYPES_H
#define AGILINK_OTA_TYPES_H

#include "omnihand/export_symbols.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace agilink {
namespace omnihand {

/**
 * @brief OTA upgrade error codes
 * 
 * Negative values are SDK-defined errors, positive values are device-returned errors.
 * 
 * | Error Code | Value | Description |
 * |------------|-------|-------------|
 * | AGILINK_SUCCESS | 0 | Operation successful |
 * | AGILINK_OTA_NOT_SUPPORTED | -1 | OTA not supported (e.g., RS485) |
 * | AGILINK_OTA_FILE_NOT_FOUND | -2 | Firmware file not found |
 * | AGILINK_OTA_FILE_EMPTY | -3 | Firmware file is empty |
 * | AGILINK_OTA_REQUEST_TIMEOUT | -4 | Upgrade request or finish response timeout |
 * | AGILINK_OTA_RESTART_TIMEOUT | -5 | Restart response timeout |
 * | AGILINK_OTA_TRANSMISSION_TIMEOUT | -6 | Data transmission timeout |
 * | AGILINK_OTA_CRC_CHECK_FAILED | -7 | CRC checksum verification failed |
 * | AGILINK_OTA_DEVICE_DISCONNECTED | -8 | Device disconnected during OTA |
 * | AGILINK_OTA_DEVICE_UNKNOWN_ERROR | 1 | Device: unknown error |
 * | AGILINK_OTA_DEVICE_DATA_READ_ERROR | 2 | Device: data read error |
 * | AGILINK_OTA_DEVICE_WRITE_ERROR | 3 | Device: write error |
 * | AGILINK_OTA_DEVICE_FIRMWARE_CORRUPTION | 4 | Device: firmware corruption (checksum mismatch) |
 * | AGILINK_OTA_DEVICE_INSUFFICIENT_SPACE | 5 | Device: insufficient storage space |
 * | AGILINK_OTA_DEVICE_VERSION_MISMATCH | 6 | Device: firmware version mismatch |
 * | AGILINK_OTA_DEVICE_ROLLBACK_FAILED | 7 | Device: firmware rollback failed |
 * | AGILINK_OTA_DEVICE_SIGNATURE_INVALID | 8 | Device: firmware signature verification failed |
 * | AGILINK_OTA_DEVICE_PARTITION_ERROR | 9 | Device: flash partition table error |
 * | AGILINK_OTA_DEVICE_ERASE_FAILED | 10 | Device: flash erase failed |
 * | AGILINK_OTA_DEVICE_PROGRAM_FAILED | 11 | Device: flash programming failed |
 * | AGILINK_OTA_DEVICE_VERIFY_FAILED | 12 | Device: flash data verification failed |
 * | AGILINK_OTA_DEVICE_TIMEOUT | 13 | Device: internal operation timeout |
 */
enum class AGIBOT_EXPORT OtaErrorCode : int {
  AGILINK_SUCCESS = 0,                    // Operation successful
  AGILINK_OTA_NOT_SUPPORTED = -1,         // OTA not supported for this communication type (e.g., RS485)
  AGILINK_OTA_FILE_NOT_FOUND = -2,        // Firmware file not found or cannot be opened
  AGILINK_OTA_FILE_EMPTY = -3,            // Firmware file is empty
  AGILINK_OTA_REQUEST_TIMEOUT = -4,       // Upgrade request or finish response timeout
  AGILINK_OTA_RESTART_TIMEOUT = -5,       // Restart response timeout
  AGILINK_OTA_TRANSMISSION_TIMEOUT = -6,  // Data transmission timeout
  AGILINK_OTA_CRC_CHECK_FAILED = -7,      // CRC checksum verification failed
  AGILINK_OTA_DEVICE_DISCONNECTED = -8,   // Device disconnected during transmission
  // Device-returned error codes (positive values from OTA result response 0x05)
  AGILINK_OTA_DEVICE_UNKNOWN_ERROR = 1,       // Device: unknown error
  AGILINK_OTA_DEVICE_DATA_READ_ERROR = 2,     // Device: data read error
  AGILINK_OTA_DEVICE_WRITE_ERROR = 3,         // Device: write error
  AGILINK_OTA_DEVICE_FIRMWARE_CORRUPTION = 4,  // Device: firmware corruption (checksum mismatch)
  AGILINK_OTA_DEVICE_INSUFFICIENT_SPACE = 5,   // Device: insufficient storage space
  AGILINK_OTA_DEVICE_VERSION_MISMATCH = 6,     // Device: firmware version mismatch
  AGILINK_OTA_DEVICE_ROLLBACK_FAILED = 7,      // Device: firmware rollback failed
  AGILINK_OTA_DEVICE_SIGNATURE_INVALID = 8,    // Device: firmware signature verification failed
  AGILINK_OTA_DEVICE_PARTITION_ERROR = 9,      // Device: flash partition table error
  AGILINK_OTA_DEVICE_ERASE_FAILED = 10,        // Device: flash erase failed
  AGILINK_OTA_DEVICE_PROGRAM_FAILED = 11,      // Device: flash programming failed
  AGILINK_OTA_DEVICE_VERIFY_FAILED = 12,       // Device: flash data verification failed
  AGILINK_OTA_DEVICE_TIMEOUT = 13              // Device: internal operation timeout
};

/**
 * @brief Converts an OtaErrorCode to a human-readable string
 * @param code Error code (can be SDK-defined or device-returned)
 * @return Description string
 */
inline const char* OtaErrorCodeToString(OtaErrorCode code) {
  switch (code) {
    case OtaErrorCode::AGILINK_SUCCESS:                     return "Success";
    case OtaErrorCode::AGILINK_OTA_NOT_SUPPORTED:           return "OTA not supported";
    case OtaErrorCode::AGILINK_OTA_FILE_NOT_FOUND:          return "Firmware file not found";
    case OtaErrorCode::AGILINK_OTA_FILE_EMPTY:              return "Firmware file is empty";
    case OtaErrorCode::AGILINK_OTA_REQUEST_TIMEOUT:         return "Upgrade request timeout";
    case OtaErrorCode::AGILINK_OTA_RESTART_TIMEOUT:         return "Restart response timeout";
    case OtaErrorCode::AGILINK_OTA_TRANSMISSION_TIMEOUT:    return "Data transmission timeout";
    case OtaErrorCode::AGILINK_OTA_CRC_CHECK_FAILED:        return "CRC checksum verification failed";
    case OtaErrorCode::AGILINK_OTA_DEVICE_DISCONNECTED:     return "Device disconnected during OTA";
    case OtaErrorCode::AGILINK_OTA_DEVICE_UNKNOWN_ERROR:    return "Device: unknown error";
    case OtaErrorCode::AGILINK_OTA_DEVICE_DATA_READ_ERROR:  return "Device: data read error";
    case OtaErrorCode::AGILINK_OTA_DEVICE_WRITE_ERROR:      return "Device: write error";
    case OtaErrorCode::AGILINK_OTA_DEVICE_FIRMWARE_CORRUPTION: return "Device: firmware corruption";
    case OtaErrorCode::AGILINK_OTA_DEVICE_INSUFFICIENT_SPACE:  return "Device: insufficient space";
    case OtaErrorCode::AGILINK_OTA_DEVICE_VERSION_MISMATCH: return "Device: version mismatch";
    case OtaErrorCode::AGILINK_OTA_DEVICE_ROLLBACK_FAILED:  return "Device: rollback failed";
    case OtaErrorCode::AGILINK_OTA_DEVICE_SIGNATURE_INVALID: return "Device: signature invalid";
    case OtaErrorCode::AGILINK_OTA_DEVICE_PARTITION_ERROR:   return "Device: partition table error";
    case OtaErrorCode::AGILINK_OTA_DEVICE_ERASE_FAILED:     return "Device: flash erase failed";
    case OtaErrorCode::AGILINK_OTA_DEVICE_PROGRAM_FAILED:   return "Device: flash programming failed";
    case OtaErrorCode::AGILINK_OTA_DEVICE_VERIFY_FAILED:    return "Device: flash verification failed";
    case OtaErrorCode::AGILINK_OTA_DEVICE_TIMEOUT:          return "Device: internal timeout";
    default:                                                 return "Unknown error code";
  }
}

/**
 * @brief Converts an integer error code to a human-readable string
 * @param code Integer error code (cast from OtaErrorCode or raw device value)
 * @return Description string
 */
inline const char* OtaErrorCodeToString(int code) {
  return OtaErrorCodeToString(static_cast<OtaErrorCode>(code));
}

/** OTA packet size in bytes, consistent with the 2KB chunking in the implementation */
constexpr int kOtaPacketSizeBytes = 2048;

/** Single packet type: fixed 2KB, padded with 0xFF if less than 2KB */
using OtaPacket = std::array<std::uint8_t, kOtaPacketSizeBytes>;

/**
 * @brief Result of loading a firmware file only (no device communication)
 * @note Check error_code == OtaErrorCode::AGILINK_SUCCESS to determine success
 * @note packets are chunked into 2KB blocks, with the last packet padded with 0xFF; can be reused by UpdateFirmware
 */
struct AGIBOT_EXPORT FirmwareLoadResult {
  int total_packets{0};          ///< Total number of packets (calculated by kOtaPacketSizeBytes)
  size_t file_size_bytes{0};     ///< File size in bytes
  OtaErrorCode error_code{OtaErrorCode::AGILINK_SUCCESS};  ///< AGILINK_SUCCESS on success, specific error code on failure
  std::vector<OtaPacket> packets;  ///< Loaded and chunked data, last packet padded with 0xFF if less than 2KB
};

/**
 * @brief OTA upgrade progress status
 * 
 * | Status | current_packet | total_packets | Description |
 * |--------|----------------|---------------|-------------|
 * | AGILINK_OTA_FILE_LOADED | 0 | total | Firmware file loaded |
 * | AGILINK_OTA_REQUESTING_UPGRADE | 0 | total | Sending upgrade request |
 * | AGILINK_OTA_UPGRADE_ACCEPTED | 0 | total | Upgrade request accepted |
 * | AGILINK_OTA_DEVICE_RECONNECTED | 0 | total | USB device reconnected (e.g. after OTA reset); app may set connect UI to "connected" |
 * | AGILINK_OTA_REQUEST_NO_REPLY | attempt | total | Nth upgrade request got no reply, reconnecting (current_packet=1 or 2) |
 * | AGILINK_OTA_TRANSMITTING | current | total | Transmitting packet current/total |
 * | AGILINK_OTA_SENDING_FINISH | total | total | Sending finish request |
 * | AGILINK_OTA_RESTARTING | total | total | Device restarting |
 * | AGILINK_OTA_VERIFYING | total | total | Verifying upgrade result |
 * | AGILINK_OTA_SUCCESS | total | total | Upgrade successful |
 * | AGILINK_OTA_ERROR | error_code | total | Error (current=OtaErrorCode or device error) |
 */
enum class AGIBOT_EXPORT OtaProgressStatus : int {
  AGILINK_OTA_FILE_LOADED = 1,        // Firmware file loaded (total=total packets)
  AGILINK_OTA_REQUESTING_UPGRADE = 2, // Sending upgrade request
  AGILINK_OTA_UPGRADE_ACCEPTED = 3,   // Upgrade request accepted by device
  AGILINK_OTA_DEVICE_RECONNECTED = 8, // USB device reconnected; app should set connect module to "connected"
  AGILINK_OTA_REQUEST_NO_REPLY = 9,   // Nth request got no reply, reconnecting (current_packet=attempt 1 or 2)
  AGILINK_OTA_TRANSMITTING = 4,       // Transmitting packets (current/total=packet progress)
  AGILINK_OTA_SENDING_FINISH = 5,     // Sending finish request
  AGILINK_OTA_RESTARTING = 6,         // Device restarting
  AGILINK_OTA_VERIFYING = 7,          // Verifying upgrade result
  AGILINK_OTA_SUCCESS = 0,            // Upgrade successful
  AGILINK_OTA_ERROR = -1              // Error occurred (current_packet=error code, see OtaErrorCode)
};

/**
 * @brief OTA upgrade progress callback function type
 * @param current_packet Current packet index (meaning depends on status, see OtaProgressStatus)
 *                       When status=ERROR, this is the error code (see OtaErrorCode)
 * @param total_packets Total number of packets
 * @param status Progress status (see OtaProgressStatus)
 * 
 * Usage example:
 * @code
 * hand->UpdateFirmware("firmware.bin", [](int current, int total, OtaProgressStatus status) {
 *     switch (status) {
 *         case OtaProgressStatus::AGILINK_OTA_FILE_LOADED:
 *             printf("File loaded, %d packets\n", total);
 *             break;
 *         case OtaProgressStatus::AGILINK_OTA_TRANSMITTING:
 *             printf("Transmitting %d/%d\n", current, total);
 *             break;
 *         case OtaProgressStatus::AGILINK_OTA_SUCCESS:
 *             printf("Upgrade successful\n");
 *             break;
 *         case OtaProgressStatus::AGILINK_OTA_ERROR:
 *             if (current < 0) {
 *                 // SDK error (see OtaErrorCode)
 *                 printf("SDK error: %d\n", current);
 *             } else {
 *                 // Device error
 *                 printf("Device error: %d\n", current);
 *             }
 *             break;
 *         default:
 *             break;
 *     }
 * });
 * @endcode
 */
using OtaProgressCallback = std::function<void(int current_packet, int total_packets, 
                                               OtaProgressStatus status)>;

/**
 * @brief Load firmware file only and calculate packet count, without device communication
 * @param file_name Firmware file path
 * @return FirmwareLoadResult containing total_packets, file_size_bytes, packets, and error_code (AGILINK_SUCCESS on success)
 * @note Consistent with the internal chunking rules of UpdateFirmware; packets are chunked into 2KB blocks with the last packet padded with 0xFF, and can be reused by the update interface
 */
AGIBOT_EXPORT FirmwareLoadResult LoadFirmwareOnly(const std::string& file_name);

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OTA_TYPES_H
