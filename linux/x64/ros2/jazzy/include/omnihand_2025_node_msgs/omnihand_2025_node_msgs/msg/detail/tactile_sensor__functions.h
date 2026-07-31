// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_2025_node_msgs/msg/tactile_sensor.h"


#ifndef OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__FUNCTIONS_H_
#define OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__FUNCTIONS_H_

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
#include "omnihand_2025_node_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__struct.h"

/// Initialize msg/TactileSensor message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * omnihand_2025_node_msgs__msg__TactileSensor
 * )) before or use
 * omnihand_2025_node_msgs__msg__TactileSensor__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
bool
omnihand_2025_node_msgs__msg__TactileSensor__init(omnihand_2025_node_msgs__msg__TactileSensor * msg);

/// Finalize msg/TactileSensor message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
void
omnihand_2025_node_msgs__msg__TactileSensor__fini(omnihand_2025_node_msgs__msg__TactileSensor * msg);

/// Create msg/TactileSensor message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * omnihand_2025_node_msgs__msg__TactileSensor__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
omnihand_2025_node_msgs__msg__TactileSensor *
omnihand_2025_node_msgs__msg__TactileSensor__create(void);

/// Destroy msg/TactileSensor message.
/**
 * It calls
 * omnihand_2025_node_msgs__msg__TactileSensor__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
void
omnihand_2025_node_msgs__msg__TactileSensor__destroy(omnihand_2025_node_msgs__msg__TactileSensor * msg);

/// Check for msg/TactileSensor message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
bool
omnihand_2025_node_msgs__msg__TactileSensor__are_equal(const omnihand_2025_node_msgs__msg__TactileSensor * lhs, const omnihand_2025_node_msgs__msg__TactileSensor * rhs);

/// Copy a msg/TactileSensor message.
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
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
bool
omnihand_2025_node_msgs__msg__TactileSensor__copy(
  const omnihand_2025_node_msgs__msg__TactileSensor * input,
  omnihand_2025_node_msgs__msg__TactileSensor * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
const rosidl_type_hash_t *
omnihand_2025_node_msgs__msg__TactileSensor__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
const rosidl_runtime_c__type_description__TypeDescription *
omnihand_2025_node_msgs__msg__TactileSensor__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
const rosidl_runtime_c__type_description__TypeSource *
omnihand_2025_node_msgs__msg__TactileSensor__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnihand_2025_node_msgs__msg__TactileSensor__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/TactileSensor messages.
/**
 * It allocates the memory for the number of elements and calls
 * omnihand_2025_node_msgs__msg__TactileSensor__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
bool
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__init(omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array, size_t size);

/// Finalize array of msg/TactileSensor messages.
/**
 * It calls
 * omnihand_2025_node_msgs__msg__TactileSensor__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
void
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__fini(omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array);

/// Create array of msg/TactileSensor messages.
/**
 * It allocates the memory for the array and calls
 * omnihand_2025_node_msgs__msg__TactileSensor__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
omnihand_2025_node_msgs__msg__TactileSensor__Sequence *
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__create(size_t size);

/// Destroy array of msg/TactileSensor messages.
/**
 * It calls
 * omnihand_2025_node_msgs__msg__TactileSensor__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
void
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__destroy(omnihand_2025_node_msgs__msg__TactileSensor__Sequence * array);

/// Check for msg/TactileSensor message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
bool
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__are_equal(const omnihand_2025_node_msgs__msg__TactileSensor__Sequence * lhs, const omnihand_2025_node_msgs__msg__TactileSensor__Sequence * rhs);

/// Copy an array of msg/TactileSensor messages.
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
ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
bool
omnihand_2025_node_msgs__msg__TactileSensor__Sequence__copy(
  const omnihand_2025_node_msgs__msg__TactileSensor__Sequence * input,
  omnihand_2025_node_msgs__msg__TactileSensor__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__FUNCTIONS_H_
