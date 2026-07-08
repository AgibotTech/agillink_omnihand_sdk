// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * WebSocket client demo for OmniHand Server.
 *
 * Start the server first:
 *   ./omnihand_ws_server 127.0.0.1 8765
 *
 * Then run this demo:
 *   ./demo_omnihand_ws_client --host 127.0.0.1 --port 8765 -d zlgcan
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include "nlohmann/json.hpp"
#include "omnihand_server/websocket/hand_ws_client.h"

using nlohmann::json;
using agilink::omnihand::service::HandWsClient;

static void PrintUsage(const char* prog) {
  std::cout << "Usage: " << prog << " [--host HOST] [--port PORT] [-d DEVICE] [--uart-port DEV]\n"
            << "  --host HOST        Server host (default: 127.0.0.1)\n"
            << "  --port PORT        Server port (default: 8765)\n"
            << "  -d DEVICE          Device type: zlgcan|hcan|socketcan|rs485|usb (default: zlgcan)\n"
            << "  --uart-port DEV    UART device path for rs485/usb (e.g. /dev/ttyUSB0)\n";
}

int main(int argc, char** argv) {
  std::string host = "127.0.0.1";
  uint16_t port = 8765;
  std::string device_type = "zlgcan";
  std::string uart_port;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--host" && i + 1 < argc) {
      host = argv[++i];
    } else if (arg == "--port" && i + 1 < argc) {
      port = static_cast<uint16_t>(std::stoi(argv[++i]));
    } else if (arg == "-d" && i + 1 < argc) {
      device_type = argv[++i];
    } else if (arg == "--uart-port" && i + 1 < argc) {
      uart_port = argv[++i];
    } else if (arg == "--help" || arg == "-h") {
      PrintUsage(argv[0]);
      return 0;
    }
  }

  try {
    HandWsClient client(HandWsClient::Options{host, port});
    std::cout << "Connecting to ws://" << host << ":" << port << " ...\n";
    client.Connect();
    std::cout << "Connected.\n";

    // Query methods by product type (no device required)
    json pt_methods = client.DescribeMethodsByProductType("omnihand_2025");
    std::cout << "Methods for omnihand_2025 (" << pt_methods["methods"].size()
              << " methods): ";
    for (const auto& m : pt_methods["methods"]) {
      std::cout << m["name"].get<std::string>() << " ";
    }
    std::cout << "\n";

    // List existing hands
    json hands = client.ListHands();
    std::cout << "Existing hands: " << hands.dump() << "\n";

    // Build CreateHand spec
    json spec;
    spec["product_type"] = "omnihand_2025";
    spec["conn_method"] = device_type;
    spec["hand_side"] = "left";
    json conn_config;
    if (device_type == "socketcan") {
      conn_config["can_interface"] = "can0";
    } else if (device_type == "rs485" || device_type == "usb") {
      if (uart_port.empty()) {
        throw std::invalid_argument("--uart-port is required for " + device_type);
      }
      conn_config["uart_port"] = uart_port;
    } else {
      conn_config["canfd_device_id"] = 0;
      conn_config["canfd_channel_id"] = 0;
    }
    spec["conn_config"] = conn_config;

    std::cout << "Creating hand with spec: " << spec.dump() << "\n";
    json created = client.CreateHand(spec);
    std::cout << "CreateHand result: " << created.dump(2) << "\n";
    int hand_id = created["hand_id"].get<int>();

    json vendor_info = client.CallMethod(hand_id, "get_vendor_info");
    std::cout << "get_vendor_info: " << vendor_info.dump(2) << "\n";

    json joint_names = client.CallMethod(hand_id, "get_joint_names");
    std::cout << "get_joint_names: " << joint_names.dump() << "\n";

    json positions = client.CallMethod(hand_id, "get_all_joint_positions");
    std::cout << "get_all_joint_positions: " << positions.dump() << "\n";

    client.RemoveHand(hand_id);
    std::cout << "Hand " << hand_id << " removed.\n";

    client.Disconnect();
    std::cout << "Disconnected.\n";
  } catch (const std::exception& e) {
    std::cerr << "[Error]: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
