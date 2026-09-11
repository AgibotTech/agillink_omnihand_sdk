// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include "omnihand/omnihand_2025.h"
#include <memory>
#include <vector>
#include <string>
#include <cstdio>
#include "agilink_logger.h"

using agilink::AgilinkLogger;

static constexpr const char* TAG = "OmniHand2025Test";

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

class OmniHand2025Test : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing based on device type
    std::string device_type = GetDeviceType();
    ASSERT_NO_THROW({
      if (device_type == "hcan") {
        hand_ = agilink::omnihand::OmniHand2025::createHandByHcan(
            agilink::omnihand::HandType::LEFT,  // hand_type: left hand
            1,                                  // hand_device_id: hand device ID
            0,                                  // canfd_device_id: USB CANFD adapter device index
            0                                   // canfd_channel_id: CAN channel index (0=can0, 1=can1)
        );
      } else {  // default: zlgcan
        hand_ = agilink::omnihand::OmniHand2025::createHandByZlgcan(
            agilink::omnihand::HandType::LEFT,  // hand_type: left hand
            1,                                  // hand_device_id: hand device ID
            0,                                  // canfd_device_id: USB CANFD adapter device index
            0                                   // canfd_channel_id: CAN channel index (0=can0, 1=can1)
        );
      }
    }) << "Failed to create OmniHand2025 instance";
    int request_interval = GetRequestInterval();
    hand_->SetRequestInterval(request_interval);
    hand_->ShowDataDetails(true);
    if (request_interval != 0) {
      AgilinkLogger::get().infof(TAG, "Using request interval: %d ms", request_interval);
    }
    AgilinkLogger::get().infof(TAG, "Using device type: %s", device_type.c_str());
  }

  void TearDown() override {
    hand_.reset();
  }

  std::unique_ptr<agilink::omnihand::OmniHand2025> hand_ = nullptr;
};

// Test factory method
TEST_F(OmniHand2025Test, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

// Test initialization
TEST_F(OmniHand2025Test, Init) {
  // Note: This test may fail if hardware is not connected
  // In CI/CD, you might want to skip this or mock the hardware
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
}

// Test vendor info (may require hardware)
TEST_F(OmniHand2025Test, GetVendorInfo) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  auto vendor_info = hand_->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo] Vendor Info:");
  AgilinkLogger::get().infof(TAG, "%s", vendor_info.ToString().c_str());
  EXPECT_EQ(vendor_info.dof, 10);  // O10 has 10 DOF
}

// Test device info
TEST_F(OmniHand2025Test, GetDeviceInfo) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] Device Info:");
  AgilinkLogger::get().infof(TAG, "%s", device_info.ToString().c_str());
  // A zero ID indicates that the request did not return a valid device.
  ASSERT_NE(device_info.hand_device_id, 0)
      << "GetDeviceInfo returned an invalid device ID";
  EXPECT_EQ(device_info.hand_device_id, 1);
}

// Test setting device ID
// Note: SetDeviceId may change device ID on hardware, making device inaccessible with original ID.
// Use with caution and only in controlled test environments.
TEST_F(OmniHand2025Test, SetDeviceId) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  // Get current device ID first
  auto current_device_info = hand_->GetDeviceInfo();
  unsigned char current_id = current_device_info.hand_device_id;

  // Abort before mutating hardware if the request did not return a valid device ID.
  ASSERT_NE(current_id, 0)
      << "GetDeviceInfo returned an invalid device ID";

  // Set to target ID (2) using current ID
  unsigned char target_id = 2;
  hand_->SetDeviceId(target_id);
  AgilinkLogger::get().infof(TAG, "[SetDeviceId] Set Device ID: %d", static_cast<int>(target_id));

  auto device_info = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info.hand_device_id, 2);

  // Reset to original
  unsigned char original_id = 1;
  hand_->SetDeviceId(original_id);
  AgilinkLogger::get().infof(TAG, "[SetDeviceId] Reset Device ID: %d", static_cast<int>(original_id));

  auto device_info1 = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info1.hand_device_id, 1);
}

