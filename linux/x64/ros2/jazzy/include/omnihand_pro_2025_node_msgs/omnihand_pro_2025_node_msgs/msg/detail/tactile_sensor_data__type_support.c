// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__rosidl_typesupport_introspection_c.h"
#include "omnihand_pro_2025_node_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__functions.h"
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__struct.h"


// Include directives for member types
// Member `channel_value`
// Member `capa_approach`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__init(message_memory);
}

void omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_fini_function(void * message_memory)
{
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__fini(message_memory);
}

size_t omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__size_function__TactileSensorData__channel_value(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint32__Sequence * member =
    (const rosidl_runtime_c__uint32__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_const_function__TactileSensorData__channel_value(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint32__Sequence * member =
    (const rosidl_runtime_c__uint32__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_function__TactileSensorData__channel_value(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint32__Sequence * member =
    (rosidl_runtime_c__uint32__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__fetch_function__TactileSensorData__channel_value(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint32_t * item =
    ((const uint32_t *)
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_const_function__TactileSensorData__channel_value(untyped_member, index));
  uint32_t * value =
    (uint32_t *)(untyped_value);
  *value = *item;
}

void omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__assign_function__TactileSensorData__channel_value(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint32_t * item =
    ((uint32_t *)
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_function__TactileSensorData__channel_value(untyped_member, index));
  const uint32_t * value =
    (const uint32_t *)(untyped_value);
  *item = *value;
}

bool omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__resize_function__TactileSensorData__channel_value(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint32__Sequence * member =
    (rosidl_runtime_c__uint32__Sequence *)(untyped_member);
  rosidl_runtime_c__uint32__Sequence__fini(member);
  return rosidl_runtime_c__uint32__Sequence__init(member, size);
}

size_t omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__size_function__TactileSensorData__capa_approach(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_const_function__TactileSensorData__capa_approach(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_function__TactileSensorData__capa_approach(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__fetch_function__TactileSensorData__capa_approach(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_const_function__TactileSensorData__capa_approach(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__assign_function__TactileSensorData__capa_approach(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_function__TactileSensorData__capa_approach(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__resize_function__TactileSensorData__capa_approach(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_member_array[6] = {
  {
    "online_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensorData, online_state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "channel_value",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensorData, channel_value),  // bytes offset in struct
    NULL,  // default value
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__size_function__TactileSensorData__channel_value,  // size() function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_const_function__TactileSensorData__channel_value,  // get_const(index) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_function__TactileSensorData__channel_value,  // get(index) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__fetch_function__TactileSensorData__channel_value,  // fetch(index, &value) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__assign_function__TactileSensorData__channel_value,  // assign(index, value) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__resize_function__TactileSensorData__channel_value  // resize(index) function pointer
  },
  {
    "normal_force",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensorData, normal_force),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "tangent_force",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensorData, tangent_force),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "tangent_force_angle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensorData, tangent_force_angle),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "capa_approach",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensorData, capa_approach),  // bytes offset in struct
    NULL,  // default value
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__size_function__TactileSensorData__capa_approach,  // size() function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_const_function__TactileSensorData__capa_approach,  // get_const(index) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__get_function__TactileSensorData__capa_approach,  // get(index) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__fetch_function__TactileSensorData__capa_approach,  // fetch(index, &value) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__assign_function__TactileSensorData__capa_approach,  // assign(index, value) function pointer
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__resize_function__TactileSensorData__capa_approach  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_members = {
  "omnihand_pro_2025_node_msgs__msg",  // message namespace
  "TactileSensorData",  // message name
  6,  // number of fields
  sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensorData),
  false,  // has_any_key_member_
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_member_array,  // message members
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_init_function,  // function to initialize message memory (memory has to be allocated)
  omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_type_support_handle = {
  0,
  &omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_members,
  get_message_typesupport_handle_function,
  &omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_hash,
  &omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_description,
  &omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnihand_pro_2025_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)() {
  if (!omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_type_support_handle.typesupport_identifier) {
    omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnihand_pro_2025_node_msgs__msg__TactileSensorData__rosidl_typesupport_introspection_c__TactileSensorData_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
