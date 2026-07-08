#ifndef AGILINK_OMNIHAND_HAND_REST_CLIENT_H
#define AGILINK_OMNIHAND_HAND_REST_CLIENT_H

#include <cstdint>
#include <memory>
#include <string>
#include "nlohmann/json.hpp"
#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {
namespace service {

// HTTP/REST client that connects to a HandRestServer.
//
// Each method opens a short-lived TCP connection, sends one HTTP request,
// reads the response, and returns the JSON body. Status >= 400 throws.
//
// Usage:
//   HandRestClient client({"localhost", 8000});
//   auto hands  = client.ListHands();
//   auto result = client.CreateHand({...});
//   int id = result["hand_id"].get<int>();
//   auto info   = client.CallMethod(id, "get_vendor_info");
//   client.RemoveHand(id);
class AGIBOT_EXPORT HandRestClient {
 public:
  struct Options {
    std::string host = "localhost";
    uint16_t port = 8000;
    int timeout_ms = 10000;
  };

  explicit HandRestClient(Options options = {});
  ~HandRestClient();
  HandRestClient(const HandRestClient&) = delete;
  HandRestClient& operator=(const HandRestClient&) = delete;

  nlohmann::json GetHealth();
  nlohmann::json CreateHand(const nlohmann::json& spec);
  nlohmann::json RemoveHand(int hand_id);
  nlohmann::json ListHands();
  nlohmann::json DescribeHand(int hand_id);
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

#endif  // AGILINK_OMNIHAND_HAND_REST_CLIENT_H
