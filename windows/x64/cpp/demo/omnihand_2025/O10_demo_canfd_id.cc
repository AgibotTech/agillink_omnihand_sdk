// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O10_demo_canfd_id.cc
 * @brief OmniHand 2025 control demo - CANFD communication (via canfd_id)
 *
 * This demo shows how to use canfd_id create and control OmniHand 2025 dexterous hand
 * Supports single-hand (left/right) and dual-hand (both) control
 *
 * Build: cmake .. && make
 * Run:
 *   ./example_canfd_id left    # Control left hand
 *   ./example_canfd_id right   # Control right hand
 *   ./example_canfd_id both    # Control both left and right hands simultaneously
 */

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"
#include "omnihand/omnihand_2025.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025Demo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand only");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand only");
  AgilinkLogger::get().infof(TAG, "  both   - Control both hands simultaneously");
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Example:");
  AgilinkLogger::get().infof(TAG, "  %s left", program_name);
  AgilinkLogger::get().infof(TAG, "  %s both", program_name);
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand2025>& hand, const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "=== %s Hand Control ===", hand_name.c_str());

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Vendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Device Info:%s", device_info.ToString().c_str());

  // ============ Read Sensor Data ============
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "=== Reading Sensor Data ===");

  // Read tactile sensor data
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Tactile Sensor Data (1D):");
  try {
    auto thumb_tactile = hand->GetTactileSensorData(agilink::omnihand::Finger::THUMB);
    {
      std::string msg;
      for (size_t i = 0; i < thumb_tactile.size(); ++i) {
        msg += std::to_string(static_cast<int>(thumb_tactile[i]));
        if (i < thumb_tactile.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "  Thumb: [%s] (unit: 1g, max: 255g)", msg.c_str());
    }

    auto index_tactile = hand->GetTactileSensorData(agilink::omnihand::Finger::INDEX);
    {
      std::string msg;
      for (size_t i = 0; i < index_tactile.size(); ++i) {
        msg += std::to_string(static_cast<int>(index_tactile[i]));
        if (i < index_tactile.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "  Index: [%s] (unit: 1g, max: 255g)", msg.c_str());
    }
  } catch (const std::exception& e) {
    AgilinkLogger::get().infof(TAG, "  Warning: %s", e.what());
  }

  // Read temperature report
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Temperature Reports:");
  auto temperatures = hand->GetAllTemperatureReport();
  {
    std::string msg;
    for (size_t i = 0; i < temperatures.size(); ++i) {
      msg += std::to_string(temperatures[i]);
      if (i < temperatures.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  All Joint Temperatures (°C): [%s]", msg.c_str());
  }

  // Read current report
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Current Reports:");
  auto currents = hand->GetAllCurrentReport();
  {
    std::string msg;
    for (size_t i = 0; i < currents.size(); ++i) {
      msg += std::to_string(currents[i]);
      if (i < currents.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  All Joint Currents: [%s]", msg.c_str());
  }

  // Read error report
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Error Reports:");
  auto errors = hand->GetAllErrorReport();
  for (size_t i = 0; i < errors.size(); ++i) {
    if (errors[i].bits.stalled_ || errors[i].bits.overheat_ || errors[i].bits.over_current_ ||
        errors[i].bits.motor_except_ || errors[i].bits.commu_except_) {
      {
        std::string msg;
        if (errors[i].bits.stalled_) msg += "Stalled ";
        if (errors[i].bits.overheat_) msg += "Overheat ";
        if (errors[i].bits.over_current_) msg += "OverCurrent ";
        if (errors[i].bits.motor_except_) msg += "MotorException ";
        if (errors[i].bits.commu_except_) msg += "CommException ";
        AgilinkLogger::get().infof(TAG, "  Joint %zu: %s", i + 1, msg.c_str());
      }
    }
  }
  if (std::all_of(errors.begin(), errors.end(), [](const auto& e) {
        return !e.bits.stalled_ && !e.bits.overheat_ && !e.bits.over_current_ &&
               !e.bits.motor_except_ && !e.bits.commu_except_;
      })) {
    AgilinkLogger::get().infof(TAG, "  No errors detected");
  }

  // Read velocity (read-only, not control)
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Joint Velocities:");
  auto velocities = hand->GetAllJointMotorVelo();
  {
    std::string msg;
    for (size_t i = 0; i < velocities.size(); ++i) {
      msg += std::to_string(velocities[i]);
      if (i < velocities.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  All Joint Velocities: [%s]", msg.c_str());
  }

  // ============ Joint Angle Control Demo ============
  // Use joint-angle control (recommended; underlying layer auto-converts)
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Setting joint angles...");
  std::vector<double> angles(10, 0.0);
  hand->SetAllActiveJointAngles(angles);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Read joint angle
  auto active_angles = hand->GetAllActiveJointAngles();
  {
    std::string msg;
    char buf[32];
    for (size_t i = 0; i < active_angles.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", active_angles[i]);
      msg += buf;
      if (i < active_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "Active Joint Angles (rad): [%s]", msg.c_str());
  }

  // Read all joint angles
  auto all_angles = hand->GetAllJointAngles();
  {
    std::string msg;
    char buf[32];
    for (size_t i = 0; i < all_angles.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", all_angles[i]);
      msg += buf;
      if (i < all_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "All Joint Angles (rad, %zu joints): [%s]", all_angles.size(), msg.c_str());
  }
}

int main(int argc, char** argv) {
  // Parse command-line arguments
  std::string mode = "left";  // default left hand
  std::string device_type = "zlgcan";  // default zlgcan

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      AgilinkLogger::get().infof(TAG, "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)");
      return 0;
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      device_type = argv[++i];
      if (device_type != "zlgcan" && device_type != "hcan") {
        AgilinkLogger::get().errorf(TAG, "[Error]: -d value must be 'zlgcan' or 'hcan', got: %s", device_type.c_str());
        return 1;
      }
    } else if (arg == "left" || arg == "right" || arg == "both") {
      mode = arg;
    } else {
      AgilinkLogger::get().errorf(TAG, "[Error]: Invalid argument: %s", arg.c_str());
      printUsage(argv[0]);
      return 1;
    }
  }

  AgilinkLogger::get().infof(TAG, "============================================");
  AgilinkLogger::get().infof(TAG, "OmniHand 2025 - CANFD Control (by canfd_id)");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "Device: %s", device_type.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

  unsigned char device_id = 1;
  unsigned char canfd_id = 0;

  // Helper function to create hand instance
  auto createHand = [&](agilink::omnihand::HandType hand_type, unsigned char channel_id) {
    if (device_type == "hcan") {
      return agilink::omnihand::OmniHand2025::createHandByHcan(
          hand_type, device_id, canfd_id, channel_id);
    } else {  // default: zlgcan
      return agilink::omnihand::OmniHand2025::createHandByZlgcan(
          hand_type, device_id, canfd_id, channel_id);
    }
  };

  if (mode == "left") {
    // Create left-hand instance
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);

    if (!left_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully");
    controlSingleHand(left_hand, "Left");
  } else if (mode == "right") {
    // Create right-hand instance
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 0);

    if (!right_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      return 1;
    }

    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized successfully");
    controlSingleHand(right_hand, "Right");
  } else if (mode == "both") {
    // both mode: create both hands
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);  // first channel
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 1);  // second channel

    if (!left_hand || !right_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create hand instances");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }

    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Both hands initialized successfully");

    // Control both hands simultaneously
    AgilinkLogger::get().infof(TAG, "");
    AgilinkLogger::get().infof(TAG, "=== Dual Hand Control ===");

    // Get device info
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();

    AgilinkLogger::get().infof(TAG, "");
    AgilinkLogger::get().infof(TAG, "Left Hand Info:");
    AgilinkLogger::get().infof(TAG, "  Model: %s", left_vendor.productModel.c_str());
    AgilinkLogger::get().infof(TAG, "  Serial: %s", left_vendor.productSeqNum.c_str());

    AgilinkLogger::get().infof(TAG, "");
    AgilinkLogger::get().infof(TAG, "Right Hand Info:");
    AgilinkLogger::get().infof(TAG, "  Model: %s", right_vendor.productModel.c_str());
    AgilinkLogger::get().infof(TAG, "  Serial: %s", right_vendor.productSeqNum.c_str());

    // ============ Joint Angle Control Demo ============
    // Use joint-angle control (recommended; underlying layer auto-converts)
    AgilinkLogger::get().infof(TAG, "");
    AgilinkLogger::get().infof(TAG, "Setting joint angles...");
    std::vector<double> left_angles(10, 0.0);
    std::vector<double> right_angles(10, 0.5);

    left_hand->SetAllActiveJointAngles(left_angles);
    right_hand->SetAllActiveJointAngles(right_angles);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Read joint angle
    auto left_angles_read = left_hand->GetAllActiveJointAngles();
    auto right_angles_read = right_hand->GetAllActiveJointAngles();

    {
      std::string msg;
      char buf[32];
      for (size_t i = 0; i < left_angles_read.size(); ++i) {
        snprintf(buf, sizeof(buf), "%.4f", left_angles_read[i]);
        msg += buf;
        if (i < left_angles_read.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "Left Hand Angles (rad): [%s]", msg.c_str());
    }

    {
      std::string msg;
      char buf[32];
      for (size_t i = 0; i < right_angles_read.size(); ++i) {
        snprintf(buf, sizeof(buf), "%.4f", right_angles_read[i]);
        msg += buf;
        if (i < right_angles_read.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "Right Hand Angles (rad): [%s]", msg.c_str());
    }
  }

  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "[Done]: Example completed successfully!");
  return 0;
}
