// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file test_omnihand_2025_usb.cc
 * @brief USB-specific tests for OmniHand 2025
 *
 * This test covers all USB protocol commands:
 * - SET_ID (0x04) - SetDeviceId
 * - SET_SINGLE_AXIS_POS (0x06) - SetJointMotorPosi
 * - GET_SINGLE_AXIS_POS (0x07) - GetJointMotorPosi
 * - SET_ALL_AXIS_POS (0x08) - SetAllJointMotorPosi
 * - GET_ALL_AXIS_POS (0x09) - GetAllJointMotorPosi
 * - GET_SINGLE_AXIS_FORCE (0x0A) - GetAllCurrentReport
 * - GET_ALL_AXIS_FORCE (0x0B) - GetAllCurrentReport
 * - GET_ALL_AXIS_TEMP (0x0C) - GetAllTemperatureReport
 * - GET_ERROR_CODE (0x0D) - GetErrorReport
 * - GET_FINGERTIP_SENSOR_DATA (0x11) - GetTactileSensorData
 * - SET_PROTECTIVE_CURRENT (0x25) - SetAllCurrentThreshold
 * - GET_ALL_AXIS_CVP (0x29) - (CVP data)
 * - SET_POS_SPEED_CUR_DATA (0x32) - MixControlByPT / MixControlByPVT
 * - GET_FW_VERSION (0xCD) - GetVendorInfo
 *
 * Usage:
 *   ./test_omnihand_2025_usb [-p PORT] [-b BAUDRATE] [-f INTERVAL]
 *
 *   Options:
 *     -p PORT      USB serial port (default: Windows COM3, Linux /dev/ttyACM0)
 *     -b BAUDRATE  Baudrate (default: 460800)
 *     -f INTERVAL  Request interval in ms (default: 500, max: 500)
 */

#include <gtest/gtest.h>
#include "omnihand/omnihand_2025.h"
#include <memory>
#include <vector>
#include "agilink_logger.h"
#include <cstdio>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>

// Global configuration (default port: Windows COM3, Linux /dev/ttyACM0)
#if defined(_WIN32)
static std::string g_usb_port = "COM3";
#else
static std::string g_usb_port = "/dev/ttyACM0";
#endif
static int g_baudrate = 460800;
static int g_request_interval = 500;  // USB default: 500ms
static int g_frame_recv_timeout = 200;
static bool g_run_dangerous_actions = false;

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025UsbTest";

TEST(OmniHand2025UsbBroadcastTest, GetDeviceInfoFromBroadcastBySerial) {
  const auto info = agilink::omnihand::OmniHand2025::GetDeviceInfoFromBroadcastBySerial(
      g_usb_port, g_baudrate);
  if (info.hand_device_id == 0) {
    GTEST_SKIP() << "No standard-protocol serial device responded to broadcast";
  }
  EXPECT_NE(info.hand_device_id, 0);
}

class OmniHand2025UsbTest : public ::testing::Test {
 protected:
  void SetUp() override {
    try {
      auto hand = agilink::omnihand::OmniHand2025::createHandByUsb(
          agilink::omnihand::HandType::LEFT,
          1,              // device_id
          g_usb_port,
          g_baudrate
      );
      hand_ = std::move(hand);

      if (hand_) {
        hand_->SetRequestInterval(g_request_interval);
        hand_->SetFrameRecvTimeout(g_frame_recv_timeout);
        device_available_ = hand_->Init();
        if (!device_available_) {
          AgilinkLogger::get().warnf(TAG, "[Warning]: USB device created but Init() failed.");
        }
        hand_->ShowDataDetails(true);  // default: stdout
      }
    } catch (const std::exception& e) {
      AgilinkLogger::get().warnf(TAG, "[Warning]: Failed to open USB port: %s", e.what());
      hand_.reset();
      device_available_ = false;
    }
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << "USB device not available";
    }
  }

  void TearDown() override {
    hand_.reset();
    AgilinkLogger::get().flush();
  }

  void RequireDevice() {
    ASSERT_TRUE(device_available_);
  }

  std::unique_ptr<agilink::omnihand::OmniHand2025> hand_;
  bool device_available_ = false;
};

// ============================================================================
// Basic Connection Tests
// ============================================================================

TEST_F(OmniHand2025UsbTest, CreateHand) {
  RequireDevice();
  ASSERT_TRUE(device_available_) << "USB hand object failed to initialize";
}

TEST_F(OmniHand2025UsbTest, Init) {
  RequireDevice();
  EXPECT_TRUE(device_available_);
}

// ============================================================================
// GET_FW_VERSION (0xCD) - GetVendorInfo
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetVendorInfo) {
  RequireDevice();

  auto vendor_info = hand_->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo] Vendor Info:\n%s", vendor_info.ToString().c_str());

  ASSERT_NE(vendor_info.dof, 0) << "GetVendorInfo timed out";
  EXPECT_EQ(vendor_info.dof, 10);
  EXPECT_FALSE(vendor_info.productModel.empty());
}

// ============================================================================
// Device ID Tests (SET_ID 0x04)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetDeviceInfo) {
  RequireDevice();

  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] Device Info:\n%s", device_info.ToString().c_str());

  // USB learns device ID from first received frame
  EXPECT_NE(device_info.hand_device_id, 0);
}

