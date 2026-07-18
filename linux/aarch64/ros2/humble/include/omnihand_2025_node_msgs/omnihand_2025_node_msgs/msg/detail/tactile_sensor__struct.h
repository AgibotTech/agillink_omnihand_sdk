// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
#define OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_

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
// Member 'dorsum'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/TactileSensor in the package omnihand_2025_node_msgs.
/**
  * OmniHand 2025 (O10) 1D tactile: one uint8[] per region; 1g per point, max 255g. 
 */
typedef struct omnihand_2025_node_msgs__msg__TactileSensor
{
  std_msgs__msg__Header header;
  /// 16 points
  rosidl_runtime_c__uint8__Sequence thumb;
  /// 18 points
  rosidl_runtime_c__uint8__Sequence index;
  /// 18 points
  rosidl_runtime_c__uint8__Sequence middle;
  /// 18 points
  rosidl_runtime_c__uint8__Sequence ring;
  /// 18 points
  rosidl_runtime_c__uint8__Sequence little;
  /// 78 points
  rosidl_runtime_c__uint8__Sequence palm;
  /// 102 points
  rosidl_runtime_c__uint8__Sequence dorsum;
} omnihand_2025_node_msgs__msg__TactileSensor;

// Struct for a sequence of omnihand_2025_node_msgs__msg__TactileSensor.
typedef struct omnihand_2025_node_msgs__msg__TactileSensor__Sequence
{
  omnihand_2025_node_msgs__msg__TactileSensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnihand_2025_node_msgs__msg__TactileSensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
