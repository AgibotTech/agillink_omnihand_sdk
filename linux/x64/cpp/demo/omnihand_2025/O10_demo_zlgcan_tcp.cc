// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O10_demo_zlgcan_tcp.cc
 * @brief OmniHand 2025 demo - ZLG CANFD over TCP (e.g. WiFi/Ethernet adapter as server)
 *
 * Connect to ZLG WiFi-to-CANFD device as TCP client. Default server: 192.168.0.178:8000.
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_2025_zlgcan_tcp [left|right] [host] [port]
 *   ./demo_omnihand_2025_zlgcan_tcp left
 *   ./demo_omnihand_2025_zlgcan_tcp right 192.168.0.178 8000
 */

#include <cstdio>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include "agilink_logger.h"
#include "omnihand/omnihand_2025.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025Demo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right] [host] [port]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand (default)");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand");
  AgilinkLogger::get().infof(TAG, "  host   - TCP server IP (default: 192.168.0.178)");
  AgilinkLogger::get().infof(TAG, "  port   - TCP server port (default: 8000)");
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Example:");
  AgilinkLogger::get().infof(TAG, "  %s left", program_name);
  AgilinkLogger::get().infof(TAG, "  %s right 192.168.0.178 8000", program_name);
}

int main(int argc, char** argv) {
  std::string mode = "left";
  std::string host = "192.168.0.178";
  uint16_t port = 8000;
  unsigned char hand_device_id = 1;
  unsigned char canfd_channel_id = 0;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      return 0;
    }
    if (arg == "left" || arg == "right") {
      mode = arg;
    } else if (i == 2 && arg.find('.') != std::string::npos) {
      host = arg;
    } else if (i == 3) {
      try {
        port = static_cast<uint16_t>(std::stoi(arg));
      } catch (...) {
        AgilinkLogger::get().errorf(TAG, "[Error]: Invalid port: %s", arg.c_str());
        return 1;
      }
    }
  }

  AgilinkLogger::get().infof(TAG, "============================================");
  AgilinkLogger::get().infof(TAG, "OmniHand 2025 - ZLG CAN over TCP");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "Server: %s:%d", host.c_str(), static_cast<int>(port));
  AgilinkLogger::get().infof(TAG, "============================================");

  auto hand_type = (mode == "right") ? agilink::omnihand::HandType::RIGHT
                                     : agilink::omnihand::HandType::LEFT;

  auto hand = agilink::omnihand::OmniHand2025::createHandByZlgCanTcp(
      hand_type, hand_device_id, host, port, canfd_channel_id);

  if (!hand) {
    AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create hand (check TCP connection to %s:%d)", host.c_str(), static_cast<int>(port));
    return 1;
  }

  if (!hand->Init()) {
    AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize hand");
    return 1;
  }

  AgilinkLogger::get().infof(TAG, "[OK]: Hand initialized via ZLG CAN TCP");

  // Vendor info
  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Vendor Info:%s", vendor_info.ToString().c_str());

  // Device info
  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Device Info: hand_device_id=%d", static_cast<int>(device_info.hand_device_id));

  // Get positions
  auto positions = hand->GetAllJointMotorPosi();
  AgilinkLogger::get().info("");
  {
    std::string msg;
    for (size_t i = 0; i < positions.size(); ++i) {
      msg += std::to_string(positions[i]);
      if (i < positions.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "Joint positions: [%s]", msg.c_str());
  }

  // Set angles and read back
  std::vector<double> angles(10, 0.0);
  hand->SetAllActiveJointAngles(angles);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  auto angles_read = hand->GetAllActiveJointAngles();
  {
    std::string msg;
    char buf[32];
    for (size_t i = 0; i < angles_read.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", angles_read[i]);
      msg += buf;
      if (i < angles_read.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "Active joint angles (rad): [%s]", msg.c_str());
  }

  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "[Done]: ZLG CAN TCP demo completed.");
  return 0;
}
