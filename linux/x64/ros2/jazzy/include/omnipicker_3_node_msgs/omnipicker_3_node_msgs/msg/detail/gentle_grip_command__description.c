// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_type_hash_t *
omnipicker_3_node_msgs__msg__GentleGripCommand__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xd4, 0xe8, 0x9a, 0x7b, 0x81, 0x31, 0xeb, 0xcc,
      0x6d, 0xc9, 0xd8, 0xe4, 0x08, 0x83, 0x4d, 0xb5,
      0xcf, 0xcb, 0x18, 0x5c, 0x79, 0xfb, 0x92, 0xce,
      0x29, 0x0b, 0x2f, 0x7f, 0xea, 0xfd, 0x39, 0x4e,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char omnipicker_3_node_msgs__msg__GentleGripCommand__TYPE_NAME[] = "omnipicker_3_node_msgs/msg/GentleGripCommand";

// Define type names, field names, and default values
static char omnipicker_3_node_msgs__msg__GentleGripCommand__FIELD_NAME__use_current_limit[] = "use_current_limit";
static char omnipicker_3_node_msgs__msg__GentleGripCommand__FIELD_NAME__current_limit_ma[] = "current_limit_ma";

static rosidl_runtime_c__type_description__Field omnipicker_3_node_msgs__msg__GentleGripCommand__FIELDS[] = {
  {
    {omnipicker_3_node_msgs__msg__GentleGripCommand__FIELD_NAME__use_current_limit, 17, 17},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnipicker_3_node_msgs__msg__GentleGripCommand__FIELD_NAME__current_limit_ma, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
omnipicker_3_node_msgs__msg__GentleGripCommand__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {omnipicker_3_node_msgs__msg__GentleGripCommand__TYPE_NAME, 44, 44},
      {omnipicker_3_node_msgs__msg__GentleGripCommand__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Enter gentle-grip mode, optionally applying an additional current limit.\n"
  "bool use_current_limit\n"
  "uint16 current_limit_ma";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
omnipicker_3_node_msgs__msg__GentleGripCommand__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {omnipicker_3_node_msgs__msg__GentleGripCommand__TYPE_NAME, 44, 44},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 122, 122},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnipicker_3_node_msgs__msg__GentleGripCommand__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *omnipicker_3_node_msgs__msg__GentleGripCommand__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
