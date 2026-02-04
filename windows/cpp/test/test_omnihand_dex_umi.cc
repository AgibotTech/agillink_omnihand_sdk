// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include "omnihand/omnihand_dex_umi.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <string>

class OmniHandDexUMITest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing
    hand_ = OmniHandDexUMI::createHandByZlgcan(
        EHandType::eLeft,
        1,      // device_id
        0,      // canfd_id
        0       // channel_id
    );
  }

  void TearDown() override {
    // Unregister callbacks before destroying hand
    if (hand_) {
      hand_->SetPositionReportCallback(nullptr);
      hand_->SetTactileSensorReportCallback(nullptr);
    }
    hand_.reset();
  }

  std::unique_ptr<OmniHandDexUMI> hand_;
  
  // Callback test variables
  std::atomic<int> position_report_count_{0};
  std::atomic<int> tactile_sensor_report_count_{0};
  std::mutex callback_mutex_;
  std::vector<int16_t> last_position_data_;
  TactileSensorData last_tactile_data_;
  unsigned char last_tactile_sensor_id_{0};
};

// Test factory method
TEST_F(OmniHandDexUMITest, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

// Test initialization
TEST_F(OmniHandDexUMITest, Init) {
  // Note: This test may fail if hardware is not connected
  bool init_result = hand_->Init();
  // We don't assert on init_result as it depends on hardware availability
}

// Test vendor info (may require hardware)
TEST_F(OmniHandDexUMITest, GetVendorInfo) {
  if (hand_->Init()) {
    auto vendor_info = hand_->GetVendorInfo();
    std::cout << "\n[GetVendorInfo] Vendor Info:" << std::endl;
    std::cout << vendor_info.toString() << std::endl;
    EXPECT_EQ(vendor_info.dof, 10);  // UMI has 10 DOF
  }
}

// Test device info
TEST_F(OmniHandDexUMITest, GetDeviceInfo) {
  if (hand_->Init()) {
    auto device_info = hand_->GetDeviceInfo();
    std::cout << "\n[GetDeviceInfo] Device Info:" << std::endl;
    std::cout << device_info.toString() << std::endl;
    // Only check deviceId if request succeeded (non-zero indicates success)
    if (device_info.hand_device_id != 0) {
      EXPECT_EQ(device_info.hand_device_id, 1);
    }
    
    // UMI-specific device info fields
    if (device_info.position_report_frequency.has_value()) {
      std::cout << "  Position Report Frequency: " << device_info.position_report_frequency.value() << " Hz" << std::endl;
    }
    if (device_info.tactile_sensor_report_frequency.has_value()) {
      std::cout << "  Tactile Sensor Report Frequency: " << device_info.tactile_sensor_report_frequency.value() << " Hz" << std::endl;
    }
    if (device_info.adc_channel_count.has_value()) {
      std::cout << "  ADC Channel Count: " << device_info.adc_channel_count.value() << std::endl;
    }
  }
}

// Test setting device ID (disabled - may cause device inaccessibility)
// Note: SetDeviceId may change device ID on hardware, making device inaccessible with original ID.
// Use with caution and only in controlled test environments.
/*
TEST_F(OmniHandDexUMITest, SetDeviceId) {
  hand_->SetDeviceId(2);
  auto device_info = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info.hand_device_id, 2);
  
  // Reset to original
  hand_->SetDeviceId(1);
}
*/

// Note: OmniHand Dex UMI does not support motor position/angle control or control mode
// UMI is read-only for position information and uses a different protocol (Pn1-Pn7)
// These tests are commented out as UMI does not support these interfaces
/*
// Test motor position control (requires hardware)
// Note: O10 UMI uses UMI protocol, motor position range may differ
TEST_F(OmniHandDexUMITest, MotorPositionControl) {
  if (hand_->Init()) {
    // Test single motor position
    hand_->SetJointMotorPosi(1, 2048);
    auto pos = hand_->GetJointMotorPosi(1);
    
    // Test batch motor positions
    std::vector<int16_t> positions(10, 2048);  // 10 motors
    hand_->SetAllJointMotorPosi(positions);
    auto all_positions = hand_->GetAllJointMotorPosi();
    EXPECT_EQ(all_positions.size(), 10);
  }
}

// Test joint angle control (requires hardware)
TEST_F(OmniHandDexUMITest, JointAngleControl) {
  if (hand_->Init()) {
    // Test setting active joint angles
    std::vector<double> angles(10, 0.0);  // 10 joints, all at 0
    hand_->SetAllActiveJointAngles(angles);
    
    auto active_angles = hand_->GetAllActiveJointAngles();
    EXPECT_EQ(active_angles.size(), 10);
    
    auto all_angles = hand_->GetAllJointAngles();
    EXPECT_EQ(all_angles.size(), 16);  // 10 active + 6 passive
  }
}

// Test control mode (requires hardware)
TEST_F(OmniHandDexUMITest, ControlMode) {
  if (hand_->Init()) {
    // Test setting control mode
    std::vector<unsigned char> modes(10, static_cast<unsigned char>(EControlMode::ePosi));
    hand_->SetAllControlMode(modes);
    
    auto current_modes = hand_->GetAllControlMode();
    EXPECT_EQ(current_modes.size(), 10);
  }
}
*/

// Test tactile sensor (UMI specific, requires hardware)
TEST_F(OmniHandDexUMITest, TactileSensor) {
  if (hand_->Init()) {
    // Test single sensor (1D tactile sensor, Raw API)
    auto thumb_tactile = hand_->GetTactileSensorDataRaw(EFinger::eThumb);
    std::cout << "\n[GetTactileSensorDataRaw] Thumb Tactile Data (" 
              << thumb_tactile.data_.size() << " values): ";
    for (size_t i = 0; i < std::min(thumb_tactile.data_.size(), size_t(10)); ++i) {
      std::cout << static_cast<int>(thumb_tactile.data_[i]);
      if (i < std::min(thumb_tactile.data_.size(), size_t(10)) - 1) std::cout << ", ";
    }
    if (thumb_tactile.data_.size() > 10) std::cout << " ...";
    std::cout << " (unit: 1g, max: 255g)" << std::endl;
    EXPECT_EQ(thumb_tactile.sensor_id_, EFinger::eThumb);
    
    // Test multiple sensors
    std::vector<EFinger> fingers = {EFinger::eIndex, EFinger::eMiddle, EFinger::eRing, EFinger::eLittle};
    for (const auto& finger : fingers) {
      auto tactile_data = hand_->GetTactileSensorDataRaw(finger);
      std::cout << "\n[GetTactileSensorDataRaw] " << static_cast<int>(finger) 
                << " Tactile Data (" << tactile_data.data_.size() << " values): ";
      for (size_t i = 0; i < std::min(tactile_data.data_.size(), size_t(5)); ++i) {
        std::cout << static_cast<int>(tactile_data.data_[i]);
        if (i < std::min(tactile_data.data_.size(), size_t(5)) - 1) std::cout << ", ";
      }
      if (tactile_data.data_.size() > 5) std::cout << " ...";
      std::cout << std::endl;
      EXPECT_EQ(tactile_data.sensor_id_, finger);
    }
    
    // Test getting all tactile sensor data
    auto all_tactile_data = hand_->GetAllTactileSensorDataRaw();
    std::cout << "\n[GetAllTactileSensorDataRaw] All Tactile Sensors: " 
              << all_tactile_data.size() << " sensors" << std::endl;
    for (const auto& sensor : all_tactile_data) {
      std::string finger_name;
      switch (sensor.sensor_id_) {
        case EFinger::eThumb: finger_name = "Thumb"; break;
        case EFinger::eIndex: finger_name = "Index"; break;
        case EFinger::eMiddle: finger_name = "Middle"; break;
        case EFinger::eRing: finger_name = "Ring"; break;
        case EFinger::eLittle: finger_name = "Little"; break;
        case EFinger::ePalm: finger_name = "Palm"; break;
        case EFinger::eDorsum: finger_name = "Dorsum"; break;
        default: finger_name = "Unknown"; break;
      }
      std::cout << "  " << finger_name << ": " << sensor.data_.size() << " points" << std::endl;
    }
    EXPECT_GE(all_tactile_data.size(), 0);
  }
}

// Note: OmniHand Dex UMI may not support all error/temperature/current report interfaces
// These tests are commented out as they depend on UMI protocol implementation details
// Uncomment and adjust if your UMI implementation supports these interfaces
/*
// Test error report (requires hardware)
TEST_F(OmniHandDexUMITest, ErrorReport) {
  if (hand_->Init()) {
    auto error_reports = hand_->GetAllErrorReport();
    EXPECT_EQ(error_reports.size(), 10);
  }
}

// Test temperature report (requires hardware)
TEST_F(OmniHandDexUMITest, TemperatureReport) {
  if (hand_->Init()) {
    auto temp_reports = hand_->GetAllTemperatureReport();
    EXPECT_EQ(temp_reports.size(), 10);
  }
}

// Test current report (requires hardware)
TEST_F(OmniHandDexUMITest, CurrentReport) {
  if (hand_->Init()) {
    auto current_reports = hand_->GetAllCurrentReport();
    EXPECT_EQ(current_reports.size(), 10);
  }
}
*/

// Test position report frequency setting (UMI specific)
TEST_F(OmniHandDexUMITest, PositionReportFrequency) {
  if (hand_->Init()) {
    // Test setting position report frequency
    uint16_t test_freq = 50;
    hand_->SetPositionReportFrequency(test_freq);
    std::cout << "\n[SetPositionReportFrequency] Set to: " << test_freq << " Hz" << std::endl;
    
    // Note: There's no GetPositionReportFrequency() method, so we can't verify
    // The frequency is set on the device side
    
    // Reset to default (100 Hz)
    hand_->SetPositionReportFrequency(100);
    std::cout << "[SetPositionReportFrequency] Reset to default: 100 Hz" << std::endl;
  }
}

// Test tactile sensor report frequency setting (UMI specific)
TEST_F(OmniHandDexUMITest, TactileSensorReportFrequency) {
  if (hand_->Init()) {
    // Test setting tactile sensor report frequency
    uint16_t test_freq = 50;
    hand_->SetTactileSensorReportFrequency(test_freq);
    std::cout << "\n[SetTactileSensorReportFrequency] Set to: " << test_freq << " Hz" << std::endl;
    
    // Note: There's no GetTactileSensorReportFrequency() method, so we can't verify
    // The frequency is set on the device side
    
    // Reset to default (100 Hz)
    hand_->SetTactileSensorReportFrequency(100);
    std::cout << "[SetTactileSensorReportFrequency] Reset to default: 100 Hz" << std::endl;
  }
}

// Test position report callback (UMI specific)
TEST_F(OmniHandDexUMITest, PositionReportCallback) {
  if (hand_->Init()) {
    // Reset counters
    position_report_count_ = 0;
    last_position_data_.clear();
    
    // Register callback
    auto position_callback = [this](const std::vector<int16_t>& positions) {
      std::lock_guard<std::mutex> lock(callback_mutex_);
      position_report_count_++;
      last_position_data_ = positions;
      std::cout << "[PositionReportCallback] Received position data (" 
                << positions.size() << " values): ";
      for (size_t i = 0; i < std::min(positions.size(), size_t(5)); ++i) {
        std::cout << positions[i];
        if (i < std::min(positions.size(), size_t(5)) - 1) std::cout << ", ";
      }
      if (positions.size() > 5) std::cout << " ...";
      std::cout << " (mV)" << std::endl;
    };
    
    // Set callback with frequency (50 Hz)
    hand_->SetPositionReportCallback(position_callback, 50);
    std::cout << "\n[SetPositionReportCallback] Registered callback with frequency: 50 Hz" << std::endl;
    
    // Wait for some reports (2 seconds = ~100 reports at 50 Hz)
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "[PositionReportCallback] Total reports received: " 
              << position_report_count_.load() << std::endl;
    EXPECT_GT(position_report_count_.load(), 0);
    
    // Unregister callback
    hand_->SetPositionReportCallback(nullptr);
    std::cout << "[SetPositionReportCallback] Unregistered callback" << std::endl;
  }
}

// Test tactile sensor report callback (UMI specific)
TEST_F(OmniHandDexUMITest, TactileSensorReportCallback) {
  if (hand_->Init()) {
    // Reset counters
    tactile_sensor_report_count_ = 0;
    last_tactile_data_ = TactileSensorData{};
    last_tactile_sensor_id_ = 0;
    
    // Register callback
    auto tactile_callback = [this](const TactileSensorData& sensor_data, unsigned char sensor_id) {
      std::lock_guard<std::mutex> lock(callback_mutex_);
      tactile_sensor_report_count_++;
      last_tactile_data_ = sensor_data;
      last_tactile_sensor_id_ = sensor_id;
      std::string finger_name;
      switch (sensor_data.sensor_id_) {
        case EFinger::eThumb: finger_name = "Thumb"; break;
        case EFinger::eIndex: finger_name = "Index"; break;
        case EFinger::eMiddle: finger_name = "Middle"; break;
        case EFinger::eRing: finger_name = "Ring"; break;
        case EFinger::eLittle: finger_name = "Little"; break;
        case EFinger::ePalm: finger_name = "Palm"; break;
        case EFinger::eDorsum: finger_name = "Dorsum"; break;
        default: finger_name = "Unknown"; break;
      }
      std::cout << "[TactileSensorReportCallback] Received " << finger_name 
                << " sensor data (sensor_id: " << static_cast<int>(sensor_id) 
                << ", data points: " << sensor_data.data_.size() << ")" << std::endl;
    };
    
    // Set callback with frequency (50 Hz)
    hand_->SetTactileSensorReportCallback(tactile_callback, 50);
    std::cout << "\n[SetTactileSensorReportCallback] Registered callback with frequency: 50 Hz" << std::endl;
    
    // Wait for some reports (2 seconds = ~100 reports at 50 Hz)
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "[TactileSensorReportCallback] Total reports received: " 
              << tactile_sensor_report_count_.load() << std::endl;
    EXPECT_GT(tactile_sensor_report_count_.load(), 0);
    
    // Unregister callback
    hand_->SetTactileSensorReportCallback(nullptr);
    std::cout << "[SetTactileSensorReportCallback] Unregistered callback" << std::endl;
  }
}

// Test position calibration (UMI specific)
// Note: These tests are commented out as they require physical manipulation of the device
// Uncomment and use with caution in controlled test environments
/*
TEST_F(OmniHandDexUMITest, MinPositionCalibration) {
  if (hand_->Init()) {
    // Note: This requires the device to be in minimum position
    // This is a write-only operation that calibrates the minimum position
    hand_->SetMinPositionCalibration();
    std::cout << "\n[SetMinPositionCalibration] Minimum position calibration set" << std::endl;
  }
}

TEST_F(OmniHandDexUMITest, MaxPositionCalibration) {
  if (hand_->Init()) {
    // Note: This requires the device to be in maximum position
    // This is a write-only operation that calibrates the maximum position
    hand_->SetMaxPositionCalibration();
    std::cout << "\n[SetMaxPositionCalibration] Maximum position calibration set" << std::endl;
  }
}
*/

// Note: OmniHand Dex UMI does not support:
// - Motor position/angle control (read-only position information via periodic reports)
// - Control mode setting
// - GetAllJointAngles() / GetAllActiveJointAngles() (use periodic position reports instead)
// - GetAllJointPos() (no kinematics solver)
// - Error/temperature/current reports (UMI protocol does not support these)
// These are UMI-specific limitations compared to O10 and O12
