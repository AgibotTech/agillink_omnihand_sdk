#ifndef AGILINK_OMNIHAND_SERVER_ADAPTER_HAND_DEVICE_H
#define AGILINK_OMNIHAND_SERVER_ADAPTER_HAND_DEVICE_H

#include <memory>
#include <string>
#include <vector>

#include "omnihand_server/service/types.h"

namespace agilink {
namespace omnihand {
namespace server {

class IHandDevice {
 public:
  virtual ~IHandDevice() = default;

  virtual ProductInfo Descriptor() const = 0;
  virtual Capabilities GetCapabilities() const = 0;
  virtual DataValue DescribeCommands() const = 0;
  virtual DataValue SnapshotState() = 0;
  virtual DataValue SnapshotDiagnostics() = 0;
  virtual DataValue SnapshotTactile() = 0;
  virtual DataValue RunCommand(const std::string& name, const DataValue& params) = 0;
};

class IHandDeviceFactory {
 public:
  virtual ~IHandDeviceFactory() = default;

  virtual std::vector<ProductInfo> ListProducts() const = 0;
  virtual std::vector<std::string> ListConnectionTypes() const = 0;
  virtual DataValue ConnectionConfigs() const = 0;
  virtual DataValue CommandsForProduct(const std::string& product_type) const = 0;
  virtual std::unique_ptr<IHandDevice> Create(const HandCreateSpec& spec) = 0;
};

}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_ADAPTER_HAND_DEVICE_H