// Test motor position control (requires hardware)
TEST_F(OmniHand2025Test, MotorPositionControl) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  // Test single motor position
  int16_t target_pos = 2048;  // Middle position (0-4096 range)
  hand_->SetJointMotorPosi(1, target_pos);
  AgilinkLogger::get().infof(TAG, "[SetJointMotorPosi] Set Joint 1 Motor Position: %d", target_pos);

  auto pos = hand_->GetJointMotorPosi(1);
  AgilinkLogger::get().infof(TAG, "[GetJointMotorPosi] Joint 1 Motor Position: %d", pos);
  // Note: Actual position may differ due to hardware constraints

  // Test batch motor positions
  std::vector<int16_t> positions(10, 2048);  // 10 motors, all at middle

  // Test SetAllJointMotorPosi - returns actual positions
  auto set_result = hand_->SetAllJointMotorPosi(positions);
  {
    std::string msg;
    for (size_t i = 0; i < set_result.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(set_result[i]);
    }
    AgilinkLogger::get().infof(TAG, "[SetAllJointMotorPosi] returned: %s", msg.c_str());
  }

  // Test GetAllJointMotorPosi separately
  auto all_positions = hand_->GetAllJointMotorPosi();
  {
    std::string msg;
    for (size_t i = 0; i < all_positions.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(all_positions[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorPosi] returned: %s", msg.c_str());
  }
  ASSERT_FALSE(all_positions.empty())
      << "GetAllJointMotorPosi returned no positions";
  {
    std::string msg;
    for (size_t i = 0; i < all_positions.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(all_positions[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorPosi] Motor Positions: %s", msg.c_str());
  }
  EXPECT_EQ(all_positions.size(), 10);
}

// Test joint angle control (requires hardware)
TEST_F(OmniHand2025Test, JointAngleControl) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  // Test setting active joint angles
  std::vector<double> angles(10, 0.0);  // 10 joints, all at 0
  hand_->SetAllActiveJointAngles(angles);
  {
    std::string msg;
    char buf[24];
    for (size_t i = 0; i < angles.size(); ++i) {
      if (i > 0) msg += ", ";
      snprintf(buf, sizeof(buf), "%.4f", angles[i]);
      msg += buf;
    }
    AgilinkLogger::get().infof(TAG, "[SetAllActiveJointAngles] Set Active Joint Angles (rad): %s", msg.c_str());
  }

  auto active_angles = hand_->GetAllActiveJointAngles();
  ASSERT_FALSE(active_angles.empty())
      << "GetAllActiveJointAngles returned no angles";
  {
    std::string msg;
    char buf[24];
    for (size_t i = 0; i < active_angles.size(); ++i) {
      if (i > 0) msg += ", ";
      snprintf(buf, sizeof(buf), "%.4f", active_angles[i]);
      msg += buf;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllActiveJointAngles] Active Joint Angles (rad): %s", msg.c_str());
  }
  EXPECT_EQ(active_angles.size(), 10);

  auto all_angles = hand_->GetAllJointAngles();
  ASSERT_FALSE(all_angles.empty())
      << "GetAllJointAngles returned no angles";
  {
    std::string msg;
    char buf[24];
    for (size_t i = 0; i < all_angles.size(); ++i) {
      if (i > 0) msg += ", ";
      snprintf(buf, sizeof(buf), "%.4f", all_angles[i]);
      msg += buf;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] All Joint Angles (rad, %zu joints): %s",
                           all_angles.size(), msg.c_str());
  }
  EXPECT_EQ(all_angles.size(), 16);  // 10 active + 6 passive
}

// Test tactile sensor (requires hardware)
TEST_F(OmniHand2025Test, TactileSensor) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  // Test GetTactileSensorData (downsampled data) for all fingers
  std::vector<agilink::omnihand::Finger> fingers = {
      agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE,
      agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE, agilink::omnihand::Finger::PALM,
      agilink::omnihand::Finger::DORSUM};

  AgilinkLogger::get().infof(TAG, "[GetTactileSensorData] Getting all sensor data:");
  for (const auto& finger : fingers) {
    auto tactile_data = hand_->GetTactileSensorData(finger);
    ASSERT_FALSE(tactile_data.empty())
        << "GetTactileSensorData returned no data for "
        << agilink::omnihand::ToString(finger);
    std::string msg;
    for (size_t i = 0; i < tactile_data.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(static_cast<int>(tactile_data[i]));
    }
    AgilinkLogger::get().infof(TAG, "  %s (%zu values): %s",
                           agilink::omnihand::ToString(finger).c_str(),
                           tactile_data.size(), msg.c_str());
  }
}