// ============================================================================
// Position Control Tests
// SET_SINGLE_AXIS_POS (0x06), GET_SINGLE_AXIS_POS (0x07)
// SET_ALL_AXIS_POS (0x08), GET_ALL_AXIS_POS (0x09)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetGetSingleAxisPos) {
  RequireDevice();

  // Position set A (different from SetGetAllAxisPos to show change)
  const int16_t safe_pos[10] = {1024, 1024, 2048, 1024, 2048, 2048, 1024, 2048, 1024, 2048};

  AgilinkLogger::get().infof(TAG, "[SetGetSingleAxisPos] Testing all 10 joints:");
  for (int joint = 1; joint <= 10; ++joint) {
    int16_t target_pos = safe_pos[joint - 1];
    hand_->SetJointMotorPosi(joint, target_pos);
    auto pos = hand_->GetJointMotorPosi(joint);
    AgilinkLogger::get().infof(TAG, "  J%d: set=%d, get=%d", joint, target_pos, pos);
    EXPECT_GE(pos, 0);
    EXPECT_LE(pos, 4096);
  }
}

TEST_F(OmniHand2025UsbTest, SetGetAllAxisPos) {
  RequireDevice();

  // Safe positions from Python demo (not all-zero to avoid limit issues)
  std::vector<int16_t> positions = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};

  // Test SetAllJointMotorPosi - returns actual positions
  auto set_result = hand_->SetAllJointMotorPosi(positions);
  {
    std::string msg;
    for (size_t i = 0; i < set_result.size(); ++i) {
      msg += std::to_string(set_result[i]);
      if (i < set_result.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[SetAllJointMotorPosi] returned %zu positions: %s", set_result.size(), msg.c_str());
  }
  EXPECT_EQ(set_result.size(), 10);

  // Test GetAllJointMotorPosi separately
  auto get_result = hand_->GetAllJointMotorPosi();
  {
    std::string msg;
    for (size_t i = 0; i < get_result.size(); ++i) {
      msg += std::to_string(get_result[i]);
      if (i < get_result.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorPosi] returned %zu positions: %s", get_result.size(), msg.c_str());
  }
  EXPECT_EQ(get_result.size(), 10);
}

// ============================================================================
// Current/Force Tests
// GET_SINGLE_AXIS_FORCE (0x0A), GET_ALL_AXIS_FORCE (0x0B)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetAllCurrentReport) {
  RequireDevice();

  auto currents = hand_->GetAllCurrentReport();
  {
    std::string msg;
    for (size_t i = 0; i < currents.size(); ++i) {
      msg += "J" + std::to_string(i+1) + ":" + std::to_string(currents[i]) + "mA";
      if (i < currents.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllCurrentReport] %s", msg.c_str());
  }

  ASSERT_EQ(currents.size(), 10u) << "GetAllCurrentReport timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetCurrentReport) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetCurrentReport] All joints:");
  for (int i = 1; i <= 10; ++i) {
    auto current = hand_->GetCurrentReport(static_cast<unsigned char>(i));
    AgilinkLogger::get().infof(TAG, "  J%d: %dmA", i, current);
  }
}

// ============================================================================
// Temperature Test (GET_ALL_AXIS_TEMP 0x0C)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetAllTemperatureReport) {
  RequireDevice();

  auto temps = hand_->GetAllTemperatureReport();
  {
    std::string msg;
    for (size_t i = 0; i < temps.size(); ++i) {
      // Use ASCII "degC" — Unicode degree sign breaks on Windows consoles (GBK shows as garbled).
      msg += "J" + std::to_string(i+1) + ":" + std::to_string(static_cast<int>(temps[i])) + " degC";
      if (i < temps.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllTemperatureReport] %s", msg.c_str());
  }

  ASSERT_EQ(temps.size(), 10u) << "GetAllTemperatureReport timed out or returned an invalid size";

  // Temperature is int8_t (-128 to 127 degC), typical motor temp: 30-80 degC
  for (auto temp : temps) {
    EXPECT_GE(temp, -40);   // Extreme cold environment
    EXPECT_LE(temp, 127);   // int8_t max
  }
}

TEST_F(OmniHand2025UsbTest, GetTemperatureReport) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetTemperatureReport] All joints:");
  for (int i = 1; i <= 10; ++i) {
    auto temp = hand_->GetTemperatureReport(static_cast<unsigned char>(i));
    AgilinkLogger::get().infof(TAG, "  J%d: %d degC", i, static_cast<int>(temp));
    EXPECT_GE(temp, -40);
    EXPECT_LE(temp, 127);
  }
}

// ============================================================================
// Error Report Test (GET_ERROR_CODE 0x0D)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetAllErrorReport) {
  RequireDevice();

  auto errors = hand_->GetAllErrorReport();
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
    AgilinkLogger::get().infof(TAG, "[GetAllErrorReport] %s", msg.c_str());
  }

  ASSERT_EQ(errors.size(), 10u) << "GetAllErrorReport timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetErrorReport) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetErrorReport] All joints:");
  for (int i = 1; i <= 10; ++i) {
    auto err = hand_->GetErrorReport(static_cast<unsigned char>(i));
    AgilinkLogger::get().infof(TAG, "  J%d: [%s%s%s%s%s]", i,
        err.bits.stalled_ ? "S" : "",
        err.bits.overheat_ ? "H" : "",
        err.bits.over_current_ ? "C" : "",
        err.bits.motor_except_ ? "M" : "",
        err.bits.commu_except_ ? "X" : "");
  }
}

