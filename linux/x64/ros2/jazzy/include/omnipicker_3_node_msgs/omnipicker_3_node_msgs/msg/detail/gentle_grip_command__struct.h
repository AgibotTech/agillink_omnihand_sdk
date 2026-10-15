// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/gentle_grip_command.h"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__STRUCT_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/GentleGripCommand in the package omnipicker_3_node_msgs.
/**
  * Enter gentle-grip mode, optionally applying an additional current limit.
 */
typedef struct omnipicker_3_node_msgs__msg__GentleGripCommand
{
  bool use_current_limit;
  uint16_t current_limit_ma;
} omnipicker_3_node_msgs__msg__GentleGripCommand;

// Struct for a sequence of omnipicker_3_node_msgs__msg__GentleGripCommand.
typedef struct omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence
{
  omnipicker_3_node_msgs__msg__GentleGripCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__STRUCT_H_
