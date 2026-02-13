// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file UMI_demo_canfd_serial.cc
 * @brief OmniHand Dex UMI 控制示例 - CANFD 通信（通过 serial_number）
 * 
 * 此示例演示如何使用设备序列号创建和读取 OmniHand Dex UMI 灵巧手数据
 * 支持单手（left/right）和双手（both）控制
 * 
 * 注意：UMI 协议是只读的，不支持位置/速度/力矩控制
 * 注意：代码中的序列号需要根据实际情况修改
 * 
 * 编译: cmake .. && make
 * 运行: 
 *   ./demo_omnihand_dex_umi_canfd_serial left    # 读取左手数据
 *   ./demo_omnihand_dex_umi_canfd_serial right   # 读取右手数据
 *   ./demo_omnihand_dex_umi_canfd_serial both    # 同时读取左右手数据
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
  std::cout << "Note: Serial numbers in code need to be modified according to actual devices" << std::endl;
  std::cout << "Note: UMI protocol is read-only, position/velocity/torque control is not supported" << std::endl;
}

void readSingleHand(std::unique_ptr<agilink::omnihand::OmniHandDexUMI>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Data Reading ===" << std::endl;

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
  std::cout << "OmniHand Dex UMI - CANFD Control (by serial_number)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 1;
  // 注意：序列号需要根据实际情况修改
  std::string left_serial = "201BFF2A";   // 左手适配器序列号（部分匹配）
  std::string right_serial = "201BFF2B";  // 右手适配器序列号（部分匹配，请根据实际情况修改）

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT,
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
    readSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT,
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
      readSingleHand(right_hand, "Right");
    } else {
      // both 模式：同时读取
      std::cout << "\n=== Dual Hand Data Reading ===" << std::endl;
      
      auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_serial,
          0
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
