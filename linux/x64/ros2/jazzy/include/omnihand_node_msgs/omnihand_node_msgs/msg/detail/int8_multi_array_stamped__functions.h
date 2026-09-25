// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from omnihand_node_msgs:msg/Int8MultiArrayStamped.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_node_msgs/msg/int8_multi_array_stamped.h"


#ifndef OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__FUNCTIONS_H_
#define OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__FUNCTIONS_H_

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
#include "omnihand_node_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "omnihand_node_msgs/msg/detail/int8_multi_array_stamped__struct.h"

/// Initialize msg/Int8MultiArrayStamped message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * omnihand_node_msgs__msg__Int8MultiArrayStamped
 * )) before or use
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
bool
omnihand_node_msgs__msg__Int8MultiArrayStamped__init(omnihand_node_msgs__msg__Int8MultiArrayStamped * msg);

/// Finalize msg/Int8MultiArrayStamped message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
void
omnihand_node_msgs__msg__Int8MultiArrayStamped__fini(omnihand_node_msgs__msg__Int8MultiArrayStamped * msg);

/// Create msg/Int8MultiArrayStamped message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
omnihand_node_msgs__msg__Int8MultiArrayStamped *
omnihand_node_msgs__msg__Int8MultiArrayStamped__create(void);

/// Destroy msg/Int8MultiArrayStamped message.
/**
 * It calls
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
void
omnihand_node_msgs__msg__Int8MultiArrayStamped__destroy(omnihand_node_msgs__msg__Int8MultiArrayStamped * msg);

/// Check for msg/Int8MultiArrayStamped message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
bool
omnihand_node_msgs__msg__Int8MultiArrayStamped__are_equal(const omnihand_node_msgs__msg__Int8MultiArrayStamped * lhs, const omnihand_node_msgs__msg__Int8MultiArrayStamped * rhs);

/// Copy a msg/Int8MultiArrayStamped message.
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
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
bool
omnihand_node_msgs__msg__Int8MultiArrayStamped__copy(
  const omnihand_node_msgs__msg__Int8MultiArrayStamped * input,
  omnihand_node_msgs__msg__Int8MultiArrayStamped * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
const rosidl_type_hash_t *
omnihand_node_msgs__msg__Int8MultiArrayStamped__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
const rosidl_runtime_c__type_description__TypeDescription *
omnihand_node_msgs__msg__Int8MultiArrayStamped__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
const rosidl_runtime_c__type_description__TypeSource *
omnihand_node_msgs__msg__Int8MultiArrayStamped__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnihand_node_msgs__msg__Int8MultiArrayStamped__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/Int8MultiArrayStamped messages.
/**
 * It allocates the memory for the number of elements and calls
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
bool
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__init(omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * array, size_t size);

/// Finalize array of msg/Int8MultiArrayStamped messages.
/**
 * It calls
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
void
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__fini(omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * array);

/// Create array of msg/Int8MultiArrayStamped messages.
/**
 * It allocates the memory for the array and calls
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence *
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__create(size_t size);

/// Destroy array of msg/Int8MultiArrayStamped messages.
/**
 * It calls
 * omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
void
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__destroy(omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * array);

/// Check for msg/Int8MultiArrayStamped message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
bool
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__are_equal(const omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * lhs, const omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * rhs);

/// Copy an array of msg/Int8MultiArrayStamped messages.
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
ROSIDL_GENERATOR_C_PUBLIC_omnihand_node_msgs
bool
omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence__copy(
  const omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * input,
  omnihand_node_msgs__msg__Int8MultiArrayStamped__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__FUNCTIONS_H_
