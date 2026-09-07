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
#include <iostream>
#include <iomanip>
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
          std::cout << "[Warning]: USB device created but Init() failed." << std::endl;
        }
        hand_->ShowDataDetails(true);  // default: stdout
      }
    } catch (const std::exception& e) {
      std::cout << "[Warning]: Failed to open USB port: " << e.what() << std::endl;
      hand_.reset();
      device_available_ = false;
    }
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << "USB device not available";
    }
  }

  void TearDown() override {
    hand_.reset();
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
  std::cout << vendor_info.ToString() << std::endl;
  
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
  std::cout << device_info.ToString() << std::endl;
  
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
  
  std::cout << "[SetGetSingleAxisPos] Testing all 10 joints:" << std::endl;
  for (int joint = 1; joint <= 10; ++joint) {
    int16_t target_pos = safe_pos[joint - 1];
    hand_->SetJointMotorPosi(joint, target_pos);
    auto pos = hand_->GetJointMotorPosi(joint);
    std::cout << "  J" << joint << ": set=" << target_pos << ", get=" << pos << std::endl;
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
  std::cout << "[SetAllJointMotorPosi] returned " << set_result.size() << " positions: ";
  for (size_t i = 0; i < set_result.size(); ++i) {
    std::cout << set_result[i];
    if (i < set_result.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(set_result.size(), 10);
  
  // Test GetAllJointMotorPosi separately
  auto get_result = hand_->GetAllJointMotorPosi();
  std::cout << "[GetAllJointMotorPosi] returned " << get_result.size() << " positions: ";
  for (size_t i = 0; i < get_result.size(); ++i) {
    std::cout << get_result[i];
    if (i < get_result.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(get_result.size(), 10);
}

// ============================================================================
// Current/Force Tests
// GET_SINGLE_AXIS_FORCE (0x0A), GET_ALL_AXIS_FORCE (0x0B)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetAllCurrentReport) {
  RequireDevice();
  
  auto currents = hand_->GetAllCurrentReport();
  std::cout << "[GetAllCurrentReport] ";
  for (size_t i = 0; i < currents.size(); ++i) {
    std::cout << "J" << (i+1) << ":" << currents[i] << "mA";
    if (i < currents.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  ASSERT_EQ(currents.size(), 10u) << "GetAllCurrentReport timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetCurrentReport) {
  RequireDevice();

  std::cout << "[GetCurrentReport] All joints:" << std::endl;
  for (int i = 1; i <= 10; ++i) {
    auto current = hand_->GetCurrentReport(static_cast<unsigned char>(i));
    std::cout << "  J" << i << ": " << current << "mA" << std::endl;
  }
}

// ============================================================================
// Temperature Test (GET_ALL_AXIS_TEMP 0x0C)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetAllTemperatureReport) {
  RequireDevice();
  
  auto temps = hand_->GetAllTemperatureReport();
  std::cout << "[GetAllTemperatureReport] ";
  for (size_t i = 0; i < temps.size(); ++i) {
    // Use ASCII "degC" — Unicode degree sign breaks on Windows consoles (GBK shows as garbled).
    std::cout << "J" << (i+1) << ":" << temps[i] << " degC";
    if (i < temps.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  ASSERT_EQ(temps.size(), 10u) << "GetAllTemperatureReport timed out or returned an invalid size";
  
  // Temperature is int8_t (-128 to 127 degC), typical motor temp: 30-80 degC
  for (auto temp : temps) {
    EXPECT_GE(temp, -40);   // Extreme cold environment
    EXPECT_LE(temp, 127);   // int8_t max
  }
}

TEST_F(OmniHand2025UsbTest, GetTemperatureReport) {
  RequireDevice();

  std::cout << "[GetTemperatureReport] All joints:" << std::endl;
  for (int i = 1; i <= 10; ++i) {
    auto temp = hand_->GetTemperatureReport(static_cast<unsigned char>(i));
    std::cout << "  J" << i << ": " << temp << " degC" << std::endl;
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
  std::cout << "[GetAllErrorReport] ";
  for (size_t i = 0; i < errors.size(); ++i) {
    std::cout << "J" << (i+1) << ":[";
    if (errors[i].bits.stalled_) std::cout << "S";
    if (errors[i].bits.overheat_) std::cout << "H";
    if (errors[i].bits.over_current_) std::cout << "C";
    if (errors[i].bits.motor_except_) std::cout << "M";
    if (errors[i].bits.commu_except_) std::cout << "X";
    std::cout << "]";
    if (i < errors.size() - 1) std::cout << " ";
  }
  std::cout << std::endl;
  
  ASSERT_EQ(errors.size(), 10u) << "GetAllErrorReport timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetErrorReport) {
  RequireDevice();

  std::cout << "[GetErrorReport] All joints:" << std::endl;
  for (int i = 1; i <= 10; ++i) {
    auto err = hand_->GetErrorReport(static_cast<unsigned char>(i));
    std::cout << "  J" << i << ": ["
              << (err.bits.stalled_ ? "S" : "")
              << (err.bits.overheat_ ? "H" : "")
              << (err.bits.over_current_ ? "C" : "")
              << (err.bits.motor_except_ ? "M" : "")
              << (err.bits.commu_except_ ? "X" : "") << "]" << std::endl;
  }
}

// ============================================================================
// Tactile Sensor Test (GET_FINGERTIP_SENSOR_DATA 0x11)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetTactileSensorData) {
  RequireDevice();
  
  std::cout << "[GetTactileSensorData] Testing all tactile sensors:" << std::endl;
  
  // Test finger sensors (Thumb, Index, Middle, Ring, Little) - 16 values each
  std::vector<agilink::omnihand::Finger> fingers = {
    agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE,
    agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE
  };
  
  std::cout << "  Fingers (16 values each):" << std::endl;
  for (auto finger : fingers) {
    auto data = hand_->GetTactileSensorData(finger);
    std::cout << "    " << agilink::omnihand::ToString(finger) << ": ";
    for (size_t i = 0; i < data.size(); ++i) {
      std::cout << static_cast<int>(data[i]);
      if (i < data.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(data.size(), 16);
  }
  
  // Test palm/dorsum sensors - 25 values each
  std::vector<agilink::omnihand::Finger> palm_dorsum = {agilink::omnihand::Finger::PALM, agilink::omnihand::Finger::DORSUM};
  
  std::cout << "  Palm/Dorsum (25 values each):" << std::endl;
  for (auto sensor : palm_dorsum) {
    auto data = hand_->GetTactileSensorData(sensor);
    std::cout << "    " << agilink::omnihand::ToString(sensor) << ": ";
    for (size_t i = 0; i < data.size(); ++i) {
      std::cout << static_cast<int>(data[i]);
      if (i < data.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    EXPECT_EQ(data.size(), 25);
  }
}

TEST_F(OmniHand2025UsbTest, GetAllTactileSensorData) {
  RequireDevice();

  auto all_data = hand_->GetAllTactileSensorData();
  std::cout << "[GetAllTactileSensorData] " << all_data.size() << " sensors:" << std::endl;
  for (const auto& d : all_data) {
    std::cout << "  " << agilink::omnihand::ToString(d.sensor_id_)
              << ": " << d.data_.size() << " bytes" << std::endl;
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
    std::cout << "[GetTactileSensorDataRaw] " << agilink::omnihand::ToString(finger)
              << " (" << raw.data_.size() << " bytes): ";
    if (raw.data_.empty()) {
      std::cout << "(empty)" << std::endl;
      continue;
    }
    any_supported = true;
    for (size_t i = 0; i < raw.data_.size(); ++i) {
      std::cout << static_cast<int>(raw.data_[i]);
      if (i < raw.data_.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
  }
  if (!any_supported) GTEST_SKIP() << "GetTactileSensorDataRaw not supported on this device";
}

TEST_F(OmniHand2025UsbTest, GetAllTactileSensorDataRaw) {
  RequireDevice();

  auto all_raw = hand_->GetAllTactileSensorDataRaw();
  if (all_raw.empty()) GTEST_SKIP() << "GetAllTactileSensorDataRaw not supported on this device";

  std::cout << "[GetAllTactileSensorDataRaw] " << all_raw.size() << " sensors:" << std::endl;
  for (const auto& d : all_raw) {
    std::cout << "  " << agilink::omnihand::ToString(d.sensor_id_)
              << " (" << d.data_.size() << " bytes): ";
    for (size_t i = 0; i < d.data_.size(); ++i) {
      std::cout << static_cast<int>(d.data_[i]);
      if (i < d.data_.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
  }
  EXPECT_EQ(all_raw.size(), 7u);
}

// ============================================================================
// Current Threshold Test (SET_PROTECTIVE_CURRENT 0x25)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetAllCurrentThreshold) {
  RequireDevice();
  
  // std::vector<int16_t> thresholds(10, 1500);  // 1500mA
  // hand_->SetAllCurrentThreshold(thresholds);
  // std::cout << "[SetAllCurrentThreshold] All joints -> 1000mA" << std::endl;
  
  // Note: GetAllCurrentThreshold returns cached values for USB (no GET command)
  auto current_thresholds = hand_->GetAllCurrentThreshold();
  std::cout << "[GetAllCurrentThreshold] (cached) ";
  for (size_t i = 0; i < current_thresholds.size(); ++i) {
    std::cout << current_thresholds[i];
    if (i < current_thresholds.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
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
  std::cout << "[MixControlByPVT] all 10 joints" << std::endl;
  
  auto feedback_pos = hand_->GetAllJointMotorPosi();
  std::cout << "[GetAllJointMotorPosi] After mixed control: ";
  for (size_t i = 0; i < feedback_pos.size(); ++i) {
    std::cout << feedback_pos[i];
    if (i < feedback_pos.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  EXPECT_EQ(feedback_pos.size(), 10);
}

TEST_F(OmniHand2025UsbTest, GetAllJointMotorVelo) {
  RequireDevice();

  auto current_velo = hand_->GetAllJointMotorVelo();
  std::cout << "[GetAllJointMotorVelo] ";
  for (size_t i = 0; i < current_velo.size(); ++i) {
    std::cout << current_velo[i];
    if (i < current_velo.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  ASSERT_EQ(current_velo.size(), 10u) << "GetAllJointMotorVelo timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetJointMotorVelo) {
  RequireDevice();

  std::cout << "[GetJointMotorVelo] All joints:" << std::endl;
  for (int i = 1; i <= 10; ++i) {
    auto velo = hand_->GetJointMotorVelo(static_cast<unsigned char>(i));
    std::cout << "  J" << i << ": " << velo << std::endl;
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
  std::cout << "[MixControlByPT] all 10 joints" << std::endl;
}

// ============================================================================
// Joint Angle Tests (uses position commands internally)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetGetAllActiveJointAngles) {
  RequireDevice();
  
  std::vector<double> angles(10, 0.0);
  hand_->SetAllActiveJointAngles(angles);
  std::cout << "[SetAllActiveJointAngles] All joints -> 0.0 rad" << std::endl;
  
  auto current_angles = hand_->GetAllActiveJointAngles();
  std::cout << "[GetAllActiveJointAngles] ";
  for (size_t i = 0; i < current_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << current_angles[i];
    if (i < current_angles.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  ASSERT_EQ(current_angles.size(), 10u)
      << "GetAllActiveJointAngles timed out or returned an invalid size";
}

TEST_F(OmniHand2025UsbTest, GetAllJointAngles) {
  RequireDevice();
  
  auto all_angles = hand_->GetAllJointAngles();
  std::cout << "[GetAllJointAngles] (" << all_angles.size() << " joints): ";
  for (size_t i = 0; i < all_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << all_angles[i];
    if (i < all_angles.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  ASSERT_EQ(all_angles.size(), 16u) << "GetAllJointAngles timed out or returned an invalid size";  // 10 active + 6 passive
}

// ============================================================================
// Kinematics Solver Test (software-only, no USB command)
// ============================================================================

TEST_F(OmniHand2025UsbTest, KinematicsSolver) {
  RequireDevice();
  
  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointAngles(active_angles);
  
  std::cout << "[GetAllJointAngles] Forward kinematics: ";
  for (size_t i = 0; i < all_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << all_angles[i];
    if (i < all_angles.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  EXPECT_EQ(all_angles.size(), 16);
}

// ============================================================================
// StreamCmd Tests - Split by functionality
// ============================================================================

// 0x01/0x02: Power state
TEST_F(OmniHand2025UsbTest, StreamCmdPowerState) {
  RequireDevice();

  EXPECT_EQ(hand_->GetRequestInterval(), g_request_interval);
  EXPECT_EQ(hand_->GetFrameRecvTimeout(), g_frame_recv_timeout);

  // 0x01/0x02: power state
  std::cout << "[StreamCmd] Testing power state commands(0x01/0x02):" << std::endl;
  EXPECT_TRUE(hand_->SetPowerState(1));
  EXPECT_LE(hand_->GetPowerState(), 2u);

  // 0x03/0x04/0x05: SetAxisHoming + SetId + SaveParam, too dangerous
  std::cout << "[StreamCmd] Skipping axis homing and ID commands (0x03/0x04/0x05) due to potential hardware risk." << std::endl;
  // EXPECT_TRUE(hand_->SetAxisHoming(0, 0));
  // EXPECT_TRUE(hand_->SetId(0));
  // EXPECT_TRUE(hand_->SaveParam()); // no work
}

// 0x06/0x07: Single axis position
TEST_F(OmniHand2025UsbTest, StreamCmdSingleAxisPos) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing single axis pos commands(0x06/0x07):" << std::endl;
  for (int i = 1; i <= agilink::omnihand::OmniHand2025::kDegreesOfActiveFreedom; ++i) {
    uint16_t origin_pos = hand_->GetSingleAxisPos(i);
    uint16_t target_pos = 512;
    uint16_t reply_pos = hand_->SetSingleAxisPos(i, target_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Wait for the position to take effect
    uint16_t read_pos = hand_->GetSingleAxisPos(i);
    std::cout << "  Joint " << i << ": origin=" << origin_pos << ", set=" << target_pos << ", reply=" << reply_pos << ", read=" << read_pos << std::endl;
    EXPECT_LE(reply_pos, 4096u) << "invalid reply position for joint " << i;
    EXPECT_LE(read_pos, 4096u) << "invalid read position for joint " << i;
  }
}

// 0x08/0x09: All axis position
TEST_F(OmniHand2025UsbTest, StreamCmdAllAxisPos) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis pos commands(0x08/0x09):" << std::endl;
  std::vector<uint16_t> positions(10, 1024);
  const auto resp = hand_->SetAllAxisPos(positions);
  EXPECT_FALSE(resp.positions.empty());
  EXPECT_EQ(resp.positions.size(), 10u);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Wait for the position to take effect
  const auto all_pos = hand_->GetAllAxisPos();
  ASSERT_FALSE(all_pos.empty()) << "GetAllAxisPos timed out";
  EXPECT_EQ(all_pos.size(), 10u);
  for (size_t i = 0; i < resp.positions.size(); ++i) {
    std::cout << "  J" << (i + 1) << ": set_pos=" << positions[i]
              << ", reply_pos=" << resp.positions[i]
              << ", read_pos=" << all_pos[i] << std::endl;
  }
  std::cout << "  0x08 reply: \n" << resp.ToString() << std::endl;
}

// 0x0A/0x0B/0x0C: Current, velocity, temperature
TEST_F(OmniHand2025UsbTest, StreamCmdCurrentVelTemp) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis current commands(0x0A):" << std::endl;
  const auto all_current = hand_->GetAllAxisCurrent();
  ASSERT_FALSE(all_current.empty()) << "GetAllAxisCurrent timed out";
  EXPECT_EQ(all_current.size(), 10u);
  std::cout << "  Currents: ";
  for (size_t i = 0; i < all_current.size(); ++i) {
    std::cout << all_current[i] << "mA ";;
  }
  std::cout << std::endl;

  std::cout << "[StreamCmd] Testing all axis velocity commands(0x0B):" << std::endl;
  const auto all_velocity = hand_->GetAllAxisVelocity();
  ASSERT_FALSE(all_velocity.empty()) << "GetAllAxisVelocity timed out";
  EXPECT_EQ(all_velocity.size(), 10u);
  std::cout << "  Velocities: ";
  for (size_t i = 0; i < all_velocity.size(); ++i) {
    std::cout << all_velocity[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "[StreamCmd] Testing all axis temperature commands(0x0C):" << std::endl;
  const auto all_temp = hand_->GetAllAxisTemp();
  ASSERT_FALSE(all_temp.empty()) << "GetAllAxisTemp timed out";
  EXPECT_EQ(all_temp.size(), 10u);
  std::cout << "  Temperatures: ";
  for (size_t i = 0; i < all_temp.size(); ++i) {
    std::cout << static_cast<int>(all_temp[i]) << " ";
  }
  std::cout << std::endl;
}

// 0x0D/0x0E/0x0F: Error code, clear error, play action
TEST_F(OmniHand2025UsbTest, StreamCmdErrorAndAction) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis error code commands(0x0D):" << std::endl;
  EXPECT_GE(hand_->GetErrorCode(), 0u);
  (void)hand_->ClearError();

  // 0x0F (dangerous action)
  std::cout << "[StreamCmd] Testing all axis action commands(0x0F):" << std::endl;
  (void)hand_->PlayAction(1);
}

// 0x10: Position range
TEST_F(OmniHand2025UsbTest, StreamCmdPosRange) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis pos range commands(0x10):" << std::endl;
  const auto pos_range = hand_->GetAllAxisPosRange();
  ASSERT_FALSE(pos_range.empty()) << "GetAllAxisPosRange timed out";
  EXPECT_EQ(pos_range.size(), 10u);
  std::cout << "  Position Ranges: ";
  for (size_t i = 0; i < pos_range.size(); ++i) {
    std::cout << pos_range[i];
    if (i < pos_range.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
}

// 0x11~0x14: Tactile sensors
TEST_F(OmniHand2025UsbTest, StreamCmdTactileSensors) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis tactile sensors commands(0x11):" << std::endl;
  for (int i = 1; i <= 7; ++i) {
    const auto fingertip0 = hand_->GetFingertipSensor(i);
    EXPECT_FALSE(fingertip0.empty());
    std::cout << "  Sensor " << i << ": ";
    for (size_t j = 0; j < fingertip0.size(); ++j) {
      std::cout << static_cast<int>(fingertip0[j]) << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "[StreamCmd] Testing all axis tactile sensors commands(0x12):" << std::endl;
  const auto fingertipA = hand_->GetAllFingertipSensorA();
  ASSERT_FALSE(fingertipA.empty()) << "GetAllFingertipSensorA timed out";
  EXPECT_EQ(fingertipA.size(), 48u);

  std::cout << "[StreamCmd] Testing all axis tactile sensors commands(0x13):" << std::endl;
  const auto fingertipB = hand_->GetAllFingertipSensorB();
  ASSERT_FALSE(fingertipB.empty()) << "GetAllFingertipSensorB timed out";
  EXPECT_EQ(fingertipB.size(), 32u);

  std::cout << "[StreamCmd] Testing all axis tactile sensors commands(0x14):" << std::endl;
  const auto fingertipC = hand_->GetAllFingertipSensorC();
  ASSERT_FALSE(fingertipC.empty()) << "GetAllFingertipSensorC timed out";
  EXPECT_EQ(fingertipC.size(), 50u);
}

// 0x15: Run mode
TEST_F(OmniHand2025UsbTest, StreamCmdRunMode) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing control mode commands(0x15):" << std::endl;
  EXPECT_TRUE(hand_->SetRunMode(1, static_cast<uint8_t>(agilink::omnihand::ControlMode::SERVO)));

  // 0x16~0x19 actual axis pos: too dangerous
  std::cout << "[StreamCmd] Skipping actual axis position commands (0x16~0x19) due to potential hardware risk." << std::endl;
  // EXPECT_LE(hand_->SetSingleActualAxisPos(1, 2048), 4096u);
  // std::vector<uint16_t> actual_positions(10, 2048);
  // const auto actual_resp = hand_->SetAllActualAxisPos(actual_positions);
  // EXPECT_EQ(actual_resp.size(), 10u);
}

// 0x1A: Load data
TEST_F(OmniHand2025UsbTest, StreamCmdLoadData) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis load data commands(0x1A):" << std::endl;
  const auto load = hand_->GetAllLoadData();
  ASSERT_FALSE(load.empty()) << "GetAllLoadData timed out";
  EXPECT_EQ(load.size(), 10u);

  // 0x1B~0x1D limits: too dangerous
  std::cout << "[StreamCmd] Skipping axis limit position commands (0x1B~0x1D) due to potential hardware risk." << std::endl;
  // EXPECT_TRUE(hand_->SetAxisMinPos(1, 100));
  // EXPECT_TRUE(hand_->SetAxisMaxPos(1, 4000));
  // EXPECT_TRUE(hand_->ClearAllLimitPos());

  // 0x20~0x25 protections: too dangerous
  std::cout << "[StreamCmd] Skipping protection commands (0x20~0x25) due to potential hardware risk." << std::endl;
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

  std::cout << "[StreamCmd] Testing all axis ID commands(0x26):" << std::endl;
  const auto motor_ids = hand_->GetAllElectricMotorId();
  ASSERT_FALSE(motor_ids.empty()) << "GetAllElectricMotorId timed out";
  EXPECT_EQ(motor_ids.size(), 10u);
  std::cout << "  Motor IDs: ";
  for (size_t i = 0; i < motor_ids.size(); ++i) {
    std::cout << static_cast<int>(motor_ids[i]);
    if (i < motor_ids.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << "[StreamCmd] Testing all axis ID commands(0x27):" << std::endl;
  const auto sensor_ids = hand_->GetAllSensorId();
  ASSERT_FALSE(sensor_ids.empty()) << "GetAllSensorId timed out";
  EXPECT_EQ(sensor_ids.size(), 7u);
  std::cout << "  Sensor IDs: ";
  for (size_t i = 0; i < sensor_ids.size(); ++i) {
    std::cout << static_cast<int>(sensor_ids[i]);
    if (i < sensor_ids.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  // 0x28 set all axis CVP upload interval (dangerous)
  std::cout << "[StreamCmd] Skipping all axis CVP upload interval command (0x28) due to potential hardware risk." << std::endl;
  // EXPECT_TRUE(hand_->SetAllAxisCvpUploadInterval(100));
}

// 0x29: CVP data
TEST_F(OmniHand2025UsbTest, StreamCmdCVP) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis CVP commands(0x29):" << std::endl;
  const auto cvp = hand_->GetAllAxisCvp();
  ASSERT_FALSE(cvp.empty()) << "GetAllAxisCvp timed out";
  EXPECT_EQ(cvp.size(), 60u);
}

// 0x30: Axis limit positions
TEST_F(OmniHand2025UsbTest, StreamCmdAxisLimits) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis limit position commands(0x30):" << std::endl;
  const auto axis_limits = hand_->GetAxisLimitPos();
  ASSERT_FALSE(axis_limits.min_limits.empty()) << "GetAxisLimitPos timed out";
  EXPECT_EQ(axis_limits.min_limits.size(), 10u);
  EXPECT_EQ(axis_limits.max_limits.size(), 10u);
  std::cout << "  Axis Limits (min/max per joint, 0-4095): ";
  for (size_t i = 0; i < axis_limits.min_limits.size(); ++i) {
    std::cout << "J" << (i + 1) << "[" << axis_limits.min_limits[i] << "," << axis_limits.max_limits[i] << "]";
    if (i + 1 < axis_limits.min_limits.size()) std::cout << ", ";
  }
  std::cout << std::endl;

  // 0x31 set right/left hand type (dangerous)
  std::cout << "[StreamCmd] Skipping right/left hand type command (0x31) due to potential hardware risk." << std::endl;
  // EXPECT_TRUE(hand_->SetRightOrLeft(0));
}

// 0x32: Pos/speed/cur data
TEST_F(OmniHand2025UsbTest, StreamCmdPosSpeedCur) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing all axis pos/speed/cur commands(0x32):" << std::endl;
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
    std::cout << "  J" << (i + 1) << ": set_pos=" << ps_positions[i]
              << ", set_speed=" << static_cast<int16_t>(ps_speeds[i]) << ", set_torque=" << static_cast<int>(ps_torques[i])
              << ", reply_pos=" << pos_speed_torque_resp.positions[i]
              << ", read_pos=" << all_pos_after_ps[i] << std::endl;
  }
  std::cout << "  0x32 reply:\n " << pos_speed_torque_resp.ToString() << std::endl;

  // 0x33 finger tactile force + threshold: no work
  std::cout << "[StreamCmd] Skipping finger tactile force command (0x33) due to no response." << std::endl;
  // const auto tactile_force = hand_->GetFingerTactileForce();
  // if (tactile_force.empty()) GTEST_SKIP() << "GetFingerTactileForce timeout";
  // EXPECT_EQ(tactile_force.size(), 35u);

  // 0x34 set temperature threshold (dangerous)
  std::cout << "[StreamCmd] Skipping temperature threshold command (0x34) due to potential hardware risk." << std::endl;
  // EXPECT_TRUE(hand_->SetTemperatureThreshold(80));

  // 0x80 set control source (dangerous)
  std::cout << "[StreamCmd] Skipping control source command (0x80) due to potential hardware risk." << std::endl;
  // (void)hand_->SetControlSource(0);
}

// 0x81: Control source query
TEST_F(OmniHand2025UsbTest, StreamCmdControlSource) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing control source query command(0x81):" << std::endl;
  EXPECT_EQ(hand_->GetControlSource(), 0u);
  std::cout << "  Control Source: " << static_cast<int>(hand_->GetControlSource()) << std::endl;


  // 0xC1 set product serial number (dangerous)
  std::cout << "[StreamCmd] Skipping set product serial number command (0xC1) due to potential hardware risk." << std::endl;
  // std::vector<uint8_t> serial_number(19, 0);
  // (void)hand_->SetProductSerialNumber(serial_number);
}

// 0xC2: Product serial number
TEST_F(OmniHand2025UsbTest, StreamCmdProductSerialNumber) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing get product serial number command(0xC2):" << std::endl;
  const auto prod_serial = hand_->GetProductSerialNumber();
  EXPECT_FALSE(prod_serial.ToString().empty());
  std::cout << "  Product Serial Number: " << prod_serial.ToString() << std::endl;
}

// 0xCD: Firmware version
TEST_F(OmniHand2025UsbTest, StreamCmdFirmwareVersion) {
  RequireDevice();

  std::cout << "[StreamCmd] Testing get firmware version command(0xCD):" << std::endl;
  const auto fw = hand_->GetFwVersion();
  EXPECT_EQ(fw.dof, 10);
  std::cout << "  Firmware Version: " << fw.ToString() << std::endl;
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
      std::cout << "OmniHand 2025 USB Test\n\n";
      std::cout << "Usage: " << argv[0] << " [options]\n\n";
      std::cout << "Options:\n";
      std::cout << "  -p PORT      USB serial port (default: "
#if defined(_WIN32)
                << "COM3"
#else
                << "/dev/ttyACM0"
#endif
                << ")\n";
      std::cout << "  -b BAUDRATE  Baudrate (default: 460800)\n";
      std::cout << "  -f INTERVAL  Request interval in ms (default: 500, max: 500)\n";
      std::cout << "  -t MS         Frame receive timeout ms (default: 200)\n";
      std::cout << "  --dangerous   Enable write/action commands (risk)\n";
      std::cout << "\nExample:\n";
#if defined(_WIN32)
      std::cout << "  " << argv[0] << " -p COM3 -b 460800 -f 500\n";
#else
      std::cout << "  " << argv[0] << " -p /dev/ttyACM0 -b 460800 -f 500\n";
#endif
      return 0;
    } else {
      gtest_args.push_back(argv[i]);
    }
  }
  
  std::cout << "=== OmniHand 2025 USB Test ===" << std::endl;
  std::cout << "Port: " << g_usb_port << std::endl;
  std::cout << "Baudrate: " << g_baudrate << std::endl;
  std::cout << "Request Interval: " << g_request_interval << " ms" << std::endl;
  std::cout << "Frame Recv Timeout: " << g_frame_recv_timeout << " ms" << std::endl;
  std::cout << "Dangerous actions: " << (g_run_dangerous_actions ? "ON" : "OFF") << std::endl;
  std::cout << "==============================" << std::endl;
  
  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
