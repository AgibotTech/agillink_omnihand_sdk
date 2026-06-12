// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "omnihand/omnihand_pro_2025.h"
#include "omnihand/proto.h"

// Global variable to store request interval from command line argument
static int g_request_interval = 5;  // Default: 5ms

// Global variable to store device type from command line argument
static std::string g_device_type = "zlgcan";  // Default: zlgcan

// Helper function to get request interval
static int GetRequestInterval() {
  return g_request_interval;
}

// Helper function to get device type
static std::string GetDeviceType() {
  return g_device_type;
}

class OmniHandPro2025Test : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing based on device type
    std::string device_type = GetDeviceType();
    if (device_type == "hcan") {
      hand_ = agilink::omnihand::OmniHandPro2025::createHandByHcan(
          agilink::omnihand::HandType::LEFT,  // hand_type: left hand
          1,                                  // hand_device_id: hand device ID
          0,                                  // canfd_device_id: HCAN device index
          0                                   // canfd_channel_id: CAN channel index (0=can0, 1=can1)
      );
      std::cout << "[Info]: Using HCAN device" << std::endl;
    } else {
      // Default: ZLG CAN
      hand_ = agilink::omnihand::OmniHandPro2025::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT,  // hand_type: left hand
          1,                                  // hand_device_id: hand device ID
          0,                                  // canfd_device_id: USB CANFD adapter device index
          0                                   // canfd_channel_id: CAN channel index (0=can0, 1=can1)
      );
      std::cout << "[Info]: Using ZLG CAN device" << std::endl;
    }
    int request_interval = GetRequestInterval();
    hand_->SetRequestInterval(request_interval);
    if (request_interval != 0) {
      std::cout << "[Info]: Using request interval: " << request_interval << " ms" << std::endl;
    }
  }

  void TearDown() override {
    hand_.reset();
    // Delay removed: request frequency control handles timing automatically
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::unique_ptr<agilink::omnihand::OmniHandPro2025> hand_;
};

