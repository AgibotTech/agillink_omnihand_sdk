#ifndef AGILINK_OMNIHAND_SERVER_SERVICE_HAND_SERVICE_H
#define AGILINK_OMNIHAND_SERVER_SERVICE_HAND_SERVICE_H

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "omnihand_server/service/types.h"
#include "omnihand_server/adapter/hand_device.h"

namespace agilink {
namespace omnihand {
namespace server {

class HandService {
 public:
  explicit HandService(std::shared_ptr<IHandDeviceFactory> factory);

  DataValue Health() const;
  DataValue ListProducts() const;
  DataValue ListConnectionTypes() const;
  DataValue ConnectionConfigs() const;
  DataValue CommandsForProduct(const std::string& product_type) const;

  DataValue CreateHand(const DataValue& request);
  DataValue CreateHand(const HandCreateSpec& spec);
  DataValue ListHands() const;
  DataValue DescribeHand(HandId hand_id) const;
  DataValue DeleteHand(HandId hand_id);
  DataValue DescribeMethods(HandId hand_id) const;
  DataValue GetCapabilities(HandId hand_id) const;
  DataValue GetState(HandId hand_id) const;
  DataValue GetDiagnostics(HandId hand_id) const;
  DataValue GetTactile(HandId hand_id) const;
  DataValue RunCommand(HandId hand_id, const std::string& name, const DataValue& params);
  DataValue ReadStream(HandId hand_id, const std::string& stream) const;

  static HandCreateSpec ParseCreateSpec(const DataValue& request);
  static std::string BuildConnectionKey(const HandCreateSpec& spec);

 private:
  struct Session {
    HandSummary summary;
    std::shared_ptr<IHandDevice> device;
  };

  Session RequireSession(HandId hand_id) const;

  std::shared_ptr<IHandDeviceFactory> factory_;
  mutable std::mutex mu_;
  HandId next_hand_id_ = 1;
  std::unordered_map<HandId, Session> sessions_;
  std::unordered_map<std::string, HandId> connection_index_;
};

}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_SERVICE_HAND_SERVICE_H