// Test tactile sensor raw data (requires hardware)
TEST_F(OmniHand2025Test, TactileSensorRaw) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  // Test all 7 sensors individually (Thumb, Index, Middle, Ring, Little, Palm, Dorsum)
  std::vector<agilink::omnihand::Finger> all_fingers = {
      agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX,
      agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING,
      agilink::omnihand::Finger::LITTLE, agilink::omnihand::Finger::PALM,
      agilink::omnihand::Finger::DORSUM};

  // Check if firmware supports raw tactile sensor data by testing Thumb first
  auto thumb_tactile = hand_->GetTactileSensorDataRaw(agilink::omnihand::Finger::THUMB);
  // Unsupported firmware is a skipped test, not an assertion failure.
  if (thumb_tactile.data_.empty()) {
    GTEST_SKIP() << "Raw tactile sensor data is not supported by this firmware version";
  }

  AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] Individual Sensors (unit: 1g, max: 255g)");
  for (const auto& finger : all_fingers) {
    auto tactile_data = hand_->GetTactileSensorDataRaw(finger);
    ASSERT_FALSE(tactile_data.data_.empty())
        << "GetTactileSensorDataRaw returned no data for "
        << agilink::omnihand::ToString(finger);
    size_t print_count = std::min(tactile_data.data_.size(), size_t(10));
    std::string msg;
    for (size_t i = 0; i < print_count; ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(static_cast<int>(tactile_data.data_[i]));
    }
    if (tactile_data.data_.size() > 10) msg += " ...";
    AgilinkLogger::get().infof(TAG, "  %s (%zu values): %s",
                           agilink::omnihand::ToString(finger).c_str(),
                           tactile_data.data_.size(), msg.c_str());
    EXPECT_EQ(tactile_data.sensor_id_, finger);
  }

  // Test getting all tactile sensor data
  auto all_tactile_data = hand_->GetAllTactileSensorDataRaw();
  ASSERT_FALSE(all_tactile_data.empty())
      << "GetAllTactileSensorDataRaw returned no sensor data";
  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] All Tactile Sensors: %zu sensors (unit: 1g, max: 255g)",
                         all_tactile_data.size());
  for (const auto& sensor : all_tactile_data) {
    size_t print_count = std::min(sensor.data_.size(), size_t(10));
    std::string msg;
    for (size_t i = 0; i < print_count; ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(static_cast<int>(sensor.data_[i]));
    }
    if (sensor.data_.size() > 10) msg += " ...";
    AgilinkLogger::get().infof(TAG, "  %s (%zu values): %s",
                           agilink::omnihand::ToString(sensor.sensor_id_).c_str(),
                           sensor.data_.size(), msg.c_str());
  }
  EXPECT_EQ(all_tactile_data.size(), all_fingers.size());
}

// Test error report (requires hardware)
TEST_F(OmniHand2025Test, ErrorReport) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  auto error_reports = hand_->GetAllErrorReport();
  ASSERT_FALSE(error_reports.empty())
      << "GetAllErrorReport returned no reports";
  {
    std::string msg = "[GetAllErrorReport] Error Reports (10 joints): ";
    bool has_errors = false;
    for (size_t i = 0; i < error_reports.size(); ++i) {
      if (i > 0) msg += " ";
      msg += "J" + std::to_string(i + 1) + ":[";
      if (error_reports[i].bits.stalled_)      { msg += "S"; has_errors = true; }
      if (error_reports[i].bits.overheat_)     { msg += "H"; has_errors = true; }
      if (error_reports[i].bits.over_current_) { msg += "C"; has_errors = true; }
      if (error_reports[i].bits.motor_except_) { msg += "M"; has_errors = true; }
      if (error_reports[i].bits.commu_except_) { msg += "X"; has_errors = true; }
      msg += "]";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    if (has_errors) {
      AgilinkLogger::get().infof(TAG, "[Note] Error flags: S=Stalled, H=Overheat, C=Over-current, M=Motor exception, X=Communication exception");
      AgilinkLogger::get().infof(TAG, "[Note] X (Communication exception) may indicate historical communication errors. This is normal if the device had previous communication timeouts.");
    }
  }
  EXPECT_EQ(error_reports.size(), 10);
}

