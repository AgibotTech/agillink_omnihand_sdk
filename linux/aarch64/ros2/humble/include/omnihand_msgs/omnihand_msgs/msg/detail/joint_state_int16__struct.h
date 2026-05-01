// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnihand_msgs:msg/JointStateInt16.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__STRUCT_H_
#define OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__STRUCT_H_

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
// Member 'name'
#include "rosidl_runtime_c/string.h"
// Member 'data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/JointStateInt16 in the package omnihand_msgs.
typedef struct omnihand_msgs__msg__JointStateInt16
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__String__Sequence name;
  rosidl_runtime_c__int16__Sequence data;
} omnihand_msgs__msg__JointStateInt16;

// Struct for a sequence of omnihand_msgs__msg__JointStateInt16.
typedef struct omnihand_msgs__msg__JointStateInt16__Sequence
{
  omnihand_msgs__msg__JointStateInt16 * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnihand_msgs__msg__JointStateInt16__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__STRUCT_H_
