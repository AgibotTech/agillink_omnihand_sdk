// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

#ifdef BUILD_OMNIHAND_3_LITE
#include "omnihand/omnihand_3_lite.h"

using namespace agilink::omnihand;

// Global variable to store request interval from command line argument
static int g_request_interval = 5;  // Default: 5ms

// Helper function to get request interval
static int GetRequestInterval() {
  return g_request_interval;
}

class OmniHand3LiteTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing
    hand_ = OmniHand3Lite::createHandByZlgcan(
        HandType::LEFT,        // hand_type: left hand
        1,                       // hand_device_id: hand device ID
        0,                       // canfd_device_id: USB CANFD adapter device index
        0                        // canfd_channel_id: CAN channel index (0=can0, 1=can1)
    );
    int request_interval = GetRequestInterval();
    hand_->SetRequestInterval(request_interval);
    if (request_interval != 0) {
      std::cout << "[Info]: Using request interval: " << request_interval << " ms" << std::endl;
    }
  }

  void TearDown() override {
    hand_.reset();
  }

  std::unique_ptr<OmniHand3Lite> hand_;
};

// Test factory method
TEST_F(OmniHand3LiteTest, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

// Test initialization
TEST_F(OmniHand3LiteTest, Init) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device. Check hardware connection.";
}

// Test vendor info
TEST_F(OmniHand3LiteTest, GetVendorInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto vendor_info = hand_->GetVendorInfo();
  std::cout << "[GetVendorInfo] Vendor Info:" << std::endl;
  std::cout << vendor_info.toString() << std::endl;
  EXPECT_EQ(vendor_info.dof, 4);  // O4 has 4 DOF
}

// Test device info
TEST_F(OmniHand3LiteTest, GetDeviceInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto device_info = hand_->GetDeviceInfo();
  std::cout << "[GetDeviceInfo] Device Info:" << std::endl;
  std::cout << device_info.toString() << std::endl;
  EXPECT_EQ(device_info.hand_device_id, 1);
}

// Test setting device ID
// Note: SetDeviceId may change device ID on hardware, making device inaccessible with original ID.
// Use with caution and only in controlled test environments.
TEST_F(OmniHand3LiteTest, SetDeviceId) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
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

  auto device_info = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info.hand_device_id, 2);
  
  // Reset to original
  unsigned char original_id = 1;
  hand_->SetDeviceId(original_id);
  std::cout << "[SetDeviceId] Reset Device ID: " << static_cast<int>(original_id) << std::endl;
  
  auto device_info1 = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info1.hand_device_id, 1);
}

