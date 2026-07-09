#ifndef AGILINK_OMNIHAND_SERVER_HANDLER_HTTP_RESPONSE_H
#define AGILINK_OMNIHAND_SERVER_HANDLER_HTTP_RESPONSE_H

#include <exception>

#include "crow.h"
#include "omnihand_server/service/types.h"
#include "omnihand_server/protocol/data_value.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {

crow::response JsonResponse(const DataValue& payload, int status = 200);
crow::response DetailResponse(const std::string& detail, int status);
crow::response ProblemResponse(const Problem& problem);
crow::response HandleException(const std::exception& exc);
crow::response RedirectToHandsSlash();

}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_HANDLER_HTTP_RESPONSE_H
