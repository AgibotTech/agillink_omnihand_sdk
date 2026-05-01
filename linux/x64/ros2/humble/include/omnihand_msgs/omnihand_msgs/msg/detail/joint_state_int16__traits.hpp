// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnihand_msgs:msg/JointStateInt16.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__TRAITS_HPP_
#define OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnihand_msgs/msg/detail/joint_state_int16__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace omnihand_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const JointStateInt16 & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: name
  {
    if (msg.name.size() == 0) {
      out << "name: []";
    } else {
      out << "name: [";
      size_t pending_items = msg.name.size();
      for (auto item : msg.name) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: data
  {
    if (msg.data.size() == 0) {
      out << "data: []";
    } else {
      out << "data: [";
      size_t pending_items = msg.data.size();
      for (auto item : msg.data) {
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
  const JointStateInt16 & msg,
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

  // member: name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.name.size() == 0) {
      out << "name: []\n";
    } else {
      out << "name:\n";
      for (auto item : msg.name) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.data.size() == 0) {
      out << "data: []\n";
    } else {
      out << "data:\n";
      for (auto item : msg.data) {
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

inline std::string to_yaml(const JointStateInt16 & msg, bool use_flow_style = false)
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

}  // namespace omnihand_msgs

namespace rosidl_generator_traits
{

[[deprecated("use omnihand_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const omnihand_msgs::msg::JointStateInt16 & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnihand_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnihand_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnihand_msgs::msg::JointStateInt16 & msg)
{
  return omnihand_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnihand_msgs::msg::JointStateInt16>()
{
  return "omnihand_msgs::msg::JointStateInt16";
}

template<>
inline const char * name<omnihand_msgs::msg::JointStateInt16>()
{
  return "omnihand_msgs/msg/JointStateInt16";
}

template<>
struct has_fixed_size<omnihand_msgs::msg::JointStateInt16>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<omnihand_msgs::msg::JointStateInt16>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<omnihand_msgs::msg::JointStateInt16>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__TRAITS_HPP_
