// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O12_demo_canfd_id.cc
 * @brief OmniHand Pro 2025 control demo - CANFD communication (via canfd_id)
 *
 * This demo shows how to use canfd_id create and control OmniHand Pro 2025 dexterous hand
 * Supports single-hand (left/right) and dual-hand (both) control
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_pro_2025_canfd_id left              # Control left hand with ZLG CAN
 *   ./demo_omnihand_pro_2025_canfd_id right -d hcan     # Control right hand with HCAN
 *   ./demo_omnihand_pro_2025_canfd_id both -d zlgcan    # Control both left and right hands simultaneously
 */

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "omnihand/omnihand_pro_2025.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both] [-d zlgcan|hcan]" << std::endl;
  std::cout << "  left   - Control left hand only" << std::endl;
  std::cout << "  right  - Control right hand only" << std::endl;
  std::cout << "  both   - Control both hands simultaneously" << std::endl;
  std::cout << "  -d, --device DEVICE" << std::endl;
  std::cout << "         Set CAN device type (zlgcan or hcan, default: zlgcan)" << std::endl;
  std::cout << std::endl;
  std::cout << "Example:" << std::endl;
  std::cout << "  " << program_name << " left" << std::endl;
  std::cout << "  " << program_name << " right -d hcan" << std::endl;
  std::cout << "  " << program_name << " both -d zlgcan" << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHandPro2025>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ Read Sensor Data ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;

  // Read 3D tactile sensor data (O12-specific)
  std::cout << "\n3D Tactile Sensor Data (O12 only):" << std::endl;
  try {
    auto thumb_sensor = hand->GetTactileSensor3DData(agilink::omnihand::Finger::THUMB);
    std::cout << "  Thumb:" << std::endl;
    std::cout << "    Online State: " << (thumb_sensor.online_state ? "Online" : "Offline") << std::endl;
    std::cout << "    Normal Force: " << thumb_sensor.normal_force << " (0.1N, max: 3000)" << std::endl;
    std::cout << "    Tangent Force: " << thumb_sensor.tangent_force << std::endl;
    std::cout << "    Tangent Force Angle: " << thumb_sensor.tangent_force_angle << "°" << std::endl;
    std::cout << "    Channel Values: [";
    for (size_t i = 0; i < agilink::omnihand::TactileSensor3DData::kChannelCount; ++i) {
      std::cout << thumb_sensor.channel_value[i];
      if (i + 1 < agilink::omnihand::TactileSensor3DData::kChannelCount) std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    auto index_sensor = hand->GetTactileSensor3DData(agilink::omnihand::Finger::INDEX);
    std::cout << "  Index:" << std::endl;
    std::cout << "    Online State: " << (index_sensor.online_state ? "Online" : "Offline") << std::endl;
    std::cout << "    Normal Force: " << index_sensor.normal_force << " (0.1N, max: 3000)" << std::endl;
    std::cout << "    Tangent Force: " << index_sensor.tangent_force << std::endl;
    std::cout << "    Tangent Force Angle: " << index_sensor.tangent_force_angle << "°" << std::endl;
  } catch (const std::exception& e) {
    std::cout << "  Warning: " << e.what() << std::endl;
  }

  // Read temperature report
  std::cout << "\nTemperature Reports:" << std::endl;
  auto temperatures = hand->GetAllTemperatureReport();
  std::cout << "  All Joint Temperatures (°C): [";
  for (size_t i = 0; i < temperatures.size(); ++i) {
    std::cout << temperatures[i];
    if (i < temperatures.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // Read current report
  std::cout << "\nCurrent Reports:" << std::endl;
  auto currents = hand->GetAllCurrentReport();
  std::cout << "  All Joint Currents: [";
  for (size_t i = 0; i < currents.size(); ++i) {
    std::cout << currents[i];
    if (i < currents.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // Read error report
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

  // Read velocity (read-only, not control)
  std::cout << "\nJoint Velocities:" << std::endl;
  auto velocities = hand->GetAllJointMotorVelo();
  std::cout << "  All Joint Velocities: [";
  for (size_t i = 0; i < velocities.size(); ++i) {
    std::cout << velocities[i];
    if (i < velocities.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // ============ Joint Angle Control Demo ============
  // Use joint-angle control (recommended; underlying layer auto-converts)
  std::cout << "\nSetting joint angles..." << std::endl;
  for (int i = 1; i <= agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom; i++)
    hand->SetControlMode(i, agilink::omnihand::ControlMode::POSITION);

  std::vector<double> angles(12, 0.6);  // O12 has 12 active joints
  hand->SetAllActiveJointAngles(angles);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  for (auto& angle : angles) {
    angle = 0.0;
  }
  hand->SetAllActiveJointAngles(angles);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Read joint angle
  auto active_angles = hand->GetAllActiveJointAngles();
  std::cout << "Active Joint Angles (rad): [";
  for (size_t i = 0; i < active_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << active_angles[i];
    if (i < active_angles.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // Read all joint angles
  auto all_angles = hand->GetAllJointAngles();
  std::cout << "All Joint Angles (rad, " << all_angles.size() << " joints): [";
  for (size_t i = 0; i < all_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << all_angles[i];
    if (i < all_angles.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

int main(int argc, char** argv) {
  // Parse command-line arguments
  std::string mode = "left";           // default left hand
  std::string device_type = "zlgcan";  // default zlgcan

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      return 0;
    } else if (arg == "-d" || arg == "--device") {
      if (i + 1 >= argc) {
        std::cerr << "[Error]: " << arg << " requires a value (zlgcan or hcan)" << std::endl;
        printUsage(argv[0]);
        return 1;
      }
      device_type = argv[++i];
      if (device_type != "zlgcan" && device_type != "hcan") {
        std::cerr << "[Error]: -d value must be 'zlgcan' or 'hcan', got: " << device_type << std::endl;
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
  std::cout << "OmniHand Pro 2025 - CANFD Control (by canfd_id)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "Device: " << device_type << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  unsigned char canfd_id = 0;

  auto createHand = [&](agilink::omnihand::HandType hand_type, unsigned char channel_id) {
    if (device_type == "hcan") {
      return agilink::omnihand::OmniHandPro2025::createHandByHcan(
          hand_type,
          device_id,
          canfd_id,
          channel_id);
    }
    return agilink::omnihand::OmniHandPro2025::createHandByZlgcan(
        hand_type,
        device_id,
        canfd_id,
        channel_id);
  };

  if (mode == "left") {
    // Create left-hand instance
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
    // Create right-hand instance
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
    // both mode: create both hands
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

    // Control both hands simultaneously
    std::cout << "\n=== Dual Hand Control ===" << std::endl;

    // Get device info
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();

    std::cout << "\nLeft Hand Info:" << std::endl;
    std::cout << "  Model: " << left_vendor.productModel << std::endl;
    std::cout << "  Serial: " << left_vendor.productSeqNum << std::endl;

    std::cout << "\nRight Hand Info:" << std::endl;
    std::cout << "  Model: " << right_vendor.productModel << std::endl;
    std::cout << "  Serial: " << right_vendor.productSeqNum << std::endl;

    // Use joint-angle control
    std::cout << "\nSetting joint angles..." << std::endl;
    std::vector<double> left_angles(12, 0.0);
    std::vector<double> right_angles(12, 0.5);

    left_hand->SetAllActiveJointAngles(left_angles);
    right_hand->SetAllActiveJointAngles(right_angles);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Read joint angle
    auto left_angles_read = left_hand->GetAllActiveJointAngles();
    auto right_angles_read = right_hand->GetAllActiveJointAngles();

    std::cout << "Left Hand Angles (rad): [";
    for (size_t i = 0; i < left_angles_read.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << left_angles_read[i];
      if (i < left_angles_read.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Right Hand Angles (rad): [";
    for (size_t i = 0; i < right_angles_read.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << right_angles_read[i];
      if (i < right_angles_read.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
  }

  std::cout << "\n[Done]: Example completed successfully!" << std::endl;
  return 0;
}
