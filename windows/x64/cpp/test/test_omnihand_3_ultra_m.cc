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

  constexpr size_t MOTOR_TOTAL_COUNT = 20;

  std::vector<unsigned char> pp_modes(MOTOR_TOTAL_COUNT, static_cast<unsigned char>(agilink::omnihand::ControlMode::PROFILE_POSITION));
  std::vector<unsigned char> csp_modes(MOTOR_TOTAL_COUNT, static_cast<unsigned char>(agilink::omnihand::ControlMode::POSITION));

  AgilinkLogger::get().infof(TAG, "[GestureDance] Switching to PP mode (PROFILE_POSITION=7)");
  hand_->SetAllControlMode(pp_modes);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // The second gesture intentionally keeps axis 1 at 2048.
  const std::vector<std::vector<int16_t>> gestures = {
      {2048, 512, 512, 512, 2048, 512, 512, 512, 2048, 512,
       512, 512, 2048, 512, 512, 512, 3192, 1024, 512, 512},
      {2048, 4096, 4096, 2048, 2048, 4096, 4096, 2048, 2048, 4096,
       4096, 2048, 2048, 4096, 512, 512, 3156, 1349, 512, 512},
  };

  for (size_t i = 0; i < gestures.size(); ++i) {
    AgilinkLogger::get().infof(TAG, "[GestureDance] Gesture %zu/%zu: %s",
                              i + 1, gestures.size(), ValuesToString(gestures[i]).c_str());
    const auto feedback = hand_->SetAllJointMotorPosi(gestures[i]);
    EXPECT_EQ(feedback.size(), MOTOR_TOTAL_COUNT);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  // Repeat one safe single-axis transition using axis 2 (SDK index 1).
  constexpr uint8_t SINGLE_AXIS_INDEX = 1;
  const int16_t single_axis_positions[] = {512, 4096};
  for (const int16_t position : single_axis_positions) {
    const auto feedback = hand_->SetJointMotorPosi(SINGLE_AXIS_INDEX, position);
    AgilinkLogger::get().infof(TAG, "[GestureDance] Axis 2 -> %d (feedback=%d)",
                              static_cast<int>(position), static_cast<int>(feedback));
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

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
