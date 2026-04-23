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
 * | >0 | varies | Device-returned error codes |
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
  AGILINK_OTA_DEVICE_DISCONNECTED = -8    // Device disconnected during transmission
  // Positive values (>0) are device-returned error codes
};

/** OTA 每包字节数，与实现中 2KB 分包一致 */
constexpr int kOtaPacketSizeBytes = 2048;

/** 单包类型：固定 2KB，不足部分以 0xFF 填充 */
using OtaPacket = std::array<std::uint8_t, kOtaPacketSizeBytes>;

/**
 * @brief 仅加载固件文件的结果（不进行设备通信）
 * @note 成功与否看 error_code == OtaErrorCode::AGILINK_SUCCESS
 * @note packets 已按 2KB 分包，最后一包不足部分已填 0xFF，可供 UpdateFirmware 复用
 */
struct AGIBOT_EXPORT FirmwareLoadResult {
  int total_packets{0};          ///< 总包数（按 kOtaPacketSizeBytes 计算）
  size_t file_size_bytes{0};     ///< 文件大小（字节）
  OtaErrorCode error_code{OtaErrorCode::AGILINK_SUCCESS};  ///< 成功为 AGILINK_SUCCESS，失败为具体错误码
  std::vector<OtaPacket> packets;  ///< 已加载并分包的数据，最后一包不足 2KB 已用 0xFF 填充
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
 * @brief 仅加载固件文件并计算包数，不与设备通信
 * @param file_name 固件文件路径
 * @return FirmwareLoadResult 含 total_packets、file_size_bytes、packets、error_code（成功时为 AGILINK_SUCCESS）
 * @note 与 UpdateFirmware 内部分包规则一致；packets 已按 2KB 分包，最后一包不足处填 0xFF，可供更新接口复用
 */
AGIBOT_EXPORT FirmwareLoadResult LoadFirmwareOnly(const std::string& file_name);

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OTA_TYPES_H
