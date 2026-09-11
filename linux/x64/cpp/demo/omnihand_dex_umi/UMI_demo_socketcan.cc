// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file UMI_demo_socketcan.cc
 * @brief OmniHand Dex UMI control demo - SocketCAN communication (Linux only)
 *
 * This demo shows how to use SocketCAN create and read OmniHand Dex UMI dexterous-hand data
 * Supports single-hand (left/right) and dual-hand (both) control
 *
 * Warning: This demo applies to environments with SocketCAN already configured (e.g. onboard CAN or other SocketCAN devices)
 * Warning: For USB CANFD devices, the ZLG library approach is recommended without additional driver setup
 * Warning: UMI protocol is read-only and does not support position/velocity/torque control
 *
 * Configure CAN interface before use:
 *   sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
 *   sudo ip link set can0 up
 *   sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
 *   sudo ip link set can1 up
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_dex_umi_socketcan left    # Read left-hand data(using can0)
 *   ./demo_omnihand_dex_umi_socketcan right   # Read right-hand data(using can0)
 *   ./demo_omnihand_dex_umi_socketcan both    # Read both left and right hand data simultaneously(using can0 and can1)
 */

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHandDexUmiDemo";

#ifdef __linux__
#include "omnihand/omnihand_dex_umi.h"

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Read left hand data only (uses can0)");
  AgilinkLogger::get().infof(TAG, "  right  - Read right hand data only (uses can0)");
  AgilinkLogger::get().infof(TAG, "  both   - Read both hands data simultaneously (uses can0 and can1)");
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Before running, configure CAN interfaces:");
  AgilinkLogger::get().infof(TAG, "  sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on");
  AgilinkLogger::get().infof(TAG, "  sudo ip link set can0 up");
  AgilinkLogger::get().infof(TAG, "  sudo ip link set can1 type can bitrate 1000000 dbitrate 5000000 fd on");
  AgilinkLogger::get().infof(TAG, "  sudo ip link set can1 up");
}

void readSingleHand(std::unique_ptr<agilink::omnihand::OmniHandDexUMI>& hand, const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "\n=== %s Hand Data Reading ===", hand_name.c_str());

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "\nVendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "\nDevice Info:%s", device_info.ToString().c_str());

  // ============ Read Sensor Data ============
  AgilinkLogger::get().infof(TAG, "\n=== Reading Sensor Data ===");

  // Note: UMI protocol supports active joint position query
  // Use GetJointMotorPosi() or GetAllJointMotorPosi() to obtain position data
  AgilinkLogger::get().infof(TAG, "\nNote: UMI protocol supports active position query.");
  AgilinkLogger::get().infof(TAG, "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data.");

  // Read tactile sensor data (1D, using Raw API)
  AgilinkLogger::get().infof(TAG, "\n1D Tactile Sensor Data (Raw):");
  try {
    auto thumb_sensor = hand->GetTactileSensorDataRaw(agilink::omnihand::Finger::THUMB);
    {
      std::string msg = "  Thumb: [";
      for (size_t i = 0; i < thumb_sensor.data_.size(); ++i) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", static_cast<int>(thumb_sensor.data_[i]));
        msg += buf;
        if (i < thumb_sensor.data_.size() - 1) msg += ", ";
      }
      msg += "] (unit: 1g, max: 255g)";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }

    auto index_sensor = hand->GetTactileSensorDataRaw(agilink::omnihand::Finger::INDEX);
    {
      std::string msg = "  Index: [";
      for (size_t i = 0; i < index_sensor.data_.size(); ++i) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", static_cast<int>(index_sensor.data_[i]));
        msg += buf;
        if (i < index_sensor.data_.size() - 1) msg += ", ";
      }
      msg += "] (unit: 1g, max: 255g)";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }

    auto middle_sensor = hand->GetTactileSensorDataRaw(agilink::omnihand::Finger::MIDDLE);
    {
      std::string msg = "  Middle: [";
      for (size_t i = 0; i < middle_sensor.data_.size(); ++i) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", static_cast<int>(middle_sensor.data_[i]));
        msg += buf;
        if (i < middle_sensor.data_.size() - 1) msg += ", ";
      }
      msg += "] (unit: 1g, max: 255g)";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }

    // Read all sensor data
    AgilinkLogger::get().infof(TAG, "\nAll Tactile Sensor Data:");
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
      AgilinkLogger::get().infof(TAG, "  %s: %zu points", finger_name.c_str(), sensor.data_.size());
    }
  } catch (const std::exception& e) {
    AgilinkLogger::get().infof(TAG, "  Warning: %s", e.what());
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Invalid argument: %s", arg.c_str());
      printUsage(argv[0]);
      return 1;
    }
  }

  AgilinkLogger::get().infof(TAG, "============================================");
  AgilinkLogger::get().infof(TAG, "OmniHand Dex UMI - SocketCAN Control");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      AgilinkLogger::get().errorf(TAG, "Please check if %s is configured and up", left_interface.c_str());
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully (%s)", left_interface.c_str());
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      return 1;
    }

    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      AgilinkLogger::get().errorf(TAG, "Please check if %s is configured and up", interface.c_str());
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized successfully (%s)", interface.c_str());

    if (mode == "right") {
      readSingleHand(right_hand, "Right");
    } else {
      // both mode: read simultaneously
      AgilinkLogger::get().infof(TAG, "\n=== Dual Hand Data Reading ===");

      auto left_hand = agilink::omnihand::OmniHandDexUMI::createHandSocketCan(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_interface
      );
      if (!left_hand || !left_hand->Init()) {
        AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand for dual mode");
        return 1;
      }

      // UMI protocol supports active position query
      AgilinkLogger::get().infof(TAG, "\nNote: UMI protocol supports active position query.");
      AgilinkLogger::get().infof(TAG, "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data.");
    }
  }

  AgilinkLogger::get().infof(TAG, "\n[Done]: Example completed successfully!");
  return 0;
}

#else
// Non-Linux platform stub
int main() {
  AgilinkLogger::get().errorf(TAG, "SocketCAN is only available on Linux!");
  return 1;
}
#endif
