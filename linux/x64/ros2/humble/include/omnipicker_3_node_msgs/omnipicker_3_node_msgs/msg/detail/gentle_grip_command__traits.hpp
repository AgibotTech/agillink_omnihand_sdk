// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__TRAITS_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace omnipicker_3_node_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const GentleGripCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: use_current_limit
  {
    out << "use_current_limit: ";
    rosidl_generator_traits::value_to_yaml(msg.use_current_limit, out);
    out << ", ";
  }

  // member: current_limit_ma
  {
    out << "current_limit_ma: ";
    rosidl_generator_traits::value_to_yaml(msg.current_limit_ma, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GentleGripCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: use_current_limit
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "use_current_limit: ";
    rosidl_generator_traits::value_to_yaml(msg.use_current_limit, out);
    out << "\n";
  }

  // member: current_limit_ma
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_limit_ma: ";
    rosidl_generator_traits::value_to_yaml(msg.current_limit_ma, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GentleGripCommand & msg, bool use_flow_style = false)
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
  const omnipicker_3_node_msgs::msg::GentleGripCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnipicker_3_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnipicker_3_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnipicker_3_node_msgs::msg::GentleGripCommand & msg)
{
  return omnipicker_3_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnipicker_3_node_msgs::msg::GentleGripCommand>()
{
  return "omnipicker_3_node_msgs::msg::GentleGripCommand";
}

template<>
inline const char * name<omnipicker_3_node_msgs::msg::GentleGripCommand>()
{
  return "omnipicker_3_node_msgs/msg/GentleGripCommand";
}

template<>
struct has_fixed_size<omnipicker_3_node_msgs::msg::GentleGripCommand>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<omnipicker_3_node_msgs::msg::GentleGripCommand>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<omnipicker_3_node_msgs::msg::GentleGripCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__TRAITS_HPP_