// Test temperature report (requires hardware)
TEST_F(OmniHand2025Test, TemperatureReport) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  auto temp_reports = hand_->GetAllTemperatureReport();
  ASSERT_FALSE(temp_reports.empty())
      << "GetAllTemperatureReport returned no reports";
  {
    std::string msg;
    for (size_t i = 0; i < temp_reports.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += "J" + std::to_string(i + 1) + ":" + std::to_string(temp_reports[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllTemperatureReport] Temperature Reports (°C): %s", msg.c_str());
  }
  EXPECT_EQ(temp_reports.size(), 10);
}

// Test current report (requires hardware)
TEST_F(OmniHand2025Test, CurrentReport) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  auto current_reports = hand_->GetAllCurrentReport();
  ASSERT_FALSE(current_reports.empty())
      << "GetAllCurrentReport returned no reports";
  {
    std::string msg;
    for (size_t i = 0; i < current_reports.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += "J" + std::to_string(i + 1) + ":" + std::to_string(current_reports[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllCurrentReport] Current Reports (mA): %s", msg.c_str());
  }
  EXPECT_EQ(current_reports.size(), 10);
}

// Test kinematics solver
TEST_F(OmniHand2025Test, KinematicsSolver) {
  ASSERT_TRUE(hand_->Init())
      << "Failed to initialize HCAN device";
  // Test forward kinematics
  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointAngles(active_angles);
  {
    std::string msg;
    char buf[24];
    size_t cnt = std::min(all_angles.size(), size_t(10));
    for (size_t i = 0; i < cnt; ++i) {
      if (i > 0) msg += ", ";
      snprintf(buf, sizeof(buf), "%.4f", all_angles[i]);
      msg += buf;
    }
    if (all_angles.size() > 10) msg += " ...";
    AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] Forward Kinematics (input: 10 active angles, output: %zu joint angles): %s",
                           all_angles.size(), msg.c_str());
  }
  ASSERT_EQ(all_angles.size(), 16);  // 10 active + 6 passive
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
          AgilinkLogger::get().errorf(TAG, "[Error]: -f value %d is out of range (0-100ms)", interval);
          return 1;
        }
      } catch (const std::exception& e) {
        AgilinkLogger::get().errorf(TAG, "[Error]: Invalid -f value: %s", argv[i + 1]);
        return 1;
      }
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      std::string device_arg = argv[i + 1];
      if (device_arg == "zlgcan" || device_arg == "hcan") {
        g_device_type = device_arg;
        ++i;  // Skip the next argument (the device type value)
        continue;
      } else {
        AgilinkLogger::get().errorf(TAG, "[Error]: -d value must be 'zlgcan' or 'hcan', got: %s",
                                device_arg.c_str());
        return 1;
      }
    } else if (arg == "--help" || arg == "-h") {
      AgilinkLogger::get().infof(TAG, "Usage: %s [-f INTERVAL] [-d DEVICE]", argv[0]);
      AgilinkLogger::get().infof(TAG, "  -f INTERVAL  Set CAN request interval (0-100ms, 0=no limit, default: 5ms)");
      AgilinkLogger::get().infof(TAG, "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)");
      AgilinkLogger::get().infof(TAG, "Example:");
      AgilinkLogger::get().infof(TAG, "  %s -f 20", argv[0]);
      AgilinkLogger::get().infof(TAG, "  %s -d hcan", argv[0]);
      AgilinkLogger::get().infof(TAG, "  %s -f 20 -d hcan", argv[0]);
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