// ============================================================================
// Tactile Sensor Test (GET_FINGERTIP_SENSOR_DATA 0x11)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetTactileSensorData) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetTactileSensorData] Testing all tactile sensors:");

  // Test finger sensors (Thumb, Index, Middle, Ring, Little) - 16 values each
  std::vector<agilink::omnihand::Finger> fingers = {
    agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE,
    agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE
  };

  AgilinkLogger::get().infof(TAG, "  Fingers (16 values each):");
  for (auto finger : fingers) {
    auto data = hand_->GetTactileSensorData(finger);
    std::string msg;
    for (size_t i = 0; i < data.size(); ++i) {
      msg += std::to_string(static_cast<int>(data[i]));
      if (i < data.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "    %s: %s", agilink::omnihand::ToString(finger).c_str(), msg.c_str());
    EXPECT_EQ(data.size(), 16);
  }

  // Test palm/dorsum sensors - 25 values each
  std::vector<agilink::omnihand::Finger> palm_dorsum = {agilink::omnihand::Finger::PALM, agilink::omnihand::Finger::DORSUM};

  AgilinkLogger::get().infof(TAG, "  Palm/Dorsum (25 values each):");
  for (auto sensor : palm_dorsum) {
    auto data = hand_->GetTactileSensorData(sensor);
    std::string msg;
    for (size_t i = 0; i < data.size(); ++i) {
      msg += std::to_string(static_cast<int>(data[i]));
      if (i < data.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "    %s: %s", agilink::omnihand::ToString(sensor).c_str(), msg.c_str());
    EXPECT_EQ(data.size(), 25);
  }
}

TEST_F(OmniHand2025UsbTest, GetAllTactileSensorData) {
  RequireDevice();

  auto all_data = hand_->GetAllTactileSensorData();
  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorData] %zu sensors:", all_data.size());
  for (const auto& d : all_data) {
    AgilinkLogger::get().infof(TAG, "  %s: %zu bytes",
        agilink::omnihand::ToString(d.sensor_id_).c_str(), d.data_.size());
  }
  EXPECT_EQ(all_data.size(), 7u);
  for (const auto& d : all_data) {
    EXPECT_FALSE(d.data_.empty());
  }
}

TEST_F(OmniHand2025UsbTest, GetTactileSensorDataRaw) {
  RequireDevice();

  std::vector<agilink::omnihand::Finger> sensors = {
    agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX,
    agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING,
    agilink::omnihand::Finger::LITTLE, agilink::omnihand::Finger::PALM,
    agilink::omnihand::Finger::DORSUM
  };

  bool any_supported = false;
  for (auto finger : sensors) {
    auto raw = hand_->GetTactileSensorDataRaw(finger);
    if (raw.data_.empty()) {
      AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] %s (%zu bytes): (empty)",
          agilink::omnihand::ToString(finger).c_str(), raw.data_.size());
      continue;
    }
    any_supported = true;
    {
      std::string msg;
      for (size_t i = 0; i < raw.data_.size(); ++i) {
        msg += std::to_string(static_cast<int>(raw.data_[i]));
        if (i < raw.data_.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "[GetTactileSensorDataRaw] %s (%zu bytes): %s",
          agilink::omnihand::ToString(finger).c_str(), raw.data_.size(), msg.c_str());
    }
  }
  if (!any_supported) GTEST_SKIP() << "GetTactileSensorDataRaw not supported on this device";
}

TEST_F(OmniHand2025UsbTest, GetAllTactileSensorDataRaw) {
  RequireDevice();

  auto all_raw = hand_->GetAllTactileSensorDataRaw();
  if (all_raw.empty()) GTEST_SKIP() << "GetAllTactileSensorDataRaw not supported on this device";

  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] %zu sensors:", all_raw.size());
  for (const auto& d : all_raw) {
    std::string msg;
    for (size_t i = 0; i < d.data_.size(); ++i) {
      msg += std::to_string(static_cast<int>(d.data_[i]));
      if (i < d.data_.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  %s (%zu bytes): %s",
        agilink::omnihand::ToString(d.sensor_id_).c_str(), d.data_.size(), msg.c_str());
  }
  EXPECT_EQ(all_raw.size(), 7u);
}

TEST_F(OmniHand2025UsbTest, GetNumOfTactileSensors) {
  RequireDevice();

  size_t num = hand_->GetNumOfTactileSensors();
  AgilinkLogger::get().infof(TAG, "[GetNumOfTactileSensors] %zu", num);
  EXPECT_GT(num, 0u);
}

TEST_F(OmniHand2025UsbTest, GetNumOfTactilePoints) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    size_t pts = hand_->GetNumOfTactilePoints(finger);
    AgilinkLogger::get().infof(TAG, "[GetNumOfTactilePoints] %s: %zu points",
        agilink::omnihand::ToString(finger).c_str(), pts);
    EXPECT_GT(pts, 0u) << "Expected >0 points for " << agilink::omnihand::ToString(finger);
  }
  EXPECT_THROW(hand_->GetNumOfTactilePoints(agilink::omnihand::Finger::UNKNOWN),
               std::invalid_argument);
}

