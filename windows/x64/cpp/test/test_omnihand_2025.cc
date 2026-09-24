// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file test_omnihand_2025.cc
 * @brief Unified OmniHand 2025 (O10) hardware tests (CANFD + USB).
 *
 * Merged from test_omnihand_2025.cc / test_omnihand_2025_canfd.cc /
 * test_omnihand_2025_usb.cc.
 *
 * Transport is selected with -d/--device:
 *   zlgcan | hcan | socketcan | zlgcantcp | usb | rs485   (default: zlgcan)
 *
 * Usage:
 *   ./test_omnihand_2025 [options] [-- gtest_options]
 *
 * Common options:
 *   -d, --device NAME  Backend: zlgcan | hcan | socketcan | zlgcantcp | usb
 *                      (default: zlgcan)
 *   -f INTERVAL        Request interval ms. CAN default 5 (max 100),
 *                      USB default 500 (max 500). Value applies to the
 *                      selected transport.
 *   --device-id ID     Target hand device ID (default: 1), unicast range
 *                      [1, 0x7f]. SetUp discovers the current ID via
 *                      broadcast, switches the device to this value, and
 *                      TearDown restores the original ID afterwards.
 *
 * CAN options (zlgcan / hcan / socketcan / zlgcantcp):
 *   -c CHANNEL         CAN channel index (zlgcan/hcan/zlgcantcp), default 0
 *   -i CANFD_ID        Adapter device index (zlgcan/hcan), default 0
 *   --can-if IFACE     SocketCAN interface name (socketcan), default can0
 *   --tcp-host HOST    ZLG CANFD-over-TCP peer host (zlgcantcp),
 *                      default 192.168.0.178
 *   --tcp-port PORT    ZLG TCP port (zlgcantcp), default 8000
 *
 * USB/RS485 options:
 *   -p PORT            Serial port (USB default: Windows COM3, Linux
 *                      /dev/ttyACM0; RS485 default: Windows COM5,
 *                      Linux /dev/ttyUSB0)
 *   -b BAUDRATE        Baudrate (default 460800)
 *   -t MS              Frame receive timeout ms (default 200)
 *
 * Examples:
 *   ./test_omnihand_2025 -d zlgcan -c 0 -i 0
 *   ./test_omnihand_2025 -d socketcan --can-if can0
 *   ./test_omnihand_2025 -d zlgcantcp --tcp-host 192.168.0.178 --tcp-port 8000
 *   ./test_omnihand_2025 -d usb -p /dev/ttyACM0
 *   ./test_omnihand_2025 -d rs485 -p /dev/ttyUSB0
 */

#include <gtest/gtest.h>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "omnihand/export_symbols.h"
#include "omnihand/omnihand_2025.h"
#include "agilink_logger.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand2025Test";

enum class Transport {
  kZlgcan,
  kHcan,
  kSocketCan,
  kZlgCanTcp,
  kUsb,
  kRs485,
};

// CANFD Global configuration
static Transport g_transport = Transport::kZlgcan;
static int g_channel_id = 0;
static int g_canfd_id = 0;
static int g_request_interval = -1;  // -1 = not set, use transport default
static std::string g_can_if = "can0";
static std::string g_tcp_host = "192.168.0.178";
static uint16_t g_tcp_port = 8000;

// USB/RS485 Global configuration (default port: Windows COM3/COM5,
// Linux /dev/ttyACM0 for USB, /dev/ttyUSB0 for RS485)
#if defined(_WIN32)
static std::string g_usb_port = "COM3";
static std::string g_rs485_port = "COM5";
#else
static std::string g_usb_port = "/dev/ttyACM0";
static std::string g_rs485_port = "/dev/ttyUSB0";
#endif
static int g_baudrate = 460800;
// Frame receive timeout (ms), applied to ALL transports via SetFrameRecvTimeout().
// Why 200 instead of the SDK CAN default (can_bus_device.h
// DEFAULT_FRAME_RECV_TIMEOUT_MS = 50): aggregate queries such as
// GetAllErrorReport require the hand firmware to poll all 10 motors in-hand
// before replying — measured ~84ms on real hardware, which exceeds the 50ms
// default and causes a spurious "request timeout" with the valid reply frame
// arriving right after the assertion failure. 200ms covers such aggregate
// commands with margin. Tune via -t if needed.
static int g_frame_recv_timeout = 200;

// Target hand device ID (--device-id). SetUp discovers the current ID via
// broadcast, switches to this value, and TearDown restores the original ID.
static int g_device_id = 1;

static const char* TransportName(Transport t) {
  switch (t) {
    case Transport::kZlgcan:    return "zlgcan";
    case Transport::kHcan:      return "hcan";
    case Transport::kSocketCan: return "socketcan";
    case Transport::kZlgCanTcp: return "zlgcantcp";
    case Transport::kUsb:       return "USB";
    case Transport::kRs485:     return "RS485";
  }
  return "unknown";
}

// USB and RS485 share OmniHand2025SerialImpl: both are private-protocol
// serial transports, so StreamCmd (0x01~0xCD) tests apply to both.
static bool IsUsbOrRs485Transport() {
  return g_transport == Transport::kUsb || g_transport == Transport::kRs485;
}

