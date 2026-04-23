// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include "omnihand/omnihand_3_ultra_m.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <string>

static int g_request_interval = 5;
static std::string g_device_type = "zlgcan";
static std::string g_can_interface = "can0";

static int GetRequestInterval() {
  return g_request_interval;
}

static std::string GetDeviceType() {
  return g_device_type;
}

class OmniHand3UltraMTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::string device_type = GetDeviceType();
    if (device_type == "hcan") {
      hand_ = agilink::omnihand::OmniHand3UltraM::createHandByHcan(
          agilink::omnihand::HandType::LEFT, 9, 0, 0);
      std::cout << "[Info]: Using HCAN device" << std::endl;
#ifdef __linux__
    } else if (device_type == "socketcan") {
      hand_ = agilink::omnihand::OmniHand3UltraM::createHandSocketCan(
          agilink::omnihand::HandType::LEFT, 9, g_can_interface);
      std::cout << "[Info]: Using SocketCAN device (" << g_can_interface << ")" << std::endl;
#endif
    } else {
      hand_ = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT, 9, 0, 0);
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
  }

  std::unique_ptr<agilink::omnihand::OmniHand3UltraM> hand_;
};

TEST_F(OmniHand3UltraMTest, CreateHand) {
  EXPECT_NE(hand_, nullptr);
}

TEST_F(OmniHand3UltraMTest, Init) {
  bool init_result = hand_->Init();
}

TEST_F(OmniHand3UltraMTest, GetVendorInfo) {
  if (hand_->Init()) {
    auto vendor_info = hand_->GetVendorInfo();
    std::cout << "[GetVendorInfo] Vendor Info:" << std::endl;
    std::cout << vendor_info.ToString() << std::endl;
    if (vendor_info.dof == 0) {
      std::cout << "[GetVendorInfo] Failed: got empty vendor info (timeout)" << std::endl;
      return;
    }
    EXPECT_EQ(vendor_info.dof, 20);
  }
}

TEST_F(OmniHand3UltraMTest, GetDeviceInfo) {
  if (hand_->Init()) {
    auto device_info = hand_->GetDeviceInfo();
    std::cout << "[GetDeviceInfo] Device Info:" << std::endl;
    std::cout << device_info.ToString() << std::endl;
    if (device_info.hand_device_id != 0) {
      EXPECT_EQ(device_info.hand_device_id, 9);
    }
  }
}

TEST_F(OmniHand3UltraMTest, SetDeviceId) {
  auto current_device_info = hand_->GetDeviceInfo();
  unsigned char current_id = current_device_info.hand_device_id;
  if (current_id == 0) return;

  unsigned char target_id = 2;
  hand_->SetDeviceId(target_id);
  std::cout << "[SetDeviceId] Set Device ID: " << static_cast<int>(target_id) << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto device_info = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info.hand_device_id, 2);

  unsigned char original_id = 9;
  hand_->SetDeviceId(original_id);
  std::cout << "[SetDeviceId] Reset Device ID: " << static_cast<int>(original_id) << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto device_info1 = hand_->GetDeviceInfo();
  EXPECT_EQ(device_info1.hand_device_id, 9);
}

