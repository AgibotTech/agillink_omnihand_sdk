// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnihand_msgs:msg/JointStateInt8.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnihand_msgs/msg/detail/joint_state_int8__rosidl_typesupport_introspection_c.h"
#include "omnihand_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnihand_msgs/msg/detail/joint_state_int8__functions.h"
#include "omnihand_msgs/msg/detail/joint_state_int8__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `name`
#include "rosidl_runtime_c/string_functions.h"
// Member `data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnihand_msgs__msg__JointStateInt8__init(message_memory);
}

void omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_fini_function(void * message_memory)
{
  omnihand_msgs__msg__JointStateInt8__fini(message_memory);
}

size_t omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__size_function__JointStateInt8__name(
  const void * untyped_member)
{
  const rosidl_runtime_c__String__Sequence * member =
    (const rosidl_runtime_c__String__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_const_function__JointStateInt8__name(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__String__Sequence * member =
    (const rosidl_runtime_c__String__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_function__JointStateInt8__name(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__String__Sequence * member =
    (rosidl_runtime_c__String__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__fetch_function__JointStateInt8__name(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const rosidl_runtime_c__String * item =
    ((const rosidl_runtime_c__String *)
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_const_function__JointStateInt8__name(untyped_member, index));
  rosidl_runtime_c__String * value =
    (rosidl_runtime_c__String *)(untyped_value);
  *value = *item;
}

void omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__assign_function__JointStateInt8__name(
  void * untyped_member, size_t index, const void * untyped_value)
{
  rosidl_runtime_c__String * item =
    ((rosidl_runtime_c__String *)
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_function__JointStateInt8__name(untyped_member, index));
  const rosidl_runtime_c__String * value =
    (const rosidl_runtime_c__String *)(untyped_value);
  *item = *value;
}

bool omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__resize_function__JointStateInt8__name(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__String__Sequence * member =
    (rosidl_runtime_c__String__Sequence *)(untyped_member);
  rosidl_runtime_c__String__Sequence__fini(member);
  return rosidl_runtime_c__String__Sequence__init(member, size);
}

size_t omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__size_function__JointStateInt8__data(
  const void * untyped_member)
{
  const rosidl_runtime_c__int8__Sequence * member =
    (const rosidl_runtime_c__int8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_const_function__JointStateInt8__data(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int8__Sequence * member =
    (const rosidl_runtime_c__int8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_function__JointStateInt8__data(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int8__Sequence * member =
    (rosidl_runtime_c__int8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__fetch_function__JointStateInt8__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int8_t * item =
    ((const int8_t *)
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_const_function__JointStateInt8__data(untyped_member, index));
  int8_t * value =
    (int8_t *)(untyped_value);
  *value = *item;
}

void omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__assign_function__JointStateInt8__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int8_t * item =
    ((int8_t *)
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_function__JointStateInt8__data(untyped_member, index));
  const int8_t * value =
    (const int8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__resize_function__JointStateInt8__data(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int8__Sequence * member =
    (rosidl_runtime_c__int8__Sequence *)(untyped_member);
  rosidl_runtime_c__int8__Sequence__fini(member);
  return rosidl_runtime_c__int8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_member_array[3] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_msgs__msg__JointStateInt8, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_msgs__msg__JointStateInt8, name),  // bytes offset in struct
    NULL,  // default value
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__size_function__JointStateInt8__name,  // size() function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_const_function__JointStateInt8__name,  // get_const(index) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_function__JointStateInt8__name,  // get(index) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__fetch_function__JointStateInt8__name,  // fetch(index, &value) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__assign_function__JointStateInt8__name,  // assign(index, value) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__resize_function__JointStateInt8__name  // resize(index) function pointer
  },
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_msgs__msg__JointStateInt8, data),  // bytes offset in struct
    NULL,  // default value
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__size_function__JointStateInt8__data,  // size() function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_const_function__JointStateInt8__data,  // get_const(index) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__get_function__JointStateInt8__data,  // get(index) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__fetch_function__JointStateInt8__data,  // fetch(index, &value) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__assign_function__JointStateInt8__data,  // assign(index, value) function pointer
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__resize_function__JointStateInt8__data  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_members = {
  "omnihand_msgs__msg",  // message namespace
  "JointStateInt8",  // message name
  3,  // number of fields
  sizeof(omnihand_msgs__msg__JointStateInt8),
  omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_member_array,  // message members
  omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_init_function,  // function to initialize message memory (memory has to be allocated)
  omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_type_support_handle = {
  0,
  &omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnihand_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_msgs, msg, JointStateInt8)() {
  omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_type_support_handle.typesupport_identifier) {
    omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnihand_msgs__msg__JointStateInt8__rosidl_typesupport_introspection_c__JointStateInt8_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
