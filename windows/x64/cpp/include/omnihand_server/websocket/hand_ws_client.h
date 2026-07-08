#ifndef AGILINK_OMNIHAND_HAND_WS_CLIENT_H
#define AGILINK_OMNIHAND_HAND_WS_CLIENT_H

#include <cstdint>
#include <memory>
#include <string>
#include "nlohmann/json.hpp"
#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {
namespace service {

// WebSocket client that connects to a HandWsServer.
//
// Usage:
//   HandWsClient client({"localhost", 8765});
//   client.Connect();
//   auto hands = client.ListHands();
//   auto created = client.CreateHand({...});
//   int id = created["hand_id"].get<int>();
//   auto info = client.CallMethod(id, "get_vendor_info");
//   client.RemoveHand(id);
//   client.Disconnect();
class AGIBOT_EXPORT HandWsClient {
 public:
  struct Options {
    std::string host = "localhost";
    uint16_t port = 8765;
    int connect_timeout_ms = 5000;
    int recv_timeout_ms = 10000;
  };

  explicit HandWsClient(Options options = {});
  ~HandWsClient();
  HandWsClient(const HandWsClient&) = delete;
  HandWsClient& operator=(const HandWsClient&) = delete;

  // Connect to the server. Throws std::runtime_error on failure.
  void Connect();
  void Disconnect();
  bool IsConnected() const;

  // Send a JSON request and block until the matching result arrives.
  // Adds "request_id" automatically. Returns the full response envelope.
  nlohmann::json Request(const nlohmann::json& msg);

  // High-level helpers — return the result payload on success, throw on error.
  nlohmann::json CreateHand(const nlohmann::json& spec);
  nlohmann::json RemoveHand(int hand_id);
  nlohmann::json ListHands();
  nlohmann::json DescribeMethods(int hand_id);
  nlohmann::json DescribeMethodsByProductType(const std::string& product_type);
  nlohmann::json CallMethod(int hand_id,
                             const std::string& method,
                             const nlohmann::json& params = nlohmann::json::object());

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace service
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_HAND_WS_CLIENT_H