TEST_F(OmniHand2025UsbTest, GetLenOfTactileDatum) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    size_t len = hand_->GetLenOfTactileDatum(finger);
    AgilinkLogger::get().infof(TAG, "[GetLenOfTactileDatum] %s: %zu bytes/point",
        agilink::omnihand::ToString(finger).c_str(), len);
    EXPECT_GT(len, 0u) << "Expected >0 bytes per datum for " << agilink::omnihand::ToString(finger);
  }
  EXPECT_THROW(hand_->GetLenOfTactileDatum(agilink::omnihand::Finger::UNKNOWN),
               std::invalid_argument);
}

TEST_F(OmniHand2025UsbTest, GetNumOfRepliedTactileFrames) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    size_t frames = hand_->GetNumOfRepliedTactileFrames(finger);
    AgilinkLogger::get().infof(TAG, "[GetNumOfRepliedTactileFrames] %s: %zu frame(s)",
        agilink::omnihand::ToString(finger).c_str(), frames);
    EXPECT_GT(frames, 0u) << "Expected >=1 frame for " << agilink::omnihand::ToString(finger);
  }
  EXPECT_THROW(hand_->GetNumOfRepliedTactileFrames(agilink::omnihand::Finger::UNKNOWN),
               std::invalid_argument);
}

TEST_F(OmniHand2025UsbTest, GetSNOfTactileSensor) {
  RequireDevice();

  for (auto finger : hand_->GetSensorOrder()) {
    std::string sn = hand_->GetSNOfTactileSensor(finger);
    AgilinkLogger::get().infof(TAG, "[GetSNOfTactileSensor] %s: \"%s\"",
        agilink::omnihand::ToString(finger).c_str(), sn.c_str());
  }
  EXPECT_THROW(hand_->GetSNOfTactileSensor(agilink::omnihand::Finger::UNKNOWN),
               std::invalid_argument);
}

// ============================================================================
// Current Threshold Test (SET_PROTECTIVE_CURRENT 0x25)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetAllCurrentThreshold) {
  RequireDevice();

  // std::vector<int16_t> thresholds(10, 1500);  // 1500mA
  // hand_->SetAllCurrentThreshold(thresholds);
  // AgilinkLogger::get().infof(TAG, "[SetAllCurrentThreshold] All joints -> 1000mA");

  // Note: GetAllCurrentThreshold returns cached values for USB (no GET command)
  auto current_thresholds = hand_->GetAllCurrentThreshold();
  {
    std::string msg;
    for (size_t i = 0; i < current_thresholds.size(); ++i) {
      msg += std::to_string(current_thresholds[i]);
      if (i < current_thresholds.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllCurrentThreshold] (cached) %s", msg.c_str());
  }

  EXPECT_EQ(current_thresholds.size(), 10);
}

// ============================================================================
// Mixed Control Test (SET_POS_SPEED_CUR_DATA 0x32)
// ============================================================================

