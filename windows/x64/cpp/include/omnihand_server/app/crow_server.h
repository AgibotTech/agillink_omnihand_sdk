#ifndef AGILINK_OMNIHAND_SERVER_APP_CROW_SERVER_H
#define AGILINK_OMNIHAND_SERVER_APP_CROW_SERVER_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "omnihand_server/service/hand_service.h"
#include "omnihand_server/handler/rest_handler.h"
#include "omnihand_server/handler/ws_handler.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {

struct CrowServerOptions {
  std::string host = "0.0.0.0";
  uint16_t port = 8000;
  int publish_interval_ms = 100;
  std::string log_level = "info";
  std::filesystem::path file_path = "";
};

class CrowServer {
 public:
  CrowServer(std::shared_ptr<HandService> service, CrowServerOptions options);
  ~CrowServer();

  CrowServer(const CrowServer&) = delete;
  CrowServer& operator=(const CrowServer&) = delete;

  void Run();
  void Stop();
  crow::App<crow::CORSHandler>& app() { return app_; }

 private:
  void ConfigureCors();

  crow::App<crow::CORSHandler> app_;
  std::shared_ptr<HandService> service_;
  CrowServerOptions options_;
  RestHandler rest_handler_;
  WsHandler ws_handler_;
};

}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_APP_CROW_SERVER_H
