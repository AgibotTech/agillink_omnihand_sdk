// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/stroke_command.hpp"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__TRAITS_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnipicker_3_node_msgs/msg/detail/stroke_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace omnipicker_3_node_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const StrokeCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: stroke_mm
  {
    out << "stroke_mm: ";
    rosidl_generator_traits::value_to_yaml(msg.stroke_mm, out);
    out << ", ";
  }

  // member: use_speed
  {
    out << "use_speed: ";
    rosidl_generator_traits::value_to_yaml(msg.use_speed, out);
    out << ", ";
  }

  // member: speed_rpm
  {
    out << "speed_rpm: ";
    rosidl_generator_traits::value_to_yaml(msg.speed_rpm, out);
    out << ", ";
  }

  // member: use_current
  {
    out << "use_current: ";
    rosidl_generator_traits::value_to_yaml(msg.use_current, out);
    out << ", ";
  }

  // member: current_ma
  {
    out << "current_ma: ";
    rosidl_generator_traits::value_to_yaml(msg.current_ma, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const StrokeCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: stroke_mm
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stroke_mm: ";
    rosidl_generator_traits::value_to_yaml(msg.stroke_mm, out);
    out << "\n";
  }

  // member: use_speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "use_speed: ";
    rosidl_generator_traits::value_to_yaml(msg.use_speed, out);
    out << "\n";
  }

  // member: speed_rpm
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "speed_rpm: ";
    rosidl_generator_traits::value_to_yaml(msg.speed_rpm, out);
    out << "\n";
  }

  // member: use_current
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "use_current: ";
    rosidl_generator_traits::value_to_yaml(msg.use_current, out);
    out << "\n";
  }

  // member: current_ma
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_ma: ";
    rosidl_generator_traits::value_to_yaml(msg.current_ma, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const StrokeCommand & msg, bool use_flow_style = false)
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

}  // namespace omnipicker_3_node_msgs

namespace rosidl_generator_traits
{

[[deprecated("use omnipicker_3_node_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const omnipicker_3_node_msgs::msg::StrokeCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnipicker_3_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnipicker_3_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnipicker_3_node_msgs::msg::StrokeCommand & msg)
{
  return omnipicker_3_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnipicker_3_node_msgs::msg::StrokeCommand>()
{
  return "omnipicker_3_node_msgs::msg::StrokeCommand";
}

template<>
inline const char * name<omnipicker_3_node_msgs::msg::StrokeCommand>()
{
  return "omnipicker_3_node_msgs/msg/StrokeCommand";
}

template<>
struct has_fixed_size<omnipicker_3_node_msgs::msg::StrokeCommand>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<omnipicker_3_node_msgs::msg::StrokeCommand>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<omnipicker_3_node_msgs::msg::StrokeCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__TRAITS_HPP_