TEST_F(OmniHand2025UsbTest, MixControlByPVT) {
  RequireDevice();

  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> velocities(10, 50);
  std::vector<int16_t> torques(10, 0);

  (void)hand_->MixControlByPVT(positions, velocities, torques);
  AgilinkLogger::get().infof(TAG, "[MixControlByPVT] all 10 joints");

  auto feedback_pos = hand_->GetAllJointMotorPosi();
  {
    std::string msg;
    for (size_t i = 0; i < feedback_pos.size(); ++i) {
      msg += std::to_string(feedback_pos[i]);
      if (i < feedback_pos.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorPosi] After mixed control: %s", msg.c_str());
  }

  EXPECT_EQ(feedback_pos.size(), 10);
}

TEST_F(OmniHand2025UsbTest, GetAllJointMotorVelo) {
  RequireDevice();

  auto current_velo = hand_->GetAllJointMotorVelo();
  {
    std::string msg;
    for (size_t i = 0; i < current_velo.size(); ++i) {
      msg += std::to_string(current_velo[i]);
      if (i < current_velo.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointMotorVelo] %s", msg.c_str());
  }

  ASSERT_EQ(current_velo.size(), 10u) << "GetAllJointMotorVelo timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetJointMotorVelo) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetJointMotorVelo] All joints:");
  for (int i = 1; i <= 10; ++i) {
    auto velo = hand_->GetJointMotorVelo(static_cast<unsigned char>(i));
    AgilinkLogger::get().infof(TAG, "  J%d: %d", i, velo);
  }
}

TEST_F(OmniHand2025UsbTest, MixControlByPT) {
  RequireDevice();

  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> torques(10, 0);

  auto result = hand_->MixControlByPT(positions, torques);
  ASSERT_EQ(result.size(), positions.size())
      << "MixControlByPT returned an unexpected number of joint results";
  ASSERT_TRUE(result.front().tgt_posi_.has_value());
  ASSERT_TRUE(result.front().tgt_torque_.has_value());
  AgilinkLogger::get().infof(TAG, "[MixControlByPT] all 10 joints");
}

// ============================================================================
// Joint Angle Tests (uses position commands internally)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetGetAllActiveJointAngles) {
  RequireDevice();

  std::vector<double> angles(10, 0.0);
  hand_->SetAllActiveJointAngles(angles);
  AgilinkLogger::get().infof(TAG, "[SetAllActiveJointAngles] All joints -> 0.0 rad");

  auto current_angles = hand_->GetAllActiveJointAngles();
  {
    std::string msg;
    for (size_t i = 0; i < current_angles.size(); ++i) {
      char buf[24]; snprintf(buf, sizeof(buf), "%.4f", current_angles[i]);
      msg += buf;
      if (i < current_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllActiveJointAngles] %s", msg.c_str());
  }

  ASSERT_EQ(current_angles.size(), 10u)
      << "GetAllActiveJointAngles timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetAllJointAngles) {
  RequireDevice();

  auto all_angles = hand_->GetAllJointAngles();
  {
    std::string msg;
    for (size_t i = 0; i < all_angles.size(); ++i) {
      char buf[24]; snprintf(buf, sizeof(buf), "%.4f", all_angles[i]);
      msg += buf;
      if (i < all_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] (%zu joints): %s", all_angles.size(), msg.c_str());
  }

  ASSERT_EQ(all_angles.size(), 16u) << "GetAllJointAngles timed out or returned an invalid size";  // 10 active + 6 passive
}

// ============================================================================
// Kinematics Solver Test (software-only, no USB command)
// ============================================================================

TEST_F(OmniHand2025UsbTest, KinematicsSolver) {
  RequireDevice();

  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointAngles(active_angles);

  {
    std::string msg;
    for (size_t i = 0; i < all_angles.size(); ++i) {
      char buf[24]; snprintf(buf, sizeof(buf), "%.4f", all_angles[i]);
      msg += buf;
      if (i < all_angles.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] Forward kinematics: %s", msg.c_str());
  }

  EXPECT_EQ(all_angles.size(), 16);
}

TEST_F(OmniHand2025UsbTest, DiscoverHandDeviceId) {
  RequireDevice();

  const uint16_t device_id = hand_->GetHandDeviceIdByBroadcast();
  AgilinkLogger::get().infof(
      TAG, "[PrivateProtocolDiscoverDeviceId] device ID: %u",
      static_cast<unsigned int>(device_id));
  EXPECT_EQ(hand_->GetHandDeviceId(), device_id);
}

// ============================================================================
// StreamCmd Tests - Split by functionality
// ============================================================================

TEST_F(OmniHand2025UsbTest, PrivateProtocolDiscoverDeviceId) {
  RequireDevice();

  const uint16_t private_device_id = hand_->GetPrivateHandDeviceIdByBroadcast();
  AgilinkLogger::get().infof(
      TAG, "[PrivateProtocolDiscoverDeviceId] device ID: %u",
      static_cast<unsigned int>(private_device_id));

  ASSERT_GT(private_device_id, 0u) << "Private-protocol broadcast returned an invalid device ID";
  ASSERT_LT(private_device_id, agilink::omnihand::kPrivateBroadcastHandDeviceId)
      << "No private-protocol device responded to broadcast";
  EXPECT_EQ(hand_->GetPrivateHandDeviceId(), private_device_id);
}

// 0x01/0x02: Power state
TEST_F(OmniHand2025UsbTest, StreamCmdPowerState) {
  RequireDevice();

  EXPECT_EQ(hand_->GetRequestInterval(), g_request_interval);
  EXPECT_EQ(hand_->GetFrameRecvTimeout(), g_frame_recv_timeout);

  // 0x01/0x02: power state
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing power state commands(0x01/0x02):");
  EXPECT_TRUE(hand_->SetPowerState(1));
  EXPECT_LE(hand_->GetPowerState(), 2u);

  // 0x03/0x04/0x05: SetAxisHoming + SetId + SaveParam, too dangerous
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping axis homing and ID commands (0x03/0x04/0x05) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetAxisHoming(0, 0));
  // EXPECT_TRUE(hand_->SetId(0));
  // EXPECT_TRUE(hand_->SaveParam()); // no work
}

// 0x06/0x07: Single axis position
TEST_F(OmniHand2025UsbTest, StreamCmdSingleAxisPos) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing single axis pos commands(0x06/0x07):");
  for (int i = 1; i <= agilink::omnihand::OmniHand2025::kDegreesOfActiveFreedom; ++i) {
    uint16_t origin_pos = hand_->GetSingleAxisPos(i);
    uint16_t target_pos = 512;
    uint16_t reply_pos = hand_->SetSingleAxisPos(i, target_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Wait for the position to take effect
    uint16_t read_pos = hand_->GetSingleAxisPos(i);
    AgilinkLogger::get().infof(TAG, "  Joint %d: origin=%u, set=%u, reply=%u, read=%u", i, origin_pos, target_pos, reply_pos, read_pos);
    EXPECT_LE(reply_pos, 4096u) << "invalid reply position for joint " << i;
    EXPECT_LE(read_pos, 4096u) << "invalid read position for joint " << i;
  }
}

// 0x08/0x09: All axis position
TEST_F(OmniHand2025UsbTest, StreamCmdAllAxisPos) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis pos commands(0x08/0x09):");
  std::vector<uint16_t> positions(10, 1024);
  const auto resp = hand_->SetAllAxisPos(positions);
  EXPECT_FALSE(resp.positions.empty());
  EXPECT_EQ(resp.positions.size(), 10u);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Wait for the position to take effect
  const auto all_pos = hand_->GetAllAxisPos();
  ASSERT_FALSE(all_pos.empty()) << "GetAllAxisPos timed out";
  EXPECT_EQ(all_pos.size(), 10u);
  for (size_t i = 0; i < resp.positions.size(); ++i) {
    AgilinkLogger::get().infof(TAG, "  J%zu: set_pos=%u, reply_pos=%u, read_pos=%u",
        i + 1, positions[i], resp.positions[i], all_pos[i]);
  }
  AgilinkLogger::get().infof(TAG, "  0x08 reply:");
  AgilinkLogger::get().infof(TAG, "%s", resp.ToString().c_str());
}

// 0x0A/0x0B/0x0C: Current, velocity, temperature
TEST_F(OmniHand2025UsbTest, StreamCmdCurrentVelTemp) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis current commands(0x0A):");
  const auto all_current = hand_->GetAllAxisCurrent();
  ASSERT_FALSE(all_current.empty()) << "GetAllAxisCurrent timed out";
  EXPECT_EQ(all_current.size(), 10u);
  {
    std::string msg;
    for (size_t i = 0; i < all_current.size(); ++i) {
      msg += std::to_string(all_current[i]) + "mA ";
    }
    AgilinkLogger::get().infof(TAG, "  Currents: %s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis velocity commands(0x0B):");
  const auto all_velocity = hand_->GetAllAxisVelocity();
  ASSERT_FALSE(all_velocity.empty()) << "GetAllAxisVelocity timed out";
  EXPECT_EQ(all_velocity.size(), 10u);
  {
    std::string msg;
    for (size_t i = 0; i < all_velocity.size(); ++i) {
      msg += std::to_string(all_velocity[i]) + " ";
    }
    AgilinkLogger::get().infof(TAG, "  Velocities: %s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis temperature commands(0x0C):");
  const auto all_temp = hand_->GetAllAxisTemp();
  ASSERT_FALSE(all_temp.empty()) << "GetAllAxisTemp timed out";
  EXPECT_EQ(all_temp.size(), 10u);
  {
    std::string msg;
    for (size_t i = 0; i < all_temp.size(); ++i) {
      msg += std::to_string(static_cast<int>(all_temp[i])) + " ";
    }
    AgilinkLogger::get().infof(TAG, "  Temperatures: %s", msg.c_str());
  }
}

// 0x0D/0x0E/0x0F: Error code, clear error, play action
TEST_F(OmniHand2025UsbTest, StreamCmdErrorAndAction) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis error code commands(0x0D):");
  EXPECT_GE(hand_->GetErrorCode(), 0u);
  (void)hand_->ClearError();

  // 0x0F (dangerous action)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis action commands(0x0F):");
  (void)hand_->PlayAction(1);
}

// 0x10: Position range
TEST_F(OmniHand2025UsbTest, StreamCmdPosRange) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis pos range commands(0x10):");
  const auto pos_range = hand_->GetAllAxisPosRange();
  ASSERT_FALSE(pos_range.empty()) << "GetAllAxisPosRange timed out";
  EXPECT_EQ(pos_range.size(), 10u);
  {
    std::string msg;
    for (size_t i = 0; i < pos_range.size(); ++i) {
      msg += std::to_string(pos_range[i]);
      if (i < pos_range.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  Position Ranges: %s", msg.c_str());
  }
}

// 0x11~0x14: Tactile sensors
TEST_F(OmniHand2025UsbTest, StreamCmdTactileSensors) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis tactile sensors commands(0x11):");
  for (int i = 1; i <= 7; ++i) {
    const auto fingertip0 = hand_->GetFingertipSensor(i);
    EXPECT_FALSE(fingertip0.empty());
    std::string msg;
    for (size_t j = 0; j < fingertip0.size(); ++j) {
      msg += std::to_string(static_cast<int>(fingertip0[j])) + " ";
    }
    AgilinkLogger::get().infof(TAG, "  Sensor %d: %s", i, msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis tactile sensors commands(0x12):");
  const auto fingertipA = hand_->GetAllFingertipSensorA();
  ASSERT_FALSE(fingertipA.empty()) << "GetAllFingertipSensorA timed out";
  EXPECT_EQ(fingertipA.size(), 48u);

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis tactile sensors commands(0x13):");
  const auto fingertipB = hand_->GetAllFingertipSensorB();
  ASSERT_FALSE(fingertipB.empty()) << "GetAllFingertipSensorB timed out";
  EXPECT_EQ(fingertipB.size(), 32u);

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis tactile sensors commands(0x14):");
  const auto fingertipC = hand_->GetAllFingertipSensorC();
  ASSERT_FALSE(fingertipC.empty()) << "GetAllFingertipSensorC timed out";
  EXPECT_EQ(fingertipC.size(), 50u);
}

// 0x15: Run mode
TEST_F(OmniHand2025UsbTest, StreamCmdRunMode) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing control mode commands(0x15):");
  EXPECT_TRUE(hand_->SetRunMode(1, static_cast<uint8_t>(agilink::omnihand::ControlMode::SERVO)));

  // 0x16~0x19 actual axis pos: too dangerous
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping actual axis position commands (0x16~0x19) due to potential hardware risk.");
  // EXPECT_LE(hand_->SetSingleActualAxisPos(1, 2048), 4096u);
  // std::vector<uint16_t> actual_positions(10, 2048);
  // const auto actual_resp = hand_->SetAllActualAxisPos(actual_positions);
  // EXPECT_EQ(actual_resp.size(), 10u);
}

// 0x1A: Load data
TEST_F(OmniHand2025UsbTest, StreamCmdLoadData) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis load data commands(0x1A):");
  const auto load = hand_->GetAllLoadData();
  ASSERT_FALSE(load.empty()) << "GetAllLoadData timed out";
  EXPECT_EQ(load.size(), 10u);

  // 0x1B~0x1D limits: too dangerous
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping axis limit position commands (0x1B~0x1D) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetAxisMinPos(1, 100));
  // EXPECT_TRUE(hand_->SetAxisMaxPos(1, 4000));
  // EXPECT_TRUE(hand_->ClearAllLimitPos());

  // 0x20~0x25 protections: too dangerous
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping protection commands (0x20~0x25) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetAllRunSpeed(std::vector<int16_t>(10, 0)));
  // EXPECT_TRUE(hand_->SetOverloadTorque(1, 0));
  // EXPECT_TRUE(hand_->SetOverloadProtectionTime(1, 0));
  // EXPECT_TRUE(hand_->SetProtectedTorque(1, 0));
  // EXPECT_TRUE(hand_->SetMinTorque(1, 0));
  // EXPECT_TRUE(hand_->SetProtectiveCurrent(1, 0));
}

