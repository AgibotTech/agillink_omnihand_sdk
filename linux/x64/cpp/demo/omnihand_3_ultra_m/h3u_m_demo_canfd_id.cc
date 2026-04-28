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

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "omnihand/omnihand_3_ultra_m.h"

void printUsage(const char* program_name) {
  std::cout << "Usage: " << program_name << " [left|right|both]" << std::endl;
  std::cout << "  left   - Control left hand only" << std::endl;
  std::cout << "  right  - Control right hand only" << std::endl;
  std::cout << "  both   - Control both hands simultaneously" << std::endl;
}

void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand3UltraM>& hand, const std::string& hand_name) {
  std::cout << "\n=== " << hand_name << " Hand Control ===" << std::endl;

  auto vendor_info = hand->GetVendorInfo();
  std::cout << "\nVendor Info:" << vendor_info.ToString() << std::endl;

  auto device_info = hand->GetDeviceInfo();
  std::cout << "\nDevice Info:" << device_info.ToString() << std::endl;

  std::cout << "\n=== Reading Sensor Data ===" << std::endl;

  std::cout << "\nTemperature Reports:" << std::endl;
  auto temperatures = hand->GetAllTemperatureReport();
  std::cout << "  All Joint Temperatures (C): [";
  for (size_t i = 0; i < temperatures.size(); ++i) {
    std::cout << temperatures[i];
    if (i < temperatures.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "\nCurrent Reports:" << std::endl;
  auto currents = hand->GetAllCurrentReport();
  std::cout << "  All Joint Currents: [";
  for (size_t i = 0; i < currents.size(); ++i) {
    std::cout << currents[i];
    if (i < currents.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "\nError Reports:" << std::endl;
  auto errors = hand->GetAllErrorReport();
  for (size_t i = 0; i < errors.size(); ++i) {
    std::string err_str = agilink::omnihand::H3UMErrorReportToString(errors[i]);
    if (err_str != "0") {
      std::cout << "  Joint " << (i + 1) << ": " << err_str << std::endl;
    }
  }
  if (std::all_of(errors.begin(), errors.end(), [](const auto& e) { return e.value_ == 0; })) {
    std::cout << "  No errors detected" << std::endl;
  }

  std::cout << "\nClear all error reports..." << std::endl;
  hand->ClearAllErrorReport();
  std::cout << "  ClearAllErrorReport: Done" << std::endl;

  std::cout << "\nJoint Velocities:" << std::endl;
  auto velocities = hand->GetAllJointMotorVelo();
  std::cout << "  All Joint Velocities: [";
  for (size_t i = 0; i < velocities.size(); ++i) {
    std::cout << velocities[i];
    if (i < velocities.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // ============ Joint angle control ============
  std::cout << "\n=== Joint Angle Control ===" << std::endl;
  std::vector<double> angles(20, 0.0);
  hand->SetAllActiveJointAngles(angles);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto active_angles = hand->GetAllActiveJointAngles();
  std::cout << "Active Joint Angles (rad): [";
  for (size_t i = 0; i < active_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << active_angles[i];
    if (i < active_angles.size() - 1) std::cout << ", ";
  }
  std::cout << "]" << std::endl;

  // ============ O20 Extended API ============
  std::cout << "\n=== O20 Extended API ===" << std::endl;

  std::cout << "\nAxis Limit Positions (Pn7 & Pn8):" << std::endl;
  auto limits = hand->GetAxisLimitPos();
  if (!limits.empty()) {
    std::cout << "  Max: [";
    for (size_t i = 0; i < limits.max_limits.size(); ++i) {
      std::cout << limits.max_limits[i];
      if (i < limits.max_limits.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "  Min: [";
    for (size_t i = 0; i < limits.min_limits.size(); ++i) {
      std::cout << limits.min_limits[i];
      if (i < limits.min_limits.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
  }

  std::cout << "\nActual Axis Positions (Pn23):" << std::endl;
  auto actual_pos = hand->GetAllActualAxisPos();
  std::cout << "  [";
  for (size_t i = 0; i < actual_pos.size(); ++i) {
    std::cout << actual_pos[i];
    if (i < actual_pos.size() - 1) std::cout << ", ";
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
  std::cout << "OmniHand 3 Ultra M (O20) - CANFD Control (by canfd_id)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 9;
  unsigned char canfd_id = 0;

  if (mode == "left") {
    auto left_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, canfd_id, 0);
    if (!left_hand) { std::cerr << "[Error]: Failed to create left hand" << std::endl; return 1; }
    if (!left_hand->Init()) { std::cerr << "[Error]: Failed to init left hand" << std::endl; return 1; }
    std::cout << "[OK]: Left hand initialized" << std::endl;
    controlSingleHand(left_hand, "Left");
  } else if (mode == "right") {
    auto right_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, canfd_id, 0);
    if (!right_hand) { std::cerr << "[Error]: Failed to create right hand" << std::endl; return 1; }
    if (!right_hand->Init()) { std::cerr << "[Error]: Failed to init right hand" << std::endl; return 1; }
    std::cout << "[OK]: Right hand initialized" << std::endl;
    controlSingleHand(right_hand, "Right");
  } else if (mode == "both") {
    auto left_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, canfd_id, 0);
    auto right_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, canfd_id, 1);
    if (!left_hand || !right_hand) { std::cerr << "[Error]: Failed to create hands" << std::endl; return 1; }
    if (!left_hand->Init()) { std::cerr << "[Error]: Failed to init left hand" << std::endl; return 1; }
    if (!right_hand->Init()) { std::cerr << "[Error]: Failed to init right hand" << std::endl; return 1; }
    std::cout << "[OK]: Both hands initialized" << std::endl;
    controlSingleHand(left_hand, "Left");
    controlSingleHand(right_hand, "Right");
  }

  std::cout << "\n[Done]: Demo completed!" << std::endl;
  return 0;
}
