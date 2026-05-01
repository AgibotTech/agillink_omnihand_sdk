// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from omnihand_msgs:msg/JointStateInt16.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "omnihand_msgs/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "omnihand_msgs/msg/detail/joint_state_int16__struct.hpp"

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

namespace omnihand_msgs
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnihand_msgs
cdr_serialize(
  const omnihand_msgs::msg::JointStateInt16 & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnihand_msgs
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  omnihand_msgs::msg::JointStateInt16 & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnihand_msgs
get_serialized_size(
  const omnihand_msgs::msg::JointStateInt16 & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnihand_msgs
max_serialized_size_JointStateInt16(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace omnihand_msgs

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_omnihand_msgs
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, omnihand_msgs, msg, JointStateInt16)();

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