// 0x26/0x27: Motor and sensor IDs
TEST_F(OmniHand2025UsbTest, StreamCmdMotorSensorIds) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis ID commands(0x26):");
  const auto motor_ids = hand_->GetAllElectricMotorId();
  ASSERT_FALSE(motor_ids.empty()) << "GetAllElectricMotorId timed out";
  EXPECT_EQ(motor_ids.size(), 10u);
  {
    std::string msg;
    for (size_t i = 0; i < motor_ids.size(); ++i) {
      msg += std::to_string(static_cast<int>(motor_ids[i]));
      if (i < motor_ids.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  Motor IDs: %s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis ID commands(0x27):");
  const auto sensor_ids = hand_->GetAllSensorId();
  ASSERT_FALSE(sensor_ids.empty()) << "GetAllSensorId timed out";
  EXPECT_EQ(sensor_ids.size(), 7u);
  {
    std::string msg;
    for (size_t i = 0; i < sensor_ids.size(); ++i) {
      msg += std::to_string(static_cast<int>(sensor_ids[i]));
      if (i < sensor_ids.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  Sensor IDs: %s", msg.c_str());
  }

  // 0x28 set all axis CVP upload interval (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping all axis CVP upload interval command (0x28) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetAllAxisCvpUploadInterval(100));
}

// 0x29: CVP data
TEST_F(OmniHand2025UsbTest, StreamCmdCVP) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis CVP commands(0x29):");
  const auto cvp = hand_->GetAllAxisCvp();
  ASSERT_FALSE(cvp.empty()) << "GetAllAxisCvp timed out";
  EXPECT_EQ(cvp.size(), 60u);
}

// 0x30: Axis limit positions
TEST_F(OmniHand2025UsbTest, StreamCmdAxisLimits) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis limit position commands(0x30):");
  const auto axis_limits = hand_->GetAxisLimitPos();
  ASSERT_FALSE(axis_limits.min_limits.empty()) << "GetAxisLimitPos timed out";
  EXPECT_EQ(axis_limits.min_limits.size(), 10u);
  EXPECT_EQ(axis_limits.max_limits.size(), 10u);
  {
    std::string msg;
    for (size_t i = 0; i < axis_limits.min_limits.size(); ++i) {
      msg += "J" + std::to_string(i + 1) + "[" + std::to_string(axis_limits.min_limits[i]) + "," + std::to_string(axis_limits.max_limits[i]) + "]";
      if (i + 1 < axis_limits.min_limits.size()) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "  Axis Limits (min/max per joint, 0-4095): %s", msg.c_str());
  }

  // 0x31 set right/left hand type (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping right/left hand type command (0x31) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetRightOrLeft(0));
}

// 0x32: Pos/speed/cur data
TEST_F(OmniHand2025UsbTest, StreamCmdPosSpeedCur) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis pos/speed/cur commands(0x32):");
  std::vector<uint16_t> ps_positions(10, 2048);
  std::vector<int16_t> ps_speeds(10, 0);
  std::vector<uint16_t> ps_torques(10, 0);
  const agilink::omnihand::SetAllAxisPosResponse pos_speed_torque_resp = hand_->SetPosSpeedTorqueData(ps_positions, ps_speeds, ps_torques);
  ASSERT_FALSE(pos_speed_torque_resp.positions.empty()) << "SetPosSpeedTorqueData failed";
  EXPECT_EQ(pos_speed_torque_resp.positions.size(), 10u);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  const auto all_pos_after_ps = hand_->GetAllAxisPos();
  ASSERT_FALSE(all_pos_after_ps.empty()) << "GetAllAxisPos after SetPosSpeedTorqueData timed out";
  EXPECT_EQ(all_pos_after_ps.size(), 10u);
  for (size_t i = 0; i < pos_speed_torque_resp.positions.size(); ++i) {
    AgilinkLogger::get().infof(TAG, "  J%zu: set_pos=%u, set_speed=%d, set_torque=%d, reply_pos=%u, read_pos=%u",
        i + 1, ps_positions[i], static_cast<int16_t>(ps_speeds[i]), static_cast<int>(ps_torques[i]),
        pos_speed_torque_resp.positions[i], all_pos_after_ps[i]);
  }
  AgilinkLogger::get().infof(TAG, "  0x32 reply:");
  AgilinkLogger::get().infof(TAG, "%s", pos_speed_torque_resp.ToString().c_str());

  // 0x33 finger tactile force + threshold: no work
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping finger tactile force command (0x33) due to no response.");
  // const auto tactile_force = hand_->GetFingerTactileForce();
  // if (tactile_force.empty()) GTEST_SKIP() << "GetFingerTactileForce timeout";
  // EXPECT_EQ(tactile_force.size(), 35u);

  // 0x34 set temperature threshold (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping temperature threshold command (0x34) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetTemperatureThreshold(80));

  // 0x80 set control source (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping control source command (0x80) due to potential hardware risk.");
  // (void)hand_->SetControlSource(0);
}

// 0x81: Control source query
TEST_F(OmniHand2025UsbTest, StreamCmdControlSource) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing control source query command(0x81):");
  EXPECT_EQ(hand_->GetControlSource(), 0u);
  AgilinkLogger::get().infof(TAG, "  Control Source: %d", static_cast<int>(hand_->GetControlSource()));


  // 0xC1 set product serial number (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping set product serial number command (0xC1) due to potential hardware risk.");
  // std::vector<uint8_t> serial_number(19, 0);
  // (void)hand_->SetProductSerialNumber(serial_number);
}

