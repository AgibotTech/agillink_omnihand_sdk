// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor__rosidl_typesupport_introspection_c.h"
#include "omnihand_pro_2025_node_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor__functions.h"
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor__struct.h"


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
#include "omnihand_pro_2025_node_msgs/msg/tactile_sensor_data.h"
// Member `thumb`
// Member `index`
// Member `middle`
// Member `ring`
// Member `little`
// Member `palm`
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  omnihand_pro_2025_node_msgs__msg__TactileSensor__init(message_memory);
}

void omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_fini_function(void * message_memory)
{
  omnihand_pro_2025_node_msgs__msg__TactileSensor__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[7] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, header),  // bytes offset in struct
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
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, thumb),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "index",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, index),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "middle",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, middle),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ring",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, ring),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "little",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, little),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "palm",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_pro_2025_node_msgs__msg__TactileSensor, palm),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_members = {
  "omnihand_pro_2025_node_msgs__msg",  // message namespace
  "TactileSensor",  // message name
  7,  // number of fields
  sizeof(omnihand_pro_2025_node_msgs__msg__TactileSensor),
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array,  // message members
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle = {
  0,
  &omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_omnihand_pro_2025_node_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensor)() {
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)();
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)();
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[3].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)();
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[4].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)();
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[5].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)();
  omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_member_array[6].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, omnihand_pro_2025_node_msgs, msg, TactileSensorData)();
  if (!omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle.typesupport_identifier) {
    omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &omnihand_pro_2025_node_msgs__msg__TactileSensor__rosidl_typesupport_introspection_c__TactileSensor_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
