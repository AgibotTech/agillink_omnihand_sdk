// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file test_omnipicker_3.cc
 * @brief CANFD tests for OmniPicker 3 (OP3), a 1-DOF gripper
 *
 * Usage:
 *   ./test_omnipicker_3 [-d TRANSPORT] [-c CHANNEL] [-i CANFD_ID] [-f INTERVAL]
 *                       [--device-id ID] [--can-if IF] [--tcp-host H] [--tcp-port P]
 *
 *   -d TRANSPORT     Backend: zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)
 *   --device-id ID   Hand device ID to address (default: 1). Unlike the O10 tests this
 *                    is only used to build requests: OP3 tests never call SetDeviceId(),
 *                    because writing the device id is inherently risky and a failed
 *                    restore leaves the unit unreachable.
 *   --can-if IF      SocketCAN interface name, socketcan only (default: can0)
 *   --tcp-host H     ZLG CANFD-over-TCP peer host, zlgcantcp only (default: 192.168.0.178)
 *   --tcp-port P     TCP port, zlgcantcp only (default: 8000)
 *   -c CHANNEL       CAN channel index for zlgcan / hcan / zlgcantcp (default: 0)
 *   -i CANFD_ID      Adapter device index for zlgcan / hcan (default: 0)
 *   -f INTERVAL      Request interval in ms (default: 5, max: 100)
 *
 * @note Registers known to be unimplemented on current OP3 firmware (0x03 current
 *       threshold, 0x12 velocity, 0x05 single-sensor tactile; see BACKLOG.md) are
 *       exercised for "does not crash, returns a sentinel" only -- they are not
 *       asserted to succeed, so this suite stays green on shipping firmware.
 */

#include <gtest/gtest.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// Deliberately not <windows.h>: it defines an ERROR macro (via wingdi.h) that collides
// with OmniLogger<>::Level::ERROR in agilink_logger.h below. Only _isatty is needed, and
// <io.h> provides it on its own. GoogleTest has already enabled VT processing on the
// console by the time this file's summary runs, so no SetConsoleMode call is required.
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

#include "agilink_logger.h"
#include "omnihand/export_symbols.h"
#include "omnihand/omnipicker_3.h"

