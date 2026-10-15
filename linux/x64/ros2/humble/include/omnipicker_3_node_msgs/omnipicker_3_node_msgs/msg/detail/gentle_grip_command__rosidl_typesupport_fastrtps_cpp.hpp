// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "omnipicker_3_node_msgs/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnipicker_3_node_msgs
cdr_serialize(
  const omnipicker_3_node_msgs::msg::GentleGripCommand & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnipicker_3_node_msgs
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  omnipicker_3_node_msgs::msg::GentleGripCommand & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnipicker_3_node_msgs
get_serialized_size(
  const omnipicker_3_node_msgs::msg::GentleGripCommand & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnipicker_3_node_msgs
max_serialized_size_GentleGripCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace omnipicker_3_node_msgs

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnipicker_3_node_msgs
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, omnipicker_3_node_msgs, msg, GentleGripCommand)();

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
