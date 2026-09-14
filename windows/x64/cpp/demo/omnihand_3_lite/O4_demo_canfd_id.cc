// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file O4_demo_canfd_id.cc
 * @brief OmniHand 3 Lite (O4) control demo - CANFD communication (via canfd_id)
 *
 * This demo shows how to use canfd_id create and control OmniHand 3 Lite dexterous hand (4 DOF)
 * Supports single-hand (left/right) and dual-hand (both) control
 *
 * Build: cmake .. && make
 * Run:
 *   ./demo_omnihand_3_lite_canfd_id left    # Control left hand
 *   ./demo_omnihand_3_lite_canfd_id right   # Control right hand
 *   ./demo_omnihand_3_lite_canfd_id both    # Control both left and right hands simultaneously
 */

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include "agilink_logger.h"
#include "omnihand/kinematics/omnihand_3_lite/omnihand_3_lite_solver.h"
#include "omnihand/omnihand_3_lite.h"
#include "omnihand/proto.h"
using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand3LiteDemo";
static agilink::omnihand::HandType handType;
void printUsage(const char* program_name) {
  AgilinkLogger::get().infof(TAG, "Usage: %s [left|right|both]", program_name);
  AgilinkLogger::get().infof(TAG, "  left   - Control left hand only");
  AgilinkLogger::get().infof(TAG, "  right  - Control right hand only");
  AgilinkLogger::get().infof(TAG, "  both   - Control both hands simultaneously");
  AgilinkLogger::get().info("");
  AgilinkLogger::get().infof(TAG, "Example:");
  AgilinkLogger::get().infof(TAG, "  %s left", program_name);
  AgilinkLogger::get().infof(TAG, "  %s both", program_name);
}

template<class Container>
static void container_print(const Container& container) {
  std::string msg;
  for (const auto& elem : container) {
    if (!msg.empty()) msg += " ";
    msg += std::to_string(elem);
  }
  AgilinkLogger::get().infof(TAG, "%p is %s", static_cast<const void*>(&container), msg.c_str());
}

