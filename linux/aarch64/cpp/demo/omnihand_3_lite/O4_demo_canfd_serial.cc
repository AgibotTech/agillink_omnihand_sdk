// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O4_demo_canfd_serial.cc
 * @brief OmniHand 3 Lite (O4) control demo - CANFD communication (via serial_number)
 *
 * This demo shows how to create and control using device serial number OmniHand 3 Lite dexterous hand (4 DOF)
 * Supports single-hand (left/right) and dual-hand (both) control
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_3_lite_canfd_serial left
 *   ./demo_omnihand_3_lite_canfd_serial right
 *   ./demo_omnihand_3_lite_canfd_serial both
 *
 * Note: serial numbers in code should be updated for your setup
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "omnihand/omnihand_3_lite.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Control left hand only" << std::endl;
  std::cout << "  right  - Control right hand only" << std::endl;
  std::cout << "  both   - Control both hands simultaneously" << std::endl;
  std::cout << std::endl;
  std::cout << "Note: Serial numbers in code need to be modified according to actual devices"
            << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand3Lite>& hand,
                       const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  std::cout << "\nTemperature Reports: [";
  auto temperatures = hand->GetAllTemperatureReport();
  for (size_t i = 0; i < temperatures.size(); ++i)
    std::cout << (i ? ", " : "") << temperatures[i];
  std::cout << "]" << std::endl;

  // ============ Gesture Control Demo ============
  std::cout << "\n=== Gesture Control ===" << std::endl;

  std::cout << "Setting gesture: FIST..." << std::endl;
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_FIST);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto fist_positions = hand->GetAllJointMotorPosi();
  std::cout << "FIST positions: [";
  for (size_t i = 0; i < fist_positions.size(); ++i) {
    std::cout << fist_positions[i];
    if (i < fist_positions.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "Setting gesture: OPEN..." << std::endl;
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_OPEN);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto open_positions = hand->GetAllJointMotorPosi();
  std::cout << "OPEN positions: [";
  for (size_t i = 0; i < open_positions.size(); ++i) {
    std::cout << open_positions[i];
    if (i < open_positions.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "\nSetting joint motor positions (0~4096)..." << std::endl;
  std::vector<int16_t> positions(
      static_cast<size_t>(agilink::omnihand::OmniHand3Lite::kDegreesOfActiveFreedom), 2048);
  hand->SetAllJointMotorPosi(positions);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto read_positions = hand->GetAllJointMotorPosi();
  std::cout << "Joint Motor Positions: [";
  for (size_t i = 0; i < read_positions.size(); ++i) {
    std::cout << read_positions[i];
    if (i < read_positions.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

int main(int argc, char** argv) {
  std::string mode = "left";
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      return 0;
    } else if (arg == "left" || arg == "right" || arg == "both") {
      mode = arg;
    } else {
      std::cerr << "[Error]: Invalid argument: " << arg << std::endl;
      printUsage(argv[0]);
      return 1;
    }
  }

  std::cout << "============================================" << std::endl;
  std::cout << "OmniHand 3 Lite (O4) - CANFD Control (by serial_number)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  // Note: serial numbers should be updated for your setup
  std::string left_serial = "201BFF2A";
  std::string right_serial = "201BFF2B";

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHand3Lite::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, left_serial, 0);
    if (!left_hand) {
      std::cerr << "[Error]: Failed to create left hand instance" << std::endl;
      std::cerr << "Please check if device with serial number is connected" << std::endl;
      return 1;
    }
    if (!left_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize left hand" << std::endl;
      return 1;
    }
    std::cout << "[OK]: Left hand initialized successfully" << std::endl;
    controlSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = agilink::omnihand::OmniHand3Lite::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, right_serial, 0);
    if (!right_hand) {
      std::cerr << "[Error]: Failed to create right hand instance" << std::endl;
      std::cerr << "Please check if device with serial number is connected" << std::endl;
      return 1;
    }
    if (!right_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize right hand" << std::endl;
      return 1;
    }
    std::cout << "[OK]: Right hand initialized successfully" << std::endl;
    if (mode == "right") {
      controlSingleHand(right_hand, "Right");
    } else {
      std::cout << "\n=== Dual Hand (Right) ===" << std::endl;
      auto right_vendor = right_hand->GetVendorInfo();
      std::cout << "Right Hand: " << right_vendor.productModel << " Serial: "
                << right_vendor.productSeqNum << std::endl;
      std::vector<int16_t> pos(4, 2048);
      right_hand->SetAllJointMotorPosi(pos);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      auto rp = right_hand->GetAllJointMotorPosi();
      std::cout << "Right positions: [";
      for (size_t i = 0; i < rp.size(); ++i) std::cout << (i ? ", " : "") << rp[i];
      std::cout << "]" << std::endl;
    }
  }

  std::cout << "\n[Done]: Example completed successfully!" << std::endl;
  return 0;
}