namespace {

namespace oh = agilink::omnihand;

enum class CanfdTransport {
  kZlgcan,
  kHcan,
  kSocketCan,
  kZlgCanTcp,
};

CanfdTransport g_transport = CanfdTransport::kZlgcan;
int g_channel_id = 0;
int g_canfd_id = 0;
int g_request_interval = 5;
int g_device_id = oh::OmniPicker3::kDefaultHandDeviceId;
std::string g_can_if = "can0";
std::string g_tcp_host = "192.168.0.178";
uint16_t g_tcp_port = 8000;

struct CapabilityResult {
  std::string name;
  bool supported;
};

std::vector<CapabilityResult> g_capability_results;

// Colors are emitted only to a real terminal: a redirected log or a CI capture would
// otherwise be littered with escape sequences.
bool StdoutSupportsColor() {
#if defined(_WIN32)
  static const bool supported = _isatty(_fileno(stdout)) != 0;
#else
  static const bool supported = isatty(fileno(stdout)) != 0;
#endif
  return supported;
}

const char* ColorGreen() { return StdoutSupportsColor() ? "\033[32m" : ""; }
const char* ColorRed() { return StdoutSupportsColor() ? "\033[31m" : ""; }
const char* ColorYellow() { return StdoutSupportsColor() ? "\033[33m" : ""; }
const char* ColorReset() { return StdoutSupportsColor() ? "\033[0m" : ""; }

using agilink::AgilinkLogger;
constexpr const char* TAG = "OmniPicker3Test";

// OP3 has exactly one active joint; every per-joint vector getter must return this many entries.
constexpr size_t kDoF = oh::OmniPicker3::kDegreesOfActiveFreedom;

CanfdTransport ParseTransport(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  if (s == "zlgcan") return CanfdTransport::kZlgcan;
  if (s == "hcan") return CanfdTransport::kHcan;
  if (s == "socketcan") return CanfdTransport::kSocketCan;
  if (s == "zlgcantcp" || s == "zlg_tcp" || s == "zlgcan_tcp") return CanfdTransport::kZlgCanTcp;
  AgilinkLogger::get().warnf(TAG, "[Warning] unknown -d %s, using zlgcan", s.c_str());
  return CanfdTransport::kZlgcan;
}

template <typename T>
std::string JoinValues(const std::vector<T>& values) {
  std::string msg;
  for (size_t i = 0; i < values.size(); ++i) {
    if (i > 0) msg += ", ";
    msg += std::to_string(values[i]);
  }
  return msg;
}

// Builds a hand on the transport selected by -d. Shared by the fixture and by
// ReturnToZeroOnce() so the closing gesture uses the same backend as the tests.
std::unique_ptr<oh::OmniPicker3> CreateHandForTransport() {
  using oh::OmniPicker3;
  // hand_type carries no meaning for a gripper: OP3 stores it but never uses it.
  constexpr oh::HandType kFactoryHandType = oh::HandType::UNKNOWN;
  const uint8_t device_id = static_cast<uint8_t>(g_device_id);

  switch (g_transport) {
    case CanfdTransport::kZlgcan:
      return OmniPicker3::createHandByZlgcan(kFactoryHandType, device_id,
                                             static_cast<uint8_t>(g_canfd_id),
                                             static_cast<uint8_t>(g_channel_id));
    case CanfdTransport::kHcan:
      return OmniPicker3::createHandByHcan(kFactoryHandType, device_id,
                                           static_cast<uint8_t>(g_canfd_id),
                                           static_cast<uint8_t>(g_channel_id));
    case CanfdTransport::kSocketCan:
#if defined(__linux__)
      return OmniPicker3::createHandSocketCan(kFactoryHandType, device_id, g_can_if);
#else
      AgilinkLogger::get().warnf(TAG, "[Warning] SocketCAN requires Linux; skipping hand creation.");
      return nullptr;
#endif
    case CanfdTransport::kZlgCanTcp:
#if OMNIHAND_ZLG_TCP_SUPPORTED
      return OmniPicker3::createHandByZlgCanTcp(kFactoryHandType, device_id, g_tcp_host, g_tcp_port,
                                                static_cast<uint8_t>(g_channel_id));
#else
      AgilinkLogger::get().warnf(TAG, "[Warning] ZLG CANFD over TCP not supported on this platform.");
      return nullptr;
#endif
  }
  return nullptr;
}

class OmniPicker3Test : public ::testing::Test {
 protected:
  void SetUp() override {
    hand_ = CreateHandForTransport();

    if (hand_) {
      hand_->SetRequestInterval(g_request_interval);
      hand_->ShowDataDetails(true);
      device_available_ = hand_->Init();
      if (!device_available_) {
        AgilinkLogger::get().warnf(TAG, "[Warning]: OP3 device created but Init() failed.");
      }
    }
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << "OmniPicker 3 device not available";
    }
  }

  void TearDown() override {
    // Unlike a per-test gesture reset, this only drops the handle: closing the gripper
    // after every case cost 500 ms and one open/close cycle each. ReturnToZeroOnce()
    // leaves the unit closed once, after the whole suite.
    hand_.reset();
    AgilinkLogger::get().flush();
  }

  void RequireDevice() { ASSERT_TRUE(device_available_); }

  std::unique_ptr<oh::OmniPicker3> hand_;
  bool device_available_ = false;
};

// ============================================================================
// Basic Connection
// ============================================================================

TEST_F(OmniPicker3Test, CreateHand) {
  ASSERT_TRUE(device_available_) << "OP3 hand object failed to initialize";
  EXPECT_EQ(hand_->GetProductType(), oh::ProductType::OMNI_PICKER_3);
}

// ============================================================================
// Static Product Constants (no hardware traffic)
// ============================================================================

TEST_F(OmniPicker3Test, ProductConstants) {
  RequireDevice();

  EXPECT_EQ(oh::OmniPicker3::GetDoA(), 1u);
  EXPECT_EQ(oh::OmniPicker3::GetNumOfJointMotors(), 1u);
  EXPECT_EQ(hand_->GetJointNames().size(), kDoF);

  const auto bound = oh::OmniPicker3::GetMinMaxMotorPosition();
  EXPECT_EQ(bound.min_value, 0);
  EXPECT_EQ(bound.max_value, 4095);
  AgilinkLogger::get().infof(TAG, "[ProductConstants] motor position range: [%d, %d]",
                             bound.min_value, bound.max_value);
}

