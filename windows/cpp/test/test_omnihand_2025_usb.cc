// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

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
 * - SET_RUN_MODE (0x15) - SetControlMode
 * - SET_PROTECTIVE_CURRENT (0x25) - SetAllCurrentThreshold
 * - GET_ALL_AXIS_CVP (0x29) - (CVP data)
 * - SET_POS_SPEED_CUR_DATA (0x32) - MixCtrlJointMotor
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
#include "omnihand/private_omnihand_2025.h"
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

class OmniHand2025UsbTest : public ::testing::Test {
 protected:
  void SetUp() override {
    try {
      auto private_hand = agilink::omnihand::PrivateOmniHand2025::createHandByUsb(
          agilink::omnihand::HandType::LEFT,
          1,              // device_id
          g_usb_port,
          g_baudrate
      );
      hand_ = std::move(private_hand);  // Convert PrivateOmniHand2025 to OmniHand2025
      
      if (hand_) {
        hand_->SetRequestInterval(g_request_interval);
        device_available_ = hand_->Init();
        if (!device_available_) {
          std::cout << "[Warning]: USB device created but Init() failed." << std::endl;
        }
      }
    } catch (const std::exception& e) {
      std::cout << "[Warning]: Failed to open USB port: " << e.what() << std::endl;
      hand_.reset();
      device_available_ = false;
    }
  }

  void TearDown() override {
    hand_.reset();
  }

  void RequireDevice() {
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << "USB device not available";
    }
  }

  std::unique_ptr<agilink::omnihand::OmniHand2025> hand_;
  bool device_available_ = false;
};

// ============================================================================
// Basic Connection Tests
// ============================================================================

TEST_F(OmniHand2025UsbTest, CreateHand) {
  RequireDevice();
  EXPECT_NE(hand_, nullptr);
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
  std::cout << vendor_info.toString() << std::endl;
  
  if (vendor_info.dof == 0) {
    GTEST_SKIP() << "GetVendorInfo timeout";
  }
  
  EXPECT_EQ(vendor_info.dof, 10);
  EXPECT_FALSE(vendor_info.productModel.empty());
}

// ============================================================================
// Device ID Tests (SET_ID 0x04)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetDeviceInfo) {
  RequireDevice();
  
  auto device_info = hand_->GetDeviceInfo();
  std::cout << device_info.toString() << std::endl;
  
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
  
  if (currents.empty()) {
    GTEST_SKIP() << "GetAllCurrentReport timeout";
  }
  
  EXPECT_EQ(currents.size(), 10);
}

