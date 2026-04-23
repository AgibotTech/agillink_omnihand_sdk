// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O10_demo_canfd_id.cc
 * @brief OmniHand 2025 控制示例 - CANFD 通信（通过 canfd_id）
 * 
 * 此示例演示如何使用 canfd_id 创建和控制 OmniHand 2025 灵巧手
 * 支持单手（left/right）和双手（both）控制
 * 
 * 编译: cmake .. && make
 * 运行: 
 *   ./example_canfd_id left    # 控制左手
 *   ./example_canfd_id right   # 控制右手
 *   ./example_canfd_id both    # 同时控制左右手
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
  std::cout << "Example:" << std::endl;
  std::cout << "  " << program_name << " left" << std::endl;
  std::cout << "  " << program_name << " both" << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand2025>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  // ============ 获取设备信息 ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ 读取传感器数据 ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;
  
  // 读取触觉传感器数据
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

  // 读取温度报告
  std::cout << "\nTemperature Reports:" << std::endl;
  auto temperatures = hand->GetAllTemperatureReport();
  std::cout << "  All Joint Temperatures (°C): [";
  for (size_t i = 0; i < temperatures.size(); ++i) {
    std::cout << temperatures[i];
    if (i < temperatures.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // 读取电流报告
  std::cout << "\nCurrent Reports:" << std::endl;
  auto currents = hand->GetAllCurrentReport();
  std::cout << "  All Joint Currents: [";
  for (size_t i = 0; i < currents.size(); ++i) {
    std::cout << currents[i];
    if (i < currents.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // 读取错误报告
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

  // 读取速度（读取，不算控制）
  std::cout << "\nJoint Velocities:" << std::endl;
  auto velocities = hand->GetAllJointMotorVelo();
  std::cout << "  All Joint Velocities: [";
  for (size_t i = 0; i < velocities.size(); ++i) {
    std::cout << velocities[i];
    if (i < velocities.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // ============ 关节角度控制示例 ============
  // 使用关节角度控制（推荐方式，底层会自动转换）
  std::cout << "\nSetting joint angles..." << std::endl;
  std::vector<double> angles(10, 0.0);
  hand->SetAllActiveJointAngles(angles);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 读取关节角度
  auto active_angles = hand->GetAllActiveJointAngles();
  std::cout << "Active Joint Angles (rad): [";
  for (size_t i = 0; i < active_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << active_angles[i];
    if (i < active_angles.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // 读取所有关节角度
  auto all_angles = hand->GetAllJointAngles();
  std::cout << "All Joint Angles (rad, " << all_angles.size() << " joints): [";
  for (size_t i = 0; i < all_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << all_angles[i];
    if (i < all_angles.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

int main(int argc, char** argv) {
  // 解析命令行参数
  std::string mode = "left";  // 默认左手
  std::string device_type = "zlgcan";  // 默认 zlgcan
  
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      std::cout << "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)" << std::endl;
      return 0;
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
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
  std::cout << "OmniHand 2025 - CANFD Control (by canfd_id)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "Device: " << device_type << std::endl;
  std::cout << "============================================" << std::endl;

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
    // 创建左手实例
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
    // 创建右手实例
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
    // both 模式：同时创建两个手
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);  // 第一个通道
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 1);  // 第二个通道

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

    // 同时控制两个手
    std::cout << "\n=== Dual Hand Control ===" << std::endl;
    
    // 获取设备信息
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();
    
    std::cout << "\nLeft Hand Info:" << std::endl;
    std::cout << "  Model: " << left_vendor.productModel << std::endl;
    std::cout << "  Serial: " << left_vendor.productSeqNum << std::endl;
    
    std::cout << "\nRight Hand Info:" << std::endl;
    std::cout << "  Model: " << right_vendor.productModel << std::endl;
    std::cout << "  Serial: " << right_vendor.productSeqNum << std::endl;

    // ============ 关节角度控制示例 ============
    // 使用关节角度控制（推荐方式，底层会自动转换）
    std::cout << "\nSetting joint angles..." << std::endl;
    std::vector<double> left_angles(10, 0.0);
    std::vector<double> right_angles(10, 0.5);

    left_hand->SetAllActiveJointAngles(left_angles);
    right_hand->SetAllActiveJointAngles(right_angles);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // 读取关节角度
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
