#ifndef AGILINK_OMNIHAND_SERVER_ROUTER_WS_ROUTER_H
#define AGILINK_OMNIHAND_SERVER_ROUTER_WS_ROUTER_H

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "omnihand_server/handler/ws_handler.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {

void RegisterWsRoutes(crow::App<crow::CORSHandler>& app, WsHandler& handler);

}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_ROUTER_WS_ROUTER_H
