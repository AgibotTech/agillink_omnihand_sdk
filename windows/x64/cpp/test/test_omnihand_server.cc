// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * Tests for OmniHand server layer: HandService, HandWsServer, HandRestServer,
 * HandWsClient, HandRestClient.
 *
 * Usage:
 *   ./test_omnihand_server                        # lifecycle tests only (no device)
 *   ./test_omnihand_server -d zlgcan              # zlgcan device
 *   ./test_omnihand_server -d hcan                # HCAN device
 *   ./test_omnihand_server -d socketcan           # SocketCAN (Linux)
 *   ./test_omnihand_server --gtest_filter=*Lifecycle*   # no device needed
 *
 * Tests that require a real device: HandServiceTest, HandServiceViaWsClientTest,
 * HandServiceViaRestClientTest.  All *Lifecycle* tests need no device.
 */

#include <gtest/gtest.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "nlohmann/json.hpp"
#include "omnihand_server/hand_service_core.h"
#include "omnihand_server/restful/hand_rest_client.h"
#include "omnihand_server/restful/hand_rest_server.h"
#include "omnihand_server/websocket/hand_ws_client.h"
#include "omnihand_server/websocket/hand_ws_server.h"

using nlohmann::json;
using namespace agilink::omnihand::service;

// ============================================================
// Global test configuration
// ============================================================
static std::string g_device_type = "zlgcan";
static std::string g_uart_port;

static std::string GetDeviceType() { return g_device_type; }

static json MakeCreateHandRequest(const std::string& product_type = "omnihand_2025",
                                   const std::string& hand_side = "left") {
  json request;
  request["product_type"] = product_type;
  request["conn_method"] = GetDeviceType();
  request["hand_side"] = hand_side;
  json conn_config;
  if (GetDeviceType() == "socketcan") {
    conn_config["can_interface"] = "can0";
  } else if (GetDeviceType() == "rs485" || GetDeviceType() == "usb") {
    conn_config["uart_port"] = g_uart_port.empty() ? "/dev/ttyUSB0" : g_uart_port;
  } else {
    conn_config["canfd_device_id"] = 0;
    conn_config["canfd_channel_id"] = 0;
  }
  request["conn_config"] = conn_config;
  return request;
}

// ============================================================
// HandService Tests  (require real device)
// ============================================================
class HandServiceTest : public ::testing::Test {
 protected:
  HandService service_;
};

TEST_F(HandServiceTest, CreateAndListHand) {
  json create_req = MakeCreateHandRequest();
  std::cout << "[CreateHand] request: " << create_req.dump() << "\n";

  json result = service_.CreateHand(
      {create_req["product_type"].get<std::string>(),
       create_req["conn_method"].get<std::string>(),
       agilink::omnihand::HandType::LEFT,
       std::nullopt,
       0,
       0,
       {},
       {},
       "can0",
       {},
       460800,
       {},
       0});

  std::cout << "[CreateHand] result: " << result.dump(2) << "\n";
  EXPECT_TRUE(result.contains("hand_id") || result.contains("error"));

  if (!result.contains("hand_id")) {
    GTEST_SKIP() << "Hand creation failed (device not connected?): "
                 << result.value("error", result.dump());
  }

  int hand_id = result["hand_id"].get<int>();
  EXPECT_GE(hand_id, 1);

  json list = service_.ListHands();
  ASSERT_TRUE(list.is_array());
  EXPECT_GE(list.size(), 1u);

  json info = service_.DescribeHand(hand_id);
  EXPECT_EQ(info["hand_id"].get<int>(), hand_id);

  json methods = service_.DescribeMethods(hand_id);
  EXPECT_TRUE(methods.contains("methods"));
  EXPECT_GT(methods["methods"].size(), 0u);

  json removed = service_.RemoveHand(hand_id);
  EXPECT_TRUE(removed.contains("hand_id") || removed.contains("ok"));
}

TEST_F(HandServiceTest, CallMethodGetVendorInfo) {
  HandConnectionSpec spec;
  spec.product_type = "omnihand_2025";
  spec.conn_method = GetDeviceType();
  spec.hand_side = agilink::omnihand::HandType::LEFT;
  if (GetDeviceType() == "socketcan") spec.can_interface = "can0";

  json created = service_.CreateHand(spec);
  if (!created.contains("hand_id")) GTEST_SKIP() << "Device not connected";
  int hand_id = created["hand_id"].get<int>();

  json result = service_.CallMethod(hand_id, "get_vendor_info");
  std::cout << "[get_vendor_info]: " << result.dump(2) << "\n";
  EXPECT_TRUE(result.is_object() || result.is_array() || result.is_null());
  service_.RemoveHand(hand_id);
}

