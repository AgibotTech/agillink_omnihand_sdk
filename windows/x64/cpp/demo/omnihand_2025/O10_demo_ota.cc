// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O10_demo_ota.cc
 * @brief OmniHand 2025 OTA firmware upgrade demo
 *
 * This demo shows how to use OTA functionality to upgrade OmniHand 2025 firmware
 * Supported communication types:
 *   - CANFD communication (ZLG CANFD, HCAN, SocketCAN) - all platforms
 *   - USB communication (Windows only; USB CDC OTA is not supported on Ubuntu)
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_2025_ota <firmware_file_path> [canfd_device_id] [canfd_channel_id] [hand_type] [hand_device_id]
 *
 * Example (CANFD):
 *   ./demo_omnihand_2025_ota ../../release/firmware/O10/ag001_hc00_app_v1.2.2_20260123.bin 0 0 right 1
 *   ./demo_omnihand_2025_ota ../../release/firmware/O10/ag001_hc00_app_v99.02.06_20260202.bin
 *
 * Example (USB, Windows only):
 *   ./demo_omnihand_2025_ota ../../release/firmware/O10/ag001_hc00_app_v1.2.2_20260123.bin COM3
 */

#include <cstdio>
#include <string>
#include <filesystem>
#include <thread>
#include <chrono>
#include "agilink_logger.h"
#include "omnihand/omnihand_2025.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025Demo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s <firmware_file_path> [canfd_device_id] [canfd_channel_id] [hand_type] [hand_device_id]", program_name);
  AgilinkLogger::get().infof(TAG, "  firmware_file_path  - Path to firmware binary file (.bin)");
  AgilinkLogger::get().infof(TAG, "  canfd_device_id     - CANFD device ID (default: 0)");
  AgilinkLogger::get().infof(TAG, "  canfd_channel_id    - CAN channel ID (default: 0)");
  AgilinkLogger::get().infof(TAG, "  hand_type           - Hand type: left or right (default: right)");
  AgilinkLogger::get().infof(TAG, "  hand_device_id      - Hand device ID (default: 1)");
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Example:");
  AgilinkLogger::get().infof(TAG, "  %s ../../release/firmware/O10/ag001_hc00_app_v1.2.2_20260123.bin 0 0 right 1", program_name);
  AgilinkLogger::get().infof(TAG, "  %s ../../release/firmware/O10/ag001_hc00_app_v99.02.06_20260202.bin", program_name);
}

