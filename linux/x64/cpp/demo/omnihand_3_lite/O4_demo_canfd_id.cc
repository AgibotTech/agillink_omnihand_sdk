// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O4_demo_canfd_id.cc
 * @brief OmniHand 3 Lite (O4) control demo - CANFD communication (via canfd_id)
 *
 * This demo shows how to use canfd_id create and control OmniHand 3 Lite dexterous hand (4 DOF)
 * Supports single-hand (left/right) and dual-hand (both) control
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_3_lite_canfd_id left    # Control left hand
 *   ./demo_omnihand_3_lite_canfd_id right   # Control right hand
 *   ./demo_omnihand_3_lite_canfd_id both    # Control both left and right hands simultaneously
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
  std::cout << "Example:" << std::endl;
  std::cout << "  " << program_name << " left" << std::endl;
  std::cout << "  " << program_name << " both" << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand3Lite>& hand,
                       const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ Read Sensor Data (O4 has no tactile sensors, only temperature/current/error) ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;

  std::cout << "\nTemperature Reports:" << std::endl;
  auto temperatures = hand->GetAllTemperatureReport();
  std::cout << "  All Joint Temperatures (°C): [";
  for (size_t i = 0; i < temperatures.size(); ++i) {
    std::cout << temperatures[i];
    if (i < temperatures.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "\nCurrent Reports:" << std::endl;
  auto currents = hand->GetAllCurrentReport();
  std::cout << "  All Joint Currents: [";
  for (size_t i = 0; i < currents.size(); ++i) {
    std::cout << currents[i];
    if (i < currents.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "\nError Reports:" << std::endl;
  auto errors = hand->GetAllErrorReport();
  for (size_t i = 0; i < errors.size(); ++i) {
    if (errors[i].bits.stalled_ || errors[i].bits.overheat_ || errors[i].bits.over_current_ ||
        errors[i].bits.motor_except_ || errors[i].bits.commu_except_) {
      std::cout << "  Joint " << (i + 1) << ": ";
      if (errors[i].bits.stalled_) std::cout << "Stalled ";
      if (errors[i].bits.overheat_) std::cout << "Overheat ";
      if (errors[i].bits.over_current_) std::cout << "OverCurrent ";
      if (errors[i].bits.motor_except_) std::cout << "MotorException ";
      if (errors[i].bits.commu_except_) std::cout << "CommException ";
      std::cout << std::endl;
    }
  }
  if (std::all_of(errors.begin(), errors.end(), [](const auto& e) {
        return !e.bits.stalled_ && !e.bits.overheat_ && !e.bits.over_current_ &&
               !e.bits.motor_except_ && !e.bits.commu_except_;
      })) {
    std::cout << "  No errors detected" << std::endl;
  }

  // ============ Gesture Control Demo ============
  std::cout << "\n=== Gesture Control ===" << std::endl;

  std::cout << "Setting gesture: FIST..." << std::endl;
  hand->SetHandGesture(agilink::omnihand::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_FIST);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto fist_positions = hand->GetAllJointMotorPosi();
  std::cout << "FIST positions: [";
  for (size_t i = 0; i < fist_positions.size(); ++i) {
    std::cout << fist_positions[i];
    if (i < fist_positions.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "Setting gesture: OPEN..." << std::endl;
  hand->SetHandGesture(agilink::omnihand::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_OPEN);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto open_positions = hand->GetAllJointMotorPosi();
  std::cout << "OPEN positions: [";
  for (size_t i = 0; i < open_positions.size(); ++i) {
    std::cout << open_positions[i];
    if (i < open_positions.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // ============ Motor Position Control Demo (O4 uses motor positions 0~4096, no angle control)============
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
  std::string device_type = "zlgcan";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      std::cout << "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)"
                << std::endl;
      return 0;
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      device_type = argv[++i];
      if (device_type != "zlgcan" && device_type != "hcan") {
        std::cerr << "[Error]: -d value must be 'zlgcan' or 'hcan', got: " << device_type
                  << std::endl;
        return 1;
      }
    } else if (arg == "left" || arg == "right" || arg == "both") {
      mode = arg;
    } else {
      std::cerr << "[Error]: Invalid argument: " << arg << std::endl;
      printUsage(argv[0]);
      return 1;
    }
  }

  std::cout << "============================================" << std::endl;
  std::cout << "OmniHand 3 Lite (O4) - CANFD Control (by canfd_id)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "Device: " << device_type << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  unsigned char canfd_id = 0;

  auto createHand = [&](agilink::omnihand::HandType hand_type, unsigned char channel_id) {
    if (device_type == "hcan") {
      return agilink::omnihand::OmniHand3Lite::createHandByHcan(hand_type, device_id, canfd_id,
                                                                channel_id);
    } else {
      return agilink::omnihand::OmniHand3Lite::createHandByZlgcan(hand_type, device_id, canfd_id,
                                                                  channel_id);
    }
  };

  if (mode == "left") {
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);
    if (!left_hand) {
      std::cerr << "[Error]: Failed to create left hand instance" << std::endl;
      return 1;
    }
    if (!left_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize left hand" << std::endl;
      return 1;
    }
    std::cout << "[OK]: Left hand initialized successfully" << std::endl;
    controlSingleHand(left_hand, "Left");
  } else if (mode == "right") {
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 0);
    if (!right_hand) {
      std::cerr << "[Error]: Failed to create right hand instance" << std::endl;
      return 1;
    }
    if (!right_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize right hand" << std::endl;
      return 1;
    }
    std::cout << "[OK]: Right hand initialized successfully" << std::endl;
    controlSingleHand(right_hand, "Right");
  } else if (mode == "both") {
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 1);
    if (!left_hand || !right_hand) {
      std::cerr << "[Error]: Failed to create hand instances" << std::endl;
      return 1;
    }
    if (!left_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize left hand" << std::endl;
      return 1;
    }
    if (!right_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize right hand" << std::endl;
      return 1;
    }
    std::cout << "[OK]: Both hands initialized successfully" << std::endl;

    std::cout << "\n=== Dual Hand Control ===" << std::endl;
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();
    std::cout << "\nLeft Hand: " << left_vendor.productModel << " Serial: "
              << left_vendor.productSeqNum << std::endl;
    std::cout << "Right Hand: " << right_vendor.productModel << " Serial: "
              << right_vendor.productSeqNum << std::endl;

    std::vector<int16_t> left_pos(4, 2048);
    std::vector<int16_t> right_pos(4, 2048);
    left_hand->SetAllJointMotorPosi(left_pos);
    right_hand->SetAllJointMotorPosi(right_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto left_read = left_hand->GetAllJointMotorPosi();
    auto right_read = right_hand->GetAllJointMotorPosi();
    std::cout << "Left positions: [";
    for (size_t i = 0; i < left_read.size(); ++i)
      std::cout << (i ? ", " : "") << left_read[i];
    std::cout << "]" << std::endl;
    std::cout << "Right positions: [";
    for (size_t i = 0; i < right_read.size(); ++i)
      std::cout << (i ? ", " : "") << right_read[i];
    std::cout << "]" << std::endl;
  }

  std::cout << "\n[Done]: Example completed successfully!" << std::endl;
  return 0;
}
