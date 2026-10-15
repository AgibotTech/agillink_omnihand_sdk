// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "agilink_logger.h"
#include "omnihand/omnihand_dex_umi.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHandDexUmiTest";

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
    AgilinkLogger::get().infof(TAG, "Using device type: %s", device_type.c_str());
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
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo] Vendor Info:\n%s", vendor_info.ToString().c_str());
  EXPECT_EQ(vendor_info.dof, 10);  // UMI has 10 DOF
}

// Test device info
TEST_F(OmniHandDexUMITest, GetDeviceInfo) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] Device Info:\n%s", device_info.ToString().c_str());
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

  AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] Reading individual sensors (unit: 1g, max: 255g):");
  for (const auto& [finger, name] : fingers) {
    auto tactile_data = hand_->GetTactileSensorDataRaw(finger);
    std::string msg = "  " + name + " (" + std::to_string(tactile_data.data_.size()) + " values): [";
    for (size_t i = 0; i < tactile_data.data_.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(static_cast<int>(tactile_data.data_[i]));
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    EXPECT_EQ(static_cast<unsigned char>(tactile_data.sensor_id_), static_cast<unsigned char>(finger));
  }
  
  // Test getting all tactile sensor data at once
  auto all_tactile_data = hand_->GetAllTactileSensorDataRaw();
  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] All %zu sensors:", all_tactile_data.size());
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
    std::string msg = "  " + finger_name + " (" + std::to_string(sensor.data_.size()) + " values): [";
    for (size_t i = 0; i < sensor.data_.size(); ++i) {
      if (i > 0) msg += ", ";
      msg += std::to_string(static_cast<int>(sensor.data_[i]));
    }
    msg += "]";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }
  EXPECT_GE(all_tactile_data.size(), 0);
}

// Test position query (UMI specific)
TEST_F(OmniHandDexUMITest, GetJointMotorPosi) {
  ASSERT_TRUE(hand_->Init()) << "Failed to initialize device";
  
  // Test single joint position query
  for (unsigned char joint_idx = 1; joint_idx <= agilink::omnihand::OmniHandDexUMI::kDegreesOfActiveFreedom; ++joint_idx) {
    int16_t pos = hand_->GetJointMotorPosi(joint_idx);
    AgilinkLogger::get().infof(TAG, "[GetJointMotorPosi] Joint %d position: %d", static_cast<int>(joint_idx), pos);
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
  std::string positions_msg = "[GetAllJointMotorPosi] All joint positions (" + std::to_string(positions.size()) + " values): ";
  for (size_t i = 0; i < positions.size(); ++i) {
    if (i > 0) positions_msg += ", ";
    positions_msg += std::to_string(positions[i]);
  }
  AgilinkLogger::get().infof(TAG, "%s", positions_msg.c_str());

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
        AgilinkLogger::get().errorf(TAG, "[Error]: -d value must be 'zlgcan' or 'hcan', got: %s", device_arg.c_str());
        return 1;
      }
    }
  }
  
  return RUN_ALL_TESTS();
}
