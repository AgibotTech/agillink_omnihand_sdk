// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnipicker_3_node_msgs:msg/GentleGripState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_state__rosidl_typesupport_introspection_c.h"
#include "omnipicker_3_node_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_state__functions.h"
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_state__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `raw_response`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnipicker_3_node_msgs__msg__GentleGripState__init(message_memory);
}

void omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_fini_function(void * message_memory)
{
  omnipicker_3_node_msgs__msg__GentleGripState__fini(message_memory);
}

size_t omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__size_function__GentleGripState__raw_response(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__get_const_function__GentleGripState__raw_response(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__get_function__GentleGripState__raw_response(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__fetch_function__GentleGripState__raw_response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__get_const_function__GentleGripState__raw_response(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__assign_function__GentleGripState__raw_response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__get_function__GentleGripState__raw_response(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__resize_function__GentleGripState__raw_response(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__GentleGripState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "raw_response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__GentleGripState, raw_response),  // bytes offset in struct
    NULL,  // default value
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__size_function__GentleGripState__raw_response,  // size() function pointer
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__get_const_function__GentleGripState__raw_response,  // get_const(index) function pointer
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__get_function__GentleGripState__raw_response,  // get(index) function pointer
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__fetch_function__GentleGripState__raw_response,  // fetch(index, &value) function pointer
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__assign_function__GentleGripState__raw_response,  // assign(index, value) function pointer
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__resize_function__GentleGripState__raw_response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_members = {
  "omnipicker_3_node_msgs__msg",  // message namespace
  "GentleGripState",  // message name
  2,  // number of fields
  sizeof(omnipicker_3_node_msgs__msg__GentleGripState),
  false,  // has_any_key_member_
  omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_member_array,  // message members
  omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_init_function,  // function to initialize message memory (memory has to be allocated)
  omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_type_support_handle = {
  0,
  &omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_members,
  get_message_typesupport_handle_function,
  &omnipicker_3_node_msgs__msg__GentleGripState__get_type_hash,
  &omnipicker_3_node_msgs__msg__GentleGripState__get_type_description,
  &omnipicker_3_node_msgs__msg__GentleGripState__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnipicker_3_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnipicker_3_node_msgs, msg, GentleGripState)() {
  omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_type_support_handle.typesupport_identifier) {
    omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnipicker_3_node_msgs__msg__GentleGripState__rosidl_typesupport_introspection_c__GentleGripState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