TEST_F(HandServiceTest, CallMethodGetJointNames) {
  HandConnectionSpec spec;
  spec.product_type = "omnihand_2025";
  spec.conn_method = GetDeviceType();
  spec.hand_side = agilink::omnihand::HandType::LEFT;
  if (GetDeviceType() == "socketcan") spec.can_interface = "can0";

  json created = service_.CreateHand(spec);
  if (!created.contains("hand_id")) GTEST_SKIP() << "Device not connected";
  int hand_id = created["hand_id"].get<int>();

  json result = service_.CallMethod(hand_id, "get_joint_names");
  std::cout << "[get_joint_names]: " << result.dump() << "\n";
  EXPECT_TRUE(result.is_array());
  service_.RemoveHand(hand_id);
}

TEST_F(HandServiceTest, CallMethodGetAllJointPositions) {
  HandConnectionSpec spec;
  spec.product_type = "omnihand_2025";
  spec.conn_method = GetDeviceType();
  spec.hand_side = agilink::omnihand::HandType::LEFT;
  if (GetDeviceType() == "socketcan") spec.can_interface = "can0";

  json created = service_.CreateHand(spec);
  if (!created.contains("hand_id")) GTEST_SKIP() << "Device not connected";
  int hand_id = created["hand_id"].get<int>();

  json result = service_.CallMethod(hand_id, "get_all_joint_positions");
  std::cout << "[get_all_joint_positions]: " << result.dump() << "\n";
  EXPECT_TRUE(result.is_array());
  if (!result.empty()) EXPECT_EQ(result.size(), 10u);
  service_.RemoveHand(hand_id);
}

TEST_F(HandServiceTest, RemoveNonExistentHandThrows) {
  EXPECT_THROW(service_.RemoveHand(999), std::out_of_range);
}

// ============================================================
// HandWsServer lifecycle tests  (no device required)
// ============================================================
class HandWsServerLifecycleTest : public ::testing::Test {};

TEST_F(HandWsServerLifecycleTest, StartStop) {
  HandWsServer::Options opts;
  opts.bind_host = "127.0.0.1";
  opts.port = 18765;

  HandWsServer server(opts);
  EXPECT_FALSE(server.IsRunning());
  server.Start();
  EXPECT_TRUE(server.IsRunning());
  server.Stop();
  EXPECT_FALSE(server.IsRunning());
}

TEST_F(HandWsServerLifecycleTest, ServiceAccessible) {
  HandWsServer::Options opts;
  opts.bind_host = "127.0.0.1";
  opts.port = 18766;

  HandWsServer server(opts);
  server.Start();
  ASSERT_TRUE(server.IsRunning());
  json list = server.service().ListHands();
  EXPECT_TRUE(list.is_array());
  EXPECT_EQ(list.size(), 0u);
  server.Stop();
}