// Request interval: CAN default 5ms (max 100), USB/RS485 default 500ms (max 500).
static int EffectiveRequestInterval() {
  const int fallback = IsUsbOrRs485Transport() ? 500 : 5;
  const int max_val = IsUsbOrRs485Transport() ? 500 : 100;
  int value = (g_request_interval >= 0) ? g_request_interval : fallback;
  if (value > max_val) value = max_val;
  return value;
}

static Transport ParseTransport(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  if (s == "zlgcan") return Transport::kZlgcan;
  if (s == "hcan") return Transport::kHcan;
  if (s == "socketcan") return Transport::kSocketCan;
  if (s == "zlgcantcp" || s == "zlg_tcp" || s == "zlgcan_tcp") return Transport::kZlgCanTcp;
  if (s == "usb") return Transport::kUsb;
  if (s == "rs485") return Transport::kRs485;
  AgilinkLogger::get().warnf(TAG, "[Warning] unknown device type '%s', using zlgcan", s.c_str());
  return Transport::kZlgcan;
}

class OmniHand2025Test : public ::testing::Test {
 protected:
  void SetUp() override {
    using agilink::omnihand::HandType;
    using agilink::omnihand::OmniHand2025;
    constexpr uint8_t kHandDeviceId = 1;

    try {
      switch (g_transport) {
        case Transport::kZlgcan:
          hand_ = OmniHand2025::createHandByZlgcan(
              HandType::LEFT, kHandDeviceId,
              static_cast<uint8_t>(g_canfd_id),
              static_cast<uint8_t>(g_channel_id));
          break;
        case Transport::kHcan:
          hand_ = OmniHand2025::createHandByHcan(
              HandType::LEFT, kHandDeviceId,
              static_cast<uint8_t>(g_canfd_id),
              static_cast<uint8_t>(g_channel_id));
          break;
        case Transport::kSocketCan:
#if defined(__linux__)
          hand_ = OmniHand2025::createHandSocketCan(HandType::LEFT, kHandDeviceId, g_can_if);
#else
          AgilinkLogger::get().warnf(TAG, "[Warning] SocketCAN requires Linux; skipping hand creation.");
          hand_ = nullptr;
#endif
          break;
        case Transport::kZlgCanTcp:
#if OMNIHAND_ZLG_TCP_SUPPORTED
          hand_ = OmniHand2025::createHandByZlgCanTcp(
              HandType::LEFT, kHandDeviceId, g_tcp_host, g_tcp_port,
              static_cast<uint8_t>(g_channel_id));
#else
          AgilinkLogger::get().warnf(TAG, "[Warning] ZLG CANFD over TCP not supported on this platform.");
          hand_ = nullptr;
#endif
          break;
        case Transport::kUsb:
          hand_ = OmniHand2025::createHandByUsb(
              HandType::LEFT, kHandDeviceId, g_usb_port, g_baudrate);
          break;
        case Transport::kRs485:
          hand_ = OmniHand2025::createHandByRs485(
              HandType::LEFT, kHandDeviceId, g_rs485_port, g_baudrate);
          break;
        default:
          hand_ = nullptr;
          break;
      }
    } catch (const std::exception& e) {
      AgilinkLogger::get().warnf(TAG, "[Warning]: Failed to create hand: %s", e.what());
      hand_.reset();
    }

    if (hand_) {
      hand_->SetRequestInterval(EffectiveRequestInterval());
      // Frame receive timeout applies to all transports: aggregate queries
      // (e.g. GetAllErrorReport polls all motors in-hand) can legitimately
      // take longer than the 50ms CAN default
      // (can_bus_device.h DEFAULT_FRAME_RECV_TIMEOUT_MS). See
      // g_frame_recv_timeout above for the full rationale.
      hand_->SetFrameRecvTimeout(g_frame_recv_timeout);
      device_available_ = hand_->Init();
      if (!device_available_) {
        AgilinkLogger::get().warnf(TAG, "[Warning]: %s device created but Init() failed.",
                                   TransportName(g_transport));
      }
      hand_->ShowDataDetails(true);
    }
    if (!hand_ || !device_available_) {
      GTEST_SKIP() << TransportName(g_transport) << " device not available";
    }

    // Discover and cache the actual standard-protocol ID before any test
    // changes it, then switch to the target ID. TearDown restores the original.
    original_device_id_ = hand_->GetNonPrivateHandDeviceIdByBroadcast();
    if (original_device_id_ == agilink::omnihand::kBroadcastHandDeviceId) {
      GTEST_SKIP() << "No standard-protocol device responded to broadcast";
    }
    AgilinkLogger::get().infof(TAG, "[SetUp] Discovered device ID: %d, switching to: %d",
                               static_cast<int>(original_device_id_), g_device_id);
    if (static_cast<uint8_t>(g_device_id) != original_device_id_) {
      original_private_device_id_ = hand_->GetPrivateHandDeviceIdByBroadcast();
      ASSERT_GT(original_private_device_id_, 0u);
      ASSERT_LT(original_private_device_id_, agilink::omnihand::kPrivateBroadcastHandDeviceId)
          << "No private-protocol device responded to broadcast";
      device_id_change_attempted_ = true;
      ASSERT_TRUE(hand_->SetHandDeviceIdByBroadcast(static_cast<uint8_t>(g_device_id)))
          << "Failed to set the standard/private protocol device IDs by broadcast";
    }
  }