int main(int argc, char* argv[]) {
  // Parse command line arguments
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  std::string firmware_path = argv[1];
  unsigned char canfd_device_id = (argc > 2) ? static_cast<unsigned char>(std::stoi(argv[2])) : 0;
  unsigned char canfd_channel_id = (argc > 3) ? static_cast<unsigned char>(std::stoi(argv[3])) : 0;
  std::string hand_type_str = (argc > 4) ? argv[4] : "right";
  unsigned char hand_device_id = (argc > 5) ? static_cast<unsigned char>(std::stoi(argv[5])) : 1;

  // Parse hand type
  agilink::omnihand::HandType hand_type;
  if (hand_type_str == "left") {
    hand_type = agilink::omnihand::HandType::LEFT;
  } else if (hand_type_str == "right") {
    hand_type = agilink::omnihand::HandType::RIGHT;
  } else {
    AgilinkLogger::get().errorf(TAG, "[ERROR]: Invalid hand type: %s. Must be 'left' or 'right'.", hand_type_str.c_str());
    return 1;
  }

  // Check if firmware file exists
  if (!std::filesystem::exists(firmware_path)) {
    AgilinkLogger::get().errorf(TAG, "[ERROR]: Firmware file not found: %s", firmware_path.c_str());
    AgilinkLogger::get().errorf(TAG, "Please check the file path and try again.");
    return 1;
  }

  // Get absolute path
  std::string absolute_firmware_path = std::filesystem::canonical(firmware_path).string();
  AgilinkLogger::get().infof(TAG, "Firmware file: %s", absolute_firmware_path.c_str());
  AgilinkLogger::get().infof(TAG, "File size: %zu bytes", static_cast<size_t>(std::filesystem::file_size(absolute_firmware_path)));

  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "=== OmniHand 2025 OTA Firmware Upgrade Demo ===");
  AgilinkLogger::get().infof(TAG, "CANFD Device ID: %d", static_cast<int>(canfd_device_id));
  AgilinkLogger::get().infof(TAG, "CAN Channel ID: %d", static_cast<int>(canfd_channel_id));
  AgilinkLogger::get().infof(TAG, "Hand Type: %s", (hand_type == agilink::omnihand::HandType::LEFT ? "Left" : "Right"));
  AgilinkLogger::get().infof(TAG, "Hand Device ID: %d", static_cast<int>(hand_device_id));
  AgilinkLogger::get().info("");

  // Create OmniHand 2025 instance (CANFD communication)
  AgilinkLogger::get().infof(TAG, "Initializing OmniHand 2025...");
  auto hand = agilink::omnihand::OmniHand2025::createHandByZlgcan(hand_type, hand_device_id, canfd_device_id, canfd_channel_id);

  if (!hand || !hand->Init()) {
    AgilinkLogger::get().errorf(TAG, "[ERROR]: Failed to initialize OmniHand 2025");
    AgilinkLogger::get().errorf(TAG, "Please check:");
    AgilinkLogger::get().errorf(TAG, "  1. CANFD device is connected");
    AgilinkLogger::get().errorf(TAG, "  2. CANFD device ID is correct");
    AgilinkLogger::get().errorf(TAG, "  3. Hand device is powered on");
    return 1;
  }

  AgilinkLogger::get().infof(TAG, "[INFO]: OmniHand 2025 initialized successfully");

  // Get vendor info before upgrade
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "=== Device Information (Before Upgrade) ===");
  auto vendor_info_before = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "Vendor Info: %s", vendor_info_before.ToString().c_str());

  // Confirm upgrade
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "=== Warning ===");
  AgilinkLogger::get().infof(TAG, "You are about to upgrade the firmware. This process may take several minutes.");
  AgilinkLogger::get().infof(TAG, "DO NOT power off or restart the device during the upgrade process!");
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Press Enter to continue or Ctrl+C to cancel...");
  std::cin.get();

  // Start OTA upgrade
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "=== Starting OTA Firmware Upgrade ===");
  AgilinkLogger::get().infof(TAG, "This may take several minutes depending on firmware size...");
  AgilinkLogger::get().infof(TAG, "Please wait and do not interrupt the process.");
  AgilinkLogger::get().info("");

  // Define progress callback
  agilink::omnihand::OtaProgressCallback progress_callback = [](int current_packet, int total_packets, agilink::omnihand::OtaProgressStatus status) {
    switch (status) {
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_FILE_LOADED:
        AgilinkLogger::get().infof(TAG, "[OTA] Firmware file loaded, total packets: %d", total_packets);
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_REQUESTING_UPGRADE:
        AgilinkLogger::get().infof(TAG, "[OTA] Requesting upgrade...");
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_UPGRADE_ACCEPTED:
        AgilinkLogger::get().infof(TAG, "[OTA] Upgrade request accepted, starting transmission...");
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_TRANSMITTING:
        {
          AgilinkLogger::get().infof(TAG, "[OTA] Transmitting: %d/%d", current_packet, total_packets);
          if (current_packet == total_packets) {
            AgilinkLogger::get().info("");
            AgilinkLogger::get().infof(TAG, "[OTA] All %d packets transmitted successfully", total_packets);
          }
        }
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_SENDING_FINISH:
        AgilinkLogger::get().infof(TAG, "[OTA] Sending finish request...");
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_RESTARTING:
        AgilinkLogger::get().infof(TAG, "[OTA] Device restarting...");
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_VERIFYING:
        AgilinkLogger::get().infof(TAG, "[OTA] Verifying upgrade result...");
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_SUCCESS:
        AgilinkLogger::get().infof(TAG, "[OTA] Upgrade successful!");
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_ERROR:
        {
          if (current_packet < 0) {
            // SDK error
            agilink::omnihand::OtaErrorCode error_code = static_cast<agilink::omnihand::OtaErrorCode>(current_packet);
            AgilinkLogger::get().error("");
            switch (error_code) {
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_FILE_NOT_FOUND:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: File not found");
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_FILE_EMPTY:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: File is empty");
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_REQUEST_TIMEOUT:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: Request timeout");
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_RESTART_TIMEOUT:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: Restart timeout");
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_TRANSMISSION_TIMEOUT:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: Transmission timeout");
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_CRC_CHECK_FAILED:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: CRC check failed");
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_NOT_SUPPORTED:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: OTA not supported");
                break;
              default:
                AgilinkLogger::get().errorf(TAG, "[OTA ERROR] SDK error: Unknown error code: %d", current_packet);
                break;
            }
          } else {
            // Device error
            AgilinkLogger::get().error("");
            AgilinkLogger::get().errorf(TAG, "[OTA ERROR] Device error code: %d", current_packet);
          }
        }
        break;
    }
  };

  try {
    auto start_time = std::chrono::steady_clock::now();

    // Call UpdateFirmware with progress callback
    hand->UpdateFirmware(absolute_firmware_path, progress_callback);

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

    AgilinkLogger::get().info("");
    AgilinkLogger::get().infof(TAG, "=== OTA Upgrade Completed ===");
    AgilinkLogger::get().infof(TAG, "Total time: %ld seconds", duration);
    AgilinkLogger::get().info("");
    AgilinkLogger::get().infof(TAG, "The device will restart automatically after upgrade.");
    AgilinkLogger::get().infof(TAG, "Please wait for the device to restart and reconnect...");

    // Wait for device to restart
    AgilinkLogger::get().info("");
    AgilinkLogger::get().infof(TAG, "Waiting for device to restart (2 seconds)...");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Try to reconnect and get vendor info
    AgilinkLogger::get().info("");
    AgilinkLogger::get().infof(TAG, "=== Reconnecting to Device ===");
    hand.reset();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    hand = agilink::omnihand::OmniHand2025::createHandByZlgcan(hand_type, hand_device_id, canfd_device_id, canfd_channel_id);
    if (!hand || !hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[WARN]: Failed to reconnect to device. Please check manually.");
      return 0;
    }

    AgilinkLogger::get().infof(TAG, "[INFO]: Reconnected successfully");

    // Get vendor info after upgrade
    AgilinkLogger::get().info("");
    AgilinkLogger::get().infof(TAG, "=== Device Information (After Upgrade) ===");
    auto vendor_info_after = hand->GetVendorInfo();
    AgilinkLogger::get().infof(TAG, "Vendor Info: %s", vendor_info_after.ToString().c_str());

    // Compare versions
    if (vendor_info_before.softwareVersion != vendor_info_after.softwareVersion) {
      AgilinkLogger::get().info("");
      AgilinkLogger::get().infof(TAG, "[SUCCESS]: Firmware version changed - Upgrade successful!");
      AgilinkLogger::get().infof(TAG, "  Before: %s", vendor_info_before.softwareVersion.ToString().c_str());
      AgilinkLogger::get().infof(TAG, "  After:  %s", vendor_info_after.softwareVersion.ToString().c_str());
    } else {
      AgilinkLogger::get().info("");
      AgilinkLogger::get().infof(TAG, "[INFO]: Firmware version unchanged (same version or upgrade failed)");
    }

  } catch (const std::exception& ex) {
    AgilinkLogger::get().errorf(TAG, "[ERROR]: OTA upgrade failed: %s", ex.what());
    return 1;
  }

  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "=== Demo Completed ===");
  return 0;
}
