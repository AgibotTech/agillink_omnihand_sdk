// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnihand_3_lite_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_3_lite_node_msgs/msg/tactile_sensor.h"


#ifndef OMNIHAND_3_LITE_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
#define OMNIHAND_3_LITE_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_

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
// Member 'thumb'
// Member 'index'
// Member 'middle'
// Member 'ring'
// Member 'little'
// Member 'palm'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/TactileSensor in the package omnihand_3_lite_node_msgs.
/**
  * OmniHand 3 Lite (H3L) 16-bit 1D tactile data from six sensor regions.
 */
typedef struct omnihand_3_lite_node_msgs__msg__TactileSensor
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__uint16__Sequence thumb;
  rosidl_runtime_c__uint16__Sequence index;
  rosidl_runtime_c__uint16__Sequence middle;
  rosidl_runtime_c__uint16__Sequence ring;
  rosidl_runtime_c__uint16__Sequence little;
  rosidl_runtime_c__uint16__Sequence palm;
} omnihand_3_lite_node_msgs__msg__TactileSensor;

// Struct for a sequence of omnihand_3_lite_node_msgs__msg__TactileSensor.
typedef struct omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence
{
  omnihand_3_lite_node_msgs__msg__TactileSensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnihand_3_lite_node_msgs__msg__TactileSensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_3_LITE_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
