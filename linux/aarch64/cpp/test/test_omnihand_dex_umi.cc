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
#include <string>

// Global variable to store device type from command line argument
static std::string g_device_type = "zlgcan";  // Default: zlgcan

// Helper function to get device type
static std::string GetDeviceType() {
  return g_device_type;
}

class OmniHandDexUMITest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create hand instance for testing based on device type
    std::string device_type = GetDeviceType();
    if (device_type == "hcan") {
      hand_ = agilink::omnihand::OmniHandDexUMI::createHandByHcan(
          agilink::omnihand::HandType::LEFT,        // hand_type: left hand
          1,                       // hand_device_id: hand device ID
          0,                       // canfd_device_id: USB CANFD adapter device index
          0                        // canfd_channel_id: CAN channel index (0=can0, 1=can1)
      );
    } else {  // default: zlgcan
      hand_ = agilink::omnihand::OmniHandDexUMI::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT,        // hand_type: left hand
          1,                       // hand_device_id: hand device ID
          0,                       // canfd_device_id: USB CANFD adapter device index
          0                        // canfd_channel_id: CAN channel index (0=can0, 1=can1)
      );
    }
    std::cout << "[Info]: Using device type: " << device_type << std::endl;
  }

  void TearDown() override {
    hand_.reset();
  }

  std::unique_ptr<agilink::omnihand::OmniHandDexUMI> hand_;
};

// Test factory method
TEST_F(OmniHandDexUMITest, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

// Test initialization
TEST_F(OmniHandDexUMITest, Init) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device. Check hardware connection.";
}

// Test vendor info
TEST_F(OmniHandDexUMITest, GetVendorInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto vendor_info = hand_->GetVendorInfo();
  std::cout << "[GetVendorInfo] Vendor Info:" << std::endl;
  std::cout << vendor_info.ToString() << std::endl;
  EXPECT_EQ(vendor_info.dof, 10);  // UMI has 10 DOF
}

// Test device info
TEST_F(OmniHandDexUMITest, GetDeviceInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto device_info = hand_->GetDeviceInfo();
  std::cout << "[GetDeviceInfo] Device Info:" << std::endl;
  std::cout << device_info.ToString() << std::endl;
  EXPECT_EQ(device_info.hand_device_id, 1);
}

