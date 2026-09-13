// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file test_omnihand_2025_canfd.cc
 * @brief CANFD-specific tests for OmniHand 2025 (public factory APIs, non-private)
 *
 * Usage:
 *   ./test_omnihand_2025_canfd [-t TRANSPORT] [-c CHANNEL] [-i CANFD_ID] [-f INTERVAL]
 *                              [--device-id ID] [--can-if IF] [--tcp-host H] [--tcp-port P]
 *
 *   -t TRANSPORT     Backend: zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)
 *   --device-id ID   Target hand device ID to test with (default: 1). SetUp discovers
 *                    the current ID via broadcast, switches to this value, and TearDown
 *                    restores the original ID.
 *   --can-if IF      SocketCAN interface name, socketcan only (default: can0)
 *   --tcp-host H     ZLG CANFD-over-TCP peer host, zlgcantcp only (default: 192.168.0.178)
 *   --tcp-port P     TCP port, zlgcantcp only (default: 8000)
 *   -c CHANNEL       CAN channel index for zlgcan / hcan / zlgcantcp (default: 0)
 *   -i CANFD_ID      Adapter device index for zlgcan / hcan (default: 0)
 *   -f INTERVAL      Request interval in ms (default: 5, max: 100)
 */

#include <gtest/gtest.h>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_2025.h"
#include <memory>
#include <vector>
#include "agilink_logger.h"
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <cctype>
#include <algorithm>

enum class CanfdTransport {
  kZlgcan,
  kHcan,
  kSocketCan,
  kZlgCanTcp,
};

static CanfdTransport g_transport = CanfdTransport::kZlgcan;
static int g_channel_id = 0;
static int g_canfd_id = 0;
static int g_request_interval = 5;  // CANFD default: 5ms
static int g_device_id = 1;         // target hand device ID (--device-id)
static std::string g_can_if = "can0";
static std::string g_tcp_host = "192.168.0.178";
static uint16_t g_tcp_port = 8000;

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025CanfdTest";

static CanfdTransport ParseTransport(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  if (s == "zlgcan") return CanfdTransport::kZlgcan;
  if (s == "hcan") return CanfdTransport::kHcan;
  if (s == "socketcan") return CanfdTransport::kSocketCan;
  if (s == "zlgcantcp" || s == "zlg_tcp" || s == "zlgcan_tcp") return CanfdTransport::kZlgCanTcp;
  AgilinkLogger::get().warnf(TAG, "[Warning] unknown -t %s, using zlgcan", s.c_str());
  return CanfdTransport::kZlgcan;
}

class OmniHand2025CanfdTest : public ::testing::Test {
 protected:
  void SetUp() override {
    using agilink::omnihand::HandType;
    using agilink::omnihand::OmniHand2025;
    // Use device ID 0 (broadcast) for initial discovery so we find the hand
    // regardless of its current ID.
    constexpr uint8_t kBroadcastId = 0;

    switch (g_transport) {
      case CanfdTransport::kZlgcan:
        hand_ = OmniHand2025::createHandByZlgcan(
            HandType::LEFT, kBroadcastId,
            static_cast<uint8_t>(g_canfd_id),
            static_cast<uint8_t>(g_channel_id));
        break;
      case CanfdTransport::kHcan:
        hand_ = OmniHand2025::createHandByHcan(
            HandType::LEFT, kBroadcastId,
            static_cast<uint8_t>(g_canfd_id),
            static_cast<uint8_t>(g_channel_id));
        break;
      case CanfdTransport::kSocketCan:
#if defined(__linux__)
        hand_ = OmniHand2025::createHandSocketCan(HandType::LEFT, kBroadcastId, g_can_if);
#else
        AgilinkLogger::get().warnf(TAG, "[Warning] SocketCAN requires Linux; skipping hand creation.");
        hand_ = nullptr;
#endif
        break;
      case CanfdTransport::kZlgCanTcp:
#if OMNIHAND_ZLG_TCP_SUPPORTED
        hand_ = OmniHand2025::createHandByZlgCanTcp(
            HandType::LEFT, kBroadcastId, g_tcp_host, g_tcp_port,
            static_cast<uint8_t>(g_channel_id));
#else
        AgilinkLogger::get().warnf(TAG, "[Warning] ZLG CANFD over TCP not supported on this platform.");
        hand_ = nullptr;
#endif
        break;
      default:
        hand_ = nullptr;
        break;
    }

    if (hand_) {
      hand_->SetRequestInterval(g_request_interval);
      hand_->ShowDataDetails(true);
      device_available_ = hand_->Init();
      if (!device_available_) {
        AgilinkLogger::get().warnf(TAG, "[Warning]: CANFD device created but Init() failed.");
      }
    }
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << "CANFD device not available";
    }

