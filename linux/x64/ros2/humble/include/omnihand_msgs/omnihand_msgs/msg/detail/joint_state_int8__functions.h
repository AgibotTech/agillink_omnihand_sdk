// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from omnihand_msgs:msg/JointStateInt8.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT8__FUNCTIONS_H_
#define OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT8__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "omnihand_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "omnihand_msgs/msg/detail/joint_state_int8__struct.h"

/// Initialize msg/JointStateInt8 message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * omnihand_msgs__msg__JointStateInt8
 * )) before or use
 * omnihand_msgs__msg__JointStateInt8__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
bool
omnihand_msgs__msg__JointStateInt8__init(omnihand_msgs__msg__JointStateInt8 * msg);

/// Finalize msg/JointStateInt8 message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
void
omnihand_msgs__msg__JointStateInt8__fini(omnihand_msgs__msg__JointStateInt8 * msg);

/// Create msg/JointStateInt8 message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * omnihand_msgs__msg__JointStateInt8__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
omnihand_msgs__msg__JointStateInt8 *
omnihand_msgs__msg__JointStateInt8__create();

/// Destroy msg/JointStateInt8 message.
/**
 * It calls
 * omnihand_msgs__msg__JointStateInt8__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
void
omnihand_msgs__msg__JointStateInt8__destroy(omnihand_msgs__msg__JointStateInt8 * msg);

/// Check for msg/JointStateInt8 message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
bool
omnihand_msgs__msg__JointStateInt8__are_equal(const omnihand_msgs__msg__JointStateInt8 * lhs, const omnihand_msgs__msg__JointStateInt8 * rhs);

/// Copy a msg/JointStateInt8 message.
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
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
bool
omnihand_msgs__msg__JointStateInt8__copy(
  const omnihand_msgs__msg__JointStateInt8 * input,
  omnihand_msgs__msg__JointStateInt8 * output);

/// Initialize array of msg/JointStateInt8 messages.
/**
 * It allocates the memory for the number of elements and calls
 * omnihand_msgs__msg__JointStateInt8__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
bool
omnihand_msgs__msg__JointStateInt8__Sequence__init(omnihand_msgs__msg__JointStateInt8__Sequence * array, size_t size);

/// Finalize array of msg/JointStateInt8 messages.
/**
 * It calls
 * omnihand_msgs__msg__JointStateInt8__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
void
omnihand_msgs__msg__JointStateInt8__Sequence__fini(omnihand_msgs__msg__JointStateInt8__Sequence * array);

/// Create array of msg/JointStateInt8 messages.
/**
 * It allocates the memory for the array and calls
 * omnihand_msgs__msg__JointStateInt8__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
omnihand_msgs__msg__JointStateInt8__Sequence *
omnihand_msgs__msg__JointStateInt8__Sequence__create(size_t size);

/// Destroy array of msg/JointStateInt8 messages.
/**
 * It calls
 * omnihand_msgs__msg__JointStateInt8__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
void
omnihand_msgs__msg__JointStateInt8__Sequence__destroy(omnihand_msgs__msg__JointStateInt8__Sequence * array);

/// Check for msg/JointStateInt8 message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
bool
omnihand_msgs__msg__JointStateInt8__Sequence__are_equal(const omnihand_msgs__msg__JointStateInt8__Sequence * lhs, const omnihand_msgs__msg__JointStateInt8__Sequence * rhs);

/// Copy an array of msg/JointStateInt8 messages.
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
ROSIDL_GENERATOR_C_PUBLIC_omnihand_msgs
bool
omnihand_msgs__msg__JointStateInt8__Sequence__copy(
  const omnihand_msgs__msg__JointStateInt8__Sequence * input,
  omnihand_msgs__msg__JointStateInt8__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT8__FUNCTIONS_H_
