// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnipicker_3_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace omnipicker_3_node_msgs
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
    if (msg.thumb.size() == 0) {
      out << "thumb: []";
    } else {
      out << "thumb: [";
      size_t pending_items = msg.thumb.size();
      for (auto item : msg.thumb) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: index
  {
    if (msg.index.size() == 0) {
      out << "index: []";
    } else {
      out << "index: [";
      size_t pending_items = msg.index.size();
      for (auto item : msg.index) {
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
    if (msg.thumb.size() == 0) {
      out << "thumb: []\n";
    } else {
      out << "thumb:\n";
      for (auto item : msg.thumb) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: index
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.index.size() == 0) {
      out << "index: []\n";
    } else {
      out << "index:\n";
      for (auto item : msg.index) {
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

}  // namespace omnipicker_3_node_msgs

namespace rosidl_generator_traits
{

[[deprecated("use omnipicker_3_node_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const omnipicker_3_node_msgs::msg::TactileSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnipicker_3_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnipicker_3_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnipicker_3_node_msgs::msg::TactileSensor & msg)
{
  return omnipicker_3_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnipicker_3_node_msgs::msg::TactileSensor>()
{
  return "omnipicker_3_node_msgs::msg::TactileSensor";
}

template<>
inline const char * name<omnipicker_3_node_msgs::msg::TactileSensor>()
{
  return "omnipicker_3_node_msgs/msg/TactileSensor";
}

template<>
struct has_fixed_size<omnipicker_3_node_msgs::msg::TactileSensor>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<omnipicker_3_node_msgs::msg::TactileSensor>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<omnipicker_3_node_msgs::msg::TactileSensor>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_
