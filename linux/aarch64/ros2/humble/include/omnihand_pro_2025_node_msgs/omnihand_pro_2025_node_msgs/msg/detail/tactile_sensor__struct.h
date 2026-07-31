// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_

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
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__struct.h"

/// Struct defined in msg/TactileSensor in the package omnihand_pro_2025_node_msgs.
/**
  * OmniHand Pro 2025 (O12) 3D tactile: one TactileSensorData per finger (thumb … palm).
 */
typedef struct omnihand_pro_2025_node_msgs__msg__TactileSensor
{
  std_msgs__msg__Header header;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData thumb;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData index;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData middle;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData ring;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData little;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData palm;
} omnihand_pro_2025_node_msgs__msg__TactileSensor;

// Struct for a sequence of omnihand_pro_2025_node_msgs__msg__TactileSensor.
typedef struct omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence
{
  omnihand_pro_2025_node_msgs__msg__TactileSensor * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnihand_pro_2025_node_msgs__msg__TactileSensor__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_H_
