// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "agilink_logger.h"
#include "omnihand/omnihand_pro_2025.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHandPro2025Demo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand only");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand only");
  AgilinkLogger::get().infof(TAG, "  both   - Control both hands simultaneously");
  AgilinkLogger::get().infof(TAG, "");
  AgilinkLogger::get().infof(TAG, "Note: Serial numbers in code need to be modified according to actual devices");
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHandPro2025>& hand, const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "\n=== %s Hand Control ===", hand_name.c_str());

  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "\nVendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "\nDevice Info:%s", device_info.ToString().c_str());

  AgilinkLogger::get().infof(TAG, "\n=== Reading Sensor Data ===");

  AgilinkLogger::get().infof(TAG, "\n3D Tactile Sensor Data (O12 only):");
  try {
    auto thumb_sensor = hand->GetTactileSensor3DData(agilink::omnihand::Finger::THUMB);
    AgilinkLogger::get().infof(TAG, "  Thumb:");
    AgilinkLogger::get().infof(TAG, "    Online State: %s", (thumb_sensor.online_state ? "Online" : "Offline"));
    AgilinkLogger::get().infof(TAG, "    Normal Force: %d (0.1N, max: 3000)", thumb_sensor.normal_force);
    AgilinkLogger::get().infof(TAG, "    Tangent Force: %.4f", thumb_sensor.tangent_force);
    AgilinkLogger::get().infof(TAG, "    Tangent Force Angle: %.4f°", thumb_sensor.tangent_force_angle);

    auto index_sensor = hand->GetTactileSensor3DData(agilink::omnihand::Finger::INDEX);
    AgilinkLogger::get().infof(TAG, "  Index:");
    AgilinkLogger::get().infof(TAG, "    Online State: %s", (index_sensor.online_state ? "Online" : "Offline"));
    AgilinkLogger::get().infof(TAG, "    Normal Force: %d (0.1N, max: 3000)", index_sensor.normal_force);
    AgilinkLogger::get().infof(TAG, "    Tangent Force: %.4f", index_sensor.tangent_force);
    AgilinkLogger::get().infof(TAG, "    Tangent Force Angle: %.4f°", index_sensor.tangent_force_angle);
  } catch (const std::exception& e) {
    AgilinkLogger::get().infof(TAG, "  Warning: %s", e.what());
  }

  // Read temperature report
  AgilinkLogger::get().infof(TAG, "\nTemperature Reports:");
  auto temperatures = hand->GetAllTemperatureReport();
  {
    std::string msg = "  All Joint Temperatures (°C): [";
    for (size_t i = 0; i < temperatures.size(); ++i) {
      char buf[32];
      snprintf(buf, sizeof(buf), "%d", static_cast<int>(temperatures[i]));
      msg += buf;
      if (i < temperatures.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // Read current report
  AgilinkLogger::get().infof(TAG, "\nCurrent Reports:");
  auto currents = hand->GetAllCurrentReport();
  {
    std::string msg = "  All Joint Currents: [";
    for (size_t i = 0; i < currents.size(); ++i) {
      char buf[32];
      snprintf(buf, sizeof(buf), "%d", static_cast<int>(currents[i]));
      msg += buf;
      if (i < currents.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // Read error report
  AgilinkLogger::get().infof(TAG, "\nError Reports:");
  auto errors = hand->GetAllErrorReport();
  for (size_t i = 0; i < errors.size(); ++i) {
    if (errors[i].bits.stalled_ || errors[i].bits.overheat_ || errors[i].bits.over_current_ ||
        errors[i].bits.motor_except_ || errors[i].bits.commu_except_) {
      std::string msg;
      char buf[32];
      snprintf(buf, sizeof(buf), "  Joint %zu: ", i + 1);
      msg += buf;
      if (errors[i].bits.stalled_) msg += "Stalled ";
      if (errors[i].bits.overheat_) msg += "Overheat ";
      if (errors[i].bits.over_current_) msg += "OverCurrent ";
      if (errors[i].bits.motor_except_) msg += "MotorException ";
      if (errors[i].bits.commu_except_) msg += "CommException ";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
  }
  if (std::all_of(errors.begin(), errors.end(), [](const auto& e) {
        return !e.bits.stalled_ && !e.bits.overheat_ && !e.bits.over_current_ &&
               !e.bits.motor_except_ && !e.bits.commu_except_;
      })) {
    AgilinkLogger::get().infof(TAG, "  No errors detected");
  }

  // Read velocity (read-only, not control)
  AgilinkLogger::get().infof(TAG, "\nJoint Velocities:");
  auto velocities = hand->GetAllJointMotorVelo();
  {
    std::string msg = "  All Joint Velocities: [";
    for (size_t i = 0; i < velocities.size(); ++i) {
      char buf[32];
      snprintf(buf, sizeof(buf), "%d", static_cast<int>(velocities[i]));
      msg += buf;
      if (i < velocities.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // ============ Joint Angle Control Demo ============
  // Use joint-angle control (recommended; underlying layer auto-converts)
  AgilinkLogger::get().infof(TAG, "\nSetting joint angles...");
  std::vector<double> angles(12, 0.0);  // O12 has 12 active joints
  hand->SetAllActiveJointAngles(angles);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Read joint angle
  auto active_angles = hand->GetAllActiveJointAngles();
  {
    std::string msg = "Active Joint Angles (rad): [";
    for (size_t i = 0; i < active_angles.size(); ++i) {
      char buf[32];
      snprintf(buf, sizeof(buf), "%.4f", active_angles[i]);
      msg += buf;
      if (i < active_angles.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
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
  AgilinkLogger::get().infof(TAG, "OmniHand Pro 2025 - CANFD Control (by serial_number)");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

  unsigned char device_id = 1;
  // Note: serial numbers should be updated for your setup
  std::string left_serial = "201BFF2A";   // left-hand adapter serial number (partial match)
  std::string right_serial = "201BFF2B";  // right-hand adapter serial number (partial match, update for your setup)

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHandPro2025::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT,
        device_id,
        left_serial,
        0
    );

    if (!left_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      AgilinkLogger::get().errorf(TAG, "Please check if device with serial number is connected");
      return 1;
    }

    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }

    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully");
    controlSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = agilink::omnihand::OmniHandPro2025::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT,
        device_id,
        right_serial,
        0
    );

    if (!right_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      AgilinkLogger::get().errorf(TAG, "Please check if device with serial number is connected");
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
      AgilinkLogger::get().infof(TAG, "\n=== Dual Hand Control ===");

      auto left_hand = agilink::omnihand::OmniHandPro2025::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT,
          device_id,
          left_serial,
          0
      );
      if (!left_hand || !left_hand->Init()) {
        AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand for dual mode");
        return 1;
      }

      // Use joint-angle control (recommended; underlying layer auto-converts)
      AgilinkLogger::get().infof(TAG, "\nSetting joint angles for both hands...");
      std::vector<double> left_angles(12, 0.0);
      std::vector<double> right_angles(12, 0.5);

      left_hand->SetAllActiveJointAngles(left_angles);
      right_hand->SetAllActiveJointAngles(right_angles);

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      auto left_angles_read = left_hand->GetAllActiveJointAngles();
      auto right_angles_read = right_hand->GetAllActiveJointAngles();

      {
        std::string msg = "Left Hand Angles (rad): [";
        for (size_t i = 0; i < left_angles_read.size(); ++i) {
          char buf[32];
          snprintf(buf, sizeof(buf), "%.4f", left_angles_read[i]);
          msg += buf;
          if (i < left_angles_read.size() - 1) msg += ", ";
        }
        msg += "]";
        AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
      }

      {
        std::string msg = "Right Hand Angles (rad): [";
        for (size_t i = 0; i < right_angles_read.size(); ++i) {
          char buf[32];
          snprintf(buf, sizeof(buf), "%.4f", right_angles_read[i]);
          msg += buf;
          if (i < right_angles_read.size() - 1) msg += ", ";
        }
        msg += "]";
        AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
      }
    }
  }

  AgilinkLogger::get().infof(TAG, "\n[Done]: Example completed successfully!");
  return 0;
}
