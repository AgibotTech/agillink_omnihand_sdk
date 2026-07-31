#ifndef AGILINK_OMNIHAND_SERVER_ADAPTER_SDK_HAND_DEVICE_FACTORY_H
#define AGILINK_OMNIHAND_SERVER_ADAPTER_SDK_HAND_DEVICE_FACTORY_H

#include <memory>
#include <string>
#include <vector>

#include "omnihand_server/adapter/hand_device.h"

namespace agilink {
namespace omnihand {
namespace server {

class SdkHandDeviceFactory final : public IHandDeviceFactory {
 public:
  std::vector<ProductInfo> ListProducts() const override;
  std::vector<std::string> ListConnectionTypes() const override;
  DataValue ConnectionConfigs() const override;
  DataValue CommandsForProduct(const std::string& product_type) const override;
  std::unique_ptr<IHandDevice> Create(const HandCreateSpec& spec) override;
};

}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_ADAPTER_SDK_HAND_DEVICE_FACTORY_H
