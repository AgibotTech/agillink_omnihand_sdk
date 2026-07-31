// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__STRUCT_H_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'channel_value'
// Member 'capa_approach'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/TactileSensorData in the package omnihand_pro_2025_node_msgs.
typedef struct omnihand_pro_2025_node_msgs__msg__TactileSensorData
{
  /// 1=sensor online; 0=sensor offline
  uint8_t online_state;
  /// 6-channel 24-bit value
  rosidl_runtime_c__uint32__Sequence channel_value;
  /// normal force (0.1 N, 0 ~ 3000)
  uint16_t normal_force;
  /// tangent force (0.1 N, 0 ~ 3000)
  uint16_t tangent_force;
  /// tangent force angle, fingertip-up = 0°, clockwise (0~359)
  uint16_t tangent_force_angle;
  /// 4 self-capacitance proximity values
  rosidl_runtime_c__uint8__Sequence capa_approach;
} omnihand_pro_2025_node_msgs__msg__TactileSensorData;

// Struct for a sequence of omnihand_pro_2025_node_msgs__msg__TactileSensorData.
typedef struct omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence
{
  omnihand_pro_2025_node_msgs__msg__TactileSensorData * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnihand_pro_2025_node_msgs__msg__TactileSensorData__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__STRUCT_H_
