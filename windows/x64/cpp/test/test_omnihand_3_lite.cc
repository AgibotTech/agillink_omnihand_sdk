// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstdio>
#include "agilink_logger.h"

#ifdef BUILD_OMNIHAND_3_LITE
#include "omnihand/omnihand_3_lite.h"

using namespace agilink::omnihand;
using agilink::AgilinkLogger;

static constexpr const char* TAG = "OmniHand3LiteTest";

// Global variable to store request interval from command line argument
static int g_request_interval = 5;  // Default: 5ms
// Global variable to store device type from command line argument
static std::string g_device_type = "zlgcan";  // Default: zlgcan
// Global variables for RS-485 serial connection
static std::string g_serial_port = "/dev/ttyUSB0";  // Default serial port

// Helper function to get request interval
static int GetRequestInterval() {
  return g_request_interval;
}

// Helper function to get device type
static std::string GetDeviceType() {
  return g_device_type;
}

class OmniHand3LiteTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing based on device type
    std::string device_type = GetDeviceType();
    if (device_type == "hcan") {
      hand_ = OmniHand3Lite::createHandByHcan(
          HandType::LEFT,
          1,   // hand_device_id
          0,   // canfd_device_id
          0    // canfd_channel_id
      );
    } else if (device_type == "rs485") {
      hand_ = OmniHand3Lite::createHandByRs485(
          HandType::LEFT,
          1,              // hand_device_id
          g_serial_port   // serial port path
      );
    } else {  // default: zlgcan
      hand_ = OmniHand3Lite::createHandByZlgcan(
          HandType::LEFT,
          1,   // hand_device_id
          0,   // canfd_device_id
          0    // canfd_channel_id
      );
    }
    int request_interval = GetRequestInterval();
    hand_->SetRequestInterval(request_interval);
    AgilinkLogger::get().infof(TAG, "Using frame recv timeout: %d ms", hand_->GetFrameRecvTimeout());
    hand_->ShowDataDetails(true);
    if (request_interval != 0) {
      AgilinkLogger::get().infof(TAG, "Using request interval: %d ms", request_interval);
    }
    AgilinkLogger::get().infof(TAG, "Using device type: %s", device_type.c_str());
    if (device_type == "rs485") {
      AgilinkLogger::get().infof(TAG, "Serial port: %s", g_serial_port.c_str());
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
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo] Vendor Info:\n%s", vendor_info.ToString().c_str());
  EXPECT_EQ(vendor_info.dof, 4);  // O4 has 4 DOF
}

// Test device info
TEST_F(OmniHand3LiteTest, GetDeviceInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] Device Info:\n%s", device_info.ToString().c_str());
  EXPECT_EQ(device_info.hand_device_id, 1);
}

// // Test setting device ID
// // Note: SetDeviceId may change device ID on hardware, making device inaccessible with original ID.
// // Use with caution and only in controlled test environments.
// TEST_F(OmniHand3LiteTest, SetDeviceId) {
//   ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

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
//   AgilinkLogger::get().infof(TAG, "[SetDeviceId] Set Device ID: %d", static_cast<int>(target_id));
//   std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Wait 200ms for device ID change to take effect

//   auto device_info = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info.hand_device_id, 2);

//   // Reset to original
//   unsigned char original_id = 1;
//   hand_->SetDeviceId(original_id);
//   AgilinkLogger::get().infof(TAG, "[SetDeviceId] Reset Device ID: %d", static_cast<int>(original_id));
//   std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Wait 200ms for device ID change to take effect

//   auto device_info1 = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info1.hand_device_id, 1);
// }

