// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_pro_2025_node_msgs/msg/tactile_sensor_data.hpp"


#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__TRAITS_HPP_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace omnihand_pro_2025_node_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const TactileSensorData & msg,
  std::ostream & out)
{
  out << "{";
  // member: online_state
  {
    out << "online_state: ";
    rosidl_generator_traits::value_to_yaml(msg.online_state, out);
    out << ", ";
  }

  // member: channel_value
  {
    if (msg.channel_value.size() == 0) {
      out << "channel_value: []";
    } else {
      out << "channel_value: [";
      size_t pending_items = msg.channel_value.size();
      for (auto item : msg.channel_value) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: normal_force
  {
    out << "normal_force: ";
    rosidl_generator_traits::value_to_yaml(msg.normal_force, out);
    out << ", ";
  }

  // member: tangent_force
  {
    out << "tangent_force: ";
    rosidl_generator_traits::value_to_yaml(msg.tangent_force, out);
    out << ", ";
  }

  // member: tangent_force_angle
  {
    out << "tangent_force_angle: ";
    rosidl_generator_traits::value_to_yaml(msg.tangent_force_angle, out);
    out << ", ";
  }

  // member: capa_approach
  {
    if (msg.capa_approach.size() == 0) {
      out << "capa_approach: []";
    } else {
      out << "capa_approach: [";
      size_t pending_items = msg.capa_approach.size();
      for (auto item : msg.capa_approach) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const TactileSensorData & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: online_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "online_state: ";
    rosidl_generator_traits::value_to_yaml(msg.online_state, out);
    out << "\n";
  }

  // member: channel_value
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.channel_value.size() == 0) {
      out << "channel_value: []\n";
    } else {
      out << "channel_value:\n";
      for (auto item : msg.channel_value) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: normal_force
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "normal_force: ";
    rosidl_generator_traits::value_to_yaml(msg.normal_force, out);
    out << "\n";
  }

  // member: tangent_force
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tangent_force: ";
    rosidl_generator_traits::value_to_yaml(msg.tangent_force, out);
    out << "\n";
  }

  // member: tangent_force_angle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "tangent_force_angle: ";
    rosidl_generator_traits::value_to_yaml(msg.tangent_force_angle, out);
    out << "\n";
  }

  // member: capa_approach
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.capa_approach.size() == 0) {
      out << "capa_approach: []\n";
    } else {
      out << "capa_approach:\n";
      for (auto item : msg.capa_approach) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const TactileSensorData & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace omnihand_pro_2025_node_msgs

namespace rosidl_generator_traits
{

[[deprecated("use omnihand_pro_2025_node_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnihand_pro_2025_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnihand_pro_2025_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg)
{
  return omnihand_pro_2025_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnihand_pro_2025_node_msgs::msg::TactileSensorData>()
{
  return "omnihand_pro_2025_node_msgs::msg::TactileSensorData";
}

template<>
inline const char * name<omnihand_pro_2025_node_msgs::msg::TactileSensorData>()
{
  return "omnihand_pro_2025_node_msgs/msg/TactileSensorData";
}

template<>
struct has_fixed_size<omnihand_pro_2025_node_msgs::msg::TactileSensorData>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<omnihand_pro_2025_node_msgs::msg::TactileSensorData>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<omnihand_pro_2025_node_msgs::msg::TactileSensorData>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__TRAITS_HPP_
