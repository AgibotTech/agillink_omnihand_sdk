// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>
#include "agilink_logger.h"
#include "omnihand/omnihand_3_ultra_m.h"
#include <chrono>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using agilink::AgilinkLogger;

static constexpr const char* TAG = "OmniHand3UltraMTest";

static int g_request_interval = 5;
static std::string g_device_type = "zlgcan";
static std::string g_can_interface = "can0";

static int GetRequestInterval() {
  return g_request_interval;
}

static std::string GetDeviceType() {
  return g_device_type;
}

template <typename T>
static std::string ValuesToString(const std::vector<T>& values) {
  std::ostringstream stream;
  for (size_t i = 0; i < values.size(); ++i) {
    if (i > 0) stream << ", ";
    stream << static_cast<long long>(values[i]);
  }
  return stream.str();
}

class OmniHand3UltraMTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::string device_type = GetDeviceType();
    if (device_type == "hcan") {
      hand_ = agilink::omnihand::OmniHand3UltraM::createHandByHcan(
          agilink::omnihand::HandType::LEFT, 9, 0, 0);
      AgilinkLogger::get().infof(TAG, "Using HCAN device");
#ifdef __linux__
    } else if (device_type == "socketcan") {
      hand_ = agilink::omnihand::OmniHand3UltraM::createHandSocketCan(
          agilink::omnihand::HandType::LEFT, 9, g_can_interface);
      AgilinkLogger::get().infof(TAG, "Using SocketCAN device (%s)", g_can_interface.c_str());
#endif
    } else {
      hand_ = agilink::omnihand::OmniHand3UltraM::createHandByZlgcan(
          agilink::omnihand::HandType::LEFT, 9, 0, 0);
      AgilinkLogger::get().infof(TAG, "Using ZLG CAN device");
    }
    int request_interval = GetRequestInterval();
    hand_->SetRequestInterval(request_interval);
    if (request_interval != 0) {
      AgilinkLogger::get().infof(TAG, "Using request interval: %d ms", request_interval);
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
    AgilinkLogger::get().infof(TAG, "[GetVendorInfo] %s", vendor_info.ToString().c_str());
    if (vendor_info.dof == 0) {
      AgilinkLogger::get().warnf(TAG, "[GetVendorInfo] Failed: got empty vendor info (timeout)");
      return;
    }
    EXPECT_EQ(vendor_info.dof, 20);
  }
}

TEST_F(OmniHand3UltraMTest, GetDeviceInfo) {
  if (hand_->Init()) {
    auto device_info = hand_->GetDeviceInfo();
    AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] %s", device_info.ToString().c_str());
    if (device_info.hand_device_id != 0) {
      EXPECT_EQ(device_info.hand_device_id, 9);
    }
  }
}

TEST_F(OmniHand3UltraMTest, GetNonPrivateHandDeviceIdByBroadcast) {
  if (!hand_->Init()) return;

  const uint8_t device_id = hand_->GetNonPrivateHandDeviceIdByBroadcast();
  ASSERT_GT(device_id, 0u) << "No H3U_M replied to the broadcast request";
  EXPECT_EQ(hand_->GetHandDeviceId(), device_id);
}

TEST_F(OmniHand3UltraMTest, MixControlRejectsInvalidArguments) {
  constexpr size_t kDof = agilink::omnihand::OmniHand3UltraM::kDegreesOfActiveFreedom;
  const std::vector<int16_t> valid(kDof, 0);
  const std::vector<int16_t> short_values(kDof - 1, 0);

  EXPECT_TRUE(hand_->MixControlByPT(short_values, short_values).empty());
  EXPECT_TRUE(hand_->MixControlByPVT(valid, short_values, valid).empty());

  const auto invalid_pt = hand_->MixControlByPT(0, 0, 0);
  EXPECT_EQ(invalid_pt.ctrl_mode_, 0);
  const auto invalid_pvt = hand_->MixControlByPVT(static_cast<uint8_t>(kDof + 1), 0, 0, 0);
  EXPECT_EQ(invalid_pvt.ctrl_mode_, 0);
}

// TEST_F(OmniHand3UltraMTest, SetDeviceId) {
//   auto current_device_info = hand_->GetDeviceInfo();
//   unsigned char current_id = current_device_info.hand_device_id;
//   if (current_id == 0) return;

//   unsigned char target_id = 2;
//   hand_->SetDeviceId(target_id);
//   AgilinkLogger::get().infof(TAG, "[SetDeviceId] Set Device ID: %d", static_cast<int>(target_id));
//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   auto device_info = hand_->GetDeviceInfo();
//   EXPECT_EQ(device_info.hand_device_id, 2);