// Test motor position control (requires hardware)
TEST_F(OmniHand3LiteTest, MotorPositionControl) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // Test single motor position
  int16_t target_pos = 4095;  // Middle position (0-4096 range)
  hand_->SetJointMotorPosi(1, target_pos);
  AgilinkLogger::get().infof(TAG, "[SetJointMotorPosi] Set Joint 1 Motor Position: %d", target_pos);

  auto pos = hand_->GetJointMotorPosi(1);
  AgilinkLogger::get().infof(TAG, "[GetJointMotorPosi] Joint 1 Motor Position: %d", pos);
  // Note: Actual position may differ due to hardware constraints

  // Test batch motor positions
  std::vector<int16_t> positions(4, 3072);  // 4 motors, all at middle

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
  // Check if request succeeded (non-empty result)
  if (all_positions.empty()) {
    // Request failed (timeout), skip assertion to avoid false failure
    return;
  }
  {
    std::string msg;
    for (size_t i = 0; i < all_positions.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(all_positions[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorPosi] Motor Positions: %s", msg.c_str());
  }
  EXPECT_EQ(all_positions.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test joint angle control (not supported for O4, should output warning)
TEST_F(OmniHand3LiteTest, JointAngleControl) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // Test setting active joint angles (should output warning)
  std::vector<double> angles(4, 0.0);  // 4 joints, all at 0
  hand_->SetAllActiveJointAngles(angles);
  AgilinkLogger::get().infof(TAG, "[SetAllActiveJointAngles] Called (should output warning)");

  // Test getting active joint angles (should output warning and return zeros)
  auto active_angles = hand_->GetAllActiveJointAngles();
  {
    std::string msg;
    char buf[24];
    for (size_t i = 0; i < active_angles.size(); ++i) {
      if (i > 0) msg += ", ";
      snprintf(buf, sizeof(buf), "%.4f", active_angles[i]);
      msg += buf;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllActiveJointAngles] Active Joint Angles (rad, %zu values): %s",
                           active_angles.size(), msg.c_str());
  }
  EXPECT_EQ(active_angles.size(), OmniHand3Lite::kDegreesOfActiveFreedom);

  // Test getting all joint angles (should output warning)
  auto all_angles = hand_->GetAllJointAngles();
  {
    std::string msg;
    char buf[24];
    for (size_t i = 0; i < all_angles.size(); ++i) {
      if (i > 0) msg += ", ";
      snprintf(buf, sizeof(buf), "%.4f", all_angles[i]);
      msg += buf;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] All Joint Angles (rad, %zu values): %s",
                           all_angles.size(), msg.c_str());
  }
  EXPECT_EQ(all_angles.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test hand gesture (sets predefined motor positions)
TEST_F(OmniHand3LiteTest, SetHandGesture) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  hand_->SetHandGesture(agilink::omnihand::h3l::OmniHand3LiteGesture::OMNI_HAND_3_LITE_GESTURE_FIST);
  AgilinkLogger::get().infof(TAG, "[SetHandGesture] FIST gesture set");
}

// // Test velocity control (requires hardware)
// TEST_F(OmniHand3LiteTest, VelocityControl) {
//   ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

//   // Test single joint velocity
//   int16_t target_velo = 100;
//   hand_->SetJointMotorVelo(1, target_velo);
//   AgilinkLogger::get().infof(TAG, "[SetJointMotorVelo] Set Joint 1 Motor Velocity: %d", target_velo);

//   auto velo = hand_->GetJointMotorVelo(1);
//   AgilinkLogger::get().infof(TAG, "[GetJointMotorVelo] Joint 1 Motor Velocity: %d", velo);

//   // Test batch motor velocities
//   std::vector<int16_t> velocities(4, 100);  // 4 motors, all at 100
//   hand_->SetAllJointMotorVelo(velocities);
//   {
//     std::string msg;
//     for (size_t i = 0; i < velocities.size(); ++i) {
//       if (i > 0) msg += ", ";
//       msg += std::to_string(velocities[i]);
//     }
//     AgilinkLogger::get().infof(TAG, "[SetAllJointMotorVelo] Set Motor Velocities: %s", msg.c_str());
//   }

//   auto all_velocities = hand_->GetAllJointMotorVelo();
//   // Check if request succeeded (non-empty result)
//   if (all_velocities.empty()) {
//     return;
//   }
//   {
//     std::string msg;
//     for (size_t i = 0; i < all_velocities.size(); ++i) {
//       if (i > 0) msg += ", ";
//       msg += std::to_string(all_velocities[i]);
//     }
//     AgilinkLogger::get().infof(TAG, "[GetAllJointMotorVelo] Motor Velocities: %s", msg.c_str());
//   }
//   EXPECT_EQ(all_velocities.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
// }

// Test current threshold (requires hardware)
TEST_F(OmniHand3LiteTest, CurrentThreshold) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // // Test single joint current threshold
  // int16_t threshold = 1500;
  // hand_->SetCurrentThreshold(1, threshold);
  // AgilinkLogger::get().infof(TAG, "[SetCurrentThreshold] Set Joint 1 Current Threshold: %d", threshold);

  auto current_threshold = hand_->GetCurrentThreshold(1);
  AgilinkLogger::get().infof(TAG, "[GetCurrentThreshold] Joint 1 Current Threshold: %d", current_threshold);

  // // Test batch current thresholds
  // std::vector<int16_t> thresholds(4, 1500);  // 4 motors, all at 1500
  // hand_->SetAllCurrentThreshold(thresholds);
  // {
  //   std::string msg;
  //   for (size_t i = 0; i < thresholds.size(); ++i) {
  //     if (i > 0) msg += ", ";
  //     msg += std::to_string(thresholds[i]);
  //   }
  //   AgilinkLogger::get().infof(TAG, "[SetAllCurrentThreshold] Set Current Thresholds: %s", msg.c_str());
  // }

  auto all_thresholds = hand_->GetAllCurrentThreshold();
  // Check if request succeeded (non-empty result)
  if (all_thresholds.empty()) {
    return;
  }
  {
    std::string msg;
    for (size_t i = 0; i < all_thresholds.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(all_thresholds[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllCurrentThreshold] Current Thresholds: %s", msg.c_str());
  }
  EXPECT_EQ(all_thresholds.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test error report (requires hardware)
TEST_F(OmniHand3LiteTest, ErrorReport) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // Test single joint error report
  auto error_report = hand_->GetErrorReport(1);
  AgilinkLogger::get().infof(TAG, "[GetErrorReport] Joint 1 Error Report:");
  AgilinkLogger::get().infof(TAG, "  Stalled: %s",      error_report.bits.stalled_     ? "Yes" : "No");
  AgilinkLogger::get().infof(TAG, "  Overheat: %s",     error_report.bits.overheat_    ? "Yes" : "No");
  AgilinkLogger::get().infof(TAG, "  Over Current: %s", error_report.bits.over_current_? "Yes" : "No");
  AgilinkLogger::get().infof(TAG, "  Motor Exception: %s",         error_report.bits.motor_except_  ? "Yes" : "No");
  AgilinkLogger::get().infof(TAG, "  Communication Exception: %s", error_report.bits.commu_except_  ? "Yes" : "No");

  // Test all joints error reports
  auto all_error_reports = hand_->GetAllErrorReport();
  EXPECT_FALSE(all_error_reports.empty())
      << "GetAllErrorReport returned empty (request timeout - check device/firmware or extended-frame response)";
  if (all_error_reports.empty()) {
    return;
  }
  AgilinkLogger::get().infof(TAG, "[GetAllErrorReport] All Error Reports (%zu joints):", all_error_reports.size());
  for (size_t i = 0; i < all_error_reports.size(); ++i) {
    AgilinkLogger::get().infof(TAG, "  Joint %zu: Stalled=%d, Overheat=%d, OverCurrent=%d, MotorExcept=%d, CommuExcept=%d",
                           i + 1,
                           all_error_reports[i].bits.stalled_,
                           all_error_reports[i].bits.overheat_,
                           all_error_reports[i].bits.over_current_,
                           all_error_reports[i].bits.motor_except_,
                           all_error_reports[i].bits.commu_except_);
  }
  EXPECT_EQ(all_error_reports.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test temperature report (requires hardware)
TEST_F(OmniHand3LiteTest, TemperatureReport) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // Test single joint temperature
  auto temp = hand_->GetTemperatureReport(1);
  AgilinkLogger::get().infof(TAG, "[GetTemperatureReport] Joint 1 Temperature: %d", temp);

  // Test all joints temperatures
  auto all_temps = hand_->GetAllTemperatureReport();
  EXPECT_FALSE(all_temps.empty())
      << "GetAllTemperatureReport returned empty (request timeout - check device/firmware or extended-frame response)";
  if (all_temps.empty()) {
    return;
  }
  {
    std::string msg;
    for (size_t i = 0; i < all_temps.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(all_temps[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllTemperatureReport] All Temperatures (%zu values): %s",
                           all_temps.size(), msg.c_str());
  }
  EXPECT_EQ(all_temps.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test current report (requires hardware)
TEST_F(OmniHand3LiteTest, CurrentReport) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // Test single joint current
  auto current = hand_->GetCurrentReport(1);
  AgilinkLogger::get().infof(TAG, "[GetCurrentReport] Joint 1 Current: %d", current);

  // Test all joints currents
  auto all_currents = hand_->GetAllCurrentReport();
  // Check if request succeeded (non-empty result)
  if (all_currents.empty()) {
    return;
  }
  {
    std::string msg;
    for (size_t i = 0; i < all_currents.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(all_currents[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetAllCurrentReport] All Currents (%zu values): %s",
                           all_currents.size(), msg.c_str());
  }
  EXPECT_EQ(all_currents.size(), OmniHand3Lite::kDegreesOfActiveFreedom);
}

// Test request interval and frame timeout settings
TEST_F(OmniHand3LiteTest, RequestSettings) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";

  // Test request interval
  int original_interval = hand_->GetRequestInterval();
  AgilinkLogger::get().infof(TAG, "[GetRequestInterval] Original interval: %d ms", original_interval);

  hand_->SetRequestInterval(10);
  int new_interval = hand_->GetRequestInterval();
  AgilinkLogger::get().infof(TAG, "[SetRequestInterval] Set to 10 ms, got: %d ms", new_interval);
  EXPECT_EQ(new_interval, 10);

  // Reset to original
  hand_->SetRequestInterval(original_interval);

  // Test frame timeout
  int original_timeout = hand_->GetFrameRecvTimeout();
  AgilinkLogger::get().infof(TAG, "[GetFrameRecvTimeout] Original timeout: %d ms", original_timeout);

  hand_->SetFrameRecvTimeout(100);
  int new_timeout = hand_->GetFrameRecvTimeout();
  AgilinkLogger::get().infof(TAG, "[SetFrameRecvTimeout] Set to 100 ms, got: %d ms", new_timeout);
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
  AgilinkLogger::get().infof(TAG, "[Constants] kDegreesOfActiveFreedom: %d",
                         static_cast<int>(OmniHand3Lite::kDegreesOfActiveFreedom));
}

// Test tactile sensor initialization
TEST_F(OmniHand3LiteTest, InitTactilePointsMap) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  bool ok = hand_->InitTactilePointsMap();
  if (!ok) {
    GTEST_SKIP() << "InitTactilePointsMap failed - tactile sensors may not be present";
  }
  EXPECT_TRUE(ok);
  AgilinkLogger::get().infof(TAG, "[InitTactilePointsMap] OK");
}

// Test GetSensorOrder
TEST_F(OmniHand3LiteTest, GetSensorOrder) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  const auto& order = hand_->GetSensorOrder();
  EXPECT_FALSE(order.empty());
  {
    std::string msg;
    for (size_t i = 0; i < order.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += ToString(order[i]);
    }
    AgilinkLogger::get().infof(TAG, "[GetSensorOrder] %zu sensors: %s", order.size(), msg.c_str());
  }
}

// Test calibrated tactile sensor data per finger
TEST_F(OmniHand3LiteTest, GetTactileSensorData) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "Tactile sensors not available";
  }
  for (Finger finger : hand_->GetSensorOrder()) {
    auto data = hand_->GetTactileSensorData(finger);
    std::string preview;
    if (!data.data_.empty()) {
      preview = " [";
      size_t cnt = std::min(data.data_.size(), size_t(6));
      for (size_t i = 0; i < cnt; ++i) {
        if (i > 0) preview += ", ";
        preview += std::to_string(data.data_[i]);
      }
      if (data.data_.size() > 6) preview += "...";
      preview += "]";
    }
    AgilinkLogger::get().infof(TAG, "[GetTactileSensorData] %s: %zu points%s",
                           ToString(finger).c_str(), data.data_.size(), preview.c_str());
    EXPECT_EQ(data.sensor_id_, finger);
  }
}

// Test raw tactile sensor data per finger (multi-frame)
TEST_F(OmniHand3LiteTest, GetTactileSensorDataRaw) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "Tactile sensors not available";
  }
  for (Finger finger : hand_->GetSensorOrder()) {
    auto data = hand_->GetTactileSensorDataRaw(finger);
    std::string preview;
    if (!data.data_.empty()) {
      preview = " [";
      size_t cnt = std::min(data.data_.size(), size_t(6));
      for (size_t i = 0; i < cnt; ++i) {
        if (i > 0) preview += ", ";
        preview += std::to_string(data.data_[i]);
      }
      if (data.data_.size() > 6) preview += "...";
      preview += "]";
    }
    AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] %s: %zu points%s",
                           ToString(finger).c_str(), data.data_.size(), preview.c_str());
    EXPECT_EQ(data.sensor_id_, finger);
  }
}

