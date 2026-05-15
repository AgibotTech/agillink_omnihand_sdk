// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file UMI_demo_canfd_id.cc
 * @brief OmniHand Dex UMI control demo - CANFD communication (via canfd_id)
 * 
 * This demo shows how to use canfd_id create and read OmniHand Dex UMI dexterous-hand data
 * Supports single-hand (left/right) and dual-hand (both) control
 * 
 * Note: UMI protocol is read-only and does not support position/velocity/torque control
 * 
 * Build: cmake .. && make
 * Run: 
 *   ./demo_omnihand_dex_umi_canfd_id left    # Read left-hand data
 *   ./demo_omnihand_dex_umi_canfd_id right   # Read right-hand data
 *   ./demo_omnihand_dex_umi_canfd_id both    # Read both left and right hand data simultaneously
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "omnihand/omnihand_dex_umi.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Read left hand data only" << std::endl;
  std::cout << "  right  - Read right hand data only" << std::endl;
  std::cout << "  both   - Read both hands data simultaneously" << std::endl;
  std::cout << std::endl;
  std::cout << "Note: UMI protocol is read-only, position/velocity/torque control is not supported" << std::endl;
}

void readSingleHand(std::unique_ptr<agilink::omnihand::OmniHandDexUMI>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Data Reading ===" << std::endl;

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ Read Sensor Data ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;
  
  // Note: UMI protocol supports active joint position query
  // Use GetJointMotorPosi() or GetAllJointMotorPosi() to obtain position data
  std::cout << "\nNote: UMI protocol supports active position query." << std::endl;
  std::cout << "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data." << std::endl;

  // Read tactile sensor data (1D, using Raw API)
  std::cout << "\n1D Tactile Sensor Data (Raw):" << std::endl;
  try {
    auto thumb_sensor = hand->GetTactileSensorDataRaw(agilink::omnihand::Finger::THUMB);
    std::cout << "  Thumb: [";
    for (size_t i = 0; i < thumb_sensor.data_.size(); ++i) {
      std::cout << static_cast<int>(thumb_sensor.data_[i]);
      if (i < thumb_sensor.data_.size() - 1) std::cout << ", ";
    }
    std::cout << "] (unit: 1g, max: 255g)" << std::endl;
    
    auto index_sensor = hand->GetTactileSensorDataRaw(agilink::omnihand::Finger::INDEX);
    std::cout << "  Index: [";
    for (size_t i = 0; i < index_sensor.data_.size(); ++i) {
      std::cout << static_cast<int>(index_sensor.data_[i]);
      if (i < index_sensor.data_.size() - 1) std::cout << ", ";
    }
    std::cout << "] (unit: 1g, max: 255g)" << std::endl;
    
    auto middle_sensor = hand->GetTactileSensorDataRaw(agilink::omnihand::Finger::MIDDLE);
    std::cout << "  Middle: [";
    for (size_t i = 0; i < middle_sensor.data_.size(); ++i) {
      std::cout << static_cast<int>(middle_sensor.data_[i]);
      if (i < middle_sensor.data_.size() - 1) std::cout << ", ";
    }
    std::cout << "] (unit: 1g, max: 255g)" << std::endl;
    
    // Read all sensor data
    std::cout << "\nAll Tactile Sensor Data:" << std::endl;
    auto all_sensors = hand->GetAllTactileSensorDataRaw();
    for (const auto& sensor : all_sensors) {
      std::string finger_name;
      switch (sensor.sensor_id_) {
        case agilink::omnihand::Finger::THUMB: finger_name = "Thumb"; break;
        case agilink::omnihand::Finger::INDEX: finger_name = "Index"; break;
        case agilink::omnihand::Finger::MIDDLE: finger_name = "Middle"; break;
        case agilink::omnihand::Finger::RING: finger_name = "Ring"; break;
        case agilink::omnihand::Finger::LITTLE: finger_name = "Little"; break;
        case agilink::omnihand::Finger::PALM: finger_name = "Palm"; break;
        // Note: UMI does not have Dorsum sensor
        default: finger_name = "Unknown"; break;
      }
      std::cout << "  " << finger_name << ": " << sensor.data_.size() << " points" << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << "  Warning: " << e.what() << std::endl;
  }
}

int main(int argc, char** argv) {
  // Parse command-line arguments?
  std::string mode = "left";  // default left hand
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
  std::cout << "OmniHand Dex UMI - CANFD Control (by canfd_id)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  unsigned char canfd_id = 0;

  if (mode == "left") {
    // Create left-hand instance
    auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT,
        device_id,
        canfd_id,
        0  // channel_id (first channel)
    );

    if (!left_hand) {
      std::cerr << "[Error]: Failed to create left hand instance" << std::endl;
      return 1;
    }

    if (!left_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize left hand" << std::endl;
      return 1;
    }

    std::cout << "[OK]: Left hand initialized successfully" << std::endl;
    readSingleHand(left_hand, "Left");
  } else if (mode == "right") {
    // Create right-hand instance
    auto right_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT,
        device_id,
        canfd_id,
        0  // channel_id (first channel)
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
    readSingleHand(right_hand, "Right");
  } else if (mode == "both") {
    // both mode: read both hands simultaneously
    auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT,
        device_id,
        canfd_id,
        0  // channel_id (first channel)
    );

    auto right_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT,
        device_id,
        canfd_id,
        1  // channel_id (second channel)
    );

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

    // Read data from both hands simultaneously
    std::cout << "\n=== Dual Hand Data Reading ===" << std::endl;
    
    // Get device info
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();
    
    std::cout << "\nLeft Hand Info:" << std::endl;
    std::cout << "  Model: " << left_vendor.productModel << std::endl;
    std::cout << "  Serial: " << left_vendor.productSeqNum << std::endl;
    
    std::cout << "\nRight Hand Info:" << std::endl;
    std::cout << "  Model: " << right_vendor.productModel << std::endl;
    std::cout << "  Serial: " << right_vendor.productSeqNum << std::endl;

    // Read positions of both hands
    std::cout << "\nReading joint positions from both hands..." << std::endl;
    // UMI protocol supports active position query
    std::cout << "\nNote: UMI protocol supports active position query." << std::endl;
    std::cout << "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data." << std::endl;
  }

  std::cout << "\n[Done]: Example completed successfully!" << std::endl;
  return 0;
}
