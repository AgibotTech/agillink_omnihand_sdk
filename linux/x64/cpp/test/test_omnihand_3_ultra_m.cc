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

// TEST_F(OmniHand3UltraMTest, SetDeviceId) {
//   auto current_device_info = hand_->GetDeviceInfo();
//   unsigned char current_id = current_device_info.hand_device_id;
//   if (current_id == 0) return;

//   unsigned char target_id = 2;
//   hand_->SetDeviceId(target_id);
//   std::cout << "[SetDeviceId] Set Device ID: " << static_cast<int>(target_id) << std::endl;
//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   auto device_info = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info.hand_device_id, 2);

//   unsigned char original_id = 9;
//   hand_->SetDeviceId(original_id);
//   std::cout << "[SetDeviceId] Reset Device ID: " << static_cast<int>(original_id) << std::endl;
//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   auto device_info1 = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info1.hand_device_id, 9);
// }

TEST_F(OmniHand3UltraMTest, GestureDance) {
  if (!hand_->Init()) return;

  constexpr int MOTOR_TOTAL_COUNT = 20;
  constexpr int TFIX = 1000;
  constexpr int TSWIG = 200;
  constexpr int TSHORT = 10;

  std::vector<unsigned char> pp_modes(MOTOR_TOTAL_COUNT, static_cast<unsigned char>(agilink::omnihand::ControlMode::PROFILE_POSITION));
  std::vector<unsigned char> csp_modes(MOTOR_TOTAL_COUNT, static_cast<unsigned char>(agilink::omnihand::ControlMode::POSITION));

  std::cout << "[GestureDance] Switching to PP mode (PROFILE_POSITION=7)" << std::endl;
  hand_->SetAllControlMode(pp_modes);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  const uint8_t idx_joint[] = {
      16,17,16,16,16,18,19,16,17,18,19,
      13,12,12,12,14,15,13,14,15,
      9,8,8,8,10,11,9,10,11,
      5,4,4,4,6,7,5,6,7,
      1,0,0,0,2,3,1,2,3,
  };
  const float ang_joint[] = {
      -40,25,-30,-50,-40,40,40,0,-15,0,0,
      40,7,-7,0,40,40,0,0,0,
      40,7,-7,0,40,40,0,0,0,
      40,7,-7,0,40,40,0,0,0,
      40,7,-7,0,40,40,0,0,0,
  };
  const uint16_t time_joint[] = {
      TFIX,TSWIG,TSWIG,TSWIG,TFIX,TFIX,TFIX,TSHORT,TSHORT,TSHORT,TSHORT,
      TFIX,TSWIG,TSWIG,TSWIG,TFIX,TFIX,TSHORT,TSHORT,TSHORT,
      TFIX,TSWIG,TSWIG,TSWIG,TFIX,TFIX,TSHORT,TSHORT,TSHORT,
      TFIX,TSWIG,TSWIG,TSWIG,TFIX,TFIX,TSHORT,TSHORT,TSHORT,
      TFIX,TSWIG,TSWIG,TSWIG,TFIX,TFIX,TSHORT,TSHORT,TSHORT,
  };
  constexpr int SINGLE_STEPS = 47;

  const float gesture_eng[][MOTOR_TOTAL_COUNT] = {
      {0,10,40,40,0,10,40,40,0,10,40,40,0,10,40,40,-55,0,15,15},
      {0,45,70,70,0,45,70,70,0,45,70,70,0,45,70,70,-40,0,30,30},
      {0,80,90,80,0,80,90,80,0,80,90,80,0,80,90,80,-65,15,35,35},
      {0,90,40,50,0,80,60,60,0,60,50,50,0,40,60,60,0,-15,0,0},
      {0,0,90,90,0,0,90,90,0,0,90,90,0,0,90,90,0,0,20,0},
      {-12,10,50,50,-6,10,50,50,0,10,50,50,6,10,50,50,-30,5,20,20},
      {-12,45,50,50,-6,10,50,50,0,10,50,50,6,10,50,50,-30,5,20,20},
      {-12,45,50,50,-6,45,50,50,0,10,50,50,6,10,50,50,-30,5,20,20},
      {0,20,10,10,0,20,10,10,0,20,10,10,0,40,45,40,-30,10,0,25},
      {0,90,90,90,0,90,90,90,-7,0,0,0,7,0,0,0,-55,15,30,20},
  };
  constexpr int GESTURE_COUNT = 10;

  auto run_gesture = [&](const float* eng) {
    std::vector<int16_t> posi(MOTOR_TOTAL_COUNT);
    for (int i = 0; i < MOTOR_TOTAL_COUNT; ++i) {
      posi[i] = static_cast<int16_t>(eng[i] * 10);
    }
    auto ret = hand_->SetAllJointMotorPosi(posi);
    EXPECT_EQ(ret.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  };

  std::vector<int16_t> zero_posi(MOTOR_TOTAL_COUNT, 0);

  std::cout << "[GestureDance] Phase 1: Zero position" << std::endl;
  hand_->SetAllJointMotorPosi(zero_posi);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  auto read_posi = hand_->GetAllJointMotorPosi();
  EXPECT_EQ(read_posi.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  std::cout << "[GestureDance] Current positions: ";
  for (size_t i = 0; i < read_posi.size(); ++i) {
    std::cout << read_posi[i];
    if (i < read_posi.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << "[GestureDance] Phase 2: Single-joint sequence (" << SINGLE_STEPS << " steps)" << std::endl;
  for (int s = 0; s < SINGLE_STEPS; ++s) {
    int16_t posi_val = static_cast<int16_t>(ang_joint[s] * 10);
    auto ret = hand_->SetJointMotorPosi(idx_joint[s], posi_val);
    std::cout << "  Step " << (s + 1) << "/" << SINGLE_STEPS
              << ": Joint " << static_cast<int>(idx_joint[s])
              << " -> " << ang_joint[s] << " deg (ret=" << ret << ")" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(time_joint[s]));
  }

  auto after_single = hand_->GetAllJointMotorPosi();
  EXPECT_EQ(after_single.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  std::cout << "[GestureDance] Positions after single-joint phase: ";
  for (size_t i = 0; i < after_single.size(); ++i) {
    std::cout << after_single[i];
    if (i < after_single.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << "[GestureDance] Phase 3: Full-hand gestures (" << GESTURE_COUNT << " gestures)" << std::endl;
  for (int g = 0; g < GESTURE_COUNT; ++g) {
    std::cout << "  Gesture " << (g + 1) << "/" << GESTURE_COUNT << std::endl;
    run_gesture(gesture_eng[g]);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto cur = hand_->GetAllJointMotorPosi();
    EXPECT_EQ(cur.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  }

  std::cout << "[GestureDance] Phase 4: Return to zero" << std::endl;
  hand_->SetAllJointMotorPosi(zero_posi);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  auto final_posi = hand_->GetAllJointMotorPosi();
  EXPECT_EQ(final_posi.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  std::cout << "[GestureDance] Final positions: ";
  for (size_t i = 0; i < final_posi.size(); ++i) {
    std::cout << final_posi[i];
    if (i < final_posi.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << "[GestureDance] Switching back to CSP mode (POSITION=0)" << std::endl;
  hand_->SetAllControlMode(csp_modes);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::cout << "[GestureDance] Done!" << std::endl;
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
      std::cout << "J" << (i+1) << ":[" << agilink::omnihand::H3UMErrorReportToString(error_reports[i]) << "]";
      if (i < error_reports.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
    EXPECT_EQ(error_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ClearErrorReport) {
  if (hand_->Init()) {
    hand_->ClearAllErrorReport();
    std::cout << "[ClearAllErrorReport] Done" << std::endl;

    hand_->ClearErrorReport(1);
    std::cout << "[ClearErrorReport] Joint 1 Done" << std::endl;
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

TEST_F(OmniHand3UltraMTest, GetAllJointMotorVelo) {
  if (hand_->Init()) {
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

TEST_F(OmniHand3UltraMTest, AxisLimitPos) {
  if (hand_->Init()) {
    auto limits = hand_->GetAxisLimitPos();
    if (limits.empty()) {
      std::cout << "[GetAxisLimitPos] Failed: empty result" << std::endl;
      return;
    }
    std::cout << "[GetAxisLimitPos] Min Limits (0.1 deg): ";
    for (size_t i = 0; i < limits.min_limits.size(); ++i) {
      std::cout << limits.min_limits[i];
      if (i < limits.min_limits.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "[GetAxisLimitPos] Max Limits (0.1 deg): ";
    for (size_t i = 0; i < limits.max_limits.size(); ++i) {
      std::cout << limits.max_limits[i];
      if (i < limits.max_limits.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(limits.min_limits.size(), 20);
    EXPECT_EQ(limits.max_limits.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ActualAxisPos) {
  if (hand_->Init()) {
    auto actual_pos = hand_->GetAllActualAxisPos();
    if (actual_pos.empty()) {
      std::cout << "[GetAllActualAxisPos] Failed: empty result" << std::endl;
      return;
    }
    std::cout << "[GetAllActualAxisPos] Actual Positions (0.1 deg): ";
    for (size_t i = 0; i < actual_pos.size(); ++i) {
      std::cout << actual_pos[i];
      if (i < actual_pos.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(actual_pos.size(), 20);

    auto single = hand_->GetSingleActualAxisPos(1);
    std::cout << "[GetSingleActualAxisPos] Joint 1: " << single << std::endl;
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
