// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O10_demo_rs485.cc
 * @brief OmniHand 2025 control demo - RS485 communication
 * 
 * This demo shows how to use RS485 serial communication to create and control OmniHand 2025 dexterous hand
 * Supports single-hand (left/right) and dual-hand (both) control
 * 
 * Build: cmake .. && make
 * Run: 
 *   ./example_rs485 left    # Control left hand
 *   ./example_rs485 right   # Control right hand
 *   ./example_rs485 both    # Control both left and right hands simultaneously
 * 
 * Note: you need to update serial port path in code (e.g. /dev/ttyUSB0 or COM3)
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
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Control left hand only" << std::endl;
  std::cout << "  right  - Control right hand only" << std::endl;
  std::cout << "  both   - Control both hands simultaneously" << std::endl;
  std::cout << std::endl;
  std::cout << "Note: Serial port paths in code need to be modified according to actual devices" << std::endl;
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
  
  // Note: RS485 does not support raw tactile sensor data reading (GetTactileSensorDataRaw)
  // But it supports GetTactileSensorData (downsampled data)
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

  // Note: RS485 does not support temperature/current/error report/velocity reads
  std::cout << "\nNote: RS485 communication does not support temperature/current/error/velocity reports" << std::endl;

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
  std::cout << "OmniHand 2025 - RS485 Control" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  // Note: serial port path should be updated for your setup
  std::string left_port = "/dev/ttyUSB0";   // Linux: /dev/ttyUSB0, Windows: COM3
  std::string right_port = "/dev/ttyUSB1";  // Linux: /dev/ttyUSB1, Windows: COM4
  int32_t baudrate = 460800;

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHand2025::createHandByRs485(
        agilink::omnihand::HandType::LEFT,
        device_id,
        left_port,
        baudrate
    );

    if (!left_hand) {
      std::cerr << "[Error]: Failed to create left hand instance" << std::endl;
      return 1;
    }

    if (!left_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize left hand" << std::endl;
      std::cerr << "Please check:" << std::endl;
      std::cerr << "  1. Serial port exists: ls -l /dev/ttyUSB*" << std::endl;
      std::cerr << "  2. Permission: sudo chmod 666 /dev/ttyUSB0" << std::endl;
      std::cerr << "  3. Device is connected and powered on" << std::endl;
      return 1;
    }

    std::cout << "[OK]: Left hand initialized successfully" << std::endl;
    controlSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = agilink::omnihand::OmniHand2025::createHandByRs485(
        agilink::omnihand::HandType::RIGHT,
        device_id,
        right_port,
        baudrate
    );

    if (!right_hand) {
      std::cerr << "[Error]: Failed to create right hand instance" << std::endl;
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
      // both mode: control simultaneously
      std::cout << "\n=== Dual Hand Control ===" << std::endl;
      
      auto left_hand = agilink::omnihand::OmniHand2025::createHandByRs485(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_port,
          baudrate
      );
      if (!left_hand || !left_hand->Init()) {
        std::cerr << "[Error]: Failed to initialize left hand for dual mode" << std::endl;
        return 1;
      }

      // Use joint-angle control (recommended; underlying layer auto-converts)
      std::cout << "\nSetting joint angles for both hands..." << std::endl;
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
