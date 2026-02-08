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
    hand_.reset();
  }

  std::unique_ptr<OmniHandDexUMI> hand_;
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
  std::cout << vendor_info.toString() << std::endl;
  EXPECT_EQ(vendor_info.dof, 10);  // UMI has 10 DOF
}

// Test device info
TEST_F(OmniHandDexUMITest, GetDeviceInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto device_info = hand_->GetDeviceInfo();
  std::cout << "[GetDeviceInfo] Device Info:" << std::endl;
  std::cout << device_info.toString() << std::endl;
  EXPECT_EQ(device_info.hand_device_id, 1);
}

// Test tactile sensor (UMI specific)
TEST_F(OmniHandDexUMITest, TactileSensor) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test all 6 sensors (UMI has: Thumb, Index, Middle, Ring, Little, Palm - no Dorsum)
  std::vector<std::pair<EFinger, std::string>> fingers = {
    {EFinger::eThumb, "Thumb"},
    {EFinger::eIndex, "Index"},
    {EFinger::eMiddle, "Middle"},
    {EFinger::eRing, "Ring"},
    {EFinger::eLittle, "Little"},
    {EFinger::ePalm, "Palm"}
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
    EXPECT_EQ(tactile_data.sensor_id_, finger);
  }
  
  // Test getting all tactile sensor data at once
  auto all_tactile_data = hand_->GetAllTactileSensorDataRaw();
  std::cout << "\n[GetAllTactileSensorDataRaw] All " << all_tactile_data.size() << " sensors:" << std::endl;
  for (const auto& sensor : all_tactile_data) {
    std::string finger_name;
    switch (sensor.sensor_id_) {
      case EFinger::eThumb: finger_name = "Thumb"; break;
      case EFinger::eIndex: finger_name = "Index"; break;
      case EFinger::eMiddle: finger_name = "Middle"; break;
      case EFinger::eRing: finger_name = "Ring"; break;
      case EFinger::eLittle: finger_name = "Little"; break;
      case EFinger::ePalm: finger_name = "Palm"; break;
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
  for (unsigned char joint_idx = 1; joint_idx <= OmniHandDexUMI::kDegreesOfActiveFreedom; ++joint_idx) {
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
  EXPECT_EQ(positions.size(), OmniHandDexUMI::kDegreesOfActiveFreedom);
}

// Test position calibration (UMI specific)
// Warning: These tests perform actual calibration - use with caution
TEST_F(OmniHandDexUMITest, MinPositionCalibration) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test all joints calibration
  hand_->SetMinPositionCalibration();
  std::cout << "[SetMinPositionCalibration] Minimum position calibration set for all joints" << std::endl;
  
  // Test single joint calibration (joint 1-10)
  for (unsigned char joint_idx = 1; joint_idx <= 10; ++joint_idx) {
    hand_->SetMinPositionCalibration(joint_idx);
    std::cout << "[SetMinPositionCalibration] Minimum position calibration set for joint " 
              << static_cast<int>(joint_idx) << std::endl;
  }
}

TEST_F(OmniHandDexUMITest, MaxPositionCalibration) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test all joints calibration
  hand_->SetMaxPositionCalibration();
  std::cout << "[SetMaxPositionCalibration] Maximum position calibration set for all joints" << std::endl;
  
  // Test single joint calibration (joint 1-10)
  for (unsigned char joint_idx = 1; joint_idx <= 10; ++joint_idx) {
    hand_->SetMaxPositionCalibration(joint_idx);
    std::cout << "[SetMaxPositionCalibration] Maximum position calibration set for joint " 
              << static_cast<int>(joint_idx) << std::endl;
  }
}