// ============================================================================
// Device / Vendor Info
// ============================================================================

TEST_F(OmniPicker3Test, GetVendorInfo) {
  RequireDevice();

  auto vendor_info = hand_->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo]\n%s", vendor_info.ToString().c_str());

  ASSERT_NE(vendor_info.dof, 0) << "GetVendorInfo timed out";
  EXPECT_EQ(vendor_info.dof, kDoF);
}

TEST_F(OmniPicker3Test, GetDeviceInfo) {
  RequireDevice();

  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo]\n%s", device_info.ToString().c_str());

  ASSERT_NE(device_info.hand_device_id, 0) << "GetDeviceInfo timed out";
  EXPECT_EQ(device_info.hand_device_id, static_cast<uint8_t>(g_device_id));
}

// ============================================================================
// Communication Settings (round-trip, no protocol traffic)
// ============================================================================

TEST_F(OmniPicker3Test, CommunicationSettingsRoundTrip) {
  RequireDevice();

  const int interval = hand_->GetRequestInterval();
  const int recv_timeout = hand_->GetFrameRecvTimeout();
  const int send_timeout = hand_->GetFrameSendTimeout();
  AgilinkLogger::get().infof(TAG, "[CommunicationSettings] interval=%dms recv=%dms send=%dms",
                             interval, recv_timeout, send_timeout);

  EXPECT_EQ(interval, g_request_interval);

  hand_->SetRequestInterval(interval + 1);
  EXPECT_EQ(hand_->GetRequestInterval(), interval + 1);
  hand_->SetRequestInterval(interval);
  EXPECT_EQ(hand_->GetRequestInterval(), interval);

  hand_->SetFrameRecvTimeout(recv_timeout + 10);
  EXPECT_EQ(hand_->GetFrameRecvTimeout(), recv_timeout + 10);
  hand_->SetFrameRecvTimeout(recv_timeout);
  EXPECT_EQ(hand_->GetFrameRecvTimeout(), recv_timeout);

  hand_->SetFrameSendTimeout(send_timeout);
  EXPECT_EQ(hand_->GetFrameSendTimeout(), send_timeout);
}

// ============================================================================
// Position Control
// ============================================================================

TEST_F(OmniPicker3Test, SetGetSingleJointPosi) {
  RequireDevice();

  constexpr int16_t kTarget = 1024;
  const int16_t set_result = hand_->SetJointMotorPosi(1, kTarget);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  const int16_t pos = hand_->GetJointMotorPosi(1);
  AgilinkLogger::get().infof(TAG, "[SetGetSingleJointPosi] target=%d set_result=%d readback=%d",
                             kTarget, set_result, pos);

  EXPECT_GE(pos, oh::OmniPicker3::GetMinMaxMotorPosition().min_value);
  EXPECT_LE(pos, oh::OmniPicker3::GetMinMaxMotorPosition().max_value);
}

TEST_F(OmniPicker3Test, SetGetAllJointPosi) {
  RequireDevice();

  const std::vector<int16_t> positions = {2048};
  auto set_result = hand_->SetAllJointMotorPosi(positions);
  AgilinkLogger::get().infof(TAG, "[SetAllJointMotorPosi] returned %zu: %s", set_result.size(),
                             JoinValues(set_result).c_str());
  ASSERT_EQ(set_result.size(), kDoF) << "SetAllJointMotorPosi timed out or returned a bad size";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto get_result = hand_->GetAllJointMotorPosi();
  AgilinkLogger::get().infof(TAG, "[GetAllJointMotorPosi] returned %zu: %s", get_result.size(),
                             JoinValues(get_result).c_str());
  ASSERT_EQ(get_result.size(), kDoF) << "GetAllJointMotorPosi timed out or returned a bad size";
}

// Out-of-range joint indices must be rejected locally, without a bus round-trip.
TEST_F(OmniPicker3Test, JointIndexBounds) {
  RequireDevice();

  EXPECT_EQ(hand_->GetJointMotorPosi(0), -1);
  EXPECT_EQ(hand_->GetJointMotorPosi(kDoF + 1), -1);
  EXPECT_EQ(hand_->SetJointMotorPosi(0, 1024), -1);
  EXPECT_EQ(hand_->SetJointMotorPosi(kDoF + 1, 1024), -1);
}

// ============================================================================
// Gesture
// ============================================================================

