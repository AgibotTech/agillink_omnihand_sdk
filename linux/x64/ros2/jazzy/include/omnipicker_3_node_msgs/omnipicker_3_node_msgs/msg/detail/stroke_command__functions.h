// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/stroke_command.h"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__FUNCTIONS_H_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "omnipicker_3_node_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "omnipicker_3_node_msgs/msg/detail/stroke_command__struct.h"

/// Initialize msg/StrokeCommand message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * omnipicker_3_node_msgs__msg__StrokeCommand
 * )) before or use
 * omnipicker_3_node_msgs__msg__StrokeCommand__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__StrokeCommand__init(omnipicker_3_node_msgs__msg__StrokeCommand * msg);

/// Finalize msg/StrokeCommand message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__StrokeCommand__fini(omnipicker_3_node_msgs__msg__StrokeCommand * msg);

/// Create msg/StrokeCommand message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * omnipicker_3_node_msgs__msg__StrokeCommand__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
omnipicker_3_node_msgs__msg__StrokeCommand *
omnipicker_3_node_msgs__msg__StrokeCommand__create(void);

/// Destroy msg/StrokeCommand message.
/**
 * It calls
 * omnipicker_3_node_msgs__msg__StrokeCommand__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__StrokeCommand__destroy(omnipicker_3_node_msgs__msg__StrokeCommand * msg);

/// Check for msg/StrokeCommand message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__StrokeCommand__are_equal(const omnipicker_3_node_msgs__msg__StrokeCommand * lhs, const omnipicker_3_node_msgs__msg__StrokeCommand * rhs);

/// Copy a msg/StrokeCommand message.
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
omnipicker_3_node_msgs__msg__StrokeCommand__copy(
  const omnipicker_3_node_msgs__msg__StrokeCommand * input,
  omnipicker_3_node_msgs__msg__StrokeCommand * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_type_hash_t *
omnipicker_3_node_msgs__msg__StrokeCommand__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_runtime_c__type_description__TypeDescription *
omnipicker_3_node_msgs__msg__StrokeCommand__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_runtime_c__type_description__TypeSource *
omnipicker_3_node_msgs__msg__StrokeCommand__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnipicker_3_node_msgs__msg__StrokeCommand__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/StrokeCommand messages.
/**
 * It allocates the memory for the number of elements and calls
 * omnipicker_3_node_msgs__msg__StrokeCommand__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__init(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array, size_t size);

/// Finalize array of msg/StrokeCommand messages.
/**
 * It calls
 * omnipicker_3_node_msgs__msg__StrokeCommand__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__fini(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array);

/// Create array of msg/StrokeCommand messages.
/**
 * It allocates the memory for the array and calls
 * omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence *
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__create(size_t size);

/// Destroy array of msg/StrokeCommand messages.
/**
 * It calls
 * omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
void
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__destroy(omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * array);

/// Check for msg/StrokeCommand message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
bool
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__are_equal(const omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * lhs, const omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * rhs);

/// Copy an array of msg/StrokeCommand messages.
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
omnipicker_3_node_msgs__msg__StrokeCommand__Sequence__copy(
  const omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * input,
  omnipicker_3_node_msgs__msg__StrokeCommand__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__FUNCTIONS_H_