// Test tactile sensor (UMI specific)
TEST_F(OmniHandDexUMITest, TactileSensor) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test all 6 sensors (UMI has: Thumb, Index, Middle, Ring, Little, Palm - no Dorsum)
  std::vector<std::pair<agilink::omnihand::Finger, std::string>> fingers = {
    {agilink::omnihand::Finger::THUMB, "Thumb"},
    {agilink::omnihand::Finger::INDEX, "Index"},
    {agilink::omnihand::Finger::MIDDLE, "Middle"},
    {agilink::omnihand::Finger::RING, "Ring"},
    {agilink::omnihand::Finger::LITTLE, "Little"},
    {agilink::omnihand::Finger::PALM, "Palm"}
  };
  
  std::cout << "[GetTactileSensorDataRaw] Reading individual sensors (unit: 1g, max: 255g):" << std::endl;
  for (const auto& [finger, name] : fingers) {
    auto tactile_data = hand_->GetTactileSensorDataRaw(finger);
    std::cout << "  " << name << " (" << tactile_data.data_.size() << " values): [";
    for (size_t i = 0; i < tactile_data.data_.size(); ++i) {
      std::cout << static_cast<int>(tactile_data.data_[i]);
      if (i < tactile_data.data_.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    EXPECT_EQ(static_cast<unsigned char>(tactile_data.sensor_id_), static_cast<unsigned char>(finger));
  }
  
  // Test getting all tactile sensor data at once
  auto all_tactile_data = hand_->GetAllTactileSensorDataRaw();
  std::cout << "\n[GetAllTactileSensorDataRaw] All " << all_tactile_data.size() << " sensors:" << std::endl;
  for (const auto& sensor : all_tactile_data) {
    std::string finger_name;
    switch (sensor.sensor_id_) {
      case agilink::omnihand::Finger::THUMB: finger_name = "Thumb"; break;
      case agilink::omnihand::Finger::INDEX: finger_name = "Index"; break;
      case agilink::omnihand::Finger::MIDDLE: finger_name = "Middle"; break;
      case agilink::omnihand::Finger::RING: finger_name = "Ring"; break;
      case agilink::omnihand::Finger::LITTLE: finger_name = "Little"; break;
      case agilink::omnihand::Finger::PALM: finger_name = "Palm"; break;
      default: finger_name = "Unknown"; break;
    }
    std::cout << "  " << finger_name << " (" << sensor.data_.size() << " values): [";
    for (size_t i = 0; i < sensor.data_.size(); ++i) {
      std::cout << static_cast<int>(sensor.data_[i]);
      if (i < sensor.data_.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
  }
  EXPECT_GE(all_tactile_data.size(), 0);
}

// Test position query (UMI specific)
TEST_F(OmniHandDexUMITest, GetJointMotorPosi) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint position query
  for (unsigned char joint_idx = 1; joint_idx <= agilink::omnihand::OmniHandDexUMI::kDegreesOfActiveFreedom; ++joint_idx) {
    int16_t pos = hand_->GetJointMotorPosi(joint_idx);
    std::cout << "[GetJointMotorPosi] Joint " << static_cast<int>(joint_idx) 
              << " position: " << pos << std::endl;
  }
  
  // Test invalid joint index
  int16_t invalid_pos = hand_->GetJointMotorPosi(0);
  EXPECT_EQ(invalid_pos, -1);
  invalid_pos = hand_->GetJointMotorPosi(11);
  EXPECT_EQ(invalid_pos, -1);
}

TEST_F(OmniHandDexUMITest, GetAllJointMotorPosi) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test all joint positions query
  std::vector<int16_t> positions = hand_->GetAllJointMotorPosi();
  std::cout << "[GetAllJointMotorPosi] All joint positions (" << positions.size() << " values): ";
  for (size_t i = 0; i < positions.size(); ++i) {
    std::cout << positions[i];
    if (i < positions.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  // Expect 10 joint positions
  EXPECT_EQ(positions.size(), agilink::omnihand::OmniHandDexUMI::kDegreesOfActiveFreedom);
}

// // Test position calibration (UMI specific)
// // Warning: These tests perform actual calibration - use with caution
// TEST_F(OmniHandDexUMITest, MinPositionCalibration) {
//   ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
//   // Test all joints calibration
//   hand_->SetMinPositionCalibration();
//   std::cout << "[SetMinPositionCalibration] Minimum position calibration set for all joints" << std::endl;
  
//   // Test single joint calibration (joint 1-10)
//   for (unsigned char joint_idx = 1; joint_idx <= 10; ++joint_idx) {
//     hand_->SetMinPositionCalibration(joint_idx);
//     std::cout << "[SetMinPositionCalibration] Minimum position calibration set for joint " 
//               << static_cast<int>(joint_idx) << std::endl;
//   }
// }

// TEST_F(OmniHandDexUMITest, MaxPositionCalibration) {
//   ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
//   // Test all joints calibration
//   hand_->SetMaxPositionCalibration();
//   std::cout << "[SetMaxPositionCalibration] Maximum position calibration set for all joints" << std::endl;
  
//   // Test single joint calibration (joint 1-10)
//   for (unsigned char joint_idx = 1; joint_idx <= 10; ++joint_idx) {
//     hand_->SetMaxPositionCalibration(joint_idx);
//     std::cout << "[SetMaxPositionCalibration] Maximum position calibration set for joint " 
//               << static_cast<int>(joint_idx) << std::endl;
//   }
// }

// Main function for gtest
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  
  // Parse command line arguments
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      std::string device_arg = argv[++i];
      if (device_arg == "zlgcan" || device_arg == "hcan") {
        g_device_type = device_arg;
      } else {
        std::cerr << "[Error]: -d value must be 'zlgcan' or 'hcan', got: " << device_arg << std::endl;
        return 1;
      }
    }
  }
  
  return RUN_ALL_TESTS();
}
