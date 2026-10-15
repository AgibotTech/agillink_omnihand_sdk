// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__rosidl_typesupport_introspection_c.h"
#include "omnipicker_3_node_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__functions.h"
#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnipicker_3_node_msgs__msg__GentleGripCommand__init(message_memory);
}

void omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_fini_function(void * message_memory)
{
  omnipicker_3_node_msgs__msg__GentleGripCommand__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_member_array[2] = {
  {
    "use_current_limit",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__GentleGripCommand, use_current_limit),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "current_limit_ma",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs__msg__GentleGripCommand, current_limit_ma),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_members = {
  "omnipicker_3_node_msgs__msg",  // message namespace
  "GentleGripCommand",  // message name
  2,  // number of fields
  sizeof(omnipicker_3_node_msgs__msg__GentleGripCommand),
  false,  // has_any_key_member_
  omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_member_array,  // message members
  omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_type_support_handle = {
  0,
  &omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_members,
  get_message_typesupport_handle_function,
  &omnipicker_3_node_msgs__msg__GentleGripCommand__get_type_hash,
  &omnipicker_3_node_msgs__msg__GentleGripCommand__get_type_description,
  &omnipicker_3_node_msgs__msg__GentleGripCommand__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnipicker_3_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnipicker_3_node_msgs, msg, GentleGripCommand)() {
  if (!omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_type_support_handle.typesupport_identifier) {
    omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnipicker_3_node_msgs__msg__GentleGripCommand__rosidl_typesupport_introspection_c__GentleGripCommand_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
