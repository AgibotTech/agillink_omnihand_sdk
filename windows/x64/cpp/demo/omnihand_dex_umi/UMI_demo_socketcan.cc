// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file UMI_demo_socketcan.cc
 * @brief OmniHand Dex UMI 控制示例 - SocketCAN 通信（仅 Linux）
 * 
 * 此示例演示如何使用 SocketCAN 创建和读取 OmniHand Dex UMI 灵巧手数据
 * 支持单手（left/right）和双手（both）控制
 * 
 * ⚠️ 注意：此示例适用于已有 SocketCAN 环境的场景（如板载 CAN、其他 SocketCAN 设备）
 * ⚠️ 对于 USB CANFD 设备，推荐使用 ZLG 库方式，无需配置驱动
 * ⚠️ UMI 协议是只读的，不支持位置/速度/力矩控制
 * 
 * 使用前需配置 CAN 接口:
 *   sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
 *   sudo ip link set can0 up
 *   sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
 *   sudo ip link set can1 up
 * 
 * 编译: cmake .. && make
 * 运行: 
 *   ./demo_omnihand_dex_umi_socketcan left    # 读取左手数据（使用 can0）
 *   ./demo_omnihand_dex_umi_socketcan right   # 读取右手数据（使用 can0）
 *   ./demo_omnihand_dex_umi_socketcan both    # 同时读取左右手数据（使用 can0 和 can1）
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#ifdef __linux__
#include "omnihand/omnihand_dex_umi.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Read left hand data only (uses can0)" << std::endl;
  std::cout << "  right  - Read right hand data only (uses can0)" << std::endl;
  std::cout << "  both   - Read both hands data simultaneously (uses can0 and can1)" << std::endl;
  std::cout << std::endl;
  std::cout << "Before running, configure CAN interfaces:" << std::endl;
  std::cout << "  sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on" << std::endl;
  std::cout << "  sudo ip link set can0 up" << std::endl;
  std::cout << "  sudo ip link set can1 type can bitrate 1000000 dbitrate 5000000 fd on" << std::endl;
  std::cout << "  sudo ip link set can1 up" << std::endl;
}

void readSingleHand(std::unique_ptr<agilink::omnihand::OmniHandDexUMI>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Data Reading ===" << std::endl;

  // ============ 获取设备信息 ============
  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  // ============ 读取传感器数据 ============
  std::cout << "\n=== Reading Sensor Data ===" << std::endl;
  
  // 注意：UMI 协议支持主动查询关节位置
  // 使用 GetJointMotorPosi() 或 GetAllJointMotorPosi() 来获取位置数据
  std::cout << "\nNote: UMI protocol supports active position query." << std::endl;
  std::cout << "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data." << std::endl;

  // 读取触觉传感器数据（1D，使用 Raw API）
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
    
    // 读取所有传感器数据
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
  std::cout << "OmniHand Dex UMI - SocketCAN Control" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  std::string left_interface = "can0";
  std::string right_interface = "can1";

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandSocketCan(
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
    readSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    std::string interface = (mode == "both") ? right_interface : left_interface;
    
    auto right_hand = agilink::omnihand::OmniHandDexUMI::createHandSocketCan(
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
      readSingleHand(right_hand, "Right");
    } else {
      // both 模式：同时读取
      std::cout << "\n=== Dual Hand Data Reading ===" << std::endl;
      
      auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandSocketCan(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_interface
      );
      if (!left_hand || !left_hand->Init()) {
        std::cerr << "[Error]: Failed to initialize left hand for dual mode" << std::endl;
        return 1;
      }

      // UMI 协议支持主动查询位置
      std::cout << "\nNote: UMI protocol supports active position query." << std::endl;
      std::cout << "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data." << std::endl;
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
