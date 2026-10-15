// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice
#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "omnipicker_3_node_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "omnipicker_3_node_msgs/msg/detail/stroke_command__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
bool cdr_serialize_omnipicker_3_node_msgs__msg__StrokeCommand(
  const omnipicker_3_node_msgs__msg__StrokeCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
bool cdr_deserialize_omnipicker_3_node_msgs__msg__StrokeCommand(
  eprosima::fastcdr::Cdr &,
  omnipicker_3_node_msgs__msg__StrokeCommand * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
size_t get_serialized_size_omnipicker_3_node_msgs__msg__StrokeCommand(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
size_t max_serialized_size_omnipicker_3_node_msgs__msg__StrokeCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
bool cdr_serialize_key_omnipicker_3_node_msgs__msg__StrokeCommand(
  const omnipicker_3_node_msgs__msg__StrokeCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
size_t get_serialized_size_key_omnipicker_3_node_msgs__msg__StrokeCommand(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
size_t max_serialized_size_key_omnipicker_3_node_msgs__msg__StrokeCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, omnipicker_3_node_msgs, msg, StrokeCommand)();

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
