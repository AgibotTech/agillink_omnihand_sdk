// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_2025_node_msgs/msg/tactile_sensor.hpp"


#ifndef OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_
#define OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace omnihand_2025_node_msgs
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
    out << ", ";
  }

  // member: middle
  {
    if (msg.middle.size() == 0) {
      out << "middle: []";
    } else {
      out << "middle: [";
      size_t pending_items = msg.middle.size();
      for (auto item : msg.middle) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: ring
  {
    if (msg.ring.size() == 0) {
      out << "ring: []";
    } else {
      out << "ring: [";
      size_t pending_items = msg.ring.size();
      for (auto item : msg.ring) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: little
  {
    if (msg.little.size() == 0) {
      out << "little: []";
    } else {
      out << "little: [";
      size_t pending_items = msg.little.size();
      for (auto item : msg.little) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: palm
  {
    if (msg.palm.size() == 0) {
      out << "palm: []";
    } else {
      out << "palm: [";
      size_t pending_items = msg.palm.size();
      for (auto item : msg.palm) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: dorsum
  {
    if (msg.dorsum.size() == 0) {
      out << "dorsum: []";
    } else {
      out << "dorsum: [";
      size_t pending_items = msg.dorsum.size();
      for (auto item : msg.dorsum) {
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

  // member: middle
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.middle.size() == 0) {
      out << "middle: []\n";
    } else {
      out << "middle:\n";
      for (auto item : msg.middle) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: ring
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.ring.size() == 0) {
      out << "ring: []\n";
    } else {
      out << "ring:\n";
      for (auto item : msg.ring) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: little
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.little.size() == 0) {
      out << "little: []\n";
    } else {
      out << "little:\n";
      for (auto item : msg.little) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: palm
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.palm.size() == 0) {
      out << "palm: []\n";
    } else {
      out << "palm:\n";
      for (auto item : msg.palm) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: dorsum
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.dorsum.size() == 0) {
      out << "dorsum: []\n";
    } else {
      out << "dorsum:\n";
      for (auto item : msg.dorsum) {
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

}  // namespace omnihand_2025_node_msgs

namespace rosidl_generator_traits
{

[[deprecated("use omnihand_2025_node_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const omnihand_2025_node_msgs::msg::TactileSensor & msg,
  std::ostream & out, size_t indentation = 0)
{
  omnihand_2025_node_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use omnihand_2025_node_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const omnihand_2025_node_msgs::msg::TactileSensor & msg)
{
  return omnihand_2025_node_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<omnihand_2025_node_msgs::msg::TactileSensor>()
{
  return "omnihand_2025_node_msgs::msg::TactileSensor";
}

template<>
inline const char * name<omnihand_2025_node_msgs::msg::TactileSensor>()
{
  return "omnihand_2025_node_msgs/msg/TactileSensor";
}

template<>
struct has_fixed_size<omnihand_2025_node_msgs::msg::TactileSensor>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<omnihand_2025_node_msgs::msg::TactileSensor>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<omnihand_2025_node_msgs::msg::TactileSensor>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__TRAITS_HPP_
