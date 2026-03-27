// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file O12_demo_socketcan.cc
 * @brief OmniHand Pro 2025 控制示例 - SocketCAN 通信（仅 Linux）
 * 
 * 此示例演示如何使用 SocketCAN 创建和控制 OmniHand Pro 2025 灵巧手
 * 支持单手（left/right）和双手（both）控制
 * 
 * ⚠️ 注意：此示例适用于已有 SocketCAN 环境的场景（如板载 CAN、其他 SocketCAN 设备）
 * ⚠️ 对于 USB CANFD 设备，推荐使用 ZLG 库方式，无需配置驱动
 * 
 * 使用前需配置 CAN 接口:
 *   sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
 *   sudo ip link set can0 up
 *   sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
 *   sudo ip link set can1 up
 * 
 * 编译: cmake .. && make
 * 运行: 
 *   ./demo_omnihand_pro_2025_socketcan left    # 控制左手（使用 can0）
 *   ./demo_omnihand_pro_2025_socketcan right   # 控制右手（使用 can0）
 *   ./demo_omnihand_pro_2025_socketcan both    # 同时控制左右手（使用 can0 和 can1）
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#ifdef __linux__
#include "omnihand/omnihand_pro_2025.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Control left hand only (uses can0)" << std::endl;
  std::cout << "  right  - Control right hand only (uses can0)" << std::endl;
  std::cout << "  both   - Control both hands simultaneously (uses can0 and can1)" << std::endl;
  std::cout << std::endl;
  std::cout << "Before running, configure CAN interfaces:" << std::endl;
  std::cout << "  sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on" << std::endl;
  std::cout << "  sudo ip link set can0 up" << std::endl;
  std::cout << "  sudo ip link set can1 type can bitrate 1000000 dbitrate 5000000 fd on" << std::endl;
  std::cout << "  sudo ip link set can1 up" << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHandPro2025>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  // ============ 获取设备信息 ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ 读取传感器数据 ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;
  
  // 读取 3D 触觉传感器数据（O12 特有）
  std::cout << "\n3D Tactile Sensor Data (O12 only):" << std::endl;
  try {
    auto thumb_sensor = hand->GetTactileSensor3DData(agilink::omnihand::Finger::THUMB);
    std::cout << "  Thumb:" << std::endl;
    std::cout << "    Online State: " << (thumb_sensor.online_state ? "Online" : "Offline") << std::endl;
    std::cout << "    Normal Force: " << thumb_sensor.normal_force << " (0.1N, max: 3000)" << std::endl;
    std::cout << "    Tangent Force: " << thumb_sensor.tangent_force << std::endl;
    std::cout << "    Tangent Force Angle: " << thumb_sensor.tangent_force_angle << "°" << std::endl;
    
    auto index_sensor = hand->GetTactileSensor3DData(agilink::omnihand::Finger::INDEX);
    std::cout << "  Index:" << std::endl;
    std::cout << "    Online State: " << (index_sensor.online_state ? "Online" : "Offline") << std::endl;
    std::cout << "    Normal Force: " << index_sensor.normal_force << " (0.1N, max: 3000)" << std::endl;
    std::cout << "    Tangent Force: " << index_sensor.tangent_force << std::endl;
    std::cout << "    Tangent Force Angle: " << index_sensor.tangent_force_angle << "°" << std::endl;
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
  std::cout << "OmniHand Pro 2025 - SocketCAN Control" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  std::string left_interface = "can0";
  std::string right_interface = "can1";

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHandPro2025::createHandSocketCan(
        agilink::omnihand::HandType::LEFT,
        device_id,
        left_interface
    );

    if (!left_hand) {
      std::cerr << "[Error]: Failed to create left hand instance" << std::endl;
      return 1;
    }

    if (!left_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize left hand" << std::endl;
      std::cerr << "Please check if " << left_interface << " is configured and up" << std::endl;
      return 1;
    }

    std::cout << "[OK]: Left hand initialized successfully (" << left_interface << ")" << std::endl;
    controlSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    std::string interface = (mode == "both") ? right_interface : left_interface;
    
    auto right_hand = agilink::omnihand::OmniHandPro2025::createHandSocketCan(
        agilink::omnihand::HandType::RIGHT,
        device_id,
        interface
    );

    if (!right_hand) {
      std::cerr << "[Error]: Failed to create right hand instance" << std::endl;
      return 1;
    }

    if (!right_hand->Init()) {
      std::cerr << "[Error]: Failed to initialize right hand" << std::endl;
      std::cerr << "Please check if " << interface << " is configured and up" << std::endl;
      return 1;
    }

    std::cout << "[OK]: Right hand initialized successfully (" << interface << ")" << std::endl;

    if (mode == "right") {
      controlSingleHand(right_hand, "Right");
    } else {
      // both 模式：同时控制
      std::cout << "\n=== Dual Hand Control ===" << std::endl;
      
      auto left_hand = agilink::omnihand::OmniHandPro2025::createHandSocketCan(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_interface
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

      std::cout << "Left Hand Angles (rad, " << left_interface << "): [";
      for (size_t i = 0; i < left_angles_read.size(); ++i) {
        std::cout << std::fixed << std::setprecision(4) << left_angles_read[i];
        if (i < left_angles_read.size() - 1) std::cout << ", ";
      }
      std::cout << "]" << std::endl;

      std::cout << "Right Hand Angles (rad, " << right_interface << "): [";
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

#else
// Non-Linux platform stub
int main() {
  std::cerr << "SocketCAN is only available on Linux!" << std::endl;
  return 1;
}
#endif
