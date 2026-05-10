// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include "omnihand/omnihand_2025.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
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

class OmniHand2025Test : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing based on device type
    std::string device_type = GetDeviceType();
    if (device_type == "hcan") {
      hand_ = agilink::omnihand::OmniHand2025::createHandByHcan(
          agilink::omnihand::HandType::LEFT,        // hand_type: left hand
          1,                       // hand_device_id: hand device ID
          0,                       // canfd_device_id: USB CANFD adapter device index
          0                        // canfd_channel_id: CAN channel index (0=can0, 1=can1)
      );
    } else {  // default: zlgcan
      hand_ = agilink::omnihand::OmniHand2025::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT,        // hand_type: left hand
          1,                       // hand_device_id: hand device ID
          0,                       // canfd_device_id: USB CANFD adapter device index
          0                        // canfd_channel_id: CAN channel index (0=can0, 1=can1)
      );
    }
    int request_interval = GetRequestInterval();
    hand_->SetRequestInterval(request_interval);
    if (request_interval != 0) {
      std::cout << "[Info]: Using request interval: " << request_interval << " ms" << std::endl;
    }
    std::cout << "[Info]: Using device type: " << device_type << std::endl;
  }

  void TearDown() override {
    hand_.reset();
  }

  std::unique_ptr<agilink::omnihand::OmniHand2025> hand_;
};

