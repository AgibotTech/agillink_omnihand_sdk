// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file h3u_m_demo_canfd_id.cc
 * @brief OmniHand 3 Ultra M (O20) control demo - CANFD (by canfd_id)
 *
 * Compile: cmake .. && make
 * Run:
 *   ./demo_omnihand_3_ultra_m_canfd_id left
 *   ./demo_omnihand_3_ultra_m_canfd_id right
 *   ./demo_omnihand_3_ultra_m_canfd_id both
 */

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"
#include "omnihand/omnihand_3_ultra_m.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand3UltraMDemo";

void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand only");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand only");
  AgilinkLogger::get().infof(TAG, "  both   - Control both hands simultaneously");
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand3UltraM>& hand, const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "\n=== %s Hand Control ===", hand_name.c_str());

  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "\nVendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "\nDevice Info:%s", device_info.ToString().c_str());

  AgilinkLogger::get().infof(TAG, "\n=== Reading Sensor Data ===");

  AgilinkLogger::get().infof(TAG, "\nTemperature Reports:");
  auto temperatures = hand->GetAllTemperatureReport();
  {
    std::string msg = "  All Joint Temperatures (C): [";
    char buf[64];
    for (size_t i = 0; i < temperatures.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", temperatures[i]);
      msg += buf;
      if (i < temperatures.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "\nCurrent Reports:");
  auto currents = hand->GetAllCurrentReport();
  {
    std::string msg = "  All Joint Currents: [";
    char buf[64];
    for (size_t i = 0; i < currents.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", currents[i]);
      msg += buf;
      if (i < currents.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "\nError Reports:");
  auto errors = hand->GetAllErrorReport();
  for (size_t i = 0; i < errors.size(); ++i) {
    std::string err_str = agilink::omnihand::H3UMErrorReportToString(errors[i]);
    if (err_str != "0") {
      AgilinkLogger::get().infof(TAG, "  Joint %zu: %s", (i + 1), err_str.c_str());
    }
  }
  if (std::all_of(errors.begin(), errors.end(), [](const auto& e) { return e.value_ == 0; })) {
    AgilinkLogger::get().infof(TAG, "  No errors detected");
  }

  AgilinkLogger::get().infof(TAG, "\nClear all error reports...");
  hand->ClearAllErrorReport();
  AgilinkLogger::get().infof(TAG, "  ClearAllErrorReport: Done");

  AgilinkLogger::get().infof(TAG, "\nJoint Velocities:");
  auto velocities = hand->GetAllJointMotorVelo();
  {
    std::string msg = "  All Joint Velocities: [";
    char buf[64];
    for (size_t i = 0; i < velocities.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", velocities[i]);
      msg += buf;
      if (i < velocities.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // ============ Joint angle control ============
  AgilinkLogger::get().infof(TAG, "\n=== Joint Angle Control ===");
  std::vector<double> angles(20, 0.0);
  hand->SetAllActiveJointAngles(angles);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto active_angles = hand->GetAllActiveJointAngles();
  {
    std::string msg = "Active Joint Angles (rad): [";
    char buf[64];
    for (size_t i = 0; i < active_angles.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", active_angles[i]);
      msg += buf;
      if (i < active_angles.size() - 1) msg += ", ";
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // ============ O20 Extended API ============
  AgilinkLogger::get().infof(TAG, "\n=== O20 Extended API ===");

  AgilinkLogger::get().infof(TAG, "\nAxis Limit Positions (Pn7 & Pn8):");
  auto limits = hand->GetAxisLimitPos();
  if (!limits.empty()) {
    {
      std::string msg = "  Max: [";
      char buf[64];
      for (size_t i = 0; i < limits.max_limits.size(); ++i) {
        snprintf(buf, sizeof(buf), "%.4f", limits.max_limits[i]);
        msg += buf;
        if (i < limits.max_limits.size() - 1) msg += ", ";
      }
      msg += "]";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
    {
      std::string msg = "  Min: [";
      char buf[64];
      for (size_t i = 0; i < limits.min_limits.size(); ++i) {
        snprintf(buf, sizeof(buf), "%.4f", limits.min_limits[i]);
        msg += buf;
        if (i < limits.min_limits.size() - 1) msg += ", ";
      }
      msg += "]";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
  }

  AgilinkLogger::get().infof(TAG, "\nActual Axis Positions (Pn23):");
  auto actual_pos = hand->GetAllActualAxisPos();
  {
    std::string msg = "  [";
    char buf[64];
    for (size_t i = 0; i < actual_pos.size(); ++i) {
      snprintf(buf, sizeof(buf), "%.4f", actual_pos[i]);
      msg += buf;
      if (i < actual_pos.size() - 1) msg += ", ";
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
  AgilinkLogger::get().infof(TAG, "OmniHand 3 Ultra M (O20) - CANFD Control (by canfd_id)");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

  unsigned char device_id = 9;
  unsigned char canfd_id = 0;

  if (mode == "left") {
    auto left_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, canfd_id, 0);
    if (!left_hand) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand"); return 1; }
    if (!left_hand->Init()) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to init left hand"); return 1; }
    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized");
    controlSingleHand(left_hand, "Left");
  } else if (mode == "right") {
    auto right_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, canfd_id, 0);
    if (!right_hand) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand"); return 1; }
    if (!right_hand->Init()) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to init right hand"); return 1; }
    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized");
    controlSingleHand(right_hand, "Right");
  } else if (mode == "both") {
    auto left_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, canfd_id, 0);
    auto right_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, canfd_id, 1);
    if (!left_hand || !right_hand) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create hands"); return 1; }
    if (!left_hand->Init()) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to init left hand"); return 1; }
    if (!right_hand->Init()) { AgilinkLogger::get().errorf(TAG, "[Error]: Failed to init right hand"); return 1; }
    AgilinkLogger::get().infof(TAG, "[OK]: Both hands initialized");
    controlSingleHand(left_hand, "Left");
    controlSingleHand(right_hand, "Right");
  }

  AgilinkLogger::get().infof(TAG, "\n[Done]: Demo completed!");
  return 0;
}