TEST_F(OmniPicker3Test, SetGetHandGesture) {
  RequireDevice();

  const std::vector<std::pair<oh::OmniPicker3Gesture, const char*>> gestures = {
      {oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_ZERO, "ZERO"},
      {oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_HALF_OPEN, "HALF_OPEN"},
      {oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_FULL_OPEN, "FULL_OPEN"},
  };

  for (const auto& gesture : gestures) {
    auto target = hand_->GetHandGesture(gesture.first);
    ASSERT_EQ(target.size(), kDoF) << "GetHandGesture(" << gesture.second << ") bad size";

    hand_->SetHandGesture(gesture.first);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto readback = hand_->GetAllJointMotorPosi();
    AgilinkLogger::get().infof(TAG, "[SetGetHandGesture] %s target=%s readback=%s", gesture.second,
                               JoinValues(target).c_str(), JoinValues(readback).c_str());
    EXPECT_EQ(readback.size(), kDoF);
  }
}

// The numeric overload shares the gesture table with the typed one.
TEST_F(OmniPicker3Test, HandGestureByNumber) {
  RequireDevice();

  for (int gesture_num = 0; gesture_num <= 2; ++gesture_num) {
    auto target = hand_->GetHandGesture(gesture_num);
    AgilinkLogger::get().infof(TAG, "[HandGestureByNumber] %d -> %s", gesture_num,
                               JoinValues(target).c_str());
    EXPECT_EQ(target.size(), kDoF);
  }
}

// ============================================================================
// Reports
// ============================================================================

TEST_F(OmniPicker3Test, GetErrorReport) {
  RequireDevice();

  auto errors = hand_->GetAllErrorReport();
  ASSERT_EQ(errors.size(), kDoF) << "GetAllErrorReport timed out or returned a bad size";
  AgilinkLogger::get().infof(TAG, "[GetAllErrorReport] J1: %s", errors[0].ToString().c_str());

  auto single = hand_->GetErrorReport(1);
  AgilinkLogger::get().infof(TAG, "[GetErrorReport(1)] %s", single.ToString().c_str());
}

TEST_F(OmniPicker3Test, GetTemperatureReport) {
  RequireDevice();

  auto temps = hand_->GetAllTemperatureReport();
  ASSERT_EQ(temps.size(), kDoF) << "GetAllTemperatureReport timed out or returned a bad size";
  AgilinkLogger::get().infof(TAG, "[GetAllTemperatureReport] %s", JoinValues(temps).c_str());

  for (auto temp : temps) {
    EXPECT_GE(temp, -40);
    EXPECT_LE(temp, 127);
  }

  const int16_t single = hand_->GetTemperatureReport(1);
  AgilinkLogger::get().infof(TAG, "[GetTemperatureReport(1)] %d", single);
}

TEST_F(OmniPicker3Test, GetCurrentReport) {
  RequireDevice();

  auto currents = hand_->GetAllCurrentReport();
  ASSERT_EQ(currents.size(), kDoF) << "GetAllCurrentReport timed out or returned a bad size";
  AgilinkLogger::get().infof(TAG, "[GetAllCurrentReport] %s mA", JoinValues(currents).c_str());

  const int16_t single = hand_->GetCurrentReport(1);
  AgilinkLogger::get().infof(TAG, "[GetCurrentReport(1)] %d mA", single);
}

// ============================================================================
// Mixed Control
// ============================================================================

TEST_F(OmniPicker3Test, MixControlByPT) {
  RequireDevice();

  const std::vector<int16_t> positions = {2048};
  const std::vector<int16_t> torques = {0};

  auto result = hand_->MixControlByPT(positions, torques);
  ASSERT_EQ(result.size(), kDoF) << "MixControlByPT timed out or returned a bad size";
  EXPECT_TRUE(result[0].tgt_posi_.has_value());
  AgilinkLogger::get().infof(TAG, "[MixControlByPT] joint_field=%u mode=%s",
                             static_cast<unsigned int>(result[0].joint_index_),
                             oh::ToString(static_cast<oh::MixControlMode>(result[0].ctrl_mode_)).c_str());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto feedback = hand_->GetAllJointMotorPosi();
  AgilinkLogger::get().infof(TAG, "[MixControlByPT] position feedback: %s",
                             JoinValues(feedback).c_str());
  EXPECT_EQ(feedback.size(), kDoF);
}

