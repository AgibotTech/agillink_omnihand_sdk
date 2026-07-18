#ifndef AGILINK_OMNIHAND_SERVER_PROTOCOL_MESSAGES_H
#define AGILINK_OMNIHAND_SERVER_PROTOCOL_MESSAGES_H

#include <string>
#include <vector>

#include "omnihand_server/service/types.h"
#include "omnihand_server/protocol/data_value.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace protocol {

struct HandCreateRequest {
  std::string hand_type;
  std::string conn_method;
  DataValue conn_config = DataValue::EmptyObject();
};

struct CommandRequest {
  HandId hand_id = 0;
  std::string method;
  DataValue params = DataValue::EmptyObject();
};

struct WsResult {
  DataValue request_id;
  bool ok = true;
  DataValue result;
  std::string error;

  DataValue ToValue() const {
    auto payload = DataValue::ObjectOf({
        {"type", DataValue("result")},
        {"request_id", request_id},
        {"ok", DataValue(ok)},
    });
    if (ok) {
      payload.Set("result", result);
    } else {
      payload.Set("error", DataValue(error));
    }
    return payload;
  }
};

struct StreamEvent {
  std::string stream;
  HandId hand_id = 0;
  DataValue data;

  DataValue ToValue() const {
    return DataValue::ObjectOf({
        {"type", DataValue("event")},
        {"stream", DataValue(stream)},
        {"hand_id", DataValue(hand_id)},
        {"data", data},
    });
  }
};

}  // namespace protocol
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_PROTOCOL_MESSAGES_H
