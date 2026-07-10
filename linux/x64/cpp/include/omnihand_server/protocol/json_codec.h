#ifndef AGILINK_OMNIHAND_SERVER_PROTOCOL_JSON_CODEC_H
#define AGILINK_OMNIHAND_SERVER_PROTOCOL_JSON_CODEC_H

#include <string>

#include "nlohmann/json.hpp"
#include "omnihand_server/protocol/data_value.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace protocol {

DataValue DataValueFromJson(const nlohmann::json& json);
nlohmann::json DataValueToJson(const DataValue& value);

DataValue ParseJsonText(const std::string& text);
std::string DumpJsonText(const DataValue& value);

}  // namespace protocol
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_PROTOCOL_JSON_CODEC_H