// MixControlByPV / MixControlByPVT are not overridden by either OP3 backend, so they
// resolve to the OmniHand base-class no-ops. Asserted here so the empty result is a
// documented property rather than a silent surprise for callers.
TEST_F(OmniPicker3Test, MixControlByPVAndPVTAreUnsupported) {
  RequireDevice();

  EXPECT_TRUE(hand_->MixControlByPV({8000}, {0}).empty());
  EXPECT_TRUE(hand_->MixControlByPVT({2048}, {8000}, {0}).empty());
  g_capability_results.push_back({"MixControlByPV / MixControlByPVT", false});
}

// ============================================================================
// Kinematics (OP3 has no solver: these are documented no-ops)
// ============================================================================

TEST_F(OmniPicker3Test, JointAngleApisAreUnsupported) {
  RequireDevice();

  // OP3 has no kinematics solver, so these APIs return a default vector of zeros rather than empty.
  // The implementation logs a warning and returns vector<double>(kDegreesOfActiveFreedom, 0.0).
  EXPECT_TRUE(hand_->SetAllActiveJointAngles({0.5}).empty());
  auto active_angles = hand_->GetAllActiveJointAngles();
  EXPECT_EQ(active_angles.size(), 1u);
  EXPECT_EQ(active_angles[0], 0.0);

  auto all_angles = hand_->GetAllJointAngles();
  EXPECT_EQ(all_angles.size(), 1u);
  EXPECT_EQ(all_angles[0], 0.0);

  auto all_angles_with_input = hand_->GetAllJointAngles({0.5});
  EXPECT_EQ(all_angles_with_input.size(), 1u);
  EXPECT_EQ(all_angles_with_input[0], 0.0);
}

// ============================================================================
// Tactile Sensors
// ============================================================================

TEST_F(OmniPicker3Test, InitTactilePointsMap) {
  RequireDevice();

  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "InitTactilePointsMap failed; tactile sensors unavailable on this unit";
  }

  const auto& sensor_order = hand_->GetSensorOrder();
  ASSERT_FALSE(sensor_order.empty());
  EXPECT_EQ(hand_->GetNumOfTactileSensors(), sensor_order.size());

  for (const auto finger : sensor_order) {
    const size_t points = hand_->GetNumOfTactilePoints(finger);
    const size_t datum_len = hand_->GetLenOfTactileDatum(finger);
    const size_t frames = hand_->GetNumOfRepliedTactileFrames(finger);
    AgilinkLogger::get().infof(TAG, "[InitTactilePointsMap] %s: %zu points, %zu bytes/point, %zu frame(s), SN=\"%s\"",
                               oh::ToString(finger).c_str(), points, datum_len, frames,
                               hand_->GetSNOfTactileSensor(finger).c_str());
    EXPECT_GT(points, 0u) << "Expected >0 points for " << oh::ToString(finger);
    EXPECT_GT(datum_len, 0u) << "Expected >0 bytes per datum for " << oh::ToString(finger);
    EXPECT_GT(frames, 0u) << "Expected >=1 frame for " << oh::ToString(finger);
    EXPECT_EQ(hand_->GetSensorDataLength(finger), points);
  }

  EXPECT_EQ(hand_->GetNumOfTactilePoints(oh::Finger::UNKNOWN), 0u);
  EXPECT_EQ(hand_->GetLenOfTactileDatum(oh::Finger::UNKNOWN), 0u);
  EXPECT_EQ(hand_->GetNumOfRepliedTactileFrames(oh::Finger::UNKNOWN), 0u);
  EXPECT_EQ(hand_->GetSNOfTactileSensor(oh::Finger::UNKNOWN), "");
}

TEST_F(OmniPicker3Test, GetAllTactileSensorDataRaw) {
  RequireDevice();

  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "InitTactilePointsMap failed; tactile sensors unavailable on this unit";
  }

  auto all_data = hand_->GetAllTactileSensorDataRaw();
  ASSERT_FALSE(all_data.empty()) << "GetAllTactileSensorDataRaw timed out";
  EXPECT_EQ(all_data.size(), hand_->GetSensorOrder().size());

  for (const auto& sensor : all_data) {
    AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] %s: %zu points",
                               oh::ToString(sensor.sensor_id_).c_str(), sensor.data_.size());
    EXPECT_EQ(sensor.data_.size(), hand_->GetNumOfTactilePoints(sensor.sensor_id_));
  }
}

