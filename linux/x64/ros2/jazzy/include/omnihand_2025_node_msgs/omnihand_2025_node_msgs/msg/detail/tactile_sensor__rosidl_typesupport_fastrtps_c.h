// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice
#ifndef OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "omnihand_2025_node_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
bool cdr_serialize_omnihand_2025_node_msgs__msg__TactileSensor(
  const omnihand_2025_node_msgs__msg__TactileSensor * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
bool cdr_deserialize_omnihand_2025_node_msgs__msg__TactileSensor(
  eprosima::fastcdr::Cdr &,
  omnihand_2025_node_msgs__msg__TactileSensor * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
size_t get_serialized_size_omnihand_2025_node_msgs__msg__TactileSensor(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
size_t max_serialized_size_omnihand_2025_node_msgs__msg__TactileSensor(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
bool cdr_serialize_key_omnihand_2025_node_msgs__msg__TactileSensor(
  const omnihand_2025_node_msgs__msg__TactileSensor * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
size_t get_serialized_size_key_omnihand_2025_node_msgs__msg__TactileSensor(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
size_t max_serialized_size_key_omnihand_2025_node_msgs__msg__TactileSensor(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_omnihand_2025_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, omnihand_2025_node_msgs, msg, TactileSensor)();

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
