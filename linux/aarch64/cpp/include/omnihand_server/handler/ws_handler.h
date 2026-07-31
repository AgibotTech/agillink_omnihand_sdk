#ifndef AGILINK_OMNIHAND_SERVER_HANDLER_WS_HANDLER_H
#define AGILINK_OMNIHAND_SERVER_HANDLER_WS_HANDLER_H

#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>

#include "crow.h"
#include "omnihand_server/service/hand_service.h"
#include "omnihand_server/protocol/data_value.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {

class WsHandler {
 public:
  explicit WsHandler(std::shared_ptr<HandService> service);

  void OnOpen(crow::websocket::connection& conn);
  void OnClose(crow::websocket::connection& conn);
  void OnMessage(crow::websocket::connection& conn, const std::string& data, bool is_binary);
  void PublishSubscribedStreams();

 private:
  struct WsClient {
    crow::websocket::connection* connection = nullptr;
    std::mutex send_mu;
    std::map<HandId, std::set<std::string>> subscriptions;
    bool alive = true;
  };

  protocol::DataValue HandleRequest(
      const protocol::DataValue& request,
      const std::shared_ptr<WsClient>& client);
  void SendWsValue(const std::shared_ptr<WsClient>& client, const protocol::DataValue& payload);

  std::shared_ptr<HandService> service_;
  std::mutex ws_mu_;
  std::unordered_map<crow::websocket::connection*, std::shared_ptr<WsClient>> clients_;
};

}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_HANDLER_WS_HANDLER_H
