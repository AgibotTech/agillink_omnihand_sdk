// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file test_omnihand_2025_canfd.cc
 * @brief CANFD-specific tests for OmniHand 2025 (public factory APIs, non-private)
 *
 * Usage:
 *   ./test_omnihand_2025_canfd [-t TRANSPORT] [-c CHANNEL] [-i CANFD_ID] [-f INTERVAL] [--can-if IF] [--tcp-host H] [--tcp-port P]
 *
 *   -t TRANSPORT  Backend: zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)
 *   --can-if IF   SocketCAN interface name, socketcan only (default: can0)
 *   --tcp-host H  ZLG CANFD-over-TCP peer host, zlgcantcp only (default: 192.168.0.178)
 *   --tcp-port P  TCP port, zlgcantcp only (default: 8000)
 *   -c CHANNEL   CAN channel index for zlgcan / hcan / zlgcantcp (default: 0)
 *   -i CANFD_ID  Adapter device index for zlgcan / hcan (default: 0)
 *   -f INTERVAL  Request interval in ms (default: 5, max: 100)
 */

#include <gtest/gtest.h>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_2025.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
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
static std::string g_can_if = "can0";
static std::string g_tcp_host = "192.168.0.178";
static uint16_t g_tcp_port = 8000;

static CanfdTransport ParseTransport(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  if (s == "zlgcan") return CanfdTransport::kZlgcan;
  if (s == "hcan") return CanfdTransport::kHcan;
  if (s == "socketcan") return CanfdTransport::kSocketCan;
  if (s == "zlgcantcp" || s == "zlg_tcp" || s == "zlgcan_tcp") return CanfdTransport::kZlgCanTcp;
  std::cerr << "[Warning] unknown -t " << s << ", using zlgcan\n";
  return CanfdTransport::kZlgcan;
}

class OmniHand2025CanfdTest : public ::testing::Test {
 protected:
  void SetUp() override {
    using agilink::omnihand::HandType;
    using agilink::omnihand::OmniHand2025;
    constexpr uint8_t kHandDeviceId = 1;

    switch (g_transport) {
      case CanfdTransport::kZlgcan:
        hand_ = OmniHand2025::createHandByZlgcan(
            HandType::LEFT, kHandDeviceId,
            static_cast<uint8_t>(g_canfd_id),
            static_cast<uint8_t>(g_channel_id));
        break;
      case CanfdTransport::kHcan:
        hand_ = OmniHand2025::createHandByHcan(
            HandType::LEFT, kHandDeviceId,
            static_cast<uint8_t>(g_canfd_id),
            static_cast<uint8_t>(g_channel_id));
        break;
      case CanfdTransport::kSocketCan:
#if defined(__linux__)
        hand_ = OmniHand2025::createHandSocketCan(HandType::LEFT, kHandDeviceId, g_can_if);
#else
        std::cout << "[Warning] SocketCAN requires Linux; skipping hand creation.\n";
        hand_ = nullptr;
#endif
        break;
      case CanfdTransport::kZlgCanTcp:
#if OMNIHAND_ZLG_TCP_SUPPORTED
        hand_ = OmniHand2025::createHandByZlgCanTcp(
            HandType::LEFT, kHandDeviceId, g_tcp_host, g_tcp_port,
            static_cast<uint8_t>(g_channel_id));
#else
        std::cout << "[Warning] ZLG CANFD over TCP not supported on this platform.\n";
        hand_ = nullptr;
#endif
        break;
      default:
        hand_ = nullptr;
        break;
    }

    if (hand_) {
      hand_->SetRequestInterval(g_request_interval);
      device_available_ = hand_->Init();
      if (!device_available_) {
        std::cout << "[Warning]: CANFD device created but Init() failed." << std::endl;
      }
    }
  }

  void TearDown() override {
    hand_.reset();
  }

  void RequireDevice() {
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << "CANFD device not available";
    }
  }

  std::unique_ptr<agilink::omnihand::OmniHand2025> hand_;
  bool device_available_ = false;
};

