// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__FUNCTIONS_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "omnipicker_3_node_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__struct.h"

/// Initialize msg/GentleGripCommand message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * omnipicker_3_node_msgs__msg__GentleGripCommand
 * )) before or use
 * omnipicker_3_node_msgs__msg__GentleGripCommand__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__GentleGripCommand__init(omnipicker_3_node_msgs__msg__GentleGripCommand * msg);

/// Finalize msg/GentleGripCommand message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__GentleGripCommand__fini(omnipicker_3_node_msgs__msg__GentleGripCommand * msg);

/// Create msg/GentleGripCommand message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * omnipicker_3_node_msgs__msg__GentleGripCommand__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
omnipicker_3_node_msgs__msg__GentleGripCommand *
omnipicker_3_node_msgs__msg__GentleGripCommand__create();

/// Destroy msg/GentleGripCommand message.
/**
 * It calls
 * omnipicker_3_node_msgs__msg__GentleGripCommand__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__GentleGripCommand__destroy(omnipicker_3_node_msgs__msg__GentleGripCommand * msg);

/// Check for msg/GentleGripCommand message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__GentleGripCommand__are_equal(const omnipicker_3_node_msgs__msg__GentleGripCommand * lhs, const omnipicker_3_node_msgs__msg__GentleGripCommand * rhs);

/// Copy a msg/GentleGripCommand message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__GentleGripCommand__copy(
  const omnipicker_3_node_msgs__msg__GentleGripCommand * input,
  omnipicker_3_node_msgs__msg__GentleGripCommand * output);

/// Initialize array of msg/GentleGripCommand messages.
/**
 * It allocates the memory for the number of elements and calls
 * omnipicker_3_node_msgs__msg__GentleGripCommand__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__init(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array, size_t size);

/// Finalize array of msg/GentleGripCommand messages.
/**
 * It calls
 * omnipicker_3_node_msgs__msg__GentleGripCommand__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__fini(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array);

/// Create array of msg/GentleGripCommand messages.
/**
 * It allocates the memory for the array and calls
 * omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence *
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__create(size_t size);

/// Destroy array of msg/GentleGripCommand messages.
/**
 * It calls
 * omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__destroy(omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * array);

/// Check for msg/GentleGripCommand message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__are_equal(const omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * lhs, const omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * rhs);

/// Copy an array of msg/GentleGripCommand messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence__copy(
  const omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * input,
  omnipicker_3_node_msgs__msg__GentleGripCommand__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__FUNCTIONS_H_
