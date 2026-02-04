// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file O12_demo_canfd_serial.cc
 * @brief OmniHand Pro 2025 控制示例 - CANFD 通信（通过 serial_number）
 * 
 * 此示例演示如何使用设备序列号创建和控制 OmniHand Pro 2025 灵巧手
 * 支持单手（left/right）和双手（both）控制
 * 
 * 编译: cmake .. && make
 * 运行: 
 *   ./demo_omnihand_pro_2025_canfd_serial left    # 控制左手
 *   ./demo_omnihand_pro_2025_canfd_serial right   # 控制右手
 *   ./demo_omnihand_pro_2025_canfd_serial both    # 同时控制左右手
 * 
 * 注意：代码中的序列号需要根据实际情况修改
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "omnihand/omnihand_pro_2025.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Control left hand only" << std::endl;
  std::cout << "  right  - Control right hand only" << std::endl;
  std::cout << "  both   - Control both hands simultaneously" << std::endl;
  std::cout << std::endl;
  std::cout << "Note: Serial numbers in code need to be modified according to actual devices" << std::endl;
}

void controlSingleHand(std::unique_ptr<OmniHandPro2025>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  // ============ 获取设备信息 ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << std::endl;
  std::cout << "  Model: " << vendor_info.productModel << std::endl;
  std::cout << "  Serial: " << vendor_info.productSeqNum << std::endl;
  std::cout << "  Hardware Version: " << static_cast<int>(vendor_info.hardwareVersion.major_)
            << "." << static_cast<int>(vendor_info.hardwareVersion.minor_)
            << "." << static_cast<int>(vendor_info.hardwareVersion.patch_) << std::endl;
  std::cout << "  Software Version: " << static_cast<int>(vendor_info.softwareVersion.major_)
            << "." << static_cast<int>(vendor_info.softwareVersion.minor_)
            << "." << static_cast<int>(vendor_info.softwareVersion.patch_) << std::endl;
  std::cout << "  Voltage: " << vendor_info.voltage << " mV" << std::endl;
  std::cout << "  DOF: " << static_cast<int>(vendor_info.dof) << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << std::endl;
  std::cout << "  Device ID: " << static_cast<int>(device_info.hand_device_id) << std::endl;
  std::cout << "  Communication Parameters:" << std::endl;
  std::cout << "    Bitrate: " << static_cast<int>(device_info.commu_params.bitrate_) << std::endl;
  std::cout << "    Sample Point: " << static_cast<int>(device_info.commu_params.sample_point_) << std::endl;
  std::cout << "    D-Bitrate: " << static_cast<int>(device_info.commu_params.dbitrate_) << std::endl;
  std::cout << "    D-Sample Point: " << static_cast<int>(device_info.commu_params.dsample_point_) << std::endl;

  // ============ 读取传感器数据 ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;
  
  // 读取 3D 触觉传感器数据（O12 特有）
  std::cout << "\n3D Tactile Sensor Data (O12 only):" << std::endl;
  try {
    auto thumb_sensor = hand->GetTactileSensor3DData(EFinger::eThumb);
    std::cout << "  Thumb:" << std::endl;
    std::cout << "    Online State: " << (thumb_sensor.online_state_ ? "Online" : "Offline") << std::endl;
    std::cout << "    Normal Force: " << thumb_sensor.normal_force_ << " (0.1N, max: 3000)" << std::endl;
    std::cout << "    Tangent Force: " << thumb_sensor.tangent_force_ << std::endl;
    std::cout << "    Tangent Force Angle: " << thumb_sensor.tangent_force_angle_ << "°" << std::endl;
    
    auto index_sensor = hand->GetTactileSensor3DData(EFinger::eIndex);
    std::cout << "  Index:" << std::endl;
    std::cout << "    Online State: " << (index_sensor.online_state_ ? "Online" : "Offline") << std::endl;
    std::cout << "    Normal Force: " << index_sensor.normal_force_ << " (0.1N, max: 3000)" << std::endl;
    std::cout << "    Tangent Force: " << index_sensor.tangent_force_ << std::endl;
    std::cout << "    Tangent Force Angle: " << index_sensor.tangent_force_angle_ << "°" << std::endl;
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
    if (errors[i].stalled_ || errors[i].overheat_ || errors[i].over_current_ || 
        errors[i].motor_except_ || errors[i].commu_except_) {
      std::cout << "  Joint " << (i + 1) << ": ";
      if (errors[i].stalled_) std::cout << "Stalled ";
      if (errors[i].overheat_) std::cout << "Overheat ";
      if (errors[i].over_current_) std::cout << "OverCurrent ";
      if (errors[i].motor_except_) std::cout << "MotorException ";
      if (errors[i].commu_except_) std::cout << "CommException ";
      std::cout << std::endl;
    }
  }
  if (std::all_of(errors.begin(), errors.end(), [](const auto& e) {
        return !e.stalled_ && !e.overheat_ && !e.over_current_ && 
               !e.motor_except_ && !e.commu_except_;
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
  std::vector<double> angles(12, 0.0);  // O12 有 12 个主动关节
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
  std::cout << "OmniHand Pro 2025 - CANFD Control (by serial_number)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  // 注意：序列号需要根据实际情况修改
  std::string left_serial = "201BFF2A";   // 左手适配器序列号（部分匹配）
  std::string right_serial = "201BFF2B";  // 右手适配器序列号（部分匹配，请根据实际情况修改）

  if (mode == "left" || mode == "both") {
    auto left_hand = OmniHandPro2025::createHandByZlgcan(
        EHandType::eLeft,
        device_id,
        left_serial,
        0
    );

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
    auto right_hand = OmniHandPro2025::createHandByZlgcan(
        EHandType::eRight,
        device_id,
        right_serial,
        0
    );

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
      // both 模式：同时控制
      std::cout << "\n=== Dual Hand Control ===" << std::endl;
      
      auto left_hand = OmniHandPro2025::createHandByZlgcan(
          EHandType::eLeft,
          device_id,
          left_serial,
          0
      );
      if (!left_hand || !left_hand->Init()) {
        std::cerr << "[Error]: Failed to initialize left hand for dual mode" << std::endl;
        return 1;
      }

      // 使用关节角度控制（推荐方式，底层会自动转换）
      std::cout << "\nSetting joint angles for both hands..." << std::endl;
      std::vector<double> left_angles(12, 0.0);
      std::vector<double> right_angles(12, 0.5);
      
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