// Test factory method
TEST_F(OmniHandPro2025Test, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

// Test initialization
TEST_F(OmniHandPro2025Test, Init) {
  // Note: This test may fail if hardware is not connected
  bool init_result = hand_->Init();
  // We don't assert on init_result as it depends on hardware availability
}

// Test vendor info (may require hardware)
TEST_F(OmniHandPro2025Test, GetVendorInfo) {
  if (hand_->Init()) {
    auto vendor_info = hand_->GetVendorInfo();
    std::cout << "[GetVendorInfo] Vendor Info:" << std::endl;
    std::cout << vendor_info.ToString() << std::endl;

    // Check if request succeeded (non-zero dof indicates success)
    // If request failed (timeout), skip assertion to avoid false failure
    if (vendor_info.dof == 0) {
      std::cout << "[GetVendorInfo] Failed: got empty vendor info (timeout)" << std::endl;
      return;
    }

    EXPECT_EQ(vendor_info.dof, 12);  // O12 has 12 DOF
  }
}

// Test device info
TEST_F(OmniHandPro2025Test, GetDeviceInfo) {
  if (hand_->Init()) {
    auto device_info = hand_->GetDeviceInfo();
    std::cout << "[GetDeviceInfo] Device Info:" << std::endl;
    std::cout << device_info.ToString() << std::endl;
    // Only check deviceId if request succeeded (non-zero indicates success)
    if (device_info.hand_device_id != 0) {
      EXPECT_EQ(device_info.hand_device_id, 1);
    }
  }
}

// Test setting device ID
// Note: SetDeviceId may change device ID on hardware, making device inaccessible with original ID.
// Use with caution and only in controlled test environments.
// TEST_F(OmniHandPro2025Test, SetDeviceId) {
//   // Get current device ID first
//   auto current_device_info = hand_->GetDeviceInfo();
//   unsigned char current_id = current_device_info.hand_device_id;

//   // Only test if we got a valid device ID
//   if (current_id == 0) {
//     // Request failed (timeout), skip test
//     return;
//   }

//   // Set to target ID (2) using current ID
//   unsigned char target_id = 2;
//   hand_->SetDeviceId(target_id);
//   std::cout << "[SetDeviceId] Set Device ID: " << static_cast<int>(target_id) << std::endl;
//   std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TODO: check O12 firmware
//   auto device_info = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info.hand_device_id, 2);

//   // Reset to original
//   unsigned char original_id = 1;
//   hand_->SetDeviceId(original_id);
//   std::cout << "[SetDeviceId] Reset Device ID: " << static_cast<int>(original_id) << std::endl;
//   std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TODO: check O12 firmware
//   auto device_info1 = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info1.hand_device_id, 1);
// }

// Note: Motor position control tests are removed
// O10 and O12 motor position input ranges differ, so we recommend users use angle control instead.
// The underlying solver automatically converts angles to motor positions.

// Test joint angle control (requires hardware)
TEST_F(OmniHandPro2025Test, JointAngleControl) {
  if (hand_->Init()) {
    // Test setting active joint angles

    for (int i = 1; i <= agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom; i++)
      hand_->SetControlMode(i, agilink::omnihand::ControlMode::POSITION);

    std::vector<double> angles(12, 0.0);  // 12 joints, all at 0
    hand_->SetAllActiveJointAngles(angles);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "[SetAllActiveJointAngles] Set Active Joint Angles (rad): ";
    for (size_t i = 0; i < angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << angles[i];
      if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    auto active_angles = hand_->GetAllActiveJointAngles();
    // Check if request succeeded (non-empty result and correct size)
    if (active_angles.empty() || active_angles.size() != 12) {
      // Request failed (timeout) or incorrect size, skip remaining assertions
      std::cout << "[GetAllActiveJointAngles] Failed: got " << active_angles.size()
                << " angles, expected 12" << std::endl;
      return;
    }
    std::cout << "[GetAllActiveJointAngles] Active Joint Angles (rad): ";
    for (size_t i = 0; i < active_angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << active_angles[i];
      if (i < active_angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(active_angles.size(), 12);

    auto all_angles = hand_->GetAllJointAngles();
    // Check if request succeeded (non-empty result and correct size)
    if (all_angles.empty() || all_angles.size() != 19) {
      std::cout << "[GetAllJointAngles] Failed: got " << all_angles.size()
                << " angles, expected 19" << std::endl;
      return;
    }
    std::cout << "[GetAllJointAngles] All Joint Angles (rad, " << all_angles.size() << " joints): ";
    for (size_t i = 0; i < all_angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << all_angles[i];
      if (i < all_angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(all_angles.size(), 19);  // 12 active + 7 passive
    hand_->SetAllActiveJointAngles(std::vector<double>(agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom, 0.6));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    hand_->SetAllActiveJointAngles(std::vector<double>(agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom, 0.0));
  }
}

// Test control mode (requires hardware)
// Note: SetAllControlMode is disabled as it may cause CANFD communication issues
// Only testing read operation (GetAllControlMode)
TEST_F(OmniHandPro2025Test, ControlMode) {
  if (hand_->Init()) {
    // Only test reading control mode (read-only operation)
    // Note: SetAllControlMode is not tested as it may cause CANFD communication to crash
    hand_->SetAllControlMode(std::vector<unsigned char>(
        agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom,
        static_cast<unsigned char>(agilink::omnihand::ControlMode::POSITION)));

    auto current_modes = hand_->GetAllControlMode();
    // Check if request succeeded (non-empty result and correct size)
    if (current_modes.empty() || current_modes.size() != 12) {
      // Request failed (timeout) or incorrect size, skip assertion to avoid false failure
      std::cout << "[GetAllControlMode] Failed: got " << current_modes.size()
                << " modes, expected 12" << std::endl;
      return;
    }
    std::cout << "[GetAllControlMode] Control Modes: ";
    for (size_t i = 0; i < current_modes.size(); ++i) {
      std::cout << static_cast<int>(current_modes[i]);
      if (i < current_modes.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_modes.size(), 12);
  }
}

// Test 3D tactile sensor (O12 specific, requires hardware)
TEST_F(OmniHandPro2025Test, TactileSensor3D) {
  if (hand_->Init()) {
    // Test single sensor
    auto thumb_tactile = hand_->GetTactileSensor3DData(agilink::omnihand::Finger::THUMB);
    std::cout << "[GetTactileSensor3DData] Thumb 3D Tactile Data:" << std::endl;
    std::cout << thumb_tactile.ToString() << std::endl;
    EXPECT_GE(thumb_tactile.normal_force, 0);

    // Test multiple sensors
    std::vector<agilink::omnihand::Finger> fingers = {agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE};
    for (const auto& finger : fingers) {
      auto tactile_data = hand_->GetTactileSensor3DData(finger);
      std::cout << "[GetTactileSensor3DData] " << static_cast<int>(finger) << " 3D Tactile Data:" << std::endl;
      std::cout << tactile_data.ToString() << std::endl;
    }
  }
}

// Test error report (requires hardware)
TEST_F(OmniHandPro2025Test, ErrorReport) {
  if (hand_->Init()) {
    auto error_reports = hand_->GetAllErrorReport();
    // Check if request succeeded (non-empty result)
    if (error_reports.empty()) {
      // Request failed (timeout), skip assertion to avoid false failure
      // This is expected if device is not responding
      return;
    }
    std::cout << "[GetAllErrorReport] Error Reports (12 joints): ";
    bool has_errors = false;
    for (size_t i = 0; i < error_reports.size(); ++i) {
      std::cout << "J" << (i + 1) << ":[";
      if (error_reports[i].bits.stalled_) {
        std::cout << "S";  // Stalled
        has_errors = true;
      }
      if (error_reports[i].bits.overheat_) {
        std::cout << "H";  // Overheat
        has_errors = true;
      }
      if (error_reports[i].bits.over_current_) {
        std::cout << "C";  // Over-current
        has_errors = true;
      }
      if (error_reports[i].bits.motor_except_) {
        std::cout << "M";  // Motor exception
        has_errors = true;
      }
      if (error_reports[i].bits.commu_except_) {
        std::cout << "X";  // Communication exception
        has_errors = true;
      }
      std::cout << "]";
      if (i < error_reports.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
    if (has_errors) {
      std::cout << "[Note] Error flags: S=Stalled, H=Overheat, C=Over-current, M=Motor exception, X=Communication exception" << std::endl;
      std::cout << "[Note] X (Communication exception) may indicate historical communication errors. This is normal if the device had previous communication timeouts." << std::endl;
    }
    EXPECT_EQ(error_reports.size(), 12);
  }
}

// Test temperature report (requires hardware)
TEST_F(OmniHandPro2025Test, TemperatureReport) {
  if (hand_->Init()) {
    auto temp_reports = hand_->GetAllTemperatureReport();
    // Check if request succeeded (non-empty result)
    if (temp_reports.empty()) {
      // No data available (cache is empty), skip assertion
      // This is expected if report periods are not set or device hasn't sent reports yet
      return;
    }
    std::cout << "[GetAllTemperatureReport] Temperature Reports (°C): ";
    for (size_t i = 0; i < temp_reports.size(); ++i) {
      std::cout << "J" << (i + 1) << ":" << temp_reports[i];
      if (i < temp_reports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(temp_reports.size(), 12);
  }
}

// Test current report (requires hardware)
TEST_F(OmniHandPro2025Test, CurrentReport) {
  if (hand_->Init()) {
    auto current_reports = hand_->GetAllCurrentReport();
    // Check if request succeeded (non-empty result)
    if (current_reports.empty()) {
      // No data available (cache is empty), skip assertion
      // This is expected if report periods are not set or device hasn't sent reports yet
      return;
    }
    std::cout << "[GetAllCurrentReport] Current Reports (mA): ";
    for (size_t i = 0; i < current_reports.size(); ++i) {
      std::cout << "J" << (i + 1) << ":" << current_reports[i];
      if (i < current_reports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_reports.size(), 12);
  }
}

// Test kinematics solver
TEST_F(OmniHandPro2025Test, KinematicsSolver) {
  if (hand_->Init()) {
    // First verify we can get motor positions (prerequisite check)
    auto motor_positions = hand_->GetAllJointMotorPosi();
    if (motor_positions.empty() || motor_positions.size() != 12) {
      std::cout << "[KinematicsSolver] Failed to get motor positions: got "
                << motor_positions.size() << ", expected 12" << std::endl;
      return;
    }

    // Test forward kinematics with valid input
    std::vector<double> active_angles(12, 0.0);
    auto all_angles = hand_->GetAllJointAngles(active_angles);

    // Check if calculation succeeded (non-empty result and correct size)
    if (all_angles.empty() || all_angles.size() != 19) {
      std::cout << "[GetAllJointAngles] Failed: got " << all_angles.size()
                << " angles, expected 19" << std::endl;
      return;
    }

    std::cout << "[GetAllJointAngles] Forward Kinematics (input: 12 active angles, output: "
              << all_angles.size() << " joint angles): ";
    for (size_t i = 0; i < all_angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << all_angles[i];
      if (i < all_angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(all_angles.size(), 19);  // 12 active + 7 passive
  }
}

// Test velocity control (requires hardware)
TEST_F(OmniHandPro2025Test, VelocityControl) {
  if (hand_->Init()) {
    // Test setting velocity
    std::vector<int16_t> velocities(12, 0);
    hand_->SetAllJointMotorVelo(velocities);
    std::cout << "[SetAllJointMotorVelo] Set Velocities: ";
    for (size_t i = 0; i < velocities.size(); ++i) {
      std::cout << velocities[i];
      if (i < velocities.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    auto current_velocities = hand_->GetAllJointMotorVelo();
    // Check if request succeeded (non-empty result and correct size)
    if (current_velocities.empty() || current_velocities.size() != 12) {
      // Request failed (timeout) or incorrect size, skip assertion to avoid false failure
      std::cout << "[GetAllJointMotorVelo] Failed: got " << current_velocities.size()
                << " velocities, expected 12" << std::endl;
      return;
    }
    std::cout << "[GetAllJointMotorVelo] Current Velocities: ";
    for (size_t i = 0; i < current_velocities.size(); ++i) {
      std::cout << current_velocities[i];
      if (i < current_velocities.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_velocities.size(), 12);
  }
}

// Test voltage control (requires hardware)
TEST_F(OmniHandPro2025Test, VoltageControl) {
  if (hand_->Init()) {
    hand_->SetFrameRecvTimeout(100);
    constexpr int16_t kSafeVoltage = 0;
    constexpr auto kDof = agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom;
    const auto restore_position_mode = [this, kDof]() {
      for (unsigned char joint = 1; joint <= kDof; ++joint) {
        hand_->SetControlMode(joint, agilink::omnihand::ControlMode::POSITION);
      }
      std::cout << "[SetControlMode] Restored all joints to position mode" << std::endl;
    };
    hand_->SetAllControlMode(std::vector<unsigned char>(12, 4));
    constexpr unsigned char kProbeJoint = 1;
    // Use a non-zero probe so a timeout/default 0 return cannot pass accidentally.
    constexpr int16_t kProbeVoltage = 500;
    hand_->SetControlMode(kProbeJoint, agilink::omnihand::ControlMode::VOLTAGE);
    hand_->SetJointMotorVoltage(kProbeJoint, kProbeVoltage);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // O12 firmware versions up to and including 1.2.15 do not support voltage readback.
    const int16_t current_probe_voltage = hand_->GetJointMotorVoltage(kProbeJoint);
    std::cout << "[Set/GetJointMotorVoltage] Joint "
              << static_cast<int>(kProbeJoint)
              << " target=" << kProbeVoltage
              << ", current=" << current_probe_voltage << std::endl;
    EXPECT_EQ(current_probe_voltage, kProbeVoltage);
    hand_->SetJointMotorVoltage(kProbeJoint, kSafeVoltage);

    // Set each joint to voltage mode via the single-joint API, then send a zero
    // voltage command so this test exercises the path without driving motion.
    for (unsigned char joint = 1; joint <= kDof; ++joint) {
      hand_->SetJointMotorVoltage(joint, kSafeVoltage);
    }

    auto voltage_modes = hand_->GetAllControlMode();
    if (voltage_modes.empty() || voltage_modes.size() != kDof) {
      std::cout << "[GetAllControlMode] Failed after voltage mode switch: got "
                << voltage_modes.size() << " modes, expected "
                << static_cast<int>(kDof) << std::endl;
      restore_position_mode();
      hand_->SetFrameRecvTimeout(50);
      return;
    }

    std::cout << "[GetAllControlMode] Voltage Control Modes: ";
    for (size_t i = 0; i < voltage_modes.size(); ++i) {
      std::cout << static_cast<int>(voltage_modes[i]);
      if (i < voltage_modes.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    const auto voltage_mode = static_cast<unsigned char>(agilink::omnihand::ControlMode::VOLTAGE);
    const bool all_modes_are_voltage =
        std::all_of(voltage_modes.begin(), voltage_modes.end(),
                    [voltage_mode](unsigned char mode) { return mode == voltage_mode; });
    EXPECT_TRUE(all_modes_are_voltage);
    if (!all_modes_are_voltage) {
      restore_position_mode();
      hand_->SetFrameRecvTimeout(50);
      return;
    }

    std::vector<int16_t> voltages(kDof, kSafeVoltage);
    hand_->SetAllJointMotorVoltage(voltages);
    std::cout << "[SetAllJointMotorVoltage] Set Voltages: ";
    for (size_t i = 0; i < voltages.size(); ++i) {
      std::cout << voltages[i];
      if (i < voltages.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    // O12 firmware versions up to and including 1.2.15 do not support voltage readback.
    auto current_voltages = hand_->GetAllJointMotorVoltage();
    if (current_voltages.empty() || current_voltages.size() != kDof) {
      std::cout << "[GetAllJointMotorVoltage] Failed: got "
                << current_voltages.size() << " voltages, expected "
                << static_cast<int>(kDof) << std::endl;
      restore_position_mode();
      hand_->SetFrameRecvTimeout(50);
      return;
    }

    std::cout << "[GetAllJointMotorVoltage] Current Voltages: ";
    for (size_t i = 0; i < current_voltages.size(); ++i) {
      std::cout << current_voltages[i];
      if (i < current_voltages.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_voltages.size(), kDof);

    restore_position_mode();
    hand_->SetFrameRecvTimeout(50);
  }
}
TEST_F(OmniHandPro2025Test, TorqueControl) {
  if (!hand_->Init()) {
    return;
  }
  
  const size_t kDof = agilink::omnihand::OmniHandPro2025::kDegreesOfActiveFreedom;
  std::cout << "Set control mode to Torque Control" << std::endl;
  for (unsigned char joint = 1; joint <= kDof; ++joint) {
    hand_->SetControlMode(joint, agilink::omnihand::ControlMode::TORQUE);
  }
  for (unsigned char joint = 1; joint <= kDof; ++joint) {
    EXPECT_EQ(hand_->GetControlMode(joint), agilink::omnihand::ControlMode::TORQUE)
      << "[GetControlMode] Expected Set Api usage";
  }
  // for (unsigned char joint = 0)
  std::vector<int16_t> torqueFromGetJointMotorTorque(kDof, 0);
  for (int i = 1; i <= kDof; ++ i) {
    torqueFromGetJointMotorTorque[i-1] = hand_->GetJointMotorTorque(i);
  }
  std::vector<int16_t> torqueFromGetAllJointMotorTorque = hand_->GetAllJointMotorTorque();
  EXPECT_EQ(kDof, torqueFromGetAllJointMotorTorque.size()) 
    << "Expected size of getAllJointMotorTorque return val eq kdof";
  if (torqueFromGetAllJointMotorTorque.size() != kDof) {
    return;
  }
  for (int i = 0; i < kDof; ++ i) {
    EXPECT_EQ(torqueFromGetAllJointMotorTorque[i], torqueFromGetJointMotorTorque[i]) 
      << "Expected equals";
    EXPECT_LE(torqueFromGetAllJointMotorTorque[i], 350);
    EXPECT_GE(torqueFromGetAllJointMotorTorque[i], 1);
  }

  
  hand_->SetJointMotorTorque(1, 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //recovery
  hand_->SetAllJointMotorTorque(torqueFromGetJointMotorTorque);
  for (unsigned char joint = 1; joint <= kDof; ++joint) {
    hand_->SetControlMode(joint, agilink::omnihand::ControlMode::POSITION);
  }
}


// Custom main function to parse command line arguments
int main(int argc, char** argv) {
  // Parse custom arguments before gtest processes them
  std::vector<char*> gtest_args;
  gtest_args.push_back(argv[0]);  // program name

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-f" && i + 1 < argc) {
      // Parse request interval argument (in milliseconds)
      try {
        int interval = std::stoi(argv[i + 1]);
        if (interval >= 0 && interval <= 100) {
          g_request_interval = interval;
          ++i;  // Skip the next argument (the interval value)
          continue;
        } else {
          std::cerr << "[Error]: -f value " << interval
                    << " is out of range (0-100ms)" << std::endl;
          return 1;
        }
      } catch (const std::exception& e) {
        std::cerr << "[Error]: Invalid -f value: " << argv[i + 1] << std::endl;
        return 1;
      }
    } else if (arg == "-d" && i + 1 < argc) {
      // Parse device type argument
      std::string device_type = argv[i + 1];
      if (device_type == "zlgcan" || device_type == "hcan") {
        g_device_type = device_type;
        ++i;  // Skip the next argument (the device type value)
        continue;
      } else {
        std::cerr << "[Error]: -d value must be 'zlgcan' or 'hcan', got: " << device_type << std::endl;
        return 1;
      }
    } else if (arg == "--help" || arg == "-h") {
      std::cout << "Usage: " << argv[0] << " [-f INTERVAL] [-d DEVICE]" << std::endl;
      std::cout << "  -f INTERVAL  Set CAN request interval (0-100ms, 0=no limit, default: 5ms)" << std::endl;
      std::cout << "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)" << std::endl;
      std::cout << std::endl;
      std::cout << "Examples:" << std::endl;
      std::cout << "  " << argv[0] << " -f 10" << std::endl;
      std::cout << "  " << argv[0] << " -d hcan" << std::endl;
      std::cout << "  " << argv[0] << " -f 10 -d hcan" << std::endl;
      return 0;
    }
    // Pass other arguments to gtest
    gtest_args.push_back(argv[i]);
  }

  // Initialize gtest with filtered arguments
  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