// 0xC2: Product serial number
TEST_F(OmniHand2025UsbTest, StreamCmdProductSerialNumber) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing get product serial number command(0xC2):");
  const auto prod_serial = hand_->GetProductSerialNumber();
  EXPECT_FALSE(prod_serial.ToString().empty());
  AgilinkLogger::get().infof(TAG, "  Product Serial Number: %s", prod_serial.ToString().c_str());
}

// 0xCD: Firmware version
TEST_F(OmniHand2025UsbTest, StreamCmdFirmwareVersion) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing get firmware version command(0xCD):");
  const auto fw = hand_->GetFwVersion();
  EXPECT_EQ(fw.dof, 10);
  AgilinkLogger::get().infof(TAG, "  Firmware Version: %s", fw.ToString().c_str());
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char** argv) {
  std::vector<char*> gtest_args;
  gtest_args.push_back(argv[0]);

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-p" && i + 1 < argc) {
      g_usb_port = argv[++i];
    } else if (arg == "-b" && i + 1 < argc) {
      g_baudrate = std::stoi(argv[++i]);
    } else if (arg == "-f" && i + 1 < argc) {
      g_request_interval = std::stoi(argv[++i]);
      if (g_request_interval > 500) g_request_interval = 500;
    } else if (arg == "-t" && i + 1 < argc) {
      g_frame_recv_timeout = std::stoi(argv[++i]);
    } else if (arg == "--dangerous") {
      g_run_dangerous_actions = true;
    } else if (arg == "--help" || arg == "-h") {
      AgilinkLogger::get().infof(TAG, "OmniHand 2025 USB Test\n");
      AgilinkLogger::get().infof(TAG, "Usage: %s [options]\n", argv[0]);
      AgilinkLogger::get().infof(TAG, "Options:");
#if defined(_WIN32)
      AgilinkLogger::get().infof(TAG, "  -p PORT      USB serial port (default: COM3)");
#else
      AgilinkLogger::get().infof(TAG, "  -p PORT      USB serial port (default: /dev/ttyACM0)");
#endif
      AgilinkLogger::get().infof(TAG, "  -b BAUDRATE  Baudrate (default: 460800)");
      AgilinkLogger::get().infof(TAG, "  -f INTERVAL  Request interval in ms (default: 500, max: 500)");
      AgilinkLogger::get().infof(TAG, "  -t MS         Frame receive timeout ms (default: 200)");
      AgilinkLogger::get().infof(TAG, "  --dangerous   Enable write/action commands (risk)");
      AgilinkLogger::get().infof(TAG, "\nExample:");
#if defined(_WIN32)
      AgilinkLogger::get().infof(TAG, "  %s -p COM3 -b 460800 -f 500", argv[0]);
#else
      AgilinkLogger::get().infof(TAG, "  %s -p /dev/ttyACM0 -b 460800 -f 500", argv[0]);
#endif
      AgilinkLogger::get().flush();
      return 0;
    } else {
      gtest_args.push_back(argv[i]);
    }
  }

  AgilinkLogger::get().infof(TAG, "=== OmniHand 2025 USB Test ===");
  AgilinkLogger::get().infof(TAG, "Port: %s", g_usb_port.c_str());
  AgilinkLogger::get().infof(TAG, "Baudrate: %d", g_baudrate);
  AgilinkLogger::get().infof(TAG, "Request Interval: %d ms", g_request_interval);
  AgilinkLogger::get().infof(TAG, "Frame Recv Timeout: %d ms", g_frame_recv_timeout);
  AgilinkLogger::get().infof(TAG, "Dangerous actions: %s", g_run_dangerous_actions ? "ON" : "OFF");
  AgilinkLogger::get().infof(TAG, "==============================");
  AgilinkLogger::get().flush();

  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
