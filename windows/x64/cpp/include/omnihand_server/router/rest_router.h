#ifndef AGILINK_OMNIHAND_SERVER_ROUTER_REST_ROUTER_H
#define AGILINK_OMNIHAND_SERVER_ROUTER_REST_ROUTER_H

#include "crow.h"
#include "crow/middlewares/cors.h"
#include "omnihand_server/handler/rest_handler.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {

void RegisterRestRoutes(crow::App<crow::CORSHandler>& app, RestHandler& handler);

}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_ROUTER_REST_ROUTER_H
