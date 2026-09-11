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

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"
#include "omnihand/omnihand_dex_umi.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHandDexUmiDemo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Read left hand data only");
  AgilinkLogger::get().infof(TAG, "  right  - Read right hand data only");
  AgilinkLogger::get().infof(TAG, "  both   - Read both hands data simultaneously");
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Note: UMI protocol is read-only, position/velocity/torque control is not supported");
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Invalid argument: %s", arg.c_str());
      printUsage(argv[0]);
      return 1;
    }
  }

  AgilinkLogger::get().infof(TAG, "============================================");
  AgilinkLogger::get().infof(TAG, "OmniHand Dex UMI - CANFD Control (by canfd_id)");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully");
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      return 1;
    }

    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized successfully");
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create hand instances");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }

    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Both hands initialized successfully");

    // Read data from both hands simultaneously
    AgilinkLogger::get().infof(TAG, "\n=== Dual Hand Data Reading ===");

    // Get device info
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();

    AgilinkLogger::get().infof(TAG, "\nLeft Hand Info:");
    AgilinkLogger::get().infof(TAG, "  Model: %s", left_vendor.productModel.c_str());
    AgilinkLogger::get().infof(TAG, "  Serial: %s", left_vendor.productSeqNum.c_str());

    AgilinkLogger::get().infof(TAG, "\nRight Hand Info:");
    AgilinkLogger::get().infof(TAG, "  Model: %s", right_vendor.productModel.c_str());
    AgilinkLogger::get().infof(TAG, "  Serial: %s", right_vendor.productSeqNum.c_str());

    // Read positions of both hands
    AgilinkLogger::get().infof(TAG, "\nReading joint positions from both hands...");
    // UMI protocol supports active position query
    AgilinkLogger::get().infof(TAG, "\nNote: UMI protocol supports active position query.");
    AgilinkLogger::get().infof(TAG, "      Use GetJointMotorPosi() or GetAllJointMotorPosi() to get position data.");
  }

  AgilinkLogger::get().infof(TAG, "\n[Done]: Example completed successfully!");
  return 0;
}
