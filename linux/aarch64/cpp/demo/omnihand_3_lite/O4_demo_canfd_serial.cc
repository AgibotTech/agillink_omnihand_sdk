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

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"
#include "omnihand/omnihand_3_lite.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand3LiteDemo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand only");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand only");
  AgilinkLogger::get().infof(TAG, "  both   - Control both hands simultaneously");
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Note: Serial numbers in code need to be modified according to actual devices");
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand3Lite>& hand,
                       const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "\n=== %s Hand Control ===", hand_name.c_str());

  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "\nVendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "\nDevice Info:%s", device_info.ToString().c_str());

  auto temperatures = hand->GetAllTemperatureReport();
  {
    std::string msg = "\nTemperature Reports: [";
    for (size_t i = 0; i < temperatures.size(); ++i) {
      if (i) msg += ", ";
      msg += std::to_string(temperatures[i]);
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // ============ Gesture Control Demo ============
  AgilinkLogger::get().infof(TAG, "\n=== Gesture Control ===");

  AgilinkLogger::get().infof(TAG, "Setting gesture: FIST...");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_FIST);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto fist_positions = hand->GetAllJointMotorPosi();
  {
    std::string msg = "FIST positions: [";
    for (size_t i = 0; i < fist_positions.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(fist_positions[i]);
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "Setting gesture: OPEN...");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_OPEN);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto open_positions = hand->GetAllJointMotorPosi();
  {
    std::string msg = "OPEN positions: [";
    for (size_t i = 0; i < open_positions.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(open_positions[i]);
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "\nSetting joint motor positions (0~4096)...");
  std::vector<int16_t> positions(
      static_cast<size_t>(agilink::omnihand::OmniHand3Lite::kDegreesOfActiveFreedom), 2048);
  hand->SetAllJointMotorPosi(positions);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto read_positions = hand->GetAllJointMotorPosi();
  {
    std::string msg = "Joint Motor Positions: [";
    for (size_t i = 0; i < read_positions.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(read_positions[i]);
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Invalid argument: %s", arg.c_str());
      printUsage(argv[0]);
      return 1;
    }
  }

  AgilinkLogger::get().infof(TAG, "============================================");
  AgilinkLogger::get().infof(TAG, "OmniHand 3 Lite (O4) - CANFD Control (by serial_number)");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

  unsigned char device_id = 1;
  // Note: serial numbers should be updated for your setup
  std::string left_serial = "201BFF2A";
  std::string right_serial = "201BFF2B";

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHand3Lite::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, left_serial, 0);
    if (!left_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      AgilinkLogger::get().errorf(TAG, "Please check if device with serial number is connected");
      return 1;
    }
    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }
    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully");
    controlSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = agilink::omnihand::OmniHand3Lite::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, right_serial, 0);
    if (!right_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      AgilinkLogger::get().errorf(TAG, "Please check if device with serial number is connected");
      return 1;
    }
    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }
    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized successfully");
    if (mode == "right") {
      controlSingleHand(right_hand, "Right");
    } else {
      AgilinkLogger::get().infof(TAG, "\n=== Dual Hand (Right) ===");
      auto right_vendor = right_hand->GetVendorInfo();
      AgilinkLogger::get().infof(TAG, "Right Hand: %s Serial: %s", right_vendor.productModel.c_str(), right_vendor.productSeqNum.c_str());
      std::vector<int16_t> pos(4, 2048);
      right_hand->SetAllJointMotorPosi(pos);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      auto rp = right_hand->GetAllJointMotorPosi();
      {
        std::string msg = "Right positions: [";
        for (size_t i = 0; i < rp.size(); ++i) {
          if (i) msg += ", ";
          msg += std::to_string(rp[i]);
        }
        msg += "]";
        AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
      }
    }
  }

  AgilinkLogger::get().infof(TAG, "\n[Done]: Example completed successfully!");
  return 0;
}