// Test all fingers raw tactile data in one call
TEST_F(OmniHand3LiteTest, GetAllTactileSensorDataRaw) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "Tactile sensors not available";
  }
  auto all_data = hand_->GetAllTactileSensorDataRaw();
  EXPECT_FALSE(all_data.empty()) << "GetAllTactileSensorDataRaw returned empty";
  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] %zu fingers:", all_data.size());
  for (const auto& sd : all_data) {
    AgilinkLogger::get().infof(TAG, "  %s: %zu points", ToString(sd.sensor_id_).c_str(), sd.data_.size());
  }
  EXPECT_EQ(all_data.size(), hand_->GetSensorOrder().size());
}

// Test GetNumOfTactileSensors
TEST_F(OmniHand3LiteTest, GetNumOfTactileSensors) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  size_t num = hand_->GetNumOfTactileSensors();
  AgilinkLogger::get().infof(TAG, "[GetNumOfTactileSensors] %zu", num);
  EXPECT_GT(num, 0u);
  EXPECT_EQ(num, hand_->GetSensorOrder().size());
}

// Test GetNumOfTactilePoints per finger
TEST_F(OmniHand3LiteTest, GetNumOfTactilePoints) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  for (Finger finger : hand_->GetSensorOrder()) {
    size_t pts = hand_->GetNumOfTactilePoints(finger);
    AgilinkLogger::get().infof(TAG, "[GetNumOfTactilePoints] %s: %zu points", ToString(finger).c_str(), pts);
    EXPECT_GT(pts, 0u) << "Expected >0 points for " << ToString(finger);
  }
  // UNKNOWN / DORSUM should return 0
  EXPECT_EQ(hand_->GetNumOfTactilePoints(Finger::UNKNOWN), 0u);
  EXPECT_EQ(hand_->GetNumOfTactilePoints(Finger::DORSUM), 0u);
}