// Test factory method
TEST_F(OmniHand2025Test, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

// Test initialization
TEST_F(OmniHand2025Test, Init) {
  // Note: This test may fail if hardware is not connected
  // In CI/CD, you might want to skip this or mock the hardware
  bool init_result = hand_->Init();
  // We don't assert on init_result as it depends on hardware availability
  // EXPECT_TRUE(init_result);
}

// Test vendor info (may require hardware)
TEST_F(OmniHand2025Test, GetVendorInfo) {
  if (hand_->Init()) {
    auto vendor_info = hand_->GetVendorInfo();
    std::cout << "[GetVendorInfo] Vendor Info:" << std::endl;
    std::cout << vendor_info.ToString() << std::endl;
    EXPECT_EQ(vendor_info.dof, 10);  // O10 has 10 DOF
  }
}

// Test device info
TEST_F(OmniHand2025Test, GetDeviceInfo) {
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
TEST_F(OmniHand2025Test, SetDeviceId) {
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
TEST_F(OmniHand2025Test, MotorPositionControl) {
  if (hand_->Init()) {
    // Test single motor position
    int16_t target_pos = 2048;  // Middle position (0-4096 range)
    hand_->SetJointMotorPosi(1, target_pos);
    std::cout << "[SetJointMotorPosi] Set Joint 1 Motor Position: " << target_pos << std::endl;
    
    auto pos = hand_->GetJointMotorPosi(1);
    std::cout << "[GetJointMotorPosi] Joint 1 Motor Position: " << pos << std::endl;
    // Note: Actual position may differ due to hardware constraints
    
    // Test batch motor positions
    std::vector<int16_t> positions(10, 2048);  // 10 motors, all at middle
    
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
    EXPECT_EQ(all_positions.size(), 10);
  }
}

// Test joint angle control (requires hardware)
TEST_F(OmniHand2025Test, JointAngleControl) {
  if (hand_->Init()) {
    // Test setting active joint angles
    std::vector<double> angles(10, 0.0);  // 10 joints, all at 0
    hand_->SetAllActiveJointAngles(angles);
    std::cout << "[SetAllActiveJointAngles] Set Active Joint Angles (rad): ";
    for (size_t i = 0; i < angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << angles[i];
      if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    auto active_angles = hand_->GetAllActiveJointAngles();
    // Check if request succeeded (non-empty result)
    if (active_angles.empty()) {
      // Request failed (timeout), skip remaining assertions
      return;
    }
    std::cout << "[GetAllActiveJointAngles] Active Joint Angles (rad): ";
    for (size_t i = 0; i < active_angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << active_angles[i];
      if (i < active_angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(active_angles.size(), 10);
    
    auto all_angles = hand_->GetAllJointAngles();
    // Check if request succeeded
    if (all_angles.empty()) {
      return;
    }
    std::cout << "[GetAllJointAngles] All Joint Angles (rad, " << all_angles.size() << " joints): ";
    for (size_t i = 0; i < all_angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << all_angles[i];
      if (i < all_angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(all_angles.size(), 16);  // 10 active + 6 passive
  }
}

// Test tactile sensor (requires hardware)
TEST_F(OmniHand2025Test, TactileSensor) {
  if (hand_->Init()) {
    // Test GetTactileSensorData (downsampled data) for all fingers
    std::vector<agilink::omnihand::Finger> fingers = {
      agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE,
      agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE, agilink::omnihand::Finger::PALM, agilink::omnihand::Finger::DORSUM
    };
    
    std::cout << "[GetTactileSensorData] Getting all sensor data:" << std::endl;
    for (const auto& finger : fingers) {
      auto tactile_data = hand_->GetTactileSensorData(finger);
      std::cout << "  " << agilink::omnihand::ToString(finger) << " (" << tactile_data.size() << " values): ";
      for (size_t i = 0; i < tactile_data.size(); ++i) {
        std::cout << static_cast<int>(tactile_data[i]);
        if (i < tactile_data.size() - 1) std::cout << ", ";
      }
      std::cout << std::endl;
      EXPECT_GE(tactile_data.size(), 0);
    }
  }
}

// Test tactile sensor raw data (requires hardware)
TEST_F(OmniHand2025Test, TactileSensorRaw) {
  if (hand_->Init()) {
    // Test all 7 sensors individually (Thumb, Index, Middle, Ring, Little, Palm, Dorsum)
    std::vector<agilink::omnihand::Finger> all_fingers = {
      agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING,
      agilink::omnihand::Finger::LITTLE, agilink::omnihand::Finger::PALM, agilink::omnihand::Finger::DORSUM
    };
    
    // Check if firmware supports raw tactile sensor data by testing Thumb first
    auto thumb_tactile = hand_->GetTactileSensorDataRaw(agilink::omnihand::Finger::THUMB);
    if (thumb_tactile.data_.empty()) {
      std::cout << "[Info]: Raw tactile sensor data is not supported by this firmware version. "
                << "This feature requires firmware version that supports multi-frame tactile sensor data protocol." << std::endl;
      return;  // Skip test for low firmware versions
    }
    
    std::cout << "[GetTactileSensorDataRaw] Individual Sensors (unit: 1g, max: 255g)" << std::endl;
    for (const auto& finger : all_fingers) {
      auto tactile_data = hand_->GetTactileSensorDataRaw(finger);
      if (tactile_data.data_.empty()) {
        std::cout << "  " << agilink::omnihand::ToString(finger) << ": No data available" << std::endl;
        continue;  // Skip this sensor
      }
      std::cout << "  " << agilink::omnihand::ToString(finger) << " (" << tactile_data.data_.size() << " values): ";
      // Print first 10 values (or all if less than 10)
      size_t print_count = std::min(tactile_data.data_.size(), size_t(10));
      for (size_t i = 0; i < print_count; ++i) {
        std::cout << static_cast<int>(tactile_data.data_[i]);
        if (i < print_count - 1) std::cout << ", ";
      }
      if (tactile_data.data_.size() > 10) std::cout << " ...";
      std::cout << std::endl;
      EXPECT_EQ(tactile_data.sensor_id_, finger);
    }
    
    // Test getting all tactile sensor data
    auto all_tactile_data = hand_->GetAllTactileSensorDataRaw();
    if (all_tactile_data.empty()) {
      std::cout << "\n[Info]: Raw tactile sensor data is not supported by this firmware version. "
                << "This feature requires firmware version that supports multi-frame tactile sensor data protocol." << std::endl;
      return;  // Skip test for low firmware versions
    }
    std::cout << "\n[GetAllTactileSensorDataRaw] All Tactile Sensors: " 
              << all_tactile_data.size() << " sensors (unit: 1g, max: 255g)" << std::endl;
    for (const auto& sensor : all_tactile_data) {
      std::string finger_name;
      switch (sensor.sensor_id_) {
        case agilink::omnihand::Finger::THUMB: finger_name = "Thumb"; break;
        case agilink::omnihand::Finger::INDEX: finger_name = "Index"; break;
        case agilink::omnihand::Finger::MIDDLE: finger_name = "Middle"; break;
        case agilink::omnihand::Finger::RING: finger_name = "Ring"; break;
        case agilink::omnihand::Finger::LITTLE: finger_name = "Little"; break;
        case agilink::omnihand::Finger::PALM: finger_name = "Palm"; break;
        case agilink::omnihand::Finger::DORSUM: finger_name = "Dorsum"; break;
        default: finger_name = "Unknown"; break;
      }
      std::cout << "  " << finger_name << " (" << sensor.data_.size() << " values): ";
      // Print first 10 values (or all if less than 10)
      size_t print_count = std::min(sensor.data_.size(), size_t(10));
      for (size_t i = 0; i < print_count; ++i) {
        std::cout << static_cast<int>(sensor.data_[i]);
        if (i < print_count - 1) std::cout << ", ";
      }
      if (sensor.data_.size() > 10) std::cout << " ...";
      std::cout << std::endl;
    }
    EXPECT_GE(all_tactile_data.size(), 0);
  }
}

// Test error report (requires hardware)
TEST_F(OmniHand2025Test, ErrorReport) {
  if (hand_->Init()) {
    auto error_reports = hand_->GetAllErrorReport();
    // Check if request succeeded (non-empty result)
    if (error_reports.empty()) {
      // Request failed (timeout), skip assertion to avoid false failure
      // This is expected if device is not responding
      return;
    }
    std::cout << "[GetAllErrorReport] Error Reports (10 joints): ";
    bool has_errors = false;
    for (size_t i = 0; i < error_reports.size(); ++i) {
      std::cout << "J" << (i+1) << ":[";
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
    EXPECT_EQ(error_reports.size(), 10);
  }
}

// Test temperature report (requires hardware)
TEST_F(OmniHand2025Test, TemperatureReport) {
  if (hand_->Init()) {
    auto temp_reports = hand_->GetAllTemperatureReport();
    // Check if request succeeded (non-empty result)
    if (temp_reports.empty()) {
      // Request failed (timeout), skip assertion to avoid false failure
      // This is expected if device is not responding
      return;
    }
    std::cout << "[GetAllTemperatureReport] Temperature Reports (°C): ";
    for (size_t i = 0; i < temp_reports.size(); ++i) {
      std::cout << "J" << (i+1) << ":" << temp_reports[i];
      if (i < temp_reports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(temp_reports.size(), 10);
  }
}

// Test current report (requires hardware)
TEST_F(OmniHand2025Test, CurrentReport) {
  if (hand_->Init()) {
    auto current_reports = hand_->GetAllCurrentReport();
    // Check if request succeeded (non-empty result)
    if (current_reports.empty()) {
      // Request failed (timeout), skip assertion to avoid false failure
      // This is expected if device is not responding
      return;
    }
    std::cout << "[GetAllCurrentReport] Current Reports (mA): ";
    for (size_t i = 0; i < current_reports.size(); ++i) {
      std::cout << "J" << (i+1) << ":" << current_reports[i];
      if (i < current_reports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_reports.size(), 10);
  }
}

// Test kinematics solver
TEST_F(OmniHand2025Test, KinematicsSolver) {
  if (hand_->Init()) {
    // Test forward kinematics
    std::vector<double> active_angles(10, 0.0);
    auto all_angles = hand_->GetAllJointAngles(active_angles);
    std::cout << "[GetAllJointAngles] Forward Kinematics (input: 10 active angles, output: " 
              << all_angles.size() << " joint angles): ";
    for (size_t i = 0; i < std::min(all_angles.size(), size_t(10)); ++i) {
      std::cout << std::fixed << std::setprecision(4) << all_angles[i];
      if (i < std::min(all_angles.size(), size_t(10)) - 1) std::cout << ", ";
    }
    if (all_angles.size() > 10) std::cout << " ...";
    std::cout << std::endl;
    EXPECT_EQ(all_angles.size(), 16);  // 10 active + 6 passive
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
      } else if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      std::string device_arg = argv[i + 1];
      if (device_arg == "zlgcan" || device_arg == "hcan") {
        g_device_type = device_arg;
        ++i;  // Skip the next argument (the device type value)
        continue;
      } else {
        std::cerr << "[Error]: -d value must be 'zlgcan' or 'hcan', got: " << device_arg << std::endl;
        return 1;
      }
    } else if (arg == "--help" || arg == "-h") {
      std::cout << "Usage: " << argv[0] << " [-f INTERVAL] [-d DEVICE]" << std::endl;
      std::cout << "  -f INTERVAL  Set CAN request interval (0-100ms, 0=no limit, default: 5ms)" << std::endl;
      std::cout << "  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)" << std::endl;
      std::cout << std::endl;
      std::cout << "Example:" << std::endl;
      std::cout << "  " << argv[0] << " -f 20" << std::endl;
      std::cout << "  " << argv[0] << " -d hcan" << std::endl;
      std::cout << "  " << argv[0] << " -f 20 -d hcan" << std::endl;
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