    // Discover and cache the actual device ID via broadcast response.
    auto info = hand_->GetDeviceInfo();
    original_device_id_ = static_cast<uint8_t>(info.hand_device_id);
    AgilinkLogger::get().infof(TAG, "[SetUp] Discovered device ID: %d, switching to: %d",
                               original_device_id_, g_device_id);
    if (static_cast<uint8_t>(g_device_id) != original_device_id_) {
      hand_->SetDeviceId(static_cast<uint8_t>(g_device_id));
    }
  }

  void TearDown() override {
    if (hand_ && device_available_ && original_device_id_ != 0) {
      if (static_cast<uint8_t>(g_device_id) != original_device_id_) {
        AgilinkLogger::get().infof(TAG, "[TearDown] Restoring device ID: %d -> %d",
                                   g_device_id, original_device_id_);
        hand_->SetDeviceId(original_device_id_);
      }
    }
    hand_.reset();
  }

  void RequireDevice() {
    ASSERT_TRUE(device_available_);
  }

  std::unique_ptr<agilink::omnihand::OmniHand2025> hand_;
  bool device_available_ = false;
  uint8_t original_device_id_ = 0;
};

// ============================================================================
// Basic Connection Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, CreateHand) {
  ASSERT_TRUE(device_available_) << "CANFD hand object failed to initialize";
}

TEST_F(OmniHand2025CanfdTest, Init) {
  RequireDevice();
  EXPECT_TRUE(device_available_);
}

// ============================================================================
// Vendor Info
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetVendorInfo) {
  RequireDevice();

  auto vendor_info = hand_->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo] Vendor Info:\n%s", vendor_info.ToString().c_str());

  ASSERT_NE(vendor_info.dof, 0) << "GetVendorInfo timed out";
  EXPECT_EQ(vendor_info.dof, 10);
}

// ============================================================================
// Device ID Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetDeviceInfo) {
  RequireDevice();

  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] Device Info:\n%s", device_info.ToString().c_str());

  ASSERT_NE(device_info.hand_device_id, 0) << "GetDeviceInfo timed out";
  EXPECT_EQ(device_info.hand_device_id, static_cast<uint8_t>(g_device_id));
}

TEST_F(OmniHand2025CanfdTest, SetDeviceId) {
  RequireDevice();

  // SetUp already switched to g_device_id; just verify the physical ID is correct.
  auto info = hand_->GetDeviceInfo();
  ASSERT_NE(info.hand_device_id, 0) << "GetDeviceInfo timed out";
  EXPECT_EQ(info.hand_device_id, static_cast<uint8_t>(g_device_id));
  AgilinkLogger::get().infof(TAG, "[SetDeviceId] Current device ID: %d (target: %d)",
                             info.hand_device_id, g_device_id);
}

// ============================================================================
// Position Control Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetSingleAxisPos) {
  RequireDevice();

  // Position set A (different from SetGetAllAxisPos to show change)
  const int16_t safe_pos[10] = {1024, 1024, 2048, 1024, 2048, 2048, 1024, 2048, 1024, 2048};

  AgilinkLogger::get().infof(TAG, "[SetGetSingleAxisPos] Testing all 10 joints:");
  for (int joint = 1; joint <= 10; ++joint) {
    int16_t target_pos = safe_pos[joint - 1];
    auto set_result = hand_->SetJointMotorPosi(joint, target_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto pos = hand_->GetJointMotorPosi(joint);
    AgilinkLogger::get().infof(TAG, "  J%d: target_pos=%d, set_result=%d, get_pos=%d", joint, target_pos, set_result, pos);
    EXPECT_GE(pos, 0);
    EXPECT_LE(pos, 4096);
  }
}

