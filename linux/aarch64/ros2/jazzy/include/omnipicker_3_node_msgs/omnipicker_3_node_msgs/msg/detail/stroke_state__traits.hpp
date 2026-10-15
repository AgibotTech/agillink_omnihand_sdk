// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnipicker_3_node_msgs:msg/StrokeState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/stroke_state.hpp"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__TRAITS_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnipicker_3_node_msgs/msg/detail/stroke_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace omnipicker_3_node_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const StrokeState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: stroke_mm
  {
    out << "stroke_mm: ";
    rosidl_generator_traits::value_to_yaml(msg.stroke_mm, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const StrokeState & msg,
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

  // member: stroke_mm
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stroke_mm: ";
    rosidl_generator_traits::value_to_yaml(msg.stroke_mm, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const StrokeState & msg, bool use_flow_style = false)
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
  const omnipicker_3_node_msgs::msg::StrokeState & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnipicker_3_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnipicker_3_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnipicker_3_node_msgs::msg::StrokeState & msg)
{
  return omnipicker_3_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnipicker_3_node_msgs::msg::StrokeState>()
{
  return "omnipicker_3_node_msgs::msg::StrokeState";
}

template<>
inline const char * name<omnipicker_3_node_msgs::msg::StrokeState>()
{
  return "omnipicker_3_node_msgs/msg/StrokeState";
}

template<>
struct has_fixed_size<omnipicker_3_node_msgs::msg::StrokeState>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<omnipicker_3_node_msgs::msg::StrokeState>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<omnipicker_3_node_msgs::msg::StrokeState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__TRAITS_HPP_
