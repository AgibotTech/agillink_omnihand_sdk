#ifndef AGILINK_OMNIHAND_SERVER_SERVICE_TYPES_H
#define AGILINK_OMNIHAND_SERVER_SERVICE_TYPES_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include "omnihand_server/protocol/data_value.h"

namespace agilink {
namespace omnihand {
namespace server {

using protocol::DataValue;
using HandId = int;

enum class HandSide {
  kLeft,
  kRight,
};

inline std::string ToString(HandSide side) {
  return side == HandSide::kRight ? "right" : "left";
}

inline HandSide ParseHandSide(const DataValue& value) {
  if (value.IsInt()) {
    return value.AsInt() == 1 ? HandSide::kRight : HandSide::kLeft;
  }
  if (!value.IsString()) {
    throw std::invalid_argument("hand_side must be a string or integer");
  }
  const std::string side = value.AsString();
  if (side == "right" || side == "RIGHT" || side == "Right") {
    return HandSide::kRight;
  }
  if (side == "left" || side == "LEFT" || side == "Left") {
    return HandSide::kLeft;
  }
  throw std::invalid_argument("Invalid hand_side: " + side);
}

struct ConnectionSpec {
  std::string type;
  DataValue config = DataValue::EmptyObject();
};

struct HandCreateSpec {
  std::string product_type;
  HandSide hand_side = HandSide::kLeft;
  ConnectionSpec connection;
};

struct ProductInfo {
  std::string product_type;
  std::string product_name;
  int dof = 0;
  bool supports_left_right = true;

  DataValue ToValue() const {
    return DataValue::ObjectOf({
        {"product_type", DataValue(product_type)},
        {"product_name", DataValue(product_name)},
        {"dof", DataValue(dof)},
        {"supports_left_right", DataValue(supports_left_right)},
    });
  }
};

struct Capabilities {
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

  DataValue ToValue() const {
    return DataValue::ObjectOf({
        {"supports_calibration", DataValue(supports_calibration)},
        {"supports_tactile", DataValue(supports_tactile)},
        {"supports_tactile_3d", DataValue(supports_tactile_3d)},
        {"supports_temperature", DataValue(supports_temperature)},
        {"supports_current", DataValue(supports_current)},
        {"supports_fault_state", DataValue(supports_fault_state)},
        {"supports_current_threshold", DataValue(supports_current_threshold)},
        {"supports_joint_position_cmd", DataValue(supports_joint_position_cmd)},
        {"supports_active_joint_angle_cmd", DataValue(supports_active_joint_angle_cmd)},
        {"supports_joint_velocity_cmd", DataValue(supports_joint_velocity_cmd)},
        {"supports_control_mode", DataValue(supports_control_mode)},
        {"supports_mixed_control", DataValue(supports_mixed_control)},
    });
  }
};

struct HandSummary {
  HandId hand_id = 0;
  std::string product_type;
  HandSide hand_side = HandSide::kLeft;
  std::string connection_type;
  std::string connection_key;
  ProductInfo descriptor;
  Capabilities capabilities;
  bool already_exists = false;

  DataValue ToValue() const {
    DataValue result = DataValue::ObjectOf({
        {"hand_id", DataValue(hand_id)},
        {"product_type", DataValue(product_type)},
        {"hand_type", DataValue(product_type)},
        {"hand_side", DataValue(ToString(hand_side))},
        {"connection_type", DataValue(connection_type)},
        {"conn_method", DataValue(connection_type)},
        {"connection_key", DataValue(connection_key)},
        {"descriptor", descriptor.ToValue()},
        {"capabilities", capabilities.ToValue()},
    });
    if (already_exists) {
      result.Set("already_exists", DataValue(true));
    }
    return result;
  }
};

struct Problem {
  std::string type = "about:blank";
  std::string title = "Internal Server Error";
  int status = 500;
  std::string detail;
  std::string code = "internal_error";

  DataValue ToValue() const {
    return DataValue::ObjectOf({
        {"type", DataValue(type)},
        {"title", DataValue(title)},
        {"status", DataValue(status)},
        {"detail", DataValue(detail)},
        {"code", DataValue(code)},
    });
  }
};

class ApplicationError : public std::runtime_error {
 public:
  explicit ApplicationError(Problem problem)
      : std::runtime_error(problem.detail), problem_(std::move(problem)) {}

  const Problem& problem() const { return problem_; }

 private:
  Problem problem_;
};

inline Problem MakeProblem(
    int status,
    std::string code,
    std::string title,
    std::string detail) {
  return Problem{
      "https://omnihand.local/problems/" + code,
      std::move(title),
      status,
      std::move(detail),
      std::move(code)};
}

}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_SERVICE_TYPES_H
