// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file h3u_m_demo_canfd_serial.cc
 * @brief OmniHand 3 Ultra M (O20) control demo - CANFD (by serial number)
 *
 * Run:
 *   ./demo_omnihand_3_ultra_m_canfd_serial left
 *   ./demo_omnihand_3_ultra_m_canfd_serial right
 *   ./demo_omnihand_3_ultra_m_canfd_serial both
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

  std::cout << "\nTemperature Reports:" << std::endl;
  auto temperatures = hand->GetAllTemperatureReport();
  std::cout << "  [";
  for (size_t i = 0; i < temperatures.size(); ++i) {
    std::cout << temperatures[i];
    if (i < temperatures.size() - 1) std::cout << ", ";
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
  std::cout << "OmniHand 3 Ultra M (O20) - CANFD Control (by serial number)" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "============================================" << std::endl;

  unsigned char device_id = 9;
  std::string left_serial = "YOUR_LEFT_SERIAL";
  std::string right_serial = "YOUR_RIGHT_SERIAL";

  if (mode == "left" || mode == "both") {
    auto left_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::LEFT, device_id, left_serial, 0);
    if (!left_hand) { std::cerr << "[Error]: Failed to create left hand" << std::endl; return 1; }
    if (!left_hand->Init()) { std::cerr << "[Error]: Failed to init left hand" << std::endl; return 1; }
    std::cout << "[OK]: Left hand initialized" << std::endl;
    controlSingleHand(left_hand, "Left");
  }

  if (mode == "right" || mode == "both") {
    auto right_hand = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
        agilink::omnihand::HandType::RIGHT, device_id, right_serial, 0);
    if (!right_hand) { std::cerr << "[Error]: Failed to create right hand" << std::endl; return 1; }
    if (!right_hand->Init()) { std::cerr << "[Error]: Failed to init right hand" << std::endl; return 1; }
    std::cout << "[OK]: Right hand initialized" << std::endl;
    controlSingleHand(right_hand, "Right");
  }

  std::cout << "\n[Done]: Demo completed!" << std::endl;
  return 0;
}