TEST_F(OmniPicker3Test, GetTactileSensorDataRawPerFinger) {
  RequireDevice();

  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "InitTactilePointsMap failed; tactile sensors unavailable on this unit";
  }

  for (const auto finger : hand_->GetSensorOrder()) {
    auto data = hand_->GetTactileSensorDataRaw(finger);
    AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] %s: %zu points",
                               oh::ToString(finger).c_str(), data.data_.size());
    EXPECT_EQ(data.sensor_id_, finger);
    EXPECT_EQ(data.data_.size(), hand_->GetNumOfTactilePoints(finger));
  }
}

// Register 0x05 (single-sensor downsampled tactile) is not implemented on current OP3
// firmware and times out (BACKLOG.md). Logged rather than asserted so the suite tracks
// firmware support without failing on it.
TEST_F(OmniPicker3Test, GetTactileSensorDataDownsampled) {
  RequireDevice();

  if (!hand_->InitTactilePointsMap()) {
    GTEST_SKIP() << "InitTactilePointsMap failed; tactile sensors unavailable on this unit";
  }

  bool any_answered = false;
  for (const auto finger : hand_->GetSensorOrder()) {
    auto data = hand_->GetTactileSensorData(finger);
    if (data.data_.empty()) {
      AgilinkLogger::get().warnf(TAG, "[GetTactileSensorData] %s: empty (register 0x05 unsupported by this firmware)",
                                 oh::ToString(finger).c_str());
      continue;
    }
    any_answered = true;
    AgilinkLogger::get().infof(TAG, "[GetTactileSensorData] %s: %zu points",
                               oh::ToString(finger).c_str(), data.data_.size());
    EXPECT_EQ(data.sensor_id_, finger);
  }
  g_capability_results.push_back({"0x05 single-sensor downsampled tactile", any_answered});
}

// ============================================================================
// Registers absent from the current firmware / protocol spec
// ============================================================================

// Register 0x12 (velocity). Absent from protocol spec rev v1.1.2 and times out on
// current firmware (BACKLOG.md); exercised for crash-freedom only.
TEST_F(OmniPicker3Test, VelocityRegisterIsDeprecated) {
  RequireDevice();

  auto all_velo = hand_->GetAllJointMotorVelo();
  const int16_t single_velo = hand_->GetJointMotorVelo(1);
  AgilinkLogger::get().infof(TAG, "[GetAllJointMotorVelo] size=%zu values=[%s], GetJointMotorVelo(1)=%d",
                             all_velo.size(), JoinValues(all_velo).c_str(), single_velo);

  const bool supported = !all_velo.empty();
  g_capability_results.push_back({"0x12 velocity register", supported});
  if (!supported) {
    AgilinkLogger::get().warnf(TAG, "[GetAllJointMotorVelo] empty (register 0x12 unsupported by this firmware)");
  } else {
    EXPECT_EQ(all_velo.size(), kDoF);
  }
}

// Register 0x03 (current threshold). Same status as 0x12 above. The write side is
// exercised with the value read back from the device, so a firmware that does answer
// is left exactly as it was found.
TEST_F(OmniPicker3Test, CurrentThresholdRegisterIsDeprecated) {
  RequireDevice();

  auto thresholds = hand_->GetAllCurrentThreshold();
  const int16_t single = hand_->GetCurrentThreshold(1);
  AgilinkLogger::get().infof(TAG, "[GetAllCurrentThreshold] size=%zu values=[%s], GetCurrentThreshold(1)=%d",
                             thresholds.size(), JoinValues(thresholds).c_str(), single);

  g_capability_results.push_back({"0x03 current threshold register", !thresholds.empty()});
  if (thresholds.empty()) {
    AgilinkLogger::get().warnf(TAG, "[GetAllCurrentThreshold] empty (register 0x03 unsupported by this firmware)");
    return;
  }

  ASSERT_EQ(thresholds.size(), kDoF);

  // Rewrite the values just read: verifies the write path without changing device state.
  hand_->SetCurrentThreshold(1, thresholds[0]);
  hand_->SetAllCurrentThreshold(thresholds);
  auto after = hand_->GetAllCurrentThreshold();
  AgilinkLogger::get().infof(TAG, "[SetAllCurrentThreshold] rewrote %s, readback %s",
                             JoinValues(thresholds).c_str(), JoinValues(after).c_str());
  EXPECT_EQ(after, thresholds);
}

