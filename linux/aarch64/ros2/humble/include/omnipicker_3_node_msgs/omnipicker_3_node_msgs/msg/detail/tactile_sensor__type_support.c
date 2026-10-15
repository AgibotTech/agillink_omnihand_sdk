// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnipicker_3_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__rosidl_typesupport_introspection_c.h"
#include "omnipicker_3_node_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__functions.h"
#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `thumb`
// Member `index`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnipicker_3_node_msgs__msg__TactileSensor__init(message_memory);
}

void omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_fini_function(void * message_memory)
{
  omnipicker_3_node_msgs__msg__TactileSensor__fini(message_memory);
}

size_t omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__thumb(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint16__Sequence * member =
    (const rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  return member->size;
}

const void * omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__thumb(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint16__Sequence * member =
    (const rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__thumb(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint16__Sequence * member =
    (rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__thumb(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint16_t * item =
    ((const uint16_t *)
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__thumb(untyped_member, index));
  uint16_t * value =
    (uint16_t *)(untyped_value);
  *value = *item;
}

void omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__thumb(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint16_t * item =
    ((uint16_t *)
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__thumb(untyped_member, index));
  const uint16_t * value =
    (const uint16_t *)(untyped_value);
  *item = *value;
}

bool omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__thumb(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint16__Sequence * member =
    (rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  rosidl_runtime_c__uint16__Sequence__fini(member);
  return rosidl_runtime_c__uint16__Sequence__init(member, size);
}

size_t omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__index(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint16__Sequence * member =
    (const rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  return member->size;
}

const void * omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__index(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint16__Sequence * member =
    (const rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__index(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint16__Sequence * member =
    (rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__index(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint16_t * item =
    ((const uint16_t *)
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__index(untyped_member, index));
  uint16_t * value =
    (uint16_t *)(untyped_value);
  *value = *item;
}

void omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__index(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint16_t * item =
    ((uint16_t *)
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__index(untyped_member, index));
  const uint16_t * value =
    (const uint16_t *)(untyped_value);
  *item = *value;
}

bool omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__index(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint16__Sequence * member =
    (rosidl_runtime_c__uint16__Sequence *)(untyped_member);
  rosidl_runtime_c__uint16__Sequence__fini(member);
  return rosidl_runtime_c__uint16__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__TactileSensor, header),  // bytes offset in struct
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
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__TactileSensor, thumb),  // bytes offset in struct
    NULL,  // default value
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__thumb,  // size() function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__thumb,  // get_const(index) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__thumb,  // get(index) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__thumb,  // fetch(index, &value) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__thumb,  // assign(index, value) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__thumb  // resize(index) function pointer
  },
  {
    "index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__TactileSensor, index),  // bytes offset in struct
    NULL,  // default value
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__size_function__TactileSensor__index,  // size() function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_const_function__TactileSensor__index,  // get_const(index) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__get_function__TactileSensor__index,  // get(index) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__fetch_function__TactileSensor__index,  // fetch(index, &value) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__assign_function__TactileSensor__index,  // assign(index, value) function pointer
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__resize_function__TactileSensor__index  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_members = {
  "omnipicker_3_node_msgs__msg",  // message namespace
  "TactileSensor",  // message name
  3,  // number of fields
  sizeof(omnipicker_3_node_msgs__msg__TactileSensor),
  omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array,  // message members
  omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle = {
  0,
  &omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnipicker_3_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnipicker_3_node_msgs, msg, TactileSensor)() {
  omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle.typesupport_identifier) {
    omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnipicker_3_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
