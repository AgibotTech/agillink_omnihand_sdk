#ifndef AGILINK_OMNIHAND_HAND_REST_SERVER_H
#define AGILINK_OMNIHAND_HAND_REST_SERVER_H

#include <cstdint>
#include <memory>
#include <string>
#include "omnihand/export_symbols.h"
#include "omnihand_server/hand_service_core.h"

namespace agilink {
namespace omnihand {
namespace service {

// HTTP/1.1 REST server exposing the same hand management API as omnihand_api.
//
// Endpoints:
//   GET  /health
//   GET  /v1/hands
//   POST /v1/hands                          body: {product_type, conn_method, conn_config, ...}
//   GET  /v1/hands/{id}
//   DELETE /v1/hands/{id}
//   GET  /v1/hands/{id}/methods
//   POST /v1/hands/{id}/methods?method=X    body: {params...}
class AGIBOT_EXPORT HandRestServer {
 public:
  struct Options {
    std::string bind_host = "0.0.0.0";
    uint16_t port = 8000;
    // Non-owning pointer to share a service with another server (e.g. HandWsServer).
    // When null the REST server creates and owns its own HandService.
    HandService* shared_service = nullptr;
  };

  explicit HandRestServer(Options options = {});
  ~HandRestServer();

  HandRestServer(const HandRestServer&) = delete;
  HandRestServer& operator=(const HandRestServer&) = delete;

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

#endif  // AGILINK_OMNIHAND_HAND_REST_SERVER_H