  void TearDown() override {
    if (hand_ && device_available_ && device_id_change_attempted_) {
      AgilinkLogger::get().infof(
          TAG, "[TearDown] Restoring device IDs: %d -> standard=%d, private=%u",
          g_device_id, static_cast<int>(original_device_id_),
          static_cast<unsigned int>(original_private_device_id_));
      // Restore through broadcast as the two-step setup operation can fail
      // after changing only one protocol ID. First align both protocols with
      // the original private ID, then restore the standard ID independently.
      EXPECT_TRUE(hand_->SetHandDeviceIdByBroadcast(
          static_cast<uint8_t>(original_private_device_id_)));
      if (original_device_id_ != original_private_device_id_) {
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
  bool device_id_change_attempted_ = false;
  uint8_t original_device_id_ = 0;
  uint16_t original_private_device_id_ = 0;
};

// Guards must be macros, not member functions: GTEST_SKIP() expands to
// `return`, which only exits the enclosing function. Inside a helper the test
// body would keep running after the guard and fail on the wrong transport.
#define REQUIRE_USB()                                                     \
  do {                                                                    \
    if (!IsUsbOrRs485Transport()) {                                       \
      GTEST_SKIP() << "USB/RS485-only test: current transport is "        \
                   << TransportName(g_transport);                         \
    }                                                                     \
  } while (0)

// ============================================================================
// Basic Connection Tests
// ============================================================================

TEST_F(OmniHand2025Test, CreateHand) {
  RequireDevice();
  ASSERT_TRUE(device_available_) << "hand object failed to initialize";
}

TEST_F(OmniHand2025Test, Init) {
  RequireDevice();
  EXPECT_TRUE(device_available_);
}

// ============================================================================
// Vendor Info
// ============================================================================

TEST_F(OmniHand2025Test, GetVendorInfo) {
  RequireDevice();

  auto vendor_info = hand_->GetVendorInfo();
  AgilinkLogger::get().infof(TAG, "[GetVendorInfo] Vendor Info:\n%s", vendor_info.ToString().c_str());

  ASSERT_NE(vendor_info.dof, 0) << "GetVendorInfo timed out";
  EXPECT_EQ(vendor_info.dof, 10);
  EXPECT_FALSE(vendor_info.productModel.empty());
}

// ============================================================================
// Device ID Tests
// ============================================================================

TEST_F(OmniHand2025Test, GetDeviceInfo) {
  RequireDevice();

  auto device_info = hand_->GetDeviceInfo();
  AgilinkLogger::get().infof(TAG, "[GetDeviceInfo] Device Info:\n%s", device_info.ToString().c_str());

  ASSERT_NE(device_info.hand_device_id, 0) << "GetDeviceInfo timed out";
  EXPECT_EQ(device_info.hand_device_id, static_cast<uint8_t>(g_device_id));
}

// SetDeviceId is exercised by SetUp: it switches the device to g_device_id
// via broadcast, and TearDown restores the ORIGINAL ID (not a hardcoded 1),
// so the device is always left in its initial state on success.
// This case just verifies the physical ID actually took effect.
TEST_F(OmniHand2025Test, SetDeviceId) {
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

TEST_F(OmniHand2025Test, SetGetSingleAxisPos) {
  RequireDevice();

  // Position set A (different from SetGetAllAxisPos to show change)
  const int16_t safe_pos[10] = {1024, 1024, 2048, 1024, 2048, 2048, 1024, 2048, 1024, 2048};

  AgilinkLogger::get().infof(TAG, "[SetGetSingleAxisPos] Testing all 10 joints:");
  for (int joint = 1; joint <= 10; ++joint) {
    int16_t target_pos = safe_pos[joint - 1];
    auto set_result = hand_->SetJointMotorPosi(joint, target_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto pos = hand_->GetJointMotorPosi(joint);
    AgilinkLogger::get().infof(TAG, "  J%d: target_pos=%d, set_result=%d, get_pos=%d",
                               joint, target_pos, set_result, pos);
    EXPECT_GE(pos, 0);
    EXPECT_LE(pos, 4096);
  }
}

TEST_F(OmniHand2025Test, SetGetAllAxisPos) {
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

TEST_F(OmniHand2025Test, GetAllCurrentReport) {
  RequireDevice();

  auto currents = hand_->GetAllCurrentReport();
  {
    std::string msg = "[GetAllCurrentReport] ";
    for (size_t i = 0; i < currents.size(); ++i) {
      msg += "J" + std::to_string(i + 1) + ":" + std::to_string(currents[i]) + "mA";
      if (i < currents.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(currents.size(), 10u) << "GetAllCurrentReport timed out or returned an invalid size";
}

// Single-joint current query (USB private protocol).
TEST_F(OmniHand2025Test, GetCurrentReport) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[GetCurrentReport] All joints:");
  for (int i = 1; i <= 10; ++i) {
    auto current = hand_->GetCurrentReport(static_cast<unsigned char>(i));
    AgilinkLogger::get().infof(TAG, "  J%d: %dmA", i, current);
  }
}

// ============================================================================
// Temperature Test
// ============================================================================

TEST_F(OmniHand2025Test, GetAllTemperatureReport) {
  RequireDevice();

  auto temps = hand_->GetAllTemperatureReport();
  // Use ASCII "degC" — Unicode degree sign breaks on Windows consoles (GBK shows as garbled).
  {
    std::string msg = "[GetAllTemperatureReport] ";
    for (size_t i = 0; i < temps.size(); ++i) {
      msg += "J" + std::to_string(i + 1) + ":" + std::to_string(static_cast<int>(temps[i])) + " degC";
      if (i < temps.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_EQ(temps.size(), 10u) << "GetAllTemperatureReport timed out or returned an invalid size";

  // Temperature is int8_t (-128 to 127 degC), typical motor temp: 30-80 degC
  for (auto temp : temps) {
    EXPECT_GE(temp, -40);  // Extreme cold environment
    EXPECT_LE(temp, 127);  // int8_t max
  }
}

// Single-joint temperature query (USB private protocol).
TEST_F(OmniHand2025Test, GetTemperatureReport) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[GetTemperatureReport] All joints:");
  for (int i = 1; i <= 10; ++i) {
    auto temp = hand_->GetTemperatureReport(static_cast<unsigned char>(i));
    AgilinkLogger::get().infof(TAG, "  J%d: %d degC", i, temp);
    EXPECT_GE(temp, -40);
    EXPECT_LE(temp, 127);
  }
}

// ============================================================================
// Error Report Test
// ============================================================================

// Note: aggregate query — the hand firmware polls all 10 motors in-hand
// before replying (~84ms measured on real hardware), so this test relies on
// the elevated frame receive timeout set in SetUp() (g_frame_recv_timeout,
// default 200ms). With the SDK CAN default of 50ms this test fails
// spuriously: the reply frame arrives right after the timeout expires.
TEST_F(OmniHand2025Test, GetAllErrorReport) {
  RequireDevice();

  auto errors = hand_->GetAllErrorReport();
  AgilinkLogger::get().infof(TAG, "[GetAllErrorReport] S:stalled, H:overheat, C:over current, M:motor exception, X: communicate exception.");
  {
    std::string msg;
    for (size_t i = 0; i < errors.size(); ++i) {
      msg += "J" + std::to_string(i + 1) + ":[";
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

// Single-joint error query (USB private protocol).
TEST_F(OmniHand2025Test, GetErrorReport) {
  REQUIRE_USB();

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
// Tactile Sensor Tests (both normal and raw data)
// ============================================================================

TEST_F(OmniHand2025Test, GetTactileSensorData) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetTactileSensorData] Testing all tactile sensors:");

  // Test finger sensors (Thumb, Index, Middle, Ring, Little) - 16 values each
  std::vector<agilink::omnihand::Finger> fingers = {
      agilink::omnihand::Finger::THUMB, agilink::omnihand::Finger::INDEX,
      agilink::omnihand::Finger::MIDDLE, agilink::omnihand::Finger::RING,
      agilink::omnihand::Finger::LITTLE};

  AgilinkLogger::get().infof(TAG, "  Fingers (16 values each):");
  for (auto finger : fingers) {
    auto data = hand_->GetTactileSensorData(finger);
    {
      std::string msg = "    " + agilink::omnihand::ToString(finger) + ": ";
      for (size_t i = 0; i < data.size(); ++i) {
        msg += std::to_string(static_cast<int>(data[i]));
        if (i < data.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
    EXPECT_EQ(data.size(), 16);
  }

  // Test palm/dorsum sensors - 25 values each
  std::vector<agilink::omnihand::Finger> palm_dorsum = {
      agilink::omnihand::Finger::PALM, agilink::omnihand::Finger::DORSUM};

  AgilinkLogger::get().infof(TAG, "  Palm/Dorsum (25 values each):");
  for (auto sensor : palm_dorsum) {
    auto data = hand_->GetTactileSensorData(sensor);
    {
      std::string msg = "    " + agilink::omnihand::ToString(sensor) + ": ";
      for (size_t i = 0; i < data.size(); ++i) {
        msg += std::to_string(static_cast<int>(data[i]));
        if (i < data.size() - 1) msg += ", ";
      }
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
    EXPECT_EQ(data.size(), 25);
  }
}

// All downsampled tactile sensors in one call (USB private protocol).
TEST_F(OmniHand2025Test, GetAllTactileSensorData) {
  REQUIRE_USB();

  auto all_data = hand_->GetAllTactileSensorData();
  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorData] %zu sensors:", all_data.size());
  for (const auto& d : all_data) {
    AgilinkLogger::get().infof(TAG, "  %s: %zu bytes",
                               agilink::omnihand::ToString(d.sensor_id_).c_str(),
                               d.data_.size());
  }
  EXPECT_EQ(all_data.size(), 7u);
  for (const auto& d : all_data) {
    EXPECT_FALSE(d.data_.empty());
  }
}

TEST_F(OmniHand2025Test, GetTactileSensorDataRaw) {
  RequireDevice();

  // Multi-frame raw protocol; not supported by all firmware versions.
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

// All raw tactile sensors (USB iterates all 7 sensors individually).
TEST_F(OmniHand2025Test, GetAllTactileSensorDataRaw) {
  RequireDevice();

  auto all_data = hand_->GetAllTactileSensorDataRaw();

  if (all_data.empty()) {
    AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] Not supported by this firmware");
    GTEST_SKIP() << "Raw tactile data not supported";
  }

  AgilinkLogger::get().infof(TAG, "[GetAllTactileSensorDataRaw] %zu sensors", all_data.size());
  for (const auto& sensor : all_data) {
    std::string msg = "  " + agilink::omnihand::ToString(sensor.sensor_id_)
                    + " (" + std::to_string(sensor.data_.size()) + " values): ";
    for (size_t i = 0; i < sensor.data_.size(); ++i) {
      msg += std::to_string(static_cast<int>(sensor.data_[i]));
      if (i < sensor.data_.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  ASSERT_FALSE(all_data.empty());
}

// ============================================================================
// Motor Velocity Tests
// ============================================================================

TEST_F(OmniHand2025Test, GetAllJointMotorVelo) {
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

TEST_F(OmniHand2025Test, GetJointMotorVelo) {
  RequireDevice();

  AgilinkLogger::get().infof(TAG, "[GetJointMotorVelo] per joint:");
  for (int joint = 1; joint <= 10; ++joint) {
    auto velo = hand_->GetJointMotorVelo(static_cast<unsigned char>(joint));
    AgilinkLogger::get().infof(TAG, "  J%d: velo=%d", joint, velo);
  }

  // CAN-only assertion (from canfd source): invalid joint indices return -1.
  // USB/RS485 serial impl does not validate indices and returns 0.
  if (!IsUsbOrRs485Transport()) {
    EXPECT_EQ(hand_->GetJointMotorVelo(0), -1);
    EXPECT_EQ(hand_->GetJointMotorVelo(11), -1);
  }
}

// ============================================================================
// Current Threshold Tests
// ============================================================================

TEST_F(OmniHand2025Test, SetGetAllCurrentThreshold) {
  RequireDevice();

  // Destructive: SetAllCurrentThreshold is disabled by default.
  // std::vector<int16_t> thresholds(10, 1500);
  // hand_->SetAllCurrentThreshold(thresholds);
  // AgilinkLogger::get().infof(TAG, "[SetAllCurrentThreshold] All joints -> 1500mA");

  // Note: USB private protocol has no GET command and CAN firmware without
  // register 0x03 support times out — both return an empty vector by design.
  auto current_thresholds = hand_->GetAllCurrentThreshold();
  if (current_thresholds.empty()) {
    AgilinkLogger::get().warnf(TAG, "[GetAllCurrentThreshold] empty (register unsupported by this firmware/transport)");
    GTEST_SKIP() << "GetAllCurrentThreshold not supported by this firmware/transport";
  }
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

TEST_F(OmniHand2025Test, MixControlByPVT) {
  RequireDevice();

  // Safe positions from Python demo (per joint)
  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  // USB/RS485 (serial source): vel=50, torque=0. CAN (canfd source): vel=8000, torque=300.
  std::vector<int16_t> velocities(10, IsUsbOrRs485Transport() ? 50 : 8000);
  std::vector<int16_t> torques(10, IsUsbOrRs485Transport() ? 0 : 300);

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

  // CAN-only single-joint PVT (from canfd source): USB/RS485 private protocol
  // replies do not carry the optional posi/velo/torque fields.
  if (!IsUsbOrRs485Transport()) {
    auto single_result = hand_->MixControlByPVT(1, safe_pos[0], 50, 0);
    ASSERT_TRUE(single_result.tgt_posi_.has_value());
    ASSERT_TRUE(single_result.tgt_velo_.has_value());
    ASSERT_TRUE(single_result.tgt_torque_.has_value());
    // O10 CAN replies expose the protocol joint number (1-based) after parsing.
    EXPECT_EQ(single_result.joint_index_, 1);
    AgilinkLogger::get().infof(TAG, "[MixControlByPVT] single joint 1 pos=%d vel=50", safe_pos[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

TEST_F(OmniHand2025Test, MixControlByPT) {
  RequireDevice();

  const int16_t safe_pos[10] = {2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096};
  std::vector<int16_t> positions(safe_pos, safe_pos + 10);
  std::vector<int16_t> torques(10, 0);

  auto result = hand_->MixControlByPT(positions, torques);
  ASSERT_EQ(result.size(), positions.size())
      << "MixControlByPT returned an unexpected number of joint results";
  AgilinkLogger::get().infof(TAG, "[MixControlByPT] all 10 joints");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // CAN-only single-joint PT (from canfd source): USB/RS485 private protocol
  // replies do not carry the optional posi/torque fields.
  if (!IsUsbOrRs485Transport()) {
    auto single_result = hand_->MixControlByPT(1, safe_pos[0], 0);
    ASSERT_TRUE(single_result.tgt_posi_.has_value());
    ASSERT_TRUE(single_result.tgt_torque_.has_value());
    // O10 CAN replies expose the protocol joint number (1-based) after parsing.
    EXPECT_EQ(single_result.joint_index_, 1);
    AgilinkLogger::get().infof(TAG, "[MixControlByPT] single joint 1 pos=%d", safe_pos[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

// ============================================================================
// Joint Angle Tests
// ============================================================================

TEST_F(OmniHand2025Test, SetGetAllActiveJointAngles) {
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

TEST_F(OmniHand2025Test, GetAllJointAngles) {
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

TEST_F(OmniHand2025Test, KinematicsSolver) {
  RequireDevice();

  std::vector<double> active_angles(10, 0.0);
  auto all_angles = hand_->GetAllJointAngles(active_angles);

  AgilinkLogger::get().infof(TAG, "[GetAllJointAngles] Forward kinematics: %zu angles", all_angles.size());

  EXPECT_EQ(all_angles.size(), 16);
}

// ============================================================================
// StreamCmd Tests (USB private protocol, 0x01~0xCD)
// ============================================================================

// 0x01/0x02: Power state
TEST_F(OmniHand2025Test, StreamCmdPowerState) {
  REQUIRE_USB();

  EXPECT_EQ(hand_->GetRequestInterval(), EffectiveRequestInterval());
  EXPECT_EQ(hand_->GetFrameRecvTimeout(), g_frame_recv_timeout);

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing power state commands(0x01/0x02):");
  EXPECT_TRUE(hand_->SetPowerState(1));
  EXPECT_LE(hand_->GetPowerState(), 2u);

  // 0x03/0x04/0x05: SetAxisHoming + SetId + SaveParam, too dangerous
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping axis homing and ID commands (0x03/0x04/0x05) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetAxisHoming(0, 0));
  // EXPECT_TRUE(hand_->SetId(0));
  // EXPECT_TRUE(hand_->SaveParam());
}

// 0x06/0x07: Single axis position
TEST_F(OmniHand2025Test, StreamCmdSingleAxisPos) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing single axis pos commands(0x06/0x07):");
  for (int i = 1; i <= agilink::omnihand::OmniHand2025::kDegreesOfActiveFreedom; ++i) {
    uint16_t origin_pos = hand_->GetSingleAxisPos(i);
    uint16_t target_pos = 512;
    uint16_t reply_pos = hand_->SetSingleAxisPos(i, target_pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Wait for the position to take effect
    uint16_t read_pos = hand_->GetSingleAxisPos(i);
    AgilinkLogger::get().infof(TAG, "  Joint %d: origin=%u, set=%u, reply=%u, read=%u",
                               i, origin_pos, target_pos, reply_pos, read_pos);
    EXPECT_LE(reply_pos, 4096u) << "invalid reply position for joint " << i;
    EXPECT_LE(read_pos, 4096u) << "invalid read position for joint " << i;
  }
}

// 0x08/0x09: All axis position
TEST_F(OmniHand2025Test, StreamCmdAllAxisPos) {
  REQUIRE_USB();

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
TEST_F(OmniHand2025Test, StreamCmdCurrentVelTemp) {
  REQUIRE_USB();

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
TEST_F(OmniHand2025Test, StreamCmdErrorAndAction) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis error code commands(0x0D):");
  EXPECT_GE(hand_->GetErrorCode(), 0u);
  (void)hand_->ClearError();

  // 0x0F (dangerous action)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis action commands(0x0F):");
  (void)hand_->PlayAction(1);
}

// 0x10: Position range
TEST_F(OmniHand2025Test, StreamCmdPosRange) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis pos range commands(0x10):");
  const auto pos_range = hand_->GetAllAxisPosRange();
  ASSERT_FALSE(pos_range.empty()) << "GetAllAxisPosRange timed out";
  EXPECT_EQ(pos_range.size(), 10u);
  {
    std::string msg = "  Position Ranges: ";
    for (size_t i = 0; i < pos_range.size(); ++i) {
      msg += std::to_string(pos_range[i]);
      if (i < pos_range.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }
}

// 0x11~0x14: Tactile sensors
TEST_F(OmniHand2025Test, StreamCmdTactileSensors) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis tactile sensors commands(0x11):");
  for (int i = 1; i <= 7; ++i) {
    const auto fingertip0 = hand_->GetFingertipSensor(i);
    EXPECT_FALSE(fingertip0.empty());
    {
      std::string msg = "  Sensor " + std::to_string(i) + ": ";
      for (size_t j = 0; j < fingertip0.size(); ++j) {
        msg += std::to_string(static_cast<int>(fingertip0[j])) + " ";
      }
      AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
    }
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
TEST_F(OmniHand2025Test, StreamCmdRunMode) {
  REQUIRE_USB();

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
TEST_F(OmniHand2025Test, StreamCmdLoadData) {
  REQUIRE_USB();

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
TEST_F(OmniHand2025Test, StreamCmdMotorSensorIds) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis ID commands(0x26):");
  const auto motor_ids = hand_->GetAllElectricMotorId();
  ASSERT_FALSE(motor_ids.empty()) << "GetAllElectricMotorId timed out";
  EXPECT_EQ(motor_ids.size(), 10u);
  {
    std::string msg = "  Motor IDs: ";
    for (size_t i = 0; i < motor_ids.size(); ++i) {
      msg += std::to_string(static_cast<int>(motor_ids[i]));
      if (i < motor_ids.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis ID commands(0x27):");
  const auto sensor_ids = hand_->GetAllSensorId();
  ASSERT_FALSE(sensor_ids.empty()) << "GetAllSensorId timed out";
  EXPECT_EQ(sensor_ids.size(), 7u);
  {
    std::string msg = "  Sensor IDs: ";
    for (size_t i = 0; i < sensor_ids.size(); ++i) {
      msg += std::to_string(static_cast<int>(sensor_ids[i]));
      if (i < sensor_ids.size() - 1) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // 0x28 set all axis CVP upload interval (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping all axis CVP upload interval command (0x28) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetAllAxisCvpUploadInterval(100));
}

// 0x29: CVP data
TEST_F(OmniHand2025Test, StreamCmdCVP) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis CVP commands(0x29):");
  const auto cvp = hand_->GetAllAxisCvp();
  ASSERT_FALSE(cvp.empty()) << "GetAllAxisCvp timed out";
  EXPECT_EQ(cvp.size(), 60u);
}

// 0x30: Axis limit positions
TEST_F(OmniHand2025Test, StreamCmdAxisLimits) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis limit position commands(0x30):");
  const auto axis_limits = hand_->GetAxisLimitPos();
  ASSERT_FALSE(axis_limits.min_limits.empty()) << "GetAxisLimitPos timed out";
  EXPECT_EQ(axis_limits.min_limits.size(), 10u);
  EXPECT_EQ(axis_limits.max_limits.size(), 10u);
  {
    std::string msg = "  Axis Limits (min/max per joint, 0-4095): ";
    for (size_t i = 0; i < axis_limits.min_limits.size(); ++i) {
      msg += "J" + std::to_string(i + 1) + "[" + std::to_string(axis_limits.min_limits[i])
           + "," + std::to_string(axis_limits.max_limits[i]) + "]";
      if (i + 1 < axis_limits.min_limits.size()) msg += ", ";
    }
    AgilinkLogger::get().infof(TAG, "%s", msg.c_str());
  }

  // 0x31 set right/left hand type (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping right/left hand type command (0x31) due to potential hardware risk.");
  // EXPECT_TRUE(hand_->SetRightOrLeft(0));
}

// 0x32: Pos/speed/cur data
TEST_F(OmniHand2025Test, StreamCmdPosSpeedCur) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing all axis pos/speed/cur commands(0x32):");
  std::vector<uint16_t> ps_positions(10, 2048);
  std::vector<int16_t> ps_speeds(10, 0);
  std::vector<uint16_t> ps_torques(10, 0);
  const agilink::omnihand::SetAllAxisPosResponse pos_speed_torque_resp =
      hand_->SetPosSpeedTorqueData(ps_positions, ps_speeds, ps_torques);
  ASSERT_FALSE(pos_speed_torque_resp.positions.empty()) << "SetPosSpeedTorqueData failed";
  EXPECT_EQ(pos_speed_torque_resp.positions.size(), 10u);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  const auto all_pos_after_ps = hand_->GetAllAxisPos();
  ASSERT_FALSE(all_pos_after_ps.empty()) << "GetAllAxisPos after SetPosSpeedTorqueData timed out";
  EXPECT_EQ(all_pos_after_ps.size(), 10u);
  for (size_t i = 0; i < pos_speed_torque_resp.positions.size(); ++i) {
    AgilinkLogger::get().infof(TAG, "  J%zu: set_pos=%u, set_speed=%d, set_torque=%d, reply_pos=%u, read_pos=%u",
                               i + 1, ps_positions[i], ps_speeds[i], ps_torques[i],
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
TEST_F(OmniHand2025Test, StreamCmdControlSource) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing control source query command(0x81):");
  EXPECT_EQ(hand_->GetControlSource(), 0u);
  AgilinkLogger::get().infof(TAG, "  Control Source: %d", static_cast<int>(hand_->GetControlSource()));

  // 0xC1 set product serial number (dangerous)
  AgilinkLogger::get().infof(TAG, "[StreamCmd] Skipping set product serial number command (0xC1) due to potential hardware risk.");
  // std::vector<uint8_t> serial_number(19, 0);
  // (void)hand_->SetProductSerialNumber(serial_number);
}

// 0xC2: Product serial number
TEST_F(OmniHand2025Test, StreamCmdProductSerialNumber) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing get product serial number command(0xC2):");
  const auto prod_serial = hand_->GetProductSerialNumber();
  EXPECT_FALSE(prod_serial.ToString().empty());
  AgilinkLogger::get().infof(TAG, "  Product Serial Number: %s", prod_serial.ToString().c_str());
}

// 0xCD: Firmware version
TEST_F(OmniHand2025Test, StreamCmdFirmwareVersion) {
  REQUIRE_USB();

  AgilinkLogger::get().infof(TAG, "[StreamCmd] Testing get firmware version command(0xCD):");
  const auto fw = hand_->GetFwVersion();
  EXPECT_EQ(fw.dof, 10);
  AgilinkLogger::get().infof(TAG, "  Firmware Version: %s", fw.ToString().c_str());
}

// ============================================================================
// Main
// ============================================================================

static void PrintUsage(const char* prog) {
  AgilinkLogger::get().infof(TAG, "OmniHand 2025 Unified Test (CANFD + USB + RS485)");
  AgilinkLogger::get().infof(TAG, "Usage: %s [options] [-- gtest_options]", prog);
  AgilinkLogger::get().infof(TAG, "Options:");
  AgilinkLogger::get().infof(TAG, "  -d, --device NAME  Backend: zlgcan | hcan | socketcan | zlgcantcp | usb | rs485 (default: zlgcan)");
  AgilinkLogger::get().infof(TAG, "  -f INTERVAL        Request interval ms (CAN: default 5, max 100; USB/RS485: default 500, max 500)");
  AgilinkLogger::get().infof(TAG, "  --device-id ID     Target hand device ID (default: 1); SetUp discovers & switches, TearDown restores");
  AgilinkLogger::get().infof(TAG, "CAN options (zlgcan/hcan/socketcan/zlgcantcp):");
  AgilinkLogger::get().infof(TAG, "  -c CHANNEL         CAN channel (zlgcan/hcan/zlgcantcp), default 0");
  AgilinkLogger::get().infof(TAG, "  -i CANFD_ID        Device index (zlgcan/hcan), default 0");
  AgilinkLogger::get().infof(TAG, "  --can-if IFACE     SocketCAN iface (socketcan), default can0");
  AgilinkLogger::get().infof(TAG, "  --tcp-host HOST    ZLG TCP host (zlgcantcp), default 192.168.0.178");
  AgilinkLogger::get().infof(TAG, "  --tcp-port PORT    ZLG TCP port (zlgcantcp), default 8000");
  AgilinkLogger::get().infof(TAG, "USB/RS485 options:");
#if defined(_WIN32)
  AgilinkLogger::get().infof(TAG, "  -p PORT            Serial port (USB default: COM3; RS485 default: COM5)");
#else
  AgilinkLogger::get().infof(TAG, "  -p PORT            Serial port (USB default: /dev/ttyACM0; RS485 default: /dev/ttyUSB0)");
#endif
  AgilinkLogger::get().infof(TAG, "  -b BAUDRATE        Baudrate (default 460800)");
  AgilinkLogger::get().infof(TAG, "  -t MS              Frame receive timeout ms, all transports (default 200)");
  AgilinkLogger::get().infof(TAG, "Examples:");
  AgilinkLogger::get().infof(TAG, "  %s -d zlgcan -c 0 -i 0", prog);
  AgilinkLogger::get().infof(TAG, "  %s -d socketcan --can-if can0", prog);
  AgilinkLogger::get().infof(TAG, "  %s -d zlgcantcp --tcp-host 192.168.0.178 --tcp-port 8000", prog);
  AgilinkLogger::get().infof(TAG, "  %s -d usb -p /dev/ttyACM0", prog);
  AgilinkLogger::get().infof(TAG, "  %s -d rs485 -p /dev/ttyUSB0", prog);
  AgilinkLogger::get().flush();
}

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
    } else if ((arg == "--can-if") && i + 1 < argc) {
      g_can_if = argv[++i];
    } else if ((arg == "--tcp-host") && i + 1 < argc) {
      g_tcp_host = argv[++i];
    } else if ((arg == "--tcp-port") && i + 1 < argc) {
      g_tcp_port = static_cast<uint16_t>(std::stoi(argv[++i]));
    } else if (arg == "-p" && i + 1 < argc) {
      // Serial port for USB (/dev/ttyACM0) and RS485 (/dev/ttyUSB0).
      if (g_transport == Transport::kRs485) {
        g_rs485_port = argv[++i];
      } else {
        g_usb_port = argv[++i];
      }
    } else if (arg == "-b" && i + 1 < argc) {
      g_baudrate = std::stoi(argv[++i]);
    } else if (arg == "-t" && i + 1 < argc) {
      // Frame receive timeout ms; must comfortably exceed the hand's
      // aggregate-reply latency (~84ms, e.g. GetAllErrorReport), see
      // g_frame_recv_timeout declaration.
      g_frame_recv_timeout = std::stoi(argv[++i]);
    } else if (arg == "--help" || arg == "-h") {
      PrintUsage(argv[0]);
      return 0;
    } else {
      gtest_args.push_back(argv[i]);
    }
  }

  if (g_device_id < 1 || g_device_id > 0x7F) {
    AgilinkLogger::get().errorf(
        TAG, "Invalid --device-id %d; expected a unicast ID in [1, 0x7f]", g_device_id);
    return 1;
  }

  AgilinkLogger::get().infof(TAG, "=== OmniHand 2025 Unified Test ===");
  AgilinkLogger::get().infof(TAG, "Transport: %s", TransportName(g_transport));
  switch (g_transport) {
    case Transport::kSocketCan:
      AgilinkLogger::get().infof(TAG, "  can_if=%s", g_can_if.c_str());
      break;
    case Transport::kZlgCanTcp:
      AgilinkLogger::get().infof(TAG, "  %s:%d", g_tcp_host.c_str(), g_tcp_port);
      break;
    case Transport::kUsb:
      AgilinkLogger::get().infof(TAG, "  port=%s, baudrate=%d", g_usb_port.c_str(), g_baudrate);
      break;
    case Transport::kRs485:
      AgilinkLogger::get().infof(TAG, "  port=%s, baudrate=%d", g_rs485_port.c_str(), g_baudrate);
      break;
    case Transport::kZlgcan:
    case Transport::kHcan:
      AgilinkLogger::get().infof(TAG, "  channel=%d, canfd_id=%d", g_channel_id, g_canfd_id);
      break;
  }
  AgilinkLogger::get().infof(TAG, "Request Interval: %d ms", EffectiveRequestInterval());
  if (IsUsbOrRs485Transport()) {
    AgilinkLogger::get().infof(TAG, "Frame Recv Timeout: %d ms", g_frame_recv_timeout);
  }
  AgilinkLogger::get().infof(TAG, "Target Device ID: %d", g_device_id);
  AgilinkLogger::get().infof(TAG, "===================================");
  AgilinkLogger::get().flush();

  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