// ============================================================================
// Basic Connection Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, CreateHand) {
  EXPECT_NE(hand_, nullptr);
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
  std::cout << vendor_info.ToString() << std::endl;
  
  if (vendor_info.dof == 0) {
    GTEST_SKIP() << "GetVendorInfo timeout";
  }
  
  EXPECT_EQ(vendor_info.dof, 10);
}

// ============================================================================
// Device ID Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetDeviceInfo) {
  RequireDevice();
  
  auto device_info = hand_->GetDeviceInfo();
  std::cout << device_info.ToString() << std::endl;
  
  if (device_info.hand_device_id == 0) {
    GTEST_SKIP() << "GetDeviceInfo timeout";
  }
  
  EXPECT_EQ(device_info.hand_device_id, 1);
}

TEST_F(OmniHand2025CanfdTest, SetDeviceId) {
  RequireDevice();
  
  auto current_info = hand_->GetDeviceInfo();
  if (current_info.hand_device_id == 0) {
    GTEST_SKIP() << "Cannot get current device ID";
  }
  
  // Change to ID 2
  hand_->SetDeviceId(2);
  std::cout << "[SetDeviceId] Set to 2" << std::endl;
  
  auto new_info = hand_->GetDeviceInfo();
  EXPECT_EQ(new_info.hand_device_id, 2);
  
  // Reset to ID 1
  hand_->SetDeviceId(1);
  std::cout << "[SetDeviceId] Reset to 1" << std::endl;
  
  auto reset_info = hand_->GetDeviceInfo();
  EXPECT_EQ(reset_info.hand_device_id, 1);
}

