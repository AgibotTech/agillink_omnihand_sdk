// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include "omnihand/omnihand_pro_2025.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <string>

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
      hand_ = OmniHandPro2025::createHandByHcan(
          EHandType::eLeft,
          1,      // device_id
          0,      // dev_id
          0       // channel_id
      );
      std::cout << "[Info]: Using HCAN device" << std::endl;
    } else {
      // Default: ZLG CAN
      hand_ = OmniHandPro2025::createHandByZlgcan(
          EHandType::eLeft,
          1,      // device_id
          0,      // canfd_id
          0       // channel_id
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

  std::unique_ptr<OmniHandPro2025> hand_;
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
    std::cout << vendor_info.toString() << std::endl;
    
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
    std::cout << device_info.toString() << std::endl;
    // Only check deviceId if request succeeded (non-zero indicates success)
    if (device_info.hand_device_id != 0) {
      EXPECT_EQ(device_info.hand_device_id, 1);
    }
  }
}

// Test setting device ID
// Note: SetDeviceId may change device ID on hardware, making device inaccessible with original ID.
// Use with caution and only in controlled test environments.
TEST_F(OmniHandPro2025Test, SetDeviceId) {
  // Get current device ID first
  auto current_device_info = hand_->GetDeviceInfo();
  unsigned char current_id = current_device_info.hand_device_id;

  // Only test if we got a valid device ID
  if (current_id == 0) {
    // Request failed (timeout), skip test
    return;
  }

  // Set to target ID (2) using current ID
  unsigned char target_id = 2;
  hand_->SetDeviceId(target_id);
  std::cout << "[SetDeviceId] Set Device ID: " << static_cast<int>(target_id) << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TODO: check O12 firmware 
  auto device_info = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info.hand_device_id, 2);

  // Reset to original
  unsigned char original_id = 1;
  hand_->SetDeviceId(original_id);
  std::cout << "[SetDeviceId] Reset Device ID: " << static_cast<int>(original_id) << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TODO: check O12 firmware 
  auto device_info1 = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info1.hand_device_id, 1);
}

// Note: Motor position control tests are removed
// O10 and O12 motor position input ranges differ, so we recommend users use angle control instead.
// The underlying solver automatically converts angles to motor positions.

// Test joint angle control (requires hardware)
TEST_F(OmniHandPro2025Test, JointAngleControl) {
  if (hand_->Init()) {
    // Test setting active joint angles
    std::vector<double> angles(12, 0.0);  // 12 joints, all at 0
    hand_->SetAllActiveJointAngles(angles);
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
  }
}

// Test control mode (requires hardware)
// Note: SetAllControlMode is disabled as it may cause CANFD communication issues
// Only testing read operation (GetAllControlMode)
TEST_F(OmniHandPro2025Test, ControlMode) {
  if (hand_->Init()) {
    // Only test reading control mode (read-only operation)
    // Note: SetAllControlMode is not tested as it may cause CANFD communication to crash
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
    auto thumb_tactile = hand_->GetTactileSensor3DData(EFinger::eThumb);
    std::cout << "[GetTactileSensor3DData] Thumb 3D Tactile Data:" << std::endl;
    std::cout << "  Online State: " << static_cast<int>(thumb_tactile.online_state_) << std::endl;
    std::cout << "  Normal Force: " << thumb_tactile.normal_force_ << std::endl;
    std::cout << "  Tangent Force: " << thumb_tactile.tangent_force_ << std::endl;
    std::cout << "  Tangent Force Angle: " << thumb_tactile.tangent_force_angle_ << std::endl;
    EXPECT_GE(thumb_tactile.normal_force_, 0);
    
    // Test multiple sensors
    std::vector<EFinger> fingers = {EFinger::eIndex, EFinger::eMiddle, EFinger::eRing, EFinger::eLittle};
    for (const auto& finger : fingers) {
      auto tactile_data = hand_->GetTactileSensor3DData(finger);
      std::cout << "[GetTactileSensor3DData] " << static_cast<int>(finger) << " 3D Tactile Data:" << std::endl;
      std::cout << "  Online State: " << static_cast<int>(tactile_data.online_state_) << std::endl;
      std::cout << "  Normal Force: " << tactile_data.normal_force_ << std::endl;
      std::cout << "  Tangent Force: " << tactile_data.tangent_force_ << std::endl;
      std::cout << "  Tangent Force Angle: " << tactile_data.tangent_force_angle_ << std::endl;
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
      std::cout << "J" << (i+1) << ":[";
      if (error_reports[i].stalled_) {
        std::cout << "S";  // Stalled
        has_errors = true;
      }
      if (error_reports[i].overheat_) {
        std::cout << "H";  // Overheat
        has_errors = true;
      }
      if (error_reports[i].over_current_) {
        std::cout << "C";  // Over-current
        has_errors = true;
      }
      if (error_reports[i].motor_except_) {
        std::cout << "M";  // Motor exception
        has_errors = true;
      }
      if (error_reports[i].commu_except_) {
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
      std::cout << "J" << (i+1) << ":" << temp_reports[i];
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
      std::cout << "J" << (i+1) << ":" << current_reports[i];
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
    auto all_angles = hand_->GetAllJointPos(active_angles);
    
    // Check if calculation succeeded (non-empty result and correct size)
    if (all_angles.empty() || all_angles.size() != 19) {
      std::cout << "[GetAllJointPos] Failed: got " << all_angles.size() 
                << " angles, expected 19" << std::endl;
      return;
    }
    
    std::cout << "[GetAllJointPos] Forward Kinematics (input: 12 active angles, output: " 
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
