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

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include "omnihand/omnihand_2025.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right] [host] [port]" << std::endl;
  std::cout << "  left   - Control left hand (default)" << std::endl;
  std::cout << "  right  - Control right hand" << std::endl;
  std::cout << "  host   - TCP server IP (default: 192.168.0.178)" << std::endl;
  std::cout << "  port   - TCP server port (default: 8000)" << std::endl;
  std::cout << std::endl;
  std::cout << "Example:" << std::endl;
  std::cout << "  " << program_name << " left" << std::endl;
  std::cout << "  " << program_name << " right 192.168.0.178 8000" << std::endl;
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
        std::cerr << "[Error]: Invalid port: " << arg << std::endl;
        return 1;
      }
    }
  }

  std::cout << "============================================" << std::endl;
  std::cout << "OmniHand 2025 - ZLG CAN over TCP" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "Server: " << host << ":" << port << std::endl;
  std::cout << "============================================" << std::endl;

  auto hand_type = (mode == "right") ? agilink::omnihand::HandType::RIGHT
                                     : agilink::omnihand::HandType::LEFT;

  auto hand = agilink::omnihand::OmniHand2025::createHandByZlgCanTcp(
      hand_type, hand_device_id, host, port, canfd_channel_id);

  if (!hand) {
    std::cerr << "[Error]: Failed to create hand (check TCP connection to " << host << ":" << port << ")" << std::endl;
    return 1;
  }

  if (!hand->Init()) {
    std::cerr << "[Error]: Failed to initialize hand" << std::endl;
    return 1;
  }

  std::cout << "[OK]: Hand initialized via ZLG CAN TCP" << std::endl;

  // Vendor info
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  // Device info
  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info: hand_device_id=" << static_cast<int>(device_info.hand_device_id) << std::endl;

  // Get positions
  auto positions = hand->GetAllJointMotorPosi();
  std::cout << "\nJoint positions: [";
  for (size_t i = 0; i < positions.size(); ++i) {
    std::cout << positions[i];
    if (i < positions.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // Set angles and read back
  std::vector<double> angles(10, 0.0);
  hand->SetAllActiveJointAngles(angles);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  auto angles_read = hand->GetAllActiveJointAngles();
  std::cout << "Active joint angles (rad): [";
  for (size_t i = 0; i < angles_read.size(); ++i) {
    std::cout << std::fixed << std::setprecision(3) << angles_read[i];
    if (i < angles_read.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "\n[Done]: ZLG CAN TCP demo completed." << std::endl;
  return 0;
}