// ============================================================================
// Position Control Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetSingleAxisPos) {
  RequireDevice();
  
  // Position set A (different from SetGetAllAxisPos to show change)
  const int16_t safe_pos[10] = {1024, 1024, 2048, 1024, 2048, 2048, 1024, 2048, 1024, 2048};
  
  std::cout << "[SetGetSingleAxisPos] Testing all 10 joints:" << std::endl;
  for (int joint = 1; joint <= 10; ++joint) {
    int16_t target_pos = safe_pos[joint - 1];
    auto set_result = hand_->SetJointMotorPosi(joint, target_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto pos = hand_->GetJointMotorPosi(joint);
    std::cout << "  J" << joint << ": target_pos=" << target_pos << ", set_result=" << set_result << ", get_pos=" << pos << std::endl;
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
  std::cout << "[SetAllJointMotorPosi] returned " << set_result.size() << " positions: ";
  for (size_t i = 0; i < set_result.size(); ++i) {
    std::cout << set_result[i];
    if (i < set_result.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  EXPECT_EQ(set_result.size(), 10);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetAllCurrentReport) {
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
// Temperature Test
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetAllTemperatureReport) {
  RequireDevice();
  
  auto temps = hand_->GetAllTemperatureReport();
  std::cout << "[GetAllTemperatureReport] ";
  for (size_t i = 0; i < temps.size(); ++i) {
    std::cout << "J" << (i+1) << ":" << temps[i];
    if (i < temps.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  if (temps.empty()) {
    GTEST_SKIP() << "GetAllTemperatureReport timeout";
  }
  
  EXPECT_EQ(temps.size(), 10);

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
  std::cout << "[GetAllErrorReport] S:stalled, H:overheat, C:over current, M:motor exception, X: communicate exception." << std::endl;
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
  
  if (errors.empty()) {
    GTEST_SKIP() << "GetAllErrorReport timeout";
  }
  
  EXPECT_EQ(errors.size(), 10);
}

// ============================================================================
// Tactile Sensor Tests (CANFD supports both normal and raw data)
// ============================================================================

TEST_F(OmniHand2025CanfdTest, GetTactileSensorData) {
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

TEST_F(OmniHand2025CanfdTest, GetTactileSensorDataRaw) {
  RequireDevice();
  
  // CANFD supports raw tactile sensor data (multi-frame protocol)
    auto data = hand_->GetTactileSensorDataRaw(agilink::omnihand::Finger::THUMB);
  
  if (data.data_.empty()) {
    std::cout << "[GetTactileSensorDataRaw] Not supported by this firmware" << std::endl;
    GTEST_SKIP() << "Raw tactile data not supported";
  }
  
  std::cout << "[GetTactileSensorDataRaw] Thumb (" << data.data_.size() << " values): ";
  for (size_t i = 0; i < std::min(data.data_.size(), size_t(10)); ++i) {
    std::cout << static_cast<int>(data.data_[i]);
    if (i < std::min(data.data_.size(), size_t(10)) - 1) std::cout << ", ";
  }
  if (data.data_.size() > 10) std::cout << " ...";
  std::cout << std::endl;
  
  EXPECT_GT(data.data_.size(), 0);
}

TEST_F(OmniHand2025CanfdTest, GetAllTactileSensorDataRaw) {
  RequireDevice();
  
  auto all_data = hand_->GetAllTactileSensorDataRaw();
  
  if (all_data.empty()) {
    std::cout << "[GetAllTactileSensorDataRaw] Not supported by this firmware" << std::endl;
    GTEST_SKIP() << "Raw tactile data not supported";
  }
  
  std::cout << "[GetAllTactileSensorDataRaw] " << all_data.size() << " sensors" << std::endl;
  for (const auto& sensor : all_data) {
    std::cout << "  " << agilink::omnihand::ToString(sensor.sensor_id_) << " (" << sensor.data_.size() << " values): ";
    for (size_t i = 0; i < sensor.data_.size(); ++i) {
      std::cout << static_cast<int>(sensor.data_[i]);
      if (i < sensor.data_.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
  }
  
  EXPECT_GE(all_data.size(), 0);
}

// ============================================================================
// Motor Velocity Control Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetAllJointMotorVelo) {
  RequireDevice();
  
  std::vector<int16_t> velocities(10, 100);
  hand_->SetAllJointMotorVelo(velocities);
  std::cout << "[SetAllJointMotorVelo] All joints -> 100" << std::endl;
  
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto current_velo = hand_->GetAllJointMotorVelo();
  std::cout << "[GetAllJointMotorVelo] ";
  for (size_t i = 0; i < current_velo.size(); ++i) {
    std::cout << current_velo[i];
    if (i < current_velo.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  if (current_velo.empty()) {
    GTEST_SKIP() << "GetAllJointMotorVelo timeout";
  }
  
  EXPECT_EQ(current_velo.size(), 10);
}

// ============================================================================
// Current Threshold Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetAllCurrentThreshold) {
  RequireDevice();
  
  std::vector<int16_t> thresholds(10, 1000);
  hand_->SetAllCurrentThreshold(thresholds);
  std::cout << "[SetAllCurrentThreshold] All joints -> 1000mA" << std::endl;
  
  auto current_thresholds = hand_->GetAllCurrentThreshold();
  std::cout << "[GetAllCurrentThreshold] ";
  for (size_t i = 0; i < current_thresholds.size(); ++i) {
    std::cout << current_thresholds[i];
    if (i < current_thresholds.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  if (current_thresholds.empty()) {
    GTEST_SKIP() << "GetAllCurrentThreshold timeout";
  }
  
  EXPECT_EQ(current_thresholds.size(), 10);
}

// ============================================================================
// Mixed Control Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, MixControlByPVT) {
  RequireDevice();
  
  // Safe positions from Python demo (per joint)
  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> velocities(10, 50);
  std::vector<int16_t> torques(10, 0);
  
  (void)hand_->MixControlByPVT(positions, velocities, torques);
  std::cout << "[MixControlByPVT] all 10 joints" << std::endl;
  
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto positions = hand_->GetAllJointMotorPosi();
  std::cout << "[GetAllJointMotorPosi] ";
  for (size_t i = 0; i < positions.size(); ++i) {
    std::cout << positions[i];
    if (i < positions.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
  
  EXPECT_EQ(positions.size(), 10);
}

TEST_F(OmniHand2025CanfdTest, SetJointMotorVelo) {
  RequireDevice();
  
  std::cout << "[SetJointMotorVelo] per joint:" << std::endl;
  for (int joint = 1; joint <= 10; ++joint) {
    hand_->SetJointMotorVelo(static_cast<unsigned char>(joint), 100);
    std::cout << "  J" << joint << ": velo=100" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  
  SUCCEED();
}

TEST_F(OmniHand2025CanfdTest, MixControlByPT) {
  RequireDevice();
  
  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> torques(10, 0);
  
  (void)hand_->MixControlByPT(positions, torques);
  std::cout << "[MixControlByPT] all 10 joints" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  
  SUCCEED();
}

// ============================================================================
// Joint Angle Tests
// ============================================================================

TEST_F(OmniHand2025CanfdTest, SetGetAllActiveJointAngles) {
  RequireDevice();
  
  std::vector<double> angles(10, 0.0);
  hand_->SetAllActiveJointAngles(angles);
  std::cout << "[SetAllActiveJointAngles] All joints -> 0.0 rad" << std::endl;
  
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

TEST_F(OmniHand2025CanfdTest, GetAllJointAngles) {
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
  
  EXPECT_EQ(all_angles.size(), 16);
}

// ============================================================================
// Kinematics Solver Test
// ============================================================================

TEST_F(OmniHand2025CanfdTest, KinematicsSolver) {
  RequireDevice();
  
  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointAngles(active_angles);
  
  std::cout << "[GetAllJointAngles] Forward kinematics: " << all_angles.size() << " angles" << std::endl;
  
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
      std::cout << "OmniHand 2025 CANFD Test\n\n";
      std::cout << "Usage: " << argv[0] << " [options]\n\n";
      std::cout << "Options:\n";
      std::cout << "  -d, --device NAME   zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)\n";
      std::cout << "  -c CHANNEL             CAN channel (zlgcan/hcan/zlgcantcp), default 0\n";
      std::cout << "  -i CANFD_ID             device index (zlgcan/hcan), default 0\n";
      std::cout << "  --can-if IFACE         SocketCAN iface (socketcan), default can0\n";
      std::cout << "  --tcp-host HOST        ZLG TCP host (zlgcantcp), default 192.168.0.178\n";
      std::cout << "  --tcp-port PORT        ZLG TCP port (zlgcantcp), default 8000\n";
      std::cout << "  -f INTERVAL            Request interval ms, default 5, max 100\n";
      std::cout << "\nExamples:\n";
      std::cout << "  " << argv[0] << " -d zlgcan -c 0 -i 0\n";
      std::cout << "  " << argv[0] << " -d socketcan --can-if can0\n";
      std::cout << "  " << argv[0] << " -d zlgcantcp --tcp-host 192.168.0.178 --tcp-port 8000\n";
      return 0;
    } else {
      gtest_args.push_back(argv[i]);
    }
  }

  std::cout << "=== OmniHand 2025 CANFD Test ===" << std::endl;
  switch (g_transport) {
    case CanfdTransport::kZlgcan:
      std::cout << "Transport: zlgcan\n";
      break;
    case CanfdTransport::kHcan:
      std::cout << "Transport: hcan\n";
      break;
    case CanfdTransport::kSocketCan:
      std::cout << "Transport: socketcan  can_if=" << g_can_if << "\n";
      break;
    case CanfdTransport::kZlgCanTcp:
      std::cout << "Transport: zlgcantcp  " << g_tcp_host << ":" << g_tcp_port << "\n";
      break;
  }
  std::cout << "Channel ID: " << g_channel_id << std::endl;
  std::cout << "CANFD ID: " << g_canfd_id << std::endl;
  std::cout << "Request Interval: " << g_request_interval << " ms" << std::endl;
  std::cout << "================================" << std::endl;
  
  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
