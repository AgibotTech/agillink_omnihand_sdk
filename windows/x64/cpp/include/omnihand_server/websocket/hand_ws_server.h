#ifndef AGILINK_OMNIHAND_HAND_WS_SERVER_H
#define AGILINK_OMNIHAND_HAND_WS_SERVER_H

#include <cstdint>
#include <memory>
#include <string>
#include "omnihand/export_symbols.h"
#include "omnihand_server/hand_service_core.h"

namespace agilink {
namespace omnihand {
namespace service {

class AGIBOT_EXPORT HandWsServer {
 public:
  struct Options {
    std::string bind_host = "0.0.0.0";
    uint16_t port = 8765;
    int default_publish_interval_ms = 100;
  };

  explicit HandWsServer(Options options = {});
  HandWsServer(Options options, HandService* shared_service);
  ~HandWsServer();

  HandWsServer(const HandWsServer&) = delete;
  HandWsServer& operator=(const HandWsServer&) = delete;

  void Start();
  void Stop();
  bool IsRunning() const;

  HandService& service();
  const HandService& service() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace service
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_HAND_WS_SERVER_H