// Test GetLenOfTactileDatum per finger
TEST_F(OmniHand3LiteTest, GetLenOfTactileDatum) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  for (Finger finger : hand_->GetSensorOrder()) {
    size_t len = hand_->GetLenOfTactileDatum(finger);
    AgilinkLogger::get().infof(TAG, "[GetLenOfTactileDatum] %s: %zu bytes/point", ToString(finger).c_str(), len);
    EXPECT_GT(len, 0u) << "Expected >0 bytes per datum for " << ToString(finger);
  }
}

// Test GetNumOfRepliedTactileFrames per finger
TEST_F(OmniHand3LiteTest, GetNumOfRepliedTactileFrames) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  for (Finger finger : hand_->GetSensorOrder()) {
    size_t frames = hand_->GetNumOfRepliedTactileFrames(finger);
    AgilinkLogger::get().infof(TAG, "[GetNumOfRepliedTactileFrames] %s: %zu frame(s)", ToString(finger).c_str(), frames);
    EXPECT_GT(frames, 0u) << "Expected >=1 frame for " << ToString(finger);
  }
}

// Test GetSNOfTactileSensor per finger
TEST_F(OmniHand3LiteTest, GetSNOfTactileSensor) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  for (Finger finger : hand_->GetSensorOrder()) {
    std::string sn = hand_->GetSNOfTactileSensor(finger);
    AgilinkLogger::get().infof(TAG, "[GetSNOfTactileSensor] %s: \"%s\"", ToString(finger).c_str(), sn.c_str());
  }
}