// Prints the two things GoogleTest's own summary cannot show: which registers this
// firmware answered, and whether the green result actually touched hardware. Written to
// stdout rather than through AgilinkLogger, so it lands after the gtest report instead
// of being interleaved by the async log worker.
void PrintRunSummary() {
  const auto& unit_test = *::testing::UnitTest::GetInstance();

  std::cout << "\n=== Firmware capability ===\n";
  if (g_capability_results.empty()) {
    std::cout << "  (not probed -- the cases that probe firmware support did not run)\n";
  } else {
    for (const auto& capability : g_capability_results) {
      std::cout << "  " << (capability.supported ? "SUPPORTED    " : "NOT SUPPORTED")
                << "  " << capability.name << "\n";
    }
  }

  const int total = unit_test.total_test_count();
  const int passed = unit_test.successful_test_count();
  const int failed = unit_test.failed_test_count();
  const int skipped = unit_test.skipped_test_count();

  std::cout << "\n=== Summary ===\n";
  std::cout << "  total: " << total << "  passed: " << passed << "  failed: " << failed
            << "  skipped: " << skipped << "\n";

  if (passed > 0) {
    std::cout << "\n  Passed tests:\n";
    for (int i = 0; i < unit_test.total_test_suite_count(); ++i) {
      const auto& suite = *unit_test.GetTestSuite(i);
      for (int j = 0; j < suite.total_test_count(); ++j) {
        const auto& test = *suite.GetTestInfo(j);
        const auto* result = test.result();
        if (result != nullptr && !result->Failed() && !result->Skipped()) {
          std::cout << "    " << ColorGreen() << "[PASSED ]" << ColorReset() << " " << suite.name()
                    << "." << test.name() << "\n";
        }
      }
    }
  }

  if (failed > 0) {
    std::cout << "\n  Failed tests:\n";
    for (int i = 0; i < unit_test.total_test_suite_count(); ++i) {
      const auto& suite = *unit_test.GetTestSuite(i);
      for (int j = 0; j < suite.total_test_count(); ++j) {
        const auto& test = *suite.GetTestInfo(j);
        const auto* result = test.result();
        if (result != nullptr && result->Failed()) {
          std::cout << "    " << ColorRed() << "[FAILED ]" << ColorReset() << " " << suite.name()
                    << "." << test.name() << "\n";
        }
      }
    }
  }

  if (skipped > 0) {
    std::cout << "\n  Skipped tests:\n";
    for (int i = 0; i < unit_test.total_test_suite_count(); ++i) {
      const auto& suite = *unit_test.GetTestSuite(i);
      for (int j = 0; j < suite.total_test_count(); ++j) {
        const auto& test = *suite.GetTestInfo(j);
        const auto* result = test.result();
        if (result != nullptr && result->Skipped()) {
          std::cout << "    " << ColorYellow() << "[SKIPPED]" << ColorReset() << " " << suite.name()
                    << "." << test.name() << "\n";
        }
      }
    }
  }

  // A suite that skipped everything still exits 0, so say plainly that nothing was
  // verified -- otherwise a green ctest run reads as "OP3 works" with no hardware present.
  if (total > 0 && skipped == total) {
    std::cout << "\n  ALL TESTS SKIPPED -- no hardware was contacted, nothing was verified.\n";
  }
  std::cout << std::endl;
}

// Leaves the gripper closed after the suite. Called once from main() instead of from
// TearDown(), so a full run costs one gesture rather than one per test case.
void ReturnToZeroOnce() {
  auto hand = CreateHandForTransport();
  if (!hand || !hand->Init()) {
    return;
  }
  hand->SetRequestInterval(g_request_interval);
  hand->SetHandGesture(oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_ZERO);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  AgilinkLogger::get().infof(TAG, "[ReturnToZeroOnce] gripper returned to zero");
}

}  // namespace

// ============================================================================
// Main
// ============================================================================

