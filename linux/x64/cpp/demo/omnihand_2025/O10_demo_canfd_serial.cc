// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O10_demo_canfd_serial.cc
 * @brief OmniHand 2025 control demo - CANFD communication (via serial_number)
 * 
 * This demo shows how to create and control using device serial number OmniHand 2025 dexterous hand
 * Supports single-hand (left/right) and dual-hand (both) control
 * 
 * Build: cmake .. && make
 * Run: 
 *   ./example_canfd_serial left              # Control left hand (default zlgcan)
 *   ./example_canfd_serial right -d hcan     # Control right hand via HCAN
 *   ./example_canfd_serial both -d socketcan # Control both via SocketCAN
 *   ./example_canfd_serial left -d zlgcan    # Control left hand via ZLG CAN
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
#include "omnihand/omnihand_2025.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both] [-d DEVICE]" << std::endl;
  std::cout << "  left   - Control left hand only" << std::endl;
  std::cout << "  right  - Control right hand only" << std::endl;
  std::cout << "  both   - Control both hands simultaneously" << std::endl;
  std::cout << "  -d DEVICE  Set CAN device type (zlgcan, hcan, or socketcan, default: zlgcan)" << std::endl;
  std::cout << std::endl;
  std::cout << "Note: Serial numbers in code need to be modified according to actual devices" << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand2025>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ Read Sensor Data ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;
  
  // Read tactile sensor data
  std::cout << "\nTactile Sensor Data (1D):" << std::endl;
  try {
    auto thumb_tactile = hand->GetTactileSensorData(agilink::omnihand::Finger::THUMB);
    std::cout << "  Thumb: [";
    for (size_t i = 0; i < thumb_tactile.size(); ++i) {
      std::cout << static_cast<int>(thumb_tactile[i]);
      if (i < thumb_tactile.size() - 1) std::cout << ", ";
    }
    std::cout << "] (unit: 1g, max: 255g)" << std::endl;
    
    auto index_tactile = hand->GetTactileSensorData(agilink::omnihand::Finger::INDEX);
    std::cout << "  Index: [";
    for (size_t i = 0; i < index_tactile.size(); ++i) {
      std::cout << static_cast<int>(index_tactile[i]);
      if (i < index_tactile.size() - 1) std::cout << ", ";
    }
    std::cout << "] (unit: 1g, max: 255g)" << std::endl;
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
  std::vector<double> angles(10, 0.0);
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
}

int main(int argc, char** argv) {
  std::string mode = "left";
  std::string device_type = "zlgcan";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      return 0;
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      device_type = argv[++i];
      if (device_type != "zlgcan" && device_type != "hcan" && device_type != "socketcan") {
        std::cerr << "[Error]: -d value must be 'zlgcan', 'hcan', or 'socketcan', got: " << device_type << std::endl;
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
  std::cout << "OmniHand 2025 - CANFD Control (by serial_number)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "Device: " << device_type << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  std::string left_serial = "201BFF2A";
  std::string right_serial = "201BFF2B";

  auto createHand = [&](agilink::omnihand::HandType hand_type, const std::string& serial, uint8_t channel_id) {
    if (device_type == "hcan") {
      return agilink::omnihand::OmniHand2025::createHandByHcan(
          hand_type, device_id, serial, channel_id);
    } else if (device_type == "socketcan") {
#ifdef __linux__
      return agilink::omnihand::OmniHand2025::createHandSocketCan(
          hand_type, device_id, serial);
#else
      std::cerr << "[Error]: SocketCAN is only supported on Linux" << std::endl;
      return std::unique_ptr<agilink::omnihand::OmniHand2025>(nullptr);
#endif
    } else {
      return agilink::omnihand::OmniHand2025::createHandByZlgcan(
          hand_type, device_id, serial, channel_id);
    }
  };

  if (mode == "left" || mode == "both") {
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, left_serial, 0);

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
    if (mode == "left") {
      controlSingleHand(left_hand, "Left");
    }
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, right_serial, 0);

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
      std::cout << "\n=== Dual Hand Control ===" << std::endl;

      auto left_hand = createHand(agilink::omnihand::HandType::LEFT, left_serial, 0);
      if (!left_hand || !left_hand->Init()) {
        std::cerr << "[Error]: Failed to initialize left hand for dual mode" << std::endl;
        return 1;
      }

      auto left_vendor = left_hand->GetVendorInfo();
      auto right_vendor = right_hand->GetVendorInfo();

      std::cout << "\nLeft Hand Info:" << std::endl;
      std::cout << "  Model: " << left_vendor.productModel << std::endl;
      std::cout << "  Serial: " << left_vendor.productSeqNum << std::endl;

      std::cout << "\nRight Hand Info:" << std::endl;
      std::cout << "  Model: " << right_vendor.productModel << std::endl;
      std::cout << "  Serial: " << right_vendor.productSeqNum << std::endl;

      std::cout << "\nSetting joint angles..." << std::endl;
      std::vector<double> left_angles(10, 0.0);
      std::vector<double> right_angles(10, 0.5);

      left_hand->SetAllActiveJointAngles(left_angles);
      right_hand->SetAllActiveJointAngles(right_angles);

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
  }

  std::cout << "\n[Done]: Example completed successfully!" << std::endl;
  return 0;
}