TEST_F(OmniHand3UltraMTest, JointAngleControl) {
  if (hand_->Init()) {
    std::vector<double> angles(20, 0.0);
    hand_->SetAllActiveJointAngles(angles);
    std::cout << "[SetAllActiveJointAngles] Set 20 joints to 0.0 rad" << std::endl;

    auto active_angles = hand_->GetAllActiveJointAngles();
    if (active_angles.empty() || active_angles.size() != 20) {
      std::cout << "[GetAllActiveJointAngles] Failed: got " << active_angles.size()
                << " angles, expected 20" << std::endl;
      return;
    }
    std::cout << "[GetAllActiveJointAngles] Active Joint Angles (rad): ";
    for (size_t i = 0; i < active_angles.size(); ++i) {
      std::cout << std::fixed << std::setprecision(4) << active_angles[i];
      if (i < active_angles.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(active_angles.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ControlMode) {
  if (hand_->Init()) {
    auto current_modes = hand_->GetAllControlMode();
    if (current_modes.empty() || current_modes.size() != 20) {
      std::cout << "[GetAllControlMode] Failed: got " << current_modes.size()
                << " modes, expected 20" << std::endl;
      return;
    }
    std::cout << "[GetAllControlMode] Control Modes: ";
    for (size_t i = 0; i < current_modes.size(); ++i) {
      std::cout << static_cast<int>(current_modes[i]);
      if (i < current_modes.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_modes.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ErrorReport) {
  if (hand_->Init()) {
    auto error_reports = hand_->GetAllErrorReport();
    if (error_reports.empty()) return;
    std::cout << "[GetAllErrorReport] Error Reports (20 joints): ";
    for (size_t i = 0; i < error_reports.size(); ++i) {
      std::cout << "J" << (i+1) << ":[";
      if (error_reports[i].bits.stalled_) std::cout << "S";
      if (error_reports[i].bits.overheat_) std::cout << "H";
      if (error_reports[i].bits.over_current_) std::cout << "C";
      if (error_reports[i].bits.motor_except_) std::cout << "M";
      if (error_reports[i].bits.commu_except_) std::cout << "X";
      std::cout << "]";
      if (i < error_reports.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
    EXPECT_EQ(error_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ClearErrorReport) {
  if (hand_->Init()) {
    bool result = hand_->ClearAllErrorReport();
    std::cout << "[ClearAllErrorReport] Result: " << (result ? "OK" : "FAILED") << std::endl;

    bool result_single = hand_->ClearErrorReport(1);
    std::cout << "[ClearErrorReport] Joint 1 Result: " << (result_single ? "OK" : "FAILED") << std::endl;
  }
}

TEST_F(OmniHand3UltraMTest, TemperatureReport) {
  if (hand_->Init()) {
    auto temp_reports = hand_->GetAllTemperatureReport();
    if (temp_reports.empty()) return;
    std::cout << "[GetAllTemperatureReport] Temperature Reports (C): ";
    for (size_t i = 0; i < temp_reports.size(); ++i) {
      std::cout << "J" << (i+1) << ":" << temp_reports[i];
      if (i < temp_reports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(temp_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, CurrentReport) {
  if (hand_->Init()) {
    auto current_reports = hand_->GetAllCurrentReport();
    if (current_reports.empty()) return;
    std::cout << "[GetAllCurrentReport] Current Reports (mA): ";
    for (size_t i = 0; i < current_reports.size(); ++i) {
      std::cout << "J" << (i+1) << ":" << current_reports[i];
      if (i < current_reports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, VelocityControl) {
  if (hand_->Init()) {
    std::vector<int16_t> velocities(20, 0);
    hand_->SetAllJointMotorVelo(velocities);
    std::cout << "[SetAllJointMotorVelo] Set 20 velocities to 0" << std::endl;

    auto current_velocities = hand_->GetAllJointMotorVelo();
    if (current_velocities.empty() || current_velocities.size() != 20) {
      std::cout << "[GetAllJointMotorVelo] Failed: got " << current_velocities.size()
                << " velocities, expected 20" << std::endl;
      return;
    }
    std::cout << "[GetAllJointMotorVelo] Current Velocities: ";
    for (size_t i = 0; i < current_velocities.size(); ++i) {
      std::cout << current_velocities[i];
      if (i < current_velocities.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(current_velocities.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, MotorMinLimit) {
  if (hand_->Init()) {
    auto min_limits = hand_->GetAllMotorMinLimit();
    if (min_limits.empty()) {
      std::cout << "[GetAllMotorMinLimit] Failed: empty result" << std::endl;
      return;
    }
    std::cout << "[GetAllMotorMinLimit] Min Limits (0.1 deg): ";
    for (size_t i = 0; i < min_limits.size(); ++i) {
      std::cout << min_limits[i];
      if (i < min_limits.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(min_limits.size(), 20);

    auto single = hand_->GetMotorMinLimit(1);
    std::cout << "[GetMotorMinLimit] Joint 1: " << single << std::endl;
  }
}

TEST_F(OmniHand3UltraMTest, MotorMaxLimit) {
  if (hand_->Init()) {
    auto max_limits = hand_->GetAllMotorMaxLimit();
    if (max_limits.empty()) {
      std::cout << "[GetAllMotorMaxLimit] Failed: empty result" << std::endl;
      return;
    }
    std::cout << "[GetAllMotorMaxLimit] Max Limits (0.1 deg): ";
    for (size_t i = 0; i < max_limits.size(); ++i) {
      std::cout << max_limits[i];
      if (i < max_limits.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(max_limits.size(), 20);

    auto single = hand_->GetMotorMaxLimit(1);
    std::cout << "[GetMotorMaxLimit] Joint 1: " << single << std::endl;
  }
}

TEST_F(OmniHand3UltraMTest, MotorActualPos) {
  if (hand_->Init()) {
    auto actual_pos = hand_->GetAllMotorActualPos();
    if (actual_pos.empty()) {
      std::cout << "[GetAllMotorActualPos] Failed: empty result" << std::endl;
      return;
    }
    std::cout << "[GetAllMotorActualPos] Actual Positions (0.1 deg): ";
    for (size_t i = 0; i < actual_pos.size(); ++i) {
      std::cout << actual_pos[i];
      if (i < actual_pos.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(actual_pos.size(), 20);

    auto single = hand_->GetMotorActualPos(1);
    std::cout << "[GetMotorActualPos] Joint 1: " << single << std::endl;
  }
}

TEST_F(OmniHand3UltraMTest, SaveParameters) {
  if (hand_->Init()) {
    bool result = hand_->SaveParameters();
    std::cout << "[SaveParameters] Result: " << (result ? "OK" : "FAILED") << std::endl;
  }
}

TEST_F(OmniHand3UltraMTest, CalibrateTactileSensor) {
  if (hand_->Init()) {
    bool result = hand_->CalibrateTactileSensor();
    std::cout << "[CalibrateTactileSensor] Result: " << (result ? "OK" : "FAILED") << std::endl;
  }
}

int main(int argc, char** argv) {
  std::vector<char*> gtest_args;
  gtest_args.push_back(argv[0]);

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-f" && i + 1 < argc) {
      try {
        int interval = std::stoi(argv[i + 1]);
        if (interval >= 0 && interval <= 100) {
          g_request_interval = interval;
          ++i;
          continue;
        } else {
          std::cerr << "[Error]: -f value " << interval << " is out of range (0-100ms)" << std::endl;
          return 1;
        }
      } catch (const std::exception& e) {
        std::cerr << "[Error]: Invalid -f value: " << argv[i + 1] << std::endl;
        return 1;
      }
    } else if (arg == "-d" && i + 1 < argc) {
      std::string device_type = argv[i + 1];
      if (device_type == "zlgcan" || device_type == "hcan" || device_type == "socketcan") {
        g_device_type = device_type;
        ++i;
        continue;
      } else {
        std::cerr << "[Error]: -d value must be 'zlgcan', 'hcan' or 'socketcan', got: " << device_type << std::endl;
        return 1;
      }
    } else if (arg == "-c" && i + 1 < argc) {
      g_can_interface = argv[i + 1];
      ++i;
      continue;
    } else if (arg == "--help" || arg == "-h") {
      std::cout << "Usage: " << argv[0] << " [-f INTERVAL] [-d DEVICE] [-c CAN_INTERFACE]" << std::endl;
      std::cout << "  -f INTERVAL       Set CAN request interval (0-100ms, default: 5ms)" << std::endl;
      std::cout << "  -d DEVICE         Set CAN device type (zlgcan, hcan or socketcan, default: zlgcan)" << std::endl;
      std::cout << "  -c CAN_INTERFACE  Set SocketCAN interface name (default: can0)" << std::endl;
      return 0;
    }
    gtest_args.push_back(argv[i]);
  }

  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