int main(int argc, char** argv) {
  std::vector<char*> gtest_args;
  gtest_args.push_back(argv[0]);

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];

    if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      g_transport = ParseTransport(argv[++i]);
    } else if ((arg == "--device-id" || arg == "--id") && i + 1 < argc) {
      g_device_id = std::stoi(argv[++i]);
    } else if (arg == "-c" && i + 1 < argc) {
      g_channel_id = std::stoi(argv[++i]);
    } else if (arg == "-i" && i + 1 < argc) {
      g_canfd_id = std::stoi(argv[++i]);
    } else if (arg == "-f" && i + 1 < argc) {
      g_request_interval = std::stoi(argv[++i]);
      if (g_request_interval > 100) g_request_interval = 100;
    } else if (arg == "--can-if" && i + 1 < argc) {
      g_can_if = argv[++i];
    } else if (arg == "--tcp-host" && i + 1 < argc) {
      g_tcp_host = argv[++i];
    } else if (arg == "--tcp-port" && i + 1 < argc) {
      g_tcp_port = static_cast<uint16_t>(std::stoi(argv[++i]));
    } else if (arg == "--help" || arg == "-h") {
      // Printed through std::cout, not AgilinkLogger: help text is a synchronous
      // one-shot and must not carry the logger's tag/timestamp prefixes.
      std::cout
          << "OmniPicker 3 Test\n\n"
          << "Usage: " << argv[0] << " [options]\n\n"
          << "Options:\n"
          << "  -d, --device NAME    zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)\n"
          << "  --device-id ID       hand device ID to address (default: 1); never written by this suite\n"
          << "  -c CHANNEL           CAN channel (zlgcan/hcan/zlgcantcp), default 0\n"
          << "  -i CANFD_ID          adapter index (zlgcan/hcan), default 0\n"
          << "  --can-if IFACE       SocketCAN iface (socketcan), default can0\n"
          << "  --tcp-host HOST      ZLG TCP host (zlgcantcp), default 192.168.0.178\n"
          << "  --tcp-port PORT      ZLG TCP port (zlgcantcp), default 8000\n"
          << "  -f INTERVAL          request interval ms, default 5, max 100\n"
          << "\nExample:\n"
          << "  " << argv[0] << " -d zlgcan -i 0 -c 0 --id 1 -f 5\n"
          << "    -d zlgcan  use the ZLG USBCANFD adapter\n"
          << "    -i 0       adapter index 0\n"
          << "    -c 0       CAN channel 0\n"
          << "    --id 1     address the gripper as device id 1\n"
          << "    -f 5       5 ms between requests\n"
          << "\nOther transports:\n"
          << "  " << argv[0] << " -d socketcan --can-if can0\n"
          << "  " << argv[0] << " -d zlgcantcp --tcp-host 192.168.0.178 --tcp-port 8000\n"
          << "\nGoogleTest flags are forwarded, e.g. --gtest_filter=*Tactile*\n"
          << std::endl;
      return 0;
    } else {
      gtest_args.push_back(argv[i]);
    }
  }

  AgilinkLogger::get().infof(TAG, "=== OmniPicker 3 Test ===");
  switch (g_transport) {
    case CanfdTransport::kZlgcan:
      AgilinkLogger::get().infof(TAG, "Transport: zlgcan");
      break;
    case CanfdTransport::kHcan:
      AgilinkLogger::get().infof(TAG, "Transport: hcan");
      break;
    case CanfdTransport::kSocketCan:
      AgilinkLogger::get().infof(TAG, "Transport: socketcan  can_if=%s", g_can_if.c_str());
      break;
    case CanfdTransport::kZlgCanTcp:
      AgilinkLogger::get().infof(TAG, "Transport: zlgcantcp  %s:%d", g_tcp_host.c_str(), g_tcp_port);
      break;
  }
  AgilinkLogger::get().infof(TAG, "Channel ID: %d", g_channel_id);
  AgilinkLogger::get().infof(TAG, "CANFD ID: %d", g_canfd_id);
  AgilinkLogger::get().infof(TAG, "Device ID: %d", g_device_id);
  AgilinkLogger::get().infof(TAG, "Request Interval: %d ms", g_request_interval);
  AgilinkLogger::get().infof(TAG, "Run with --help to list all options.");
  AgilinkLogger::get().infof(TAG, "=========================");

  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  const int result = RUN_ALL_TESTS();
  ReturnToZeroOnce();
  // Drain the async logger first, so the summary is not interleaved with buffered log lines.
  AgilinkLogger::get().flush();
  PrintRunSummary();
  return result;
}
