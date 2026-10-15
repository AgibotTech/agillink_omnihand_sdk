// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnipicker_3_node_msgs:msg/GentleGripState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/gentle_grip_state.h"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__STRUCT_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__STRUCT_H_

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
// Member 'raw_response'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/GentleGripState in the package omnipicker_3_node_msgs.
typedef struct omnipicker_3_node_msgs__msg__GentleGripState
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__uint8__Sequence raw_response;
} omnipicker_3_node_msgs__msg__GentleGripState;

// Struct for a sequence of omnipicker_3_node_msgs__msg__GentleGripState.
typedef struct omnipicker_3_node_msgs__msg__GentleGripState__Sequence
{
  omnipicker_3_node_msgs__msg__GentleGripState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnipicker_3_node_msgs__msg__GentleGripState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__STRUCT_H_