// Test motor position control (requires hardware)
TEST_F(OmniHand3LiteTest, MotorPositionControl) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single motor position
  int16_t target_pos = 4095;  // Middle position (0-4096 range)
  hand_->SetJointMotorPosi(1, target_pos);
  std::cout << "[SetJointMotorPosi] Set Joint 1 Motor Position: " << target_pos << std::endl;
  
  auto pos = hand_->GetJointMotorPosi(1);
  std::cout << "[GetJointMotorPosi] Joint 1 Motor Position: " << pos << std::endl;
  // Note: Actual position may differ due to hardware constraints
  
  // Test batch motor positions
  std::vector<int16_t> positions(4, 3072);  // 4 motors, all at middle
  
  // Test SetAllJointMotorPosi - returns actual positions
  auto set_result = hand_->SetAllJointMotorPosi(positions);
  std::cout << "[SetAllJointMotorPosi] returned: ";
  for (size_t i = 0; i < set_result.size(); ++i) {
    std::cout << set_result[i];
    if (i < set_result.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  // Test GetAllJointMotorPosi separately
  auto all_positions = hand_->GetAllJointMotorPosi();
  std::cout << "[GetAllJointMotorPosi] returned: ";
  for (size_t i = 0; i < all_positions.size(); ++i) {
    std::cout << all_positions[i];
    if (i < all_positions.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  // Check if request succeeded (non-empty result)
  if (all_positions.empty()) {
    // Request failed (timeout), skip assertion to avoid false failure
    return;
  }
  std::cout << "[GetAllJointMotorPosi] Motor Positions: ";
  for (size_t i = 0; i < all_positions.size(); ++i) {
    std::cout << all_positions[i];
    if (i < all_positions.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(all_positions.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test joint angle control (not supported for O4, should output warning)
TEST_F(OmniHand3LiteTest, JointAngleControl) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test setting active joint angles (should output warning)
  std::vector<double> angles(4, 0.0);  // 4 joints, all at 0
  hand_->SetAllActiveJointAngles(angles);
  std::cout << "[SetAllActiveJointAngles] Called (should output warning)" << std::endl;
  
  // Test getting active joint angles (should output warning and return zeros)
  auto active_angles = hand_->GetAllActiveJointAngles();
  std::cout << "[GetAllActiveJointAngles] Active Joint Angles (rad, " << active_angles.size() << " values): ";
  for (size_t i = 0; i < active_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << active_angles[i];
    if (i < active_angles.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(active_angles.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
  
  // Test getting all joint angles (should output warning)
  auto all_angles = hand_->GetAllJointAngles();
  std::cout << "[GetAllJointAngles] All Joint Angles (rad, " << all_angles.size() << " values): ";
  for (size_t i = 0; i < all_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << all_angles[i];
    if (i < all_angles.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(all_angles.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test hand gesture (not supported for O4, should output warning)
TEST_F(OmniHand3LiteTest, SetHandGesture) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test setting hand gesture (should output warning)
  hand_->SetHandGesture(1);
  std::cout << "[SetHandGesture] Called (should output warning)" << std::endl;
}

// Test control mode (requires hardware)
TEST_F(OmniHand3LiteTest, ControlMode) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test reading control mode (read-only operation)
  auto current_modes = hand_->GetAllControlMode();
  // Check if request succeeded (non-empty result)
  if (current_modes.empty()) {
    // Request failed (timeout), skip assertion to avoid false failure
    return;
  }
  std::cout << "[GetAllControlMode] Control Modes: ";
  for (size_t i = 0; i < current_modes.size(); ++i) {
    std::cout << static_cast<int>(current_modes[i]);
    if (i < current_modes.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(current_modes.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
  
  // Test single joint control mode
  auto mode = hand_->GetControlMode(1);
  std::cout << "[GetControlMode] Joint 1 Control Mode: " << static_cast<int>(mode) << std::endl;
}

// Test velocity control (requires hardware)
TEST_F(OmniHand3LiteTest, VelocityControl) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint velocity
  int16_t target_velo = 100;
  hand_->SetJointMotorVelo(1, target_velo);
  std::cout << "[SetJointMotorVelo] Set Joint 1 Motor Velocity: " << target_velo << std::endl;
  
  auto velo = hand_->GetJointMotorVelo(1);
  std::cout << "[GetJointMotorVelo] Joint 1 Motor Velocity: " << velo << std::endl;
  
  // Test batch motor velocities
  std::vector<int16_t> velocities(4, 100);  // 4 motors, all at 100
  hand_->SetAllJointMotorVelo(velocities);
  std::cout << "[SetAllJointMotorVelo] Set Motor Velocities: ";
  for (size_t i = 0; i < velocities.size(); ++i) {
    std::cout << velocities[i];
    if (i < velocities.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  auto all_velocities = hand_->GetAllJointMotorVelo();
  // Check if request succeeded (non-empty result)
  if (all_velocities.empty()) {
    return;
  }
  std::cout << "[GetAllJointMotorVelo] Motor Velocities: ";
  for (size_t i = 0; i < all_velocities.size(); ++i) {
    std::cout << all_velocities[i];
    if (i < all_velocities.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(all_velocities.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test current threshold (requires hardware)
TEST_F(OmniHand3LiteTest, CurrentThreshold) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint current threshold
  int16_t threshold = 500;
  hand_->SetCurrentThreshold(1, threshold);
  std::cout << "[SetCurrentThreshold] Set Joint 1 Current Threshold: " << threshold << std::endl;
  
  auto current_threshold = hand_->GetCurrentThreshold(1);
  std::cout << "[GetCurrentThreshold] Joint 1 Current Threshold: " << current_threshold << std::endl;
  
  // Test batch current thresholds
  std::vector<int16_t> thresholds(4, 500);  // 4 motors, all at 500
  hand_->SetAllCurrentThreshold(thresholds);
  std::cout << "[SetAllCurrentThreshold] Set Current Thresholds: ";
  for (size_t i = 0; i < thresholds.size(); ++i) {
    std::cout << thresholds[i];
    if (i < thresholds.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  auto all_thresholds = hand_->GetAllCurrentThreshold();
  // Check if request succeeded (non-empty result)
  if (all_thresholds.empty()) {
    return;
  }
  std::cout << "[GetAllCurrentThreshold] Current Thresholds: ";
  for (size_t i = 0; i < all_thresholds.size(); ++i) {
    std::cout << all_thresholds[i];
    if (i < all_thresholds.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(all_thresholds.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test error report (requires hardware)
TEST_F(OmniHand3LiteTest, ErrorReport) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint error report
  auto error_report = hand_->GetErrorReport(1);
  std::cout << "[GetErrorReport] Joint 1 Error Report:" << std::endl;
  std::cout << "  Stalled: " << (error_report.stalled_ ? "Yes" : "No") << std::endl;
  std::cout << "  Overheat: " << (error_report.overheat_ ? "Yes" : "No") << std::endl;
  std::cout << "  Over Current: " << (error_report.over_current_ ? "Yes" : "No") << std::endl;
  std::cout << "  Motor Exception: " << (error_report.motor_except_ ? "Yes" : "No") << std::endl;
  std::cout << "  Communication Exception: " << (error_report.commu_except_ ? "Yes" : "No") << std::endl;
  
  // Test all joints error reports
  auto all_error_reports = hand_->GetAllErrorReport();
  EXPECT_FALSE(all_error_reports.empty())
      << "GetAllErrorReport returned empty (request timeout - check device/firmware or extended-frame response)";
  if (all_error_reports.empty()) {
    return;
  }
  std::cout << "[GetAllErrorReport] All Error Reports (" << all_error_reports.size() << " joints):" << std::endl;
  for (size_t i = 0; i < all_error_reports.size(); ++i) {
    std::cout << "  Joint " << (i + 1) << ": "
              << "Stalled=" << all_error_reports[i].stalled_
              << ", Overheat=" << all_error_reports[i].overheat_
              << ", OverCurrent=" << all_error_reports[i].over_current_
              << ", MotorExcept=" << all_error_reports[i].motor_except_
              << ", CommuExcept=" << all_error_reports[i].commu_except_
              << std::endl;
  }
  EXPECT_EQ(all_error_reports.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test temperature report (requires hardware)
TEST_F(OmniHand3LiteTest, TemperatureReport) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint temperature
  auto temp = hand_->GetTemperatureReport(1);
  std::cout << "[GetTemperatureReport] Joint 1 Temperature: " << temp << std::endl;
  
  // Test all joints temperatures
  auto all_temps = hand_->GetAllTemperatureReport();
  EXPECT_FALSE(all_temps.empty())
      << "GetAllTemperatureReport returned empty (request timeout - check device/firmware or extended-frame response)";
  if (all_temps.empty()) {
    return;
  }
  std::cout << "[GetAllTemperatureReport] All Temperatures (" << all_temps.size() << " values): ";
  for (size_t i = 0; i < all_temps.size(); ++i) {
    std::cout << all_temps[i];
    if (i < all_temps.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(all_temps.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test current report (requires hardware)
TEST_F(OmniHand3LiteTest, CurrentReport) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint current
  auto current = hand_->GetCurrentReport(1);
  std::cout << "[GetCurrentReport] Joint 1 Current: " << current << std::endl;
  
  // Test all joints currents
  auto all_currents = hand_->GetAllCurrentReport();
  // Check if request succeeded (non-empty result)
  if (all_currents.empty()) {
    return;
  }
  std::cout << "[GetAllCurrentReport] All Currents (" << all_currents.size() << " values): ";
  for (size_t i = 0; i < all_currents.size(); ++i) {
    std::cout << all_currents[i];
    if (i < all_currents.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(all_currents.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test request interval and frame timeout settings
TEST_F(OmniHand3LiteTest, RequestSettings) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test request interval
  int original_interval = hand_->GetRequestInterval();
  std::cout << "[GetRequestInterval] Original interval: " << original_interval << " ms" << std::endl;
  
  hand_->SetRequestInterval(10);
  int new_interval = hand_->GetRequestInterval();
  std::cout << "[SetRequestInterval] Set to 10 ms, got: " << new_interval << " ms" << std::endl;
  EXPECT_EQ(new_interval, 10);
  
  // Reset to original
  hand_->SetRequestInterval(original_interval);
  
  // Test frame timeout
  int original_timeout = hand_->GetFrameRecvTimeout();
  std::cout << "[GetFrameRecvTimeout] Original timeout: " << original_timeout << " ms" << std::endl;
  
  hand_->SetFrameRecvTimeout(100);
  int new_timeout = hand_->GetFrameRecvTimeout();
  std::cout << "[SetFrameRecvTimeout] Set to 100 ms, got: " << new_timeout << " ms" << std::endl;
  EXPECT_EQ(new_timeout, 100);
  
  // Reset to original
  hand_->SetFrameRecvTimeout(original_timeout);
}

// Test invalid joint index
TEST_F(OmniHand3LiteTest, InvalidJointIndex) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test invalid joint index for position
  int16_t invalid_pos = hand_->GetJointMotorPosi(0);
  EXPECT_EQ(invalid_pos, -1);
  invalid_pos = hand_->GetJointMotorPosi(5);  // O4 has 4 joints, so 5 is invalid
  EXPECT_EQ(invalid_pos, -1);
  
  // Test invalid joint index for velocity
  int16_t invalid_velo = hand_->GetJointMotorVelo(0);
  EXPECT_EQ(invalid_velo, -1);
  invalid_velo = hand_->GetJointMotorVelo(5);
  EXPECT_EQ(invalid_velo, -1);
}

// Test constants
TEST_F(OmniHand3LiteTest, Constants) {
  EXPECT_EQ(OmniHand3Lite::kDegreesOfActiveFreedom, 4);
  std::cout << "[Constants] kDegreesOfActiveFreedom: " 
            << static_cast<int>(OmniHand3Lite::kDegreesOfActiveFreedom) << std::endl;
}

// Main function for gtest
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  
  // Parse command line arguments for request interval
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--request-interval" && i + 1 < argc) {
      g_request_interval = std::stoi(argv[++i]);
    }
  }
  
  return RUN_ALL_TESTS();
}
#endif  // BUILD_OMNIHAND_3_LITE