#ifndef AGILINK_OMNIHAND_SERVER_HANDLER_REST_HANDLER_H
#define AGILINK_OMNIHAND_SERVER_HANDLER_REST_HANDLER_H

#include <memory>
#include <string>

#include "crow.h"
#include "omnihand_server/service/hand_service.h"
#include "omnihand_server/protocol/data_value.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {

class RestHandler {
 public:
  explicit RestHandler(std::shared_ptr<HandService> service);

  crow::response Health() const;
  crow::response VendorInfo() const;
  crow::response GetHandType(const crow::request& req) const;
  crow::response GetMethodsByProductType(const crow::request& req) const;
  crow::response GetHand(HandId hand_id) const;
  crow::response DeleteHand(HandId hand_id) const;
  crow::response GetMethodsByHandId(HandId hand_id) const;
  crow::response CallMethodByHandId(const crow::request& req, HandId hand_id) const;
  crow::response HandsCollection(const crow::request& req) const;
  crow::response NotFound() const;

 private:
  protocol::DataValue ParsePythonCreateBody(const crow::request& req) const;
  protocol::DataValue ParseJsonBody(const crow::request& req) const;

  std::shared_ptr<HandService> service_;
};

}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_HANDLER_REST_HANDLER_H
