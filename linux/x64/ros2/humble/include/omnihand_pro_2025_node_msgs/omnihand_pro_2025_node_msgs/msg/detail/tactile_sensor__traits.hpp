// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'thumb'
// Member 'index'
// Member 'middle'
// Member 'ring'
// Member 'little'
// Member 'palm'
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__traits.hpp"

namespace omnihand_pro_2025_node_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const TactileSensor & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: thumb
  {
    out << "thumb: ";
    to_flow_style_yaml(msg.thumb, out);
    out << ", ";
  }

  // member: index
  {
    out << "index: ";
    to_flow_style_yaml(msg.index, out);
    out << ", ";
  }

  // member: middle
  {
    out << "middle: ";
    to_flow_style_yaml(msg.middle, out);
    out << ", ";
  }

  // member: ring
  {
    out << "ring: ";
    to_flow_style_yaml(msg.ring, out);
    out << ", ";
  }

  // member: little
  {
    out << "little: ";
    to_flow_style_yaml(msg.little, out);
    out << ", ";
  }

  // member: palm
  {
    out << "palm: ";
    to_flow_style_yaml(msg.palm, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const TactileSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: thumb
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "thumb:\n";
    to_block_style_yaml(msg.thumb, out, indentation + 2);
  }

  // member: index
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "index:\n";
    to_block_style_yaml(msg.index, out, indentation + 2);
  }

  // member: middle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "middle:\n";
    to_block_style_yaml(msg.middle, out, indentation + 2);
  }

  // member: ring
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ring:\n";
    to_block_style_yaml(msg.ring, out, indentation + 2);
  }

  // member: little
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "little:\n";
    to_block_style_yaml(msg.little, out, indentation + 2);
  }

  // member: palm
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "palm:\n";
    to_block_style_yaml(msg.palm, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const TactileSensor & msg, bool use_flow_style = false)
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
  const omnihand_pro_2025_node_msgs::msg::TactileSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnihand_pro_2025_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnihand_pro_2025_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnihand_pro_2025_node_msgs::msg::TactileSensor & msg)
{
  return omnihand_pro_2025_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnihand_pro_2025_node_msgs::msg::TactileSensor>()
{
  return "omnihand_pro_2025_node_msgs::msg::TactileSensor";
}

template<>
inline const char * name<omnihand_pro_2025_node_msgs::msg::TactileSensor>()
{
  return "omnihand_pro_2025_node_msgs/msg/TactileSensor";
}

template<>
struct has_fixed_size<omnihand_pro_2025_node_msgs::msg::TactileSensor>
  : std::integral_constant<bool, has_fixed_size<omnihand_pro_2025_node_msgs::msg::TactileSensorData>::value && has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<omnihand_pro_2025_node_msgs::msg::TactileSensor>
  : std::integral_constant<bool, has_bounded_size<omnihand_pro_2025_node_msgs::msg::TactileSensorData>::value && has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<omnihand_pro_2025_node_msgs::msg::TactileSensor>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_
