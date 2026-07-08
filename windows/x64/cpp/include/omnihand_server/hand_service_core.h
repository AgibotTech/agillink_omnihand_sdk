#ifndef AGILINK_OMNIHAND_HAND_SERVICE_CORE_H
#define AGILINK_OMNIHAND_HAND_SERVICE_CORE_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "nlohmann/json.hpp"
#include "omnihand/export_symbols.h"
#include "omnihand/proto.h"

namespace agilink {
namespace omnihand {

class OmniHand;

namespace service {

struct AGIBOT_EXPORT HandConnectionSpec {
  std::string product_type;
  std::string conn_method;
  HandType hand_side = HandType::LEFT;
  std::optional<uint8_t> hand_device_id;
  uint8_t canfd_device_id = 0;
  uint8_t canfd_channel_id = 0;
  std::string usbcanfd_serial_number;
  std::string hcan_serial_number;
  std::string can_interface = "can0";
  std::string uart_port;
  int32_t baudrate = 460800;
  std::string host;
  uint16_t port = 0;
  std::string marvin_controller_ip;
};

struct AGIBOT_EXPORT HandDescriptor {
  ProductType product_type = ProductType::UNKNOWN;
  std::string product_key;
  std::string product_name;
  int dof = 0;
  bool supports_left_right = true;
};

struct AGIBOT_EXPORT HandCapabilities {
  bool supports_calibration = false;
  bool supports_tactile = false;
  bool supports_tactile_3d = false;
  bool supports_temperature = false;
  bool supports_current = false;
  bool supports_fault_state = false;
  bool supports_current_threshold = false;
  bool supports_joint_position_cmd = false;
  bool supports_active_joint_angle_cmd = false;
  bool supports_joint_velocity_cmd = false;
  bool supports_control_mode = false;
  bool supports_mixed_control = false;
};

class AGIBOT_EXPORT HandSession {
 public:
  HandSession(
      int hand_id,
      std::string product_type,
      std::string conn_method,
      std::string conn_key,
      HandType hand_side,
      std::unique_ptr<OmniHand> hand,
      HandDescriptor descriptor,
      HandCapabilities capabilities);
  ~HandSession();

  HandSession(const HandSession&) = delete;
  HandSession& operator=(const HandSession&) = delete;

  int hand_id() const { return hand_id_; }
  const std::string& product_type() const { return product_type_; }
  const std::string& conn_method() const { return conn_method_; }
  const std::string& conn_key() const { return conn_key_; }
  HandType hand_side() const { return hand_side_; }
  const HandDescriptor& descriptor() const { return descriptor_; }
  const HandCapabilities& capabilities() const { return capabilities_; }

  nlohmann::json ToJson() const;
  nlohmann::json SnapshotState() const;
  nlohmann::json SnapshotDiagnostics() const;
  nlohmann::json SnapshotTactile() const;
  nlohmann::json DescribeMethods() const;
  nlohmann::json CallMethod(
      const std::string& method_name,
      const nlohmann::json& params = nlohmann::json::object());

 private:
  OmniHand* hand() const { return hand_.get(); }

  int hand_id_;
  std::string product_type_;
  std::string conn_method_;
  std::string conn_key_;
  HandType hand_side_;
  std::unique_ptr<OmniHand> hand_;
  HandDescriptor descriptor_;
  HandCapabilities capabilities_;
  mutable std::mutex sdk_mutex_;
};

class AGIBOT_EXPORT HandService {
 public:
  HandService() = default;

  nlohmann::json CreateHand(const HandConnectionSpec& spec);
  nlohmann::json RemoveHand(int hand_id);
  nlohmann::json ListHands() const;
  nlohmann::json DescribeHand(int hand_id) const;
  nlohmann::json DescribeMethods(int hand_id) const;
  nlohmann::json DescribeMethodsByProductType(const std::string& product_type) const;
  nlohmann::json CallMethod(
      int hand_id,
      const std::string& method_name,
      const nlohmann::json& params = nlohmann::json::object());
  nlohmann::json ReadStream(int hand_id, const std::string& stream) const;
  nlohmann::json GetSupportedTypes(const std::string& query_type) const;
  nlohmann::json ProcessMessage(const nlohmann::json& request);

 private:
  HandSession& RequireSession(int hand_id);
  const HandSession& RequireSession(int hand_id) const;

  mutable std::mutex mu_;
  int next_hand_id_ = 1;
  std::unordered_map<int, std::unique_ptr<HandSession>> sessions_;
};

}  // namespace service
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_HAND_SERVICE_CORE_H