template<class T>
static T val_silder(const T& min_val, const T& max_val, double controller) {
  controller = std::clamp(controller, 0., 1.);
  return (max_val - min_val) * controller + min_val;
}
void controlSingleHand(std::unique_ptr<agilink::omnihand::OmniHand3Lite>& hand,
                       const std::string& hand_name) {
  AgilinkLogger::get().infof(TAG, "\n=== %s Hand Control ===", hand_name.c_str());

  // ============ Get Device Info ============
  auto vendor_info = hand->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "\nVendor Info:%s", vendor_info.ToString().c_str());

  auto device_info = hand->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "\nDevice Info:%s", device_info.ToString().c_str());

  // ============ Read Sensor Data (O4 has no tactile sensors, only temperature/current/error) ============
  AgilinkLogger::get().infof(TAG, "\n=== Reading Sensor Data ===");

  AgilinkLogger::get().infof(TAG, "\nTemperature Reports:");
  auto temperatures = hand->GetAllTemperatureReport();
  {
    std::string msg = "  All Joint Temperatures (°C): [";
    for (size_t i = 0; i < temperatures.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(temperatures[i]);
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "\nCurrent Reports:");
  auto currents = hand->GetAllCurrentReport();
  {
    std::string msg = "  All Joint Currents: [";
    for (size_t i = 0; i < currents.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(currents[i]);
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "\nError Reports:");
  auto errors = hand->GetAllErrorReport();
  for (size_t i = 0; i < errors.size(); ++i) {
    if (errors[i].bits.stalled_ || errors[i].bits.overheat_ || errors[i].bits.over_current_ ||
        errors[i].bits.motor_except_ || errors[i].bits.commu_except_) {
      std::string msg = "  Joint ";
      msg += std::to_string(i + 1);
      msg += ": ";
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

  // ============ Gesture Control Demo ============
  AgilinkLogger::get().infof(TAG, "\n=== Gesture Control ===");

  AgilinkLogger::get().infof(TAG, "Setting gesture: FIST...");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_FIST);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  AgilinkLogger::get().infof(TAG, "Setting gesture: Zero...");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_ALL_ZERO);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  AgilinkLogger::get().infof(TAG, "Setting gesture: Open...");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_OPEN);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // ============ Motor Control Demo ==============
  AgilinkLogger::get().infof(TAG, "\n=== Motor Control Test ===");
  for (int i = 0; i < agilink::omnihand::OmniHand3Lite::kDegreesOfActiveFreedom; i ++) {
    hand->SetJointMotorPosi(i + 1, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    hand->SetJointMotorPosi(i + 1, 2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    hand->SetJointMotorPosi(i + 1, 4095);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    hand->SetJointMotorPosi(i + 1, 2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    hand->SetJointMotorPosi(i + 1, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  AgilinkLogger::get().infof(TAG, "All Joint Set To Zeros");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_ALL_ZERO);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  AgilinkLogger::get().infof(TAG, "All Joint Set To Open");
  hand->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_OPEN);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto joints = hand->GetAllActiveJointAngles();
  container_print(joints);

  hand->SetAllActiveJointAngles(joints);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  for (int i = 0; i < agilink::omnihand::OmniHand3Lite::kDegreesOfActiveFreedom; i ++) {
    auto range = agilink::omnihand::OmniHand3Lite::GetMinMaxActivateJointAngle(i + 1, handType);
    for (int j = 0; j <= 5; j ++) {
      joints[i] = val_silder(range.min_value, range.max_value, j * 0.2);
      hand->SetAllActiveJointAngles(joints);
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    for (int j = 0; j <= 5; j ++) {
      joints[i] = val_silder(range.min_value, range.max_value, 1 -  j * 0.2);
      hand->SetAllActiveJointAngles(joints);
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
  }
  hand->SetAllJointMotorPosi({0, 0, 0, 0});
}

int main(int argc, char** argv) {
  std::string mode = "left";
  std::string device_type = "zlgcan";

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage(argv[0]);
      AgilinkLogger::get().infof(TAG, "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)");
      return 0;
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      device_type = argv[++i];
      if (device_type != "zlgcan" && device_type != "hcan") {
        AgilinkLogger::get().errorf(TAG, "[Error]: -d value must be 'zlgcan' or 'hcan', got: %s", device_type.c_str());
        return 1;
      }
    } else if (arg == "left" || arg == "right" || arg == "both") {
      mode = arg;
    } else {
      AgilinkLogger::get().errorf(TAG, "[Error]: Invalid argument: %s", arg.c_str());
      printUsage(argv[0]);
      return 1;
    }
  }

  AgilinkLogger::get().infof(TAG, "============================================");
  AgilinkLogger::get().infof(TAG, "OmniHand 3 Lite (O4) - CANFD Control (by canfd_id)");
  AgilinkLogger::get().infof(TAG, "Mode: %s", mode.c_str());
  AgilinkLogger::get().infof(TAG, "Device: %s", device_type.c_str());
  AgilinkLogger::get().infof(TAG, "============================================");

  unsigned char device_id = 1;
  unsigned char canfd_id = 0;

  auto createHand = [&](agilink::omnihand::HandType hand_type, unsigned char channel_id) {
    if (device_type == "hcan") {
      return agilink::omnihand::OmniHand3Lite::createHandByHcan(hand_type, device_id, canfd_id,
                                                                channel_id);
    } else {
      return agilink::omnihand::OmniHand3Lite::createHandByZlgcan(hand_type, device_id, canfd_id,
                                                                  channel_id);
    }
  };

  if (mode == "left") {
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);
    handType = agilink::omnihand::HandType::LEFT;
    if (!left_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create left hand instance");
      return 1;
    }
    if (!left_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize left hand");
      return 1;
    }
    AgilinkLogger::get().infof(TAG, "[OK]: Left hand initialized successfully");
    controlSingleHand(left_hand, "Left");
  } else if (mode == "right") {
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 0);
    handType = agilink::omnihand::HandType::RIGHT;
    if (!right_hand) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to create right hand instance");
      return 1;
    }
    if (!right_hand->Init()) {
      AgilinkLogger::get().errorf(TAG, "[Error]: Failed to initialize right hand");
      return 1;
    }
    AgilinkLogger::get().infof(TAG, "[OK]: Right hand initialized successfully");
    controlSingleHand(right_hand, "Right");
  } else if (mode == "both") {
    auto left_hand = createHand(agilink::omnihand::HandType::LEFT, 0);
    auto right_hand = createHand(agilink::omnihand::HandType::RIGHT, 1);
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

    AgilinkLogger::get().infof(TAG, "\n=== Dual Hand Control ===");
    auto left_vendor = left_hand->GetVendorInfo();
    auto right_vendor = right_hand->GetVendorInfo();
    AgilinkLogger::get().infof(TAG, "\nLeft Hand: %s Serial: %s", left_vendor.productModel.c_str(), left_vendor.productSeqNum.c_str());
    AgilinkLogger::get().infof(TAG, "Right Hand: %s Serial: %s", right_vendor.productModel.c_str(), right_vendor.productSeqNum.c_str());

    std::vector<int16_t> left_pos(4, 2048);
    std::vector<int16_t> right_pos(4, 2048);
    left_hand->SetAllJointMotorPosi(left_pos);
    right_hand->SetAllJointMotorPosi(right_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto left_read = left_hand->GetAllJointMotorPosi();
    auto right_read = right_hand->GetAllJointMotorPosi();
    {
      std::string msg = "Left positions: [";
      for (size_t i = 0; i < left_read.size(); ++i) {
        if (i) msg += ", ";
        msg += std::to_string(left_read[i]);
      }
      msg += "]";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
    {
      std::string msg = "Right positions: [";
      for (size_t i = 0; i < right_read.size(); ++i) {
        if (i) msg += ", ";
        msg += std::to_string(right_read[i]);
      }
      msg += "]";
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
  }

  AgilinkLogger::get().infof(TAG, "\n[Done]: Example completed successfully!");
  return 0;
}