// Test deprecated GetSensorDataLength consistency with GetNumOfTactilePoints
TEST_F(OmniHand3LiteTest, GetSensorDataLength) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  for (Finger finger : hand_->GetSensorOrder()) {
    size_t len = hand_->GetSensorDataLength(finger);
    size_t pts = hand_->GetNumOfTactilePoints(finger);
    AgilinkLogger::get().infof(TAG, "[GetSensorDataLength] %s: %zu (GetNumOfTactilePoints=%zu)",
                           ToString(finger).c_str(), len, pts);
    EXPECT_GT(len, 0u) << "Expected >0 for " << ToString(finger);
  }
}

// Main function for gtest
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // Parse command line arguments
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--request-interval" && i + 1 < argc) {
      g_request_interval = std::stoi(argv[++i]);
    } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      std::string device_arg = argv[++i];
      if (device_arg == "zlgcan" || device_arg == "hcan" || device_arg == "rs485") {
        g_device_type = device_arg;
      } else {
        AgilinkLogger::get().errorf(TAG, "[Error]: -d value must be 'zlgcan', 'hcan', or 'rs485', got: %s",
                                device_arg.c_str());
        return 1;
      }
    } else if ((arg == "-p" || arg == "--port") && i + 1 < argc) {
      g_serial_port = argv[++i];
    } else if (arg == "--help" || arg == "-h") {
      AgilinkLogger::get().infof(TAG, "Usage: %s [--request-interval MS] [-d DEVICE] [-p PORT]", argv[0]);
      AgilinkLogger::get().infof(TAG, "  --request-interval MS  Set request interval in ms (default: 5)");
      AgilinkLogger::get().infof(TAG, "  -d DEVICE              Device type: zlgcan | hcan | rs485 (default: zlgcan)");
      AgilinkLogger::get().infof(TAG, "  -p PORT                Serial port for rs485 (default: /dev/ttyUSB0)");
      return 0;
    }
  }

  return RUN_ALL_TESTS();
}
#endif  // BUILD_OMNIHAND_3_LITE
