// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__rosidl_typesupport_introspection_c.h"
#include "omnihand_2025_node_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__functions.h"
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `thumb`
// Member `index`
// Member `middle`
// Member `ring`
// Member `little`
// Member `palm`
// Member `dorsum`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnihand_2025_node_msgs__msg__TactileSensor__init(message_memory);
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_fini_function(void * message_memory)
{
  omnihand_2025_node_msgs__msg__TactileSensor__fini(message_memory);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__thumb(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__thumb(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__thumb(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__thumb(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__thumb(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__thumb(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__thumb(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__thumb(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__index(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__index(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__index(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__index(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__index(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__index(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__index(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__index(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__middle(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__middle(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__middle(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__middle(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__middle(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__middle(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__middle(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__middle(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__ring(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__ring(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__ring(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__ring(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__ring(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__ring(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__ring(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__ring(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__little(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__little(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__little(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__little(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__little(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__little(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__little(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__little(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__palm(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__palm(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__palm(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__palm(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__palm(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__palm(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__palm(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__palm(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

size_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__dorsum(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__dorsum(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__dorsum(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__dorsum(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__dorsum(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__dorsum(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__dorsum(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__dorsum(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[8] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "thumb",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, thumb),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__thumb,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__thumb,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__thumb,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__thumb,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__thumb,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__thumb  // resize(index) function pointer
  },
  {
    "index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, index),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__index,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__index,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__index,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__index,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__index,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__index  // resize(index) function pointer
  },
  {
    "middle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, middle),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__middle,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__middle,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__middle,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__middle,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__middle,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__middle  // resize(index) function pointer
  },
  {
    "ring",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, ring),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__ring,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__ring,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__ring,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__ring,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__ring,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__ring  // resize(index) function pointer
  },
  {
    "little",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, little),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__little,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__little,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__little,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__little,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__little,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__little  // resize(index) function pointer
  },
  {
    "palm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, palm),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__palm,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__palm,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__palm,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__palm,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__palm,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__palm  // resize(index) function pointer
  },
  {
    "dorsum",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs__msg__TactileSensor, dorsum),  // bytes offset in struct
    NULL,  // default value
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__dorsum,  // size() function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__dorsum,  // get_const(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__dorsum,  // get(index) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__dorsum,  // fetch(index, &value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__dorsum,  // assign(index, value) function pointer
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__dorsum  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_members = {
  "omnihand_2025_node_msgs__msg",  // message namespace
  "TactileSensor",  // message name
  8,  // number of fields
  sizeof(omnihand_2025_node_msgs__msg__TactileSensor),
  omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array,  // message members
  omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle = {
  0,
  &omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnihand_2025_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_2025_node_msgs, msg, TactileSensor)() {
  omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle.typesupport_identifier) {
    omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnihand_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