//   unsigned char original_id = 9;
//   hand_->SetDeviceId(original_id);
//   AgilinkLogger::get().infof(TAG, "[SetDeviceId] Reset Device ID: %d", static_cast<int>(original_id));
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

  AgilinkLogger::get().infof(TAG, "[GestureDance] Switching to PP mode (PROFILE_POSITION=7)");
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

  AgilinkLogger::get().infof(TAG, "[GestureDance] Phase 1: Zero position");
  hand_->SetAllJointMotorPosi(zero_posi);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  auto read_posi = hand_->GetAllJointMotorPosi();
  EXPECT_EQ(read_posi.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  AgilinkLogger::get().infof(TAG, "[GestureDance] Current positions: %s",
                            ValuesToString(read_posi).c_str());

  AgilinkLogger::get().infof(TAG, "[GestureDance] Phase 2: Single-joint sequence (%d steps)", SINGLE_STEPS);
  for (int s = 0; s < SINGLE_STEPS; ++s) {
    int16_t posi_val = static_cast<int16_t>(ang_joint[s] * 10);
    auto ret = hand_->SetJointMotorPosi(idx_joint[s], posi_val);
    AgilinkLogger::get().infof(TAG, "  Step %d/%d: Joint %d -> %.1f deg (ret=%d)",
                              s + 1, SINGLE_STEPS, static_cast<int>(idx_joint[s]),
                              static_cast<double>(ang_joint[s]), static_cast<int>(ret));
    std::this_thread::sleep_for(std::chrono::milliseconds(time_joint[s]));
  }

  auto after_single = hand_->GetAllJointMotorPosi();
  EXPECT_EQ(after_single.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  AgilinkLogger::get().infof(TAG, "[GestureDance] Positions after single-joint phase: %s",
                            ValuesToString(after_single).c_str());

  AgilinkLogger::get().infof(TAG, "[GestureDance] Phase 3: Full-hand gestures (%d gestures)", GESTURE_COUNT);
  for (int g = 0; g < GESTURE_COUNT; ++g) {
    AgilinkLogger::get().infof(TAG, "  Gesture %d/%d", g + 1, GESTURE_COUNT);
    run_gesture(gesture_eng[g]);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto cur = hand_->GetAllJointMotorPosi();
    EXPECT_EQ(cur.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  }

  AgilinkLogger::get().infof(TAG, "[GestureDance] Phase 4: Return to zero");
  hand_->SetAllJointMotorPosi(zero_posi);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  auto final_posi = hand_->GetAllJointMotorPosi();
  EXPECT_EQ(final_posi.size(), static_cast<size_t>(MOTOR_TOTAL_COUNT));
  AgilinkLogger::get().infof(TAG, "[GestureDance] Final positions: %s",
                            ValuesToString(final_posi).c_str());

  AgilinkLogger::get().infof(TAG, "[GestureDance] Switching back to CSP mode (POSITION=0)");
  hand_->SetAllControlMode(csp_modes);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  AgilinkLogger::get().infof(TAG, "[GestureDance] Done!");
}

TEST_F(OmniHand3UltraMTest, ControlMode) {
  if (hand_->Init()) {
    auto current_modes = hand_->GetAllControlMode();
    if (current_modes.empty() || current_modes.size() != 20) {
      AgilinkLogger::get().warnf(TAG, "[GetAllControlMode] Failed: got %zu modes, expected 20",
                                current_modes.size());
      return;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllControlMode] Control Modes: %s",
                              ValuesToString(current_modes).c_str());
    EXPECT_EQ(current_modes.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ErrorReport) {
  if (hand_->Init()) {
    auto error_reports = hand_->GetAllErrorReport();
    if (error_reports.empty()) return;
    std::ostringstream reports;
    for (size_t i = 0; i < error_reports.size(); ++i) {
      if (i > 0) reports << " ";
      reports << "J" << (i + 1) << ":["
              << agilink::omnihand::H3UMErrorReportToString(error_reports[i]) << "]";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllErrorReport] Error Reports (20 joints): %s",
                              reports.str().c_str());
    EXPECT_EQ(error_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ClearErrorReport) {
  if (hand_->Init()) {
    hand_->ClearAllErrorReport();
    AgilinkLogger::get().infof(TAG, "[ClearAllErrorReport] Done");

    hand_->ClearErrorReport(1);
    AgilinkLogger::get().infof(TAG, "[ClearErrorReport] Joint 1 Done");
  }
}

TEST_F(OmniHand3UltraMTest, TemperatureReport) {
  if (hand_->Init()) {
    auto temp_reports = hand_->GetAllTemperatureReport();
    if (temp_reports.empty()) return;
    std::ostringstream reports;
    for (size_t i = 0; i < temp_reports.size(); ++i) {
      if (i > 0) reports << ", ";
      reports << "J" << (i + 1) << ":" << temp_reports[i];
    }
    AgilinkLogger::get().infof(TAG, "[GetAllTemperatureReport] Temperature Reports (C): %s",
                              reports.str().c_str());
    EXPECT_EQ(temp_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, CurrentReport) {
  if (hand_->Init()) {
    auto current_reports = hand_->GetAllCurrentReport();
    if (current_reports.empty()) return;
    std::ostringstream reports;
    for (size_t i = 0; i < current_reports.size(); ++i) {
      if (i > 0) reports << ", ";
      reports << "J" << (i + 1) << ":" << current_reports[i];
    }
    AgilinkLogger::get().infof(TAG, "[GetAllCurrentReport] Current Reports (mA): %s",
                              reports.str().c_str());
    EXPECT_EQ(current_reports.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, GetAllJointMotorVelo) {
  if (hand_->Init()) {
    auto current_velocities = hand_->GetAllJointMotorVelo();
    if (current_velocities.empty() || current_velocities.size() != 20) {
      AgilinkLogger::get().warnf(TAG, "[GetAllJointMotorVelo] Failed: got %zu velocities, expected 20",
                                current_velocities.size());
      return;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorVelo] Current Velocities: %s",
                              ValuesToString(current_velocities).c_str());
    EXPECT_EQ(current_velocities.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, AxisLimitPos) {
  if (hand_->Init()) {
    auto limits = hand_->GetAxisLimitPos();
    if (limits.empty()) {
      AgilinkLogger::get().warnf(TAG, "[GetAxisLimitPos] Failed: empty result");
      return;
    }
    AgilinkLogger::get().infof(TAG, "[GetAxisLimitPos] Min Limits (0.1 deg): %s",
                              ValuesToString(limits.min_limits).c_str());
    AgilinkLogger::get().infof(TAG, "[GetAxisLimitPos] Max Limits (0.1 deg): %s",
                              ValuesToString(limits.max_limits).c_str());
    EXPECT_EQ(limits.min_limits.size(), 20);
    EXPECT_EQ(limits.max_limits.size(), 20);
  }
}

TEST_F(OmniHand3UltraMTest, ActualAxisPos) {
  if (hand_->Init()) {
    auto actual_pos = hand_->GetAllActualAxisPos();
    if (actual_pos.empty()) {
      AgilinkLogger::get().warnf(TAG, "[GetAllActualAxisPos] Failed: empty result");
      return;
    }
    AgilinkLogger::get().infof(TAG, "[GetAllActualAxisPos] Actual Positions (0.1 deg): %s",
                              ValuesToString(actual_pos).c_str());
    EXPECT_EQ(actual_pos.size(), 20);

    auto single = hand_->GetSingleActualAxisPos(1);
    AgilinkLogger::get().infof(TAG, "[GetSingleActualAxisPos] Joint 1: %d", static_cast<int>(single));
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
          AgilinkLogger::get().errorf(TAG, "-f value %d is out of range (0-100ms)", interval);
          return 1;
        }
      } catch (const std::exception& e) {
        AgilinkLogger::get().errorf(TAG, "Invalid -f value: %s", argv[i + 1]);
        return 1;
      }
    } else if (arg == "-d" && i + 1 < argc) {
      std::string device_type = argv[i + 1];
      if (device_type == "zlgcan" || device_type == "hcan" || device_type == "socketcan") {
        g_device_type = device_type;
        ++i;
        continue;
      } else {
        AgilinkLogger::get().errorf(TAG,
                                   "-d value must be 'zlgcan', 'hcan' or 'socketcan', got: %s",
                                   device_type.c_str());
        return 1;
      }
    } else if (arg == "-c" && i + 1 < argc) {
      g_can_interface = argv[i + 1];
      ++i;
      continue;
    } else if (arg == "--help" || arg == "-h") {
      AgilinkLogger::get().infof(TAG, "Usage: %s [-f INTERVAL] [-d DEVICE] [-c CAN_INTERFACE]", argv[0]);
      AgilinkLogger::get().infof(TAG, "  -f INTERVAL       Set CAN request interval (0-100ms, default: 5ms)");
      AgilinkLogger::get().infof(TAG, "  -d DEVICE         Set CAN device type (zlgcan, hcan or socketcan, default: zlgcan)");
      AgilinkLogger::get().infof(TAG, "  -c CAN_INTERFACE  Set SocketCAN interface name (default: can0)");
      return 0;
    }
    gtest_args.push_back(argv[i]);
  }

  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
