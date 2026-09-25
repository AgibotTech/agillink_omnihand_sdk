// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from omnihand_node_msgs:msg/Int16MultiArrayStamped.idl
// generated code does not contain a copyright notice
#ifndef OMNIHAND_NODE_MSGS__MSG__DETAIL__INT16_MULTI_ARRAY_STAMPED__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define OMNIHAND_NODE_MSGS__MSG__DETAIL__INT16_MULTI_ARRAY_STAMPED__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "omnihand_node_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "omnihand_node_msgs/msg/detail/int16_multi_array_stamped__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
bool cdr_serialize_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  const omnihand_node_msgs__msg__Int16MultiArrayStamped * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
bool cdr_deserialize_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  eprosima::fastcdr::Cdr &,
  omnihand_node_msgs__msg__Int16MultiArrayStamped * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
size_t get_serialized_size_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
size_t max_serialized_size_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
bool cdr_serialize_key_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  const omnihand_node_msgs__msg__Int16MultiArrayStamped * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
size_t get_serialized_size_key_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
size_t max_serialized_size_key_omnihand_node_msgs__msg__Int16MultiArrayStamped(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, omnihand_node_msgs, msg, Int16MultiArrayStamped)();

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_NODE_MSGS__MSG__DETAIL__INT16_MULTI_ARRAY_STAMPED__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
