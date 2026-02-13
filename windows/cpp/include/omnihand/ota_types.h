// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file ota_types.h
 * @brief OTA (Over-The-Air) upgrade type definitions
 * @note This header defines all OTA-related types including status, error codes, and callbacks
 */

#pragma once

#include <functional>

/**
 * @brief OTA upgrade error codes
 * 
 * Negative values are SDK-defined errors, positive values are device-returned errors.
 * 
 * | Error Code | Value | Description |
 * |------------|-------|-------------|
 * | SUCCESS | 0 | Operation successful |
 * | NOT_SUPPORTED | -1 | OTA not supported (e.g., RS485) |
 * | FILE_NOT_FOUND | -2 | Firmware file not found |
 * | FILE_EMPTY | -3 | Firmware file is empty |
 * | REQUEST_TIMEOUT | -4 | Upgrade request or finish response timeout |
 * | RESTART_TIMEOUT | -5 | Restart response timeout |
 * | TRANSMISSION_TIMEOUT | -6 | Data transmission timeout |
 * | CRC_CHECK_FAILED | -7 | CRC checksum verification failed |
 * | >0 | varies | Device-returned error codes |
 */
enum class OtaErrorCode : int {
  SUCCESS = 0,               // Operation successful
  NOT_SUPPORTED = -1,        // OTA not supported for this communication type (e.g., RS485)
  FILE_NOT_FOUND = -2,       // Firmware file not found or cannot be opened
  FILE_EMPTY = -3,           // Firmware file is empty
  REQUEST_TIMEOUT = -4,      // Upgrade request or finish response timeout
  RESTART_TIMEOUT = -5,      // Restart response timeout
  TRANSMISSION_TIMEOUT = -6, // Data transmission timeout
  CRC_CHECK_FAILED = -7      // CRC checksum verification failed
  // Positive values (>0) are device-returned error codes
};

/**
 * @brief OTA upgrade progress status
 * 
 * | Status | current_packet | total_packets | Description |
 * |--------|----------------|---------------|-------------|
 * | FILE_LOADED | 0 | total | Firmware file loaded |
 * | REQUESTING_UPGRADE | 0 | total | Sending upgrade request |
 * | UPGRADE_ACCEPTED | 0 | total | Upgrade request accepted |
 * | TRANSMITTING | current | total | Transmitting packet current/total |
 * | SENDING_FINISH | total | total | Sending finish request |
 * | RESTARTING | total | total | Device restarting |
 * | VERIFYING | total | total | Verifying upgrade result |
 * | SUCCESS | total | total | Upgrade successful |
 * | ERROR | error_code | total | Error (current=OtaErrorCode or device error) |
 */
enum class OtaProgressStatus : int {
  FILE_LOADED = 1,        // Firmware file loaded (total=total packets)
  REQUESTING_UPGRADE = 2, // Sending upgrade request
  UPGRADE_ACCEPTED = 3,   // Upgrade request accepted by device
  TRANSMITTING = 4,       // Transmitting packets (current/total=packet progress)
  SENDING_FINISH = 5,     // Sending finish request
  RESTARTING = 6,         // Device restarting
  VERIFYING = 7,          // Verifying upgrade result
  SUCCESS = 0,            // Upgrade successful
  ERROR = -1              // Error occurred (current_packet=error code, see OtaErrorCode)
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
 *         case OtaProgressStatus::FILE_LOADED:
 *             printf("File loaded, %d packets\n", total);
 *             break;
 *         case OtaProgressStatus::TRANSMITTING:
 *             printf("Transmitting %d/%d\n", current, total);
 *             break;
 *         case OtaProgressStatus::SUCCESS:
 *             printf("Upgrade successful\n");
 *             break;
 *         case OtaProgressStatus::ERROR:
 *             if (current < 0) {
 *                 // SDK error (see OtaErrorCode)
 *                 printf("SDK error: %d\n", current);
 *             } else {
 *                 // Device error
 *                 printf("Device error: %d\n", current);
 *             }
 *             break;
 *         // ...
 *     }
 * });
 * @endcode
 */
using OtaProgressCallback = std::function<void(int current_packet, int total_packets, 
                                               OtaProgressStatus status)>;
