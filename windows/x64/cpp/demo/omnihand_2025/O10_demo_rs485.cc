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

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"
#include "omnihand/omnihand_2025.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025Demo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand only");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand only");
  AgilinkLogger::get().infof(TAG, "  both   - Control both hands simultaneously");
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Note: Serial port paths in code need to be modified according to actual devices");
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand2025>& hand, const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "=== %s Hand Control ===", hand_name.c_str());

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Vendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Device Info:%s", device_info.ToString().c_str());

  // ============ Read Sensor Data ============
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "=== Reading Sensor Data ===");

  // Note: RS485 does not support raw tactile sensor data reading (GetTactileSensorDataRaw)
  // But it supports GetTactileSensorData (downsampled data)
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Tactile Sensor Data (1D):");
  try {
    auto thumb_tactile = hand->GetTactileSensorData(agilink::omnihand::Finger::THUMB);
    {
      std::string msg;
      for (size_t i = 0; i < thumb_tactile.size(); ++i) {
        msg += std::to_string(static_cast<int>(thumb_tactile[i]));
        if (i < thumb_tactile.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "  Thumb: [%s] (unit: 1g, max: 255g)", msg.c_str());
    }

    auto index_tactile = hand->GetTactileSensorData(agilink::omnihand::Finger::INDEX);
    {
      std::string msg;
      for (size_t i = 0; i < index_tactile.size(); ++i) {
        msg += std::to_string(static_cast<int>(index_tactile[i]));
        if (i < index_tactile.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "  Index: [%s] (unit: 1g, max: 255g)", msg.c_str());
    }
  } catch (const std::exception& e) {
    AgilinkLogger::get().infof(TAG, "  Warning: %s", e.what());
  }

  // Note: RS485 does not support temperature/current/error report/velocity reads
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Note: RS485 communication does not support temperature/current/error/velocity reports");

  // ============ Joint Angle Control Demo ============
  // Use joint-angle control (recommended; underlying layer auto-converts)
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Setting joint angles...");
  std::vector<double> angles(10, 0.0);
  hand->SetAllActiveJointAngles(angles);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Read joint angle
  auto active_angles = hand->GetAllActiveJointAngles();
  {
    std::string msg;
    char buf[32];
    for (size_t i = 0; i < active_angles.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", active_angles[i]);
      msg += buf;
      if (i < active_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "Active Joint Angles (rad): [%s]", msg.c_str());
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
  AgilinkLogger::get().infof(TAG, "OmniHand 2025 - RS485 Control");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      AgilinkLogger::get().errorf(TAG, "Please check:");
      AgilinkLogger::get().errorf(TAG, "  1. Serial port exists: ls -l /dev/ttyUSB*");
      AgilinkLogger::get().errorf(TAG, "  2. Permission: sudo chmod 666 /dev/ttyUSB0");
      AgilinkLogger::get().errorf(TAG, "  3. Device is connected and powered on");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully");
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
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      return 1;
    }

    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized successfully");

    if (mode == "right") {
      controlSingleHand(right_hand, "Right");
    } else {
      // both mode: control simultaneously
      AgilinkLogger::get().infof(TAG, "");
      AgilinkLogger::get().infof(TAG, "=== Dual Hand Control ===");

      auto left_hand = agilink::omnihand::OmniHand2025::createHandByRs485(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_port,
          baudrate
      );
      if (!left_hand || !left_hand->Init()) {
        AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand for dual mode");
        return 1;
      }

      // Use joint-angle control (recommended; underlying layer auto-converts)
      AgilinkLogger::get().infof(TAG, "");
      AgilinkLogger::get().infof(TAG, "Setting joint angles for both hands...");
      std::vector<double> left_angles(10, 0.0);
      std::vector<double> right_angles(10, 0.5);

      left_hand->SetAllActiveJointAngles(left_angles);
      right_hand->SetAllActiveJointAngles(right_angles);

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      auto left_angles_read = left_hand->GetAllActiveJointAngles();
      auto right_angles_read = right_hand->GetAllActiveJointAngles();

      {
        std::string msg;
        char buf[32];
        for (size_t i = 0; i < left_angles_read.size(); ++i) {
          snprintf(buf, sizeof(buf), "%.4f", left_angles_read[i]);
          msg += buf;
          if (i < left_angles_read.size() - 1) msg += ", ";
        }
        AgilinkLogger::get().infof(TAG, "Left Hand Angles (rad): [%s]", msg.c_str());
      }

      {
        std::string msg;
        char buf[32];
        for (size_t i = 0; i < right_angles_read.size(); ++i) {
          snprintf(buf, sizeof(buf), "%.4f", right_angles_read[i]);
          msg += buf;
          if (i < right_angles_read.size() - 1) msg += ", ";
        }
        AgilinkLogger::get().infof(TAG, "Right Hand Angles (rad): [%s]", msg.c_str());
      }
    }
  }

  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "[Done]: Example completed successfully!");
  return 0;
}
