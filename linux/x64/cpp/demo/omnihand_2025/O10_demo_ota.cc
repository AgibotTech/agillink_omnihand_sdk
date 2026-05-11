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

#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <thread>
#include <chrono>
#include "omnihand/omnihand_2025.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " <firmware_file_path> [canfd_device_id] [canfd_channel_id] [hand_type] [hand_device_id]" << std::endl;
  std::cout << "  firmware_file_path  - Path to firmware binary file (.bin)" << std::endl;
  std::cout << "  canfd_device_id     - CANFD device ID (default: 0)" << std::endl;
  std::cout << "  canfd_channel_id    - CAN channel ID (default: 0)" << std::endl;
  std::cout << "  hand_type           - Hand type: left or right (default: right)" << std::endl;
  std::cout << "  hand_device_id      - Hand device ID (default: 1)" << std::endl;
  std::cout << std::endl;
  std::cout << "Example:" << std::endl;
  std::cout << "  " << program_name << " ../../release/firmware/O10/ag001_hc00_app_v1.2.2_20260123.bin 0 0 right 1" << std::endl;
  std::cout << "  " << program_name << " ../../release/firmware/O10/ag001_hc00_app_v99.02.06_20260202.bin" << std::endl;
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
    std::cerr << "[ERROR]: Invalid hand type: " << hand_type_str << ". Must be 'left' or 'right'." << std::endl;
    return 1;
  }

  // Check if firmware file exists
  if (!std::filesystem::exists(firmware_path)) {
    std::cerr << "[ERROR]: Firmware file not found: " << firmware_path << std::endl;
    std::cerr << "Please check the file path and try again." << std::endl;
    return 1;
  }

  // Get absolute path
  std::string absolute_firmware_path = std::filesystem::canonical(firmware_path).string();
  std::cout << "Firmware file: " << absolute_firmware_path << std::endl;
  std::cout << "File size: " << std::filesystem::file_size(absolute_firmware_path) << " bytes" << std::endl;

  std::cout << "\n=== OmniHand 2025 OTA Firmware Upgrade Demo ===" << std::endl;
  std::cout << "CANFD Device ID: " << static_cast<int>(canfd_device_id) << std::endl;
  std::cout << "CAN Channel ID: " << static_cast<int>(canfd_channel_id) << std::endl;
  std::cout << "Hand Type: " << (hand_type == agilink::omnihand::HandType::LEFT ? "Left" : "Right") << std::endl;
  std::cout << "Hand Device ID: " << static_cast<int>(hand_device_id) << std::endl;
  std::cout << std::endl;

  // Create OmniHand 2025 instance (CANFD communication)
  std::cout << "Initializing OmniHand 2025..." << std::endl;
  auto hand = agilink::omnihand::OmniHand2025::createHandByZlgcan(hand_type, hand_device_id, canfd_device_id, canfd_channel_id);
  
  if (!hand || !hand->Init()) {
    std::cerr << "[ERROR]: Failed to initialize OmniHand 2025" << std::endl;
    std::cerr << "Please check:" << std::endl;
    std::cerr << "  1. CANFD device is connected" << std::endl;
    std::cerr << "  2. CANFD device ID is correct" << std::endl;
    std::cerr << "  3. Hand device is powered on" << std::endl;
    return 1;
  }

  std::cout << "[INFO]: OmniHand 2025 initialized successfully" << std::endl;

  // Get vendor info before upgrade
  std::cout << "\n=== Device Information (Before Upgrade) ===" << std::endl;
  auto vendor_info_before = hand->GetVendorInfo();
  std::cout << "Vendor Info: " << vendor_info_before.ToString() << std::endl;

  // Confirm upgrade
  std::cout << "\n=== Warning ===" << std::endl;
  std::cout << "You are about to upgrade the firmware. This process may take several minutes." << std::endl;
  std::cout << "DO NOT power off or restart the device during the upgrade process!" << std::endl;
  std::cout << "\nPress Enter to continue or Ctrl+C to cancel..." << std::endl;
  std::cin.get();

  // Start OTA upgrade
  std::cout << "\n=== Starting OTA Firmware Upgrade ===" << std::endl;
  std::cout << "This may take several minutes depending on firmware size..." << std::endl;
  std::cout << "Please wait and do not interrupt the process." << std::endl;
  std::cout << std::endl;

  // Define progress callback
  agilink::omnihand::OtaProgressCallback progress_callback = [](int current_packet, int total_packets, agilink::omnihand::OtaProgressStatus status) {
    switch (status) {
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_FILE_LOADED:
        std::cout << "[OTA] Firmware file loaded, total packets: " << total_packets << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_REQUESTING_UPGRADE:
        std::cout << "[OTA] Requesting upgrade..." << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_UPGRADE_ACCEPTED:
        std::cout << "[OTA] Upgrade request accepted, starting transmission..." << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_TRANSMITTING:
        {
          std::cout << "[OTA] Transmitting: " << current_packet << "/" << total_packets << std::endl;
          if (current_packet == total_packets) {
            std::cout << std::endl;
            std::cout << "[OTA] All " << total_packets << " packets transmitted successfully" << std::endl;
          }
        }
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_SENDING_FINISH:
        std::cout << "[OTA] Sending finish request..." << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_RESTARTING:
        std::cout << "[OTA] Device restarting..." << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_VERIFYING:
        std::cout << "[OTA] Verifying upgrade result..." << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_SUCCESS:
        std::cout << "[OTA] Upgrade successful!" << std::endl;
        break;
      case agilink::omnihand::OtaProgressStatus::AGILINK_OTA_ERROR:
        {
          if (current_packet < 0) {
            // SDK error
            agilink::omnihand::OtaErrorCode error_code = static_cast<agilink::omnihand::OtaErrorCode>(current_packet);
            std::cerr << "\n[OTA ERROR] SDK error: ";
            switch (error_code) {
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_FILE_NOT_FOUND:
                std::cerr << "File not found";
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_FILE_EMPTY:
                std::cerr << "File is empty";
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_REQUEST_TIMEOUT:
                std::cerr << "Request timeout";
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_RESTART_TIMEOUT:
                std::cerr << "Restart timeout";
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_TRANSMISSION_TIMEOUT:
                std::cerr << "Transmission timeout";
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_CRC_CHECK_FAILED:
                std::cerr << "CRC check failed";
                break;
              case agilink::omnihand::OtaErrorCode::AGILINK_OTA_NOT_SUPPORTED:
                std::cerr << "OTA not supported";
                break;
              default:
                std::cerr << "Unknown error code: " << current_packet;
                break;
            }
            std::cerr << std::endl;
          } else {
            // Device error
            std::cerr << "\n[OTA ERROR] Device error code: " << current_packet << std::endl;
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
    
    std::cout << "\n=== OTA Upgrade Completed ===" << std::endl;
    std::cout << "Total time: " << duration << " seconds" << std::endl;
    std::cout << "\nThe device will restart automatically after upgrade." << std::endl;
    std::cout << "Please wait for the device to restart and reconnect..." << std::endl;
    
    // Wait for device to restart
    std::cout << "\nWaiting for device to restart (2 seconds)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Try to reconnect and get vendor info
    std::cout << "\n=== Reconnecting to Device ===" << std::endl;
    hand.reset();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    hand = agilink::omnihand::OmniHand2025::createHandByZlgcan(hand_type, hand_device_id, canfd_device_id, canfd_channel_id);
    if (!hand || !hand->Init()) {
      std::cerr << "[WARN]: Failed to reconnect to device. Please check manually." << std::endl;
      return 0;
    }
    
    std::cout << "[INFO]: Reconnected successfully" << std::endl;
    
    // Get vendor info after upgrade
    std::cout << "\n=== Device Information (After Upgrade) ===" << std::endl;
    auto vendor_info_after = hand->GetVendorInfo();
    std::cout << "Vendor Info: " << vendor_info_after.ToString() << std::endl;
    
    // Compare versions
    if (vendor_info_before.softwareVersion != vendor_info_after.softwareVersion) {
      std::cout << "\n[SUCCESS]: Firmware version changed - Upgrade successful!" << std::endl;
      std::cout << "  Before: " << vendor_info_before.softwareVersion.ToString() << std::endl;
      std::cout << "  After:  " << vendor_info_after.softwareVersion.ToString() << std::endl;
    } else {
      std::cout << "\n[INFO]: Firmware version unchanged (same version or upgrade failed)" << std::endl;
    }
    
  } catch (const std::exception& ex) {
    std::cerr << "[ERROR]: OTA upgrade failed: " << ex.what() << std::endl;
    return 1;
  }

  std::cout << "\n=== Demo Completed ===" << std::endl;
  return 0;
}