TEST_F(OmniHand2025CanfdTest, SetGetAllAxisPos) {
  RequireDevice();

  // Safe positions from Python demo (not all-zero to avoid limit issues)
  std::vector<int16_t> positions = {2048, 2048, 4096, 0, 4096, 4096, 0, 4096, 0, 4096};

  // Test SetAllJointMotorPosi - returns actual positions
  auto set_result = hand_->SetAllJointMotorPosi(positions);
  {
    std::string msg = "[SetAllJointMotorPosi] returned " + std::to_string(set_result.size()) + " positions: ";
    for (size_t i = 0; i < set_result.size(); ++i) {
      msg += std::to_string(set_result[i]);
      if (i < set_result.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }
  EXPECT_EQ(set_result.size(), 10);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Test GetAllJointMotorPosi separately
  auto get_result = hand_->GetAllJointMotorPosi();
  {
    std::string msg = "[GetAllJointMotorPosi] returned " + std::to_string(get_result.size()) + " positions: ";
    for (size_t i = 0; i < get_result.size(); ++i) {
      msg += std::to_string(get_result[i]);
      if (i < get_result.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }
  EXPECT_EQ(get_result.size(), 10);
}

// ============================================================================
// Current/Force Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetAllCurrentReport) {
  RequireDevice();

  auto currents = hand_->GetAllCurrentReport();
  {
    std::string msg = "[GetAllCurrentReport] ";
    for (size_t i = 0; i < currents.size(); ++i) {
      msg += "J" + std::to_string(i+1) + ":" + std::to_string(currents[i]) + "mA";
      if (i < currents.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(currents.size(), 10u) << "GetAllCurrentReport timed out or returned an invalid size";
}

// ============================================================================
// Temperature Test
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetAllTemperatureReport) {
  RequireDevice();

  auto temps = hand_->GetAllTemperatureReport();
  {
    std::string msg = "[GetAllTemperatureReport] ";
    for (size_t i = 0; i < temps.size(); ++i) {
      msg += "J" + std::to_string(i+1) + ":" + std::to_string(static_cast<int>(temps[i]));
      if (i < temps.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(temps.size(), 10u) << "GetAllTemperatureReport timed out or returned an invalid size";

  // Temperature is int8_t (-128 to 127 degC), typical motor temp: 30-80 degC
  for (auto temp : temps) {
    EXPECT_GE(temp, -40);   // Extreme cold environment
    EXPECT_LE(temp, 127);   // int8_t max
  }
}

// ============================================================================
// Error Report Test
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetAllErrorReport) {
  RequireDevice();

  auto errors = hand_->GetAllErrorReport();
  AgilinkLogger::get().infof(TAG, "[GetAllErrorReport] S:stalled, H:overheat, C:over current, M:motor exception, X: communicate exception.");
  {
    std::string msg;
    for (size_t i = 0; i < errors.size(); ++i) {
      msg += "J" + std::to_string(i+1) + ":[";
      if (errors[i].bits.stalled_) msg += "S";
      if (errors[i].bits.overheat_) msg += "H";
      if (errors[i].bits.over_current_) msg += "C";
      if (errors[i].bits.motor_except_) msg += "M";
      if (errors[i].bits.commu_except_) msg += "X";
      msg += "]";
      if (i < errors.size() - 1) msg += " ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(errors.size(), 10u) << "GetAllErrorReport timed out or returned an invalid size";
}

// ============================================================================
// Tactile Sensor Tests (CANFD supports both normal and raw data)
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetTactileSensorData) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetTactileSensorData] Testing all tactile sensors:");

  // Test finger sensors (Thumb, Index, Middle, Ring, Little) - 16 values each
    std::vector<agilink::omnihand::Finger> fingers = {
    agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE
  };

  AgilinkLogger::get().infof(TAG, "  Fingers (16 values each):");
  for (auto finger : fingers) {
    auto data = hand_->GetTactileSensorData(finger);
    {
      std::string msg = "    " + std::string(agilink::omnihand::ToString(finger)) + ": ";
      for (size_t i = 0; i < data.size(); ++i) {
        msg += std::to_string(static_cast<int>(data[i]));
        if (i < data.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
    EXPECT_EQ(data.size(), 16);
  }

  // Test palm/dorsum sensors - 25 values each
  std::vector<agilink::omnihand::Finger> palm_dorsum = {agilink::omnihand::Finger::PALM, agilink::omnihand::Finger::DORSUM};

  AgilinkLogger::get().infof(TAG, "  Palm/Dorsum (25 values each):");
  for (auto sensor : palm_dorsum) {
    auto data = hand_->GetTactileSensorData(sensor);
    {
      std::string msg = "    " + std::string(agilink::omnihand::ToString(sensor)) + ": ";
      for (size_t i = 0; i < data.size(); ++i) {
        msg += std::to_string(static_cast<int>(data[i]));
        if (i < data.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
    EXPECT_EQ(data.size(), 25);
  }
}

TEST_F(OmniHand2025CanfdTest, GetTactileSensorDataRaw) {
  RequireDevice();

  // CANFD supports raw tactile sensor data (multi-frame protocol)
    auto data = hand_->GetTactileSensorDataRaw(agilink::omnihand::Finger::THUMB);

  if (data.data_.empty()) {
    AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] Not supported by this firmware");
    GTEST_SKIP() << "Raw tactile data not supported";
  }

  {
    std::string msg = "[GetTactileSensorDataRaw] Thumb (" + std::to_string(data.data_.size()) + " values): ";
    for (size_t i = 0; i < std::min(data.data_.size(), size_t(10)); ++i) {
      msg += std::to_string(static_cast<int>(data.data_[i]));
      if (i < std::min(data.data_.size(), size_t(10)) - 1) msg += ", ";
    }
    if (data.data_.size() > 10) msg += " ...";
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  EXPECT_GT(data.data_.size(), 0);
}

TEST_F(OmniHand2025CanfdTest, GetAllTactileSensorDataRaw) {
  RequireDevice();

  auto all_data = hand_->GetAllTactileSensorDataRaw();

  if (all_data.empty()) {
    AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] Not supported by this firmware");
    GTEST_SKIP() << "Raw tactile data not supported";
  }

  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] %zu sensors", all_data.size());
  for (const auto& sensor : all_data) {
    {
      std::string msg = "  " + std::string(agilink::omnihand::ToString(sensor.sensor_id_)) + " (" + std::to_string(sensor.data_.size()) + " values): ";
      for (size_t i = 0; i < sensor.data_.size(); ++i) {
        msg += std::to_string(static_cast<int>(sensor.data_[i]));
        if (i < sensor.data_.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
  }

  ASSERT_FALSE(all_data.empty());
}

TEST_F(OmniHand2025CanfdTest, GetNumOfTactileSensors) {
  RequireDevice();

  size_t num = hand_->GetNumOfTactileSensors();
  AgilinkLogger::get().infof(TAG, "[GetNumOfTactileSensors] %zu", num);
  EXPECT_GT(num, 0u);
}

TEST_F(OmniHand2025CanfdTest, GetNumOfTactilePoints) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    size_t pts = hand_->GetNumOfTactilePoints(finger);
    AgilinkLogger::get().infof(TAG, "[GetNumOfTactilePoints] %s: %zu points", agilink::omnihand::ToString(finger), pts);
    EXPECT_GT(pts, 0u) << "Expected >0 points for " << agilink::omnihand::ToString(finger);
  }
  EXPECT_EQ(hand_->GetNumOfTactilePoints(agilink::omnihand::Finger::UNKNOWN), 0u);
}

TEST_F(OmniHand2025CanfdTest, GetLenOfTactileDatum) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    size_t len = hand_->GetLenOfTactileDatum(finger);
    AgilinkLogger::get().infof(TAG, "[GetLenOfTactileDatum] %s: %zu bytes/point", agilink::omnihand::ToString(finger), len);
    EXPECT_GT(len, 0u) << "Expected >0 bytes per datum for " << agilink::omnihand::ToString(finger);
  }
  EXPECT_EQ(hand_->GetLenOfTactileDatum(agilink::omnihand::Finger::UNKNOWN), 0u);
}

TEST_F(OmniHand2025CanfdTest, GetNumOfRepliedTactileFrames) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    if (finger == agilink::omnihand::Finger::DORSUM) continue;
    size_t frames = hand_->GetNumOfRepliedTactileFrames(finger);
    AgilinkLogger::get().infof(TAG, "[GetNumOfRepliedTactileFrames] %s: %zu frame(s)", agilink::omnihand::ToString(finger), frames);
    EXPECT_GT(frames, 0u) << "Expected >=1 frame for " << agilink::omnihand::ToString(finger);
  }
  EXPECT_EQ(hand_->GetNumOfRepliedTactileFrames(agilink::omnihand::Finger::DORSUM), 0u);
  EXPECT_EQ(hand_->GetNumOfRepliedTactileFrames(agilink::omnihand::Finger::UNKNOWN), 0u);
}

TEST_F(OmniHand2025CanfdTest, GetSNOfTactileSensor) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    if (finger == agilink::omnihand::Finger::DORSUM) continue;
    std::string sn = hand_->GetSNOfTactileSensor(finger);
    AgilinkLogger::get().infof(TAG, "[GetSNOfTactileSensor] %s: \"%s\"", agilink::omnihand::ToString(finger), sn.c_str());
  }
  EXPECT_EQ(hand_->GetSNOfTactileSensor(agilink::omnihand::Finger::DORSUM), "");
  EXPECT_EQ(hand_->GetSNOfTactileSensor(agilink::omnihand::Finger::UNKNOWN), "");
}

// ============================================================================
// Motor Velocity Control Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetAllJointMotorVelo) {
  RequireDevice();

  auto current_velo = hand_->GetAllJointMotorVelo();
  {
    std::string msg = "[GetAllJointMotorVelo] ";
    for (size_t i = 0; i < current_velo.size(); ++i) {
      msg += std::to_string(current_velo[i]);
      if (i < current_velo.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(current_velo.size(), 10u) << "GetAllJointMotorVelo timed out or returned an invalid size";
}

// ============================================================================
// Current Threshold Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetAllCurrentThreshold) {
  RequireDevice();

  // std::vector<int16_t> thresholds(10, 1500);
  // hand_->SetAllCurrentThreshold(thresholds);
  // AgilinkLogger::get().infof(TAG, "[SetAllCurrentThreshold] All joints -> 1000mA");

  auto current_thresholds = hand_->GetAllCurrentThreshold();
  {
    std::string msg = "[GetAllCurrentThreshold] ";
    for (size_t i = 0; i < current_thresholds.size(); ++i) {
      msg += std::to_string(current_thresholds[i]);
      if (i < current_thresholds.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(current_thresholds.size(), 10u)
      << "GetAllCurrentThreshold timed out or returned an invalid size";
}

// ============================================================================
// Mixed Control Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, MixControlByPVT) {
  RequireDevice();

  // Safe positions from Python demo (per joint)
  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> velocities(10, 8000);
  std::vector<int16_t> torques(10, 300);

  auto result = hand_->MixControlByPVT(positions, velocities, torques);
  ASSERT_EQ(result.size(), positions.size())
      << "MixControlByPVT returned an unexpected number of joint results";
  AgilinkLogger::get().infof(TAG, "[MixControlByPVT] all 10 joints");

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto feedback_pos = hand_->GetAllJointMotorPosi();
  {
    std::string msg = "[GetAllJointMotorPosi] ";
    for (size_t i = 0; i < feedback_pos.size(); ++i) {
      msg += std::to_string(feedback_pos[i]);
      if (i < feedback_pos.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  EXPECT_EQ(feedback_pos.size(), 10);

  auto single_result = hand_->MixControlByPVT(1, safe_pos[0], 50, 0);
  ASSERT_TRUE(single_result.tgt_posi_.has_value());
  ASSERT_TRUE(single_result.tgt_velo_.has_value());
  ASSERT_TRUE(single_result.tgt_torque_.has_value());
  // O10 CAN replies expose the protocol joint number (1-based) after parsing.
  EXPECT_EQ(single_result.joint_index_, 1);
  AgilinkLogger::get().infof(TAG, "[MixControlByPVT] single joint 1 pos=%d vel=50", safe_pos[0]);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

TEST_F(OmniHand2025CanfdTest, GetJointMotorVelo) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetJointMotorVelo] per joint:");
  for (int joint = 1; joint <= 10; ++joint) {
    auto velo = hand_->GetJointMotorVelo(static_cast<unsigned char>(joint));
    AgilinkLogger::get().infof(TAG, "  J%d: velo=%d", joint, velo);
  }

  EXPECT_EQ(hand_->GetJointMotorVelo(0), -1);
  EXPECT_EQ(hand_->GetJointMotorVelo(11), -1);
}

TEST_F(OmniHand2025CanfdTest, MixControlByPT) {
  RequireDevice();

  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> torques(10, 0);

  auto result = hand_->MixControlByPT(positions, torques);
  ASSERT_EQ(result.size(), positions.size())
      << "MixControlByPT returned an unexpected number of joint results";
  AgilinkLogger::get().infof(TAG, "[MixControlByPT] all 10 joints");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  auto single_result = hand_->MixControlByPT(1, safe_pos[0], 0);
  ASSERT_TRUE(single_result.tgt_posi_.has_value());
  ASSERT_TRUE(single_result.tgt_torque_.has_value());
  // O10 CAN replies expose the protocol joint number (1-based) after parsing.
  EXPECT_EQ(single_result.joint_index_, 1);
  AgilinkLogger::get().infof(TAG, "[MixControlByPT] single joint 1 pos=%d", safe_pos[0]);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

}

// ============================================================================
// Joint Angle Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetAllActiveJointAngles) {
  RequireDevice();

  std::vector<double> angles(10, 0.0);
  hand_->SetAllActiveJointAngles(angles);
  AgilinkLogger::get().infof(TAG, "[SetAllActiveJointAngles] All joints -> 0.0 rad");

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto current_angles = hand_->GetAllActiveJointAngles();
  {
    std::string msg = "[GetAllActiveJointAngles] ";
    for (size_t i = 0; i < current_angles.size(); ++i) {
      char buf[24]; snprintf(buf, sizeof(buf), "%.4f", current_angles[i]); msg += buf;
      if (i < current_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(current_angles.size(), 10u)
      << "GetAllActiveJointAngles timed out or returned an invalid size";
}

TEST_F(OmniHand2025CanfdTest, GetAllJointAngles) {
  RequireDevice();

  auto all_angles = hand_->GetAllJointAngles();
  {
    std::string msg = "[GetAllJointAngles] (" + std::to_string(all_angles.size()) + " joints): ";
    for (size_t i = 0; i < all_angles.size(); ++i) {
      char buf[24]; snprintf(buf, sizeof(buf), "%.4f", all_angles[i]); msg += buf;
      if (i < all_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(all_angles.size(), 16u) << "GetAllJointAngles timed out or returned an invalid size";
}

// ============================================================================
// Kinematics Solver Test
// ============================================================================

TEST_F(OmniHand2025CanfdTest, KinematicsSolver) {
  RequireDevice();

  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointAngles(active_angles);

  AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] Forward kinematics: %zu angles", all_angles.size());

  EXPECT_EQ(all_angles.size(), 16);
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char** argv) {
  std::vector<char*> gtest_args;
  gtest_args.push_back(argv[0]);

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

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
    } else if ((arg == "--can-if") && i + 1 < argc) {
      g_can_if = argv[++i];
    } else if ((arg == "--tcp-host") && i + 1 < argc) {
      g_tcp_host = argv[++i];
    } else if ((arg == "--tcp-port") && i + 1 < argc) {
      g_tcp_port = static_cast<uint16_t>(std::stoi(argv[++i]));
    } else if (arg == "--help" || arg == "-h") {
      AgilinkLogger::get().infof(TAG, "OmniHand 2025 CANFD Test\n");
      AgilinkLogger::get().infof(TAG, "Usage: %s [options]\n", argv[0]);
      AgilinkLogger::get().infof(TAG, "Options:");
      AgilinkLogger::get().infof(TAG, "  -d, --device NAME    zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)");
      AgilinkLogger::get().infof(TAG, "  --device-id ID       target hand device ID (default: 1); SetUp discovers & switches, TearDown restores");
      AgilinkLogger::get().infof(TAG, "  -c CHANNEL             CAN channel (zlgcan/hcan/zlgcantcp), default 0");
      AgilinkLogger::get().infof(TAG, "  -i CANFD_ID             device index (zlgcan/hcan), default 0");
      AgilinkLogger::get().infof(TAG, "  --can-if IFACE         SocketCAN iface (socketcan), default can0");
      AgilinkLogger::get().infof(TAG, "  --tcp-host HOST        ZLG TCP host (zlgcantcp), default 192.168.0.178");
      AgilinkLogger::get().infof(TAG, "  --tcp-port PORT        ZLG TCP port (zlgcantcp), default 8000");
      AgilinkLogger::get().infof(TAG, "  -f INTERVAL            Request interval ms, default 5, max 100");
      AgilinkLogger::get().infof(TAG, "\nExamples:");
      AgilinkLogger::get().infof(TAG, "  %s -d zlgcan -c 0 -i 0", argv[0]);
      AgilinkLogger::get().infof(TAG, "  %s -d socketcan --can-if can0", argv[0]);
      AgilinkLogger::get().infof(TAG, "  %s -d zlgcantcp --tcp-host 192.168.0.178 --tcp-port 8000", argv[0]);
      return 0;
    } else {
      gtest_args.push_back(argv[i]);
    }
  }

  AgilinkLogger::get().infof(TAG, "=== OmniHand 2025 CANFD Test ===");
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
  AgilinkLogger::get().infof(TAG, "Target Device ID: %d", g_device_id);
  AgilinkLogger::get().infof(TAG, "Request Interval: %d ms", g_request_interval);
  AgilinkLogger::get().infof(TAG, "================================");

  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
