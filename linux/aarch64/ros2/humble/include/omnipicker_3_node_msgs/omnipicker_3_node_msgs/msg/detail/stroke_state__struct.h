// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnipicker_3_node_msgs:msg/StrokeState.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__STRUCT_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__STRUCT_H_

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

/// Struct defined in msg/StrokeState in the package omnipicker_3_node_msgs.
typedef struct omnipicker_3_node_msgs__msg__StrokeState
{
  std_msgs__msg__Header header;
  double stroke_mm;
} omnipicker_3_node_msgs__msg__StrokeState;

// Struct for a sequence of omnipicker_3_node_msgs__msg__StrokeState.
typedef struct omnipicker_3_node_msgs__msg__StrokeState__Sequence
{
  omnipicker_3_node_msgs__msg__StrokeState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnipicker_3_node_msgs__msg__StrokeState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__STRUCT_H_
