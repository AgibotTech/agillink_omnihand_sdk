// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnipicker_3_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/tactile_sensor.h"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_

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
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/TactileSensor in the package omnipicker_3_node_msgs.
/**
  * OmniPicker 3 (OP3) 16-bit 1D tactile data from its two sensor regions.
 */
typedef struct omnipicker_3_node_msgs__msg__TactileSensor
{
  std_msgs__msg__Header header;
  rosidl_runtime_c__uint16__Sequence thumb;
  rosidl_runtime_c__uint16__Sequence index;
} omnipicker_3_node_msgs__msg__TactileSensor;

// Struct for a sequence of omnipicker_3_node_msgs__msg__TactileSensor.
typedef struct omnipicker_3_node_msgs__msg__TactileSensor__Sequence
{
  omnipicker_3_node_msgs__msg__TactileSensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnipicker_3_node_msgs__msg__TactileSensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