TEST_F(HandWsServerLifecycleTest, StartStopMultipleTimes) {
  HandWsServer::Options opts;
  opts.bind_host = "127.0.0.1";
  opts.port = 18767;

  HandWsServer server(opts);
  for (int i = 0; i < 3; ++i) {
    server.Start();
    EXPECT_TRUE(server.IsRunning()) << "iteration " << i;
    server.Stop();
    EXPECT_FALSE(server.IsRunning()) << "iteration " << i;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

// ============================================================
// HandRestServer lifecycle tests  (no device required)
// ============================================================
class HandRestServerLifecycleTest : public ::testing::Test {};

TEST_F(HandRestServerLifecycleTest, StartStop) {
  HandRestServer::Options opts;
  opts.bind_host = "127.0.0.1";
  opts.port = 18800;

  HandRestServer server(opts);
  EXPECT_FALSE(server.IsRunning());
  server.Start();
  EXPECT_TRUE(server.IsRunning());
  server.Stop();
  EXPECT_FALSE(server.IsRunning());
}

TEST_F(HandRestServerLifecycleTest, ServiceAccessible) {
  HandRestServer::Options opts;
  opts.bind_host = "127.0.0.1";
  opts.port = 18801;

  HandRestServer server(opts);
  server.Start();
  ASSERT_TRUE(server.IsRunning());
  json list = server.service().ListHands();
  EXPECT_TRUE(list.is_array());
  EXPECT_EQ(list.size(), 0u);
  server.Stop();
}

TEST_F(HandRestServerLifecycleTest, SharedServiceWithWsServer) {
  HandWsServer::Options ws_opts;
  ws_opts.bind_host = "127.0.0.1";
  ws_opts.port = 18810;

  HandWsServer ws_server(ws_opts);
  ws_server.Start();
  ASSERT_TRUE(ws_server.IsRunning());

  HandRestServer::Options rest_opts;
  rest_opts.bind_host = "127.0.0.1";
  rest_opts.port = 18811;
  rest_opts.shared_service = &ws_server.service();

  HandRestServer rest_server(rest_opts);
  rest_server.Start();
  ASSERT_TRUE(rest_server.IsRunning());
  EXPECT_EQ(&ws_server.service(), &rest_server.service());

  rest_server.Stop();
  ws_server.Stop();
}

// ============================================================
// HandWsClient lifecycle tests  (no device required)
// ============================================================
class HandWsClientLifecycleTest : public ::testing::Test {
 protected:
  static constexpr uint16_t kPort = 18870;

  void SetUp() override {
    HandWsServer::Options opts;
    opts.bind_host = "127.0.0.1";
    opts.port = kPort;
    server_.reset(new HandWsServer(opts));
    server_->Start();
    ASSERT_TRUE(server_->IsRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  void TearDown() override { server_->Stop(); }

  std::unique_ptr<HandWsServer> server_;
};

TEST_F(HandWsClientLifecycleTest, ConnectDisconnect) {
  HandWsClient client({"127.0.0.1", kPort});
  EXPECT_FALSE(client.IsConnected());
  EXPECT_NO_THROW(client.Connect());
  EXPECT_TRUE(client.IsConnected());
  client.Disconnect();
  EXPECT_FALSE(client.IsConnected());
}

TEST_F(HandWsClientLifecycleTest, ListHandsEmpty) {
  HandWsClient client({"127.0.0.1", kPort});
  client.Connect();
  json hands = client.ListHands();
  std::cout << "[WsClient ListHands]: " << hands.dump() << "\n";
  EXPECT_TRUE(hands.is_array());
  EXPECT_EQ(hands.size(), 0u);
}

TEST_F(HandWsClientLifecycleTest, DescribeMethodsByProductType) {
  HandWsClient client({"127.0.0.1", kPort});
  client.Connect();
  json result = client.DescribeMethodsByProductType("omnihand_2025");
  std::cout << "[WsClient DescribeMethodsByProductType]: methods count="
            << result["methods"].size() << "\n";
  EXPECT_TRUE(result.contains("methods"));
  EXPECT_TRUE(result["methods"].is_array());
  EXPECT_FALSE(result["methods"].empty());
  EXPECT_EQ(result.value("product_type", ""), "omnihand_2025");
  EXPECT_TRUE(result["hand_id"].is_null());
}

TEST_F(HandWsClientLifecycleTest, DescribeMethodsByProductTypeUnknown) {
  HandWsClient client({"127.0.0.1", kPort});
  client.Connect();
  EXPECT_THROW(client.DescribeMethodsByProductType("nonexistent_product"), std::runtime_error);
}

// ============================================================
// HandRestClient lifecycle tests  (no device required)
// ============================================================
class HandRestClientLifecycleTest : public ::testing::Test {
 protected:
  static constexpr uint16_t kPort = 18880;

  void SetUp() override {
    HandRestServer::Options opts;
    opts.bind_host = "127.0.0.1";
    opts.port = kPort;
    server_.reset(new HandRestServer(opts));
    server_->Start();
    ASSERT_TRUE(server_->IsRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  void TearDown() override { server_->Stop(); }

  std::unique_ptr<HandRestServer> server_;
};

TEST_F(HandRestClientLifecycleTest, GetHealth) {
  HandRestClient client({"127.0.0.1", kPort});
  json health = client.GetHealth();
  std::cout << "[RestClient GetHealth]: " << health.dump() << "\n";
  EXPECT_EQ(health.value("status", ""), "ok");
}

TEST_F(HandRestClientLifecycleTest, ListHandsEmpty) {
  HandRestClient client({"127.0.0.1", kPort});
  json hands = client.ListHands();
  std::cout << "[RestClient ListHands]: " << hands.dump() << "\n";
  EXPECT_TRUE(hands.is_array());
  EXPECT_EQ(hands.size(), 0u);
}

TEST_F(HandRestClientLifecycleTest, DescribeMethodsByProductType) {
  HandRestClient client({"127.0.0.1", kPort});
  json result = client.DescribeMethodsByProductType("omnihand_2025");
  std::cout << "[RestClient DescribeMethodsByProductType]: methods count="
            << result["methods"].size() << "\n";
  EXPECT_TRUE(result.contains("methods"));
  EXPECT_TRUE(result["methods"].is_array());
  EXPECT_FALSE(result["methods"].empty());
  EXPECT_EQ(result.value("product_type", ""), "omnihand_2025");
}

TEST_F(HandRestClientLifecycleTest, DescribeMethodsByProductTypeUnknown) {
  HandRestClient client({"127.0.0.1", kPort});
  EXPECT_THROW(client.DescribeMethodsByProductType("nonexistent_product"), std::runtime_error);
}

// ============================================================
// HandServiceViaWsClientTest  (require real device)
// ============================================================
class HandServiceViaWsClientTest : public ::testing::Test {
 protected:
  static constexpr uint16_t kPort = 18871;

  void SetUp() override {
    HandWsServer::Options opts;
    opts.bind_host = "127.0.0.1";
    opts.port = kPort;
    server_.reset(new HandWsServer(opts));
    server_->Start();
    ASSERT_TRUE(server_->IsRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    client_.reset(new HandWsClient({"127.0.0.1", kPort}));
    client_->Connect();
  }

  void TearDown() override {
    client_->Disconnect();
    server_->Stop();
  }

  std::unique_ptr<HandWsServer> server_;
  std::unique_ptr<HandWsClient> client_;
};

TEST_F(HandServiceViaWsClientTest, CreateCallRemoveHand) {
  json spec = MakeCreateHandRequest();
  json created;
  try {
    created = client_->CreateHand(spec);
  } catch (const std::exception& e) {
    GTEST_SKIP() << "Device not connected: " << e.what();
  }
  std::cout << "[WsClient CreateHand]: " << created.dump(2) << "\n";
  ASSERT_TRUE(created.contains("hand_id"));
  int hand_id = created["hand_id"].get<int>();

  json vendor_info = client_->CallMethod(hand_id, "get_vendor_info");
  std::cout << "[WsClient get_vendor_info]: " << vendor_info.dump() << "\n";

  json joint_names = client_->CallMethod(hand_id, "get_joint_names");
  std::cout << "[WsClient get_joint_names]: " << joint_names.dump() << "\n";
  EXPECT_TRUE(joint_names.is_array());

  client_->RemoveHand(hand_id);
}

// ============================================================
// HandServiceViaRestClientTest  (require real device)
// ============================================================
class HandServiceViaRestClientTest : public ::testing::Test {
 protected:
  static constexpr uint16_t kPort = 18881;

  void SetUp() override {
    HandRestServer::Options opts;
    opts.bind_host = "127.0.0.1";
    opts.port = kPort;
    server_.reset(new HandRestServer(opts));
    server_->Start();
    ASSERT_TRUE(server_->IsRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    client_.reset(new HandRestClient({"127.0.0.1", kPort}));
  }

  void TearDown() override { server_->Stop(); }

  std::unique_ptr<HandRestServer> server_;
  std::unique_ptr<HandRestClient> client_;
};

TEST_F(HandServiceViaRestClientTest, CreateCallRemoveHand) {
  json spec = MakeCreateHandRequest();
  json created;
  try {
    created = client_->CreateHand(spec);
  } catch (const std::exception& e) {
    GTEST_SKIP() << "Device not connected: " << e.what();
  }
  std::cout << "[RestClient CreateHand]: " << created.dump(2) << "\n";
  ASSERT_TRUE(created.contains("hand_id"));
  int hand_id = created["hand_id"].get<int>();

  json vendor_info = client_->CallMethod(hand_id, "get_vendor_info");
  std::cout << "[RestClient get_vendor_info]: " << vendor_info.dump() << "\n";

  json joint_names = client_->CallMethod(hand_id, "get_joint_names");
  std::cout << "[RestClient get_joint_names]: " << joint_names.dump() << "\n";
  EXPECT_TRUE(joint_names.is_array());

  client_->RemoveHand(hand_id);
}

// ============================================================
// Custom main: parse -d / --device before handing off to gtest
// ============================================================
int main(int argc, char** argv) {
  std::vector<char*> gtest_args;
  gtest_args.push_back(argv[0]);

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-d" || arg == "--device") && i + 1 < argc) {
      std::string device_arg = argv[i + 1];
      if (device_arg == "zlgcan" || device_arg == "hcan" || device_arg == "socketcan" ||
          device_arg == "rs485" || device_arg == "usb") {
        g_device_type = device_arg;
        ++i;
        continue;
      } else {
        std::cerr << "[Error]: -d value must be zlgcan, hcan, socketcan, rs485, or usb; got: "
                  << device_arg << "\n";
        return 1;
      }
    } else if ((arg == "--uart-port") && i + 1 < argc) {
      g_uart_port = argv[++i];
      continue;
    } else if (arg == "--help" || arg == "-h") {
      std::cout << "Usage: " << argv[0] << " [-d DEVICE] [--uart-port PATH] [gtest options...]\n"
                << "  -d DEVICE          CAN/UART device type (zlgcan|hcan|socketcan|rs485|usb, default: zlgcan)\n"
                << "  --uart-port PATH   UART device path for rs485/usb (e.g. /dev/ttyUSB0)\n"
                << "                     Used by HandServiceTest and HandServiceVia*ClientTest.\n"
                << "                     *Lifecycle* tests need no device.\n\n";
    }
    gtest_args.push_back(argv[i]);
  }

  std::cout << "[Info]: Device type: " << g_device_type << "\n";
  int gtest_argc = static_cast<int>(gtest_args.size());
  ::testing::InitGoogleTest(&gtest_argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
