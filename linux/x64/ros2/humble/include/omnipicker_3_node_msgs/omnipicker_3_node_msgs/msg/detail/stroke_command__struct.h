// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__STRUCT_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/StrokeCommand in the package omnipicker_3_node_msgs.
/**
  * OP3 distance control. stroke_mm is millimetres; speed/current use SDK units.
 */
typedef struct omnipicker_3_node_msgs__msg__StrokeCommand
{
  double stroke_mm;
  bool use_speed;
  int16_t speed_rpm;
  bool use_current;
  int16_t current_ma;
} omnipicker_3_node_msgs__msg__StrokeCommand;

// Struct for a sequence of omnipicker_3_node_msgs__msg__StrokeCommand.
typedef struct omnipicker_3_node_msgs__msg__StrokeCommand__Sequence
{
  omnipicker_3_node_msgs__msg__StrokeCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnipicker_3_node_msgs__msg__StrokeCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__STRUCT_H_