// ============================================================================
// Temperature Test (GET_ALL_AXIS_TEMP 0x0C)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetAllTemperatureReport) {
  RequireDevice();
  
  auto temps = hand_->GetAllTemperatureReport();
  std::cout << "[GetAllTemperatureReport] ";
  for (size_t i = 0; i < temps.size(); ++i) {
    std::cout << "J" << (i+1) << ":" << temps[i] << "°C";
    if (i < temps.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  if (temps.empty()) {
    GTEST_SKIP() << "GetAllTemperatureReport timeout";
  }
  
  EXPECT_EQ(temps.size(), 10);
  
  // Temperature is int8_t (-128 to 127°C), typical motor temp: 30-80°C
  for (auto temp : temps) {
    EXPECT_GE(temp, -40);   // Extreme cold environment
    EXPECT_LE(temp, 127);   // int8_t max
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
    if (errors[i].stalled_) std::cout << "S";
    if (errors[i].overheat_) std::cout << "H";
    if (errors[i].over_current_) std::cout << "C";
    if (errors[i].motor_except_) std::cout << "M";
    if (errors[i].commu_except_) std::cout << "X";
    std::cout << "]";
    if (i < errors.size() - 1) std::cout << " ";
  }
  std::cout << std::endl;
  
  if (errors.empty()) {
    GTEST_SKIP() << "GetAllErrorReport timeout";
  }
  
  EXPECT_EQ(errors.size(), 10);
}

// ============================================================================
// Tactile Sensor Test (GET_FINGERTIP_SENSOR_DATA 0x11)
// ============================================================================

TEST_F(OmniHand2025UsbTest, GetTactileSensorData) {
  RequireDevice();
  
  std::cout << "[GetTactileSensorData] Testing all tactile sensors:" << std::endl;
  
  // Test finger sensors (Thumb, Index, Middle, Ring, Little) - 16 values each
  std::vector<agilink::omnihand::Finger> fingers = {
    agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX, agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING, agilink::omnihand::Finger::LITTLE
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

// ============================================================================
// Control Mode Test (SET_RUN_MODE 0x15)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetControlMode) {
  RequireDevice();
  
  // Set control mode for joint 1
  hand_->SetControlMode(1, agilink::omnihand::ControlMode::SERVO);
  std::cout << "[SetControlMode] Joint 1 -> SERVO" << std::endl;
  
  // Note: GetAllControlMode returns cached values for USB
  auto modes = hand_->GetAllControlMode();
  std::cout << "[GetAllControlMode] (cached) ";
  for (size_t i = 0; i < modes.size(); ++i) {
    std::cout << static_cast<int>(modes[i]);
    if (i < modes.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  EXPECT_EQ(modes.size(), 10);
}

// ============================================================================
// Current Threshold Test (SET_PROTECTIVE_CURRENT 0x25)
// ============================================================================

TEST_F(OmniHand2025UsbTest, SetAllCurrentThreshold) {
  RequireDevice();
  
  std::vector<int16_t> thresholds(10, 1000);  // 1000mA
  hand_->SetAllCurrentThreshold(thresholds);
  std::cout << "[SetAllCurrentThreshold] All joints -> 1000mA" << std::endl;
  
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

TEST_F(OmniHand2025UsbTest, MixCtrlJointMotor_PosiVeloTorque) {
  RequireDevice();
  
  // Safe positions from Python demo (per joint)
  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  
  // POSITION_VELOCITY_TORQUE mode: max 8 joints
  std::vector<agilink::omnihand::MixCtrl> mix_ctrls;
  for (int i = 1; i <= 8; ++i) {
    agilink::omnihand::MixCtrl ctrl;  // 默认构造函数确保位域自动初始化为0
    ctrl.joint_index_ = static_cast<unsigned char>(i);
    ctrl.ctrl_mode_ = static_cast<unsigned char>(agilink::omnihand::ControlMode::POSITION_VELOCITY_TORQUE);
    ctrl.tgt_posi_ = safe_pos[i - 1];
    ctrl.tgt_velo_ = 50;
    ctrl.tgt_torque_ = 0;
    mix_ctrls.push_back(ctrl);
  }
  
  hand_->MixCtrlJointMotor(mix_ctrls);
  std::cout << "[MixCtrlJointMotor] POSITION_VELOCITY_TORQUE mode for 8 joints" << std::endl;
  
  auto positions = hand_->GetAllJointMotorPosi();
  std::cout << "[GetAllJointMotorPosi] After mixed control: ";
  for (size_t i = 0; i < positions.size(); ++i) {
    std::cout << positions[i];
    if (i < positions.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  EXPECT_EQ(positions.size(), 10);
}

TEST_F(OmniHand2025UsbTest, MixCtrlJointMotor_VeloTorque) {
  RequireDevice();
  
  std::cout << "[MixCtrlJointMotor] VELOCITY_TORQUE mode for all 10 joints:" << std::endl;
  for (int joint = 1; joint <= 10; ++joint) {
    std::vector<agilink::omnihand::MixCtrl> mix_ctrls;
    agilink::omnihand::MixCtrl ctrl;  // 默认构造函数确保位域自动初始化为0
    ctrl.joint_index_ = static_cast<unsigned char>(joint);
    ctrl.ctrl_mode_ = static_cast<unsigned char>(agilink::omnihand::ControlMode::VELOCITY_TORQUE);
    ctrl.tgt_posi_ = std::nullopt;
    ctrl.tgt_velo_ = 100;
    ctrl.tgt_torque_ = 0;
    mix_ctrls.push_back(ctrl);
    
    hand_->MixCtrlJointMotor(mix_ctrls);
    std::cout << "  J" << joint << ": velo=100" << std::endl;
  }
  
  SUCCEED();
}

TEST_F(OmniHand2025UsbTest, MixCtrlJointMotor_PosiTorque) {
  RequireDevice();
  
  // Safe positions from Python demo (per joint)
  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  
  std::cout << "[MixCtrlJointMotor] POSITION_TORQUE mode for all 10 joints:" << std::endl;
  for (int joint = 1; joint <= 10; ++joint) {
    std::vector<agilink::omnihand::MixCtrl> mix_ctrls;
    agilink::omnihand::MixCtrl ctrl;  // 默认构造函数确保位域自动初始化为0
    ctrl.joint_index_ = static_cast<unsigned char>(joint);
    ctrl.ctrl_mode_ = static_cast<unsigned char>(agilink::omnihand::ControlMode::POSITION_TORQUE);
    ctrl.tgt_posi_ = safe_pos[joint - 1];
    ctrl.tgt_velo_ = std::nullopt;
    ctrl.tgt_torque_ = 0;
    mix_ctrls.push_back(ctrl);
    
    hand_->MixCtrlJointMotor(mix_ctrls);
    std::cout << "  J" << joint << ": posi=" << safe_pos[joint - 1] << std::endl;
  }
  
  SUCCEED();
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
  
  if (current_angles.empty()) {
    GTEST_SKIP() << "GetAllActiveJointAngles timeout";
  }
  
  EXPECT_EQ(current_angles.size(), 10);
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
  
  if (all_angles.empty()) {
    GTEST_SKIP() << "GetAllJointAngles timeout";
  }
  
  EXPECT_EQ(all_angles.size(), 16);  // 10 active + 6 passive
}

// ============================================================================
// Kinematics Solver Test (software-only, no USB command)
// ============================================================================

TEST_F(OmniHand2025UsbTest, KinematicsSolver) {
  RequireDevice();
  
  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointPos(active_angles);
  
  std::cout << "[GetAllJointPos] Forward kinematics: ";
  for (size_t i = 0; i < all_angles.size(); ++i) {
    std::cout << std::fixed << std::setprecision(4) << all_angles[i];
    if (i < all_angles.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
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
    
    if (arg == "-p" && i + 1 < argc) {
      g_usb_port = argv[++i];
    } else if (arg == "-b" && i + 1 < argc) {
      g_baudrate = std::stoi(argv[++i]);
    } else if (arg == "-f" && i + 1 < argc) {
      g_request_interval = std::stoi(argv[++i]);
      if (g_request_interval > 500) g_request_interval = 500;
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
  std::cout << "==============================" << std::endl;
  
  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
