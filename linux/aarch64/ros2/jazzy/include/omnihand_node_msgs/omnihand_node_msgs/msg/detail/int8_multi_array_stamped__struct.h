// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnihand_node_msgs:msg/Int8MultiArrayStamped.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_node_msgs/msg/int8_multi_array_stamped.h"


#ifndef OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__STRUCT_H_
#define OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'layout'
#include "std_msgs/msg/detail/multi_array_layout__struct.h"
// Member 'data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/Int8MultiArrayStamped in the package omnihand_node_msgs.
typedef struct omnihand_node_msgs__msg__Int8MultiArrayStamped
{
  std_msgs__msg__Header header;
  std_msgs__msg__MultiArrayLayout layout;
  rosidl_runtime_c__int8__Sequence data;
} omnihand_node_msgs__msg__Int8MultiArrayStamped;

// Struct for a sequence of omnihand_node_msgs__msg__Int8MultiArrayStamped.
typedef struct omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence
{
  omnihand_node_msgs__msg__Int8MultiArrayStamped * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__STRUCT_H_
