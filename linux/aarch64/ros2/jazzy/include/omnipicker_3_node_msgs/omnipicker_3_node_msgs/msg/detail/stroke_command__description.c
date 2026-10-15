// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice

#include "omnipicker_3_node_msgs/msg/detail/stroke_command__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_omnipicker_3_node_msgs
const rosidl_type_hash_t *
omnipicker_3_node_msgs__msg__StrokeCommand__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xd1, 0x00, 0xe7, 0x34, 0xef, 0x11, 0x92, 0xa1,
      0x7c, 0x3e, 0x66, 0x6e, 0x59, 0xce, 0x11, 0xb4,
      0x99, 0x50, 0x45, 0xe9, 0x75, 0xe0, 0x66, 0x2a,
      0x8d, 0x16, 0x3d, 0x0f, 0x26, 0x52, 0x02, 0x58,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char omnipicker_3_node_msgs__msg__StrokeCommand__TYPE_NAME[] = "omnipicker_3_node_msgs/msg/StrokeCommand";

// Define type names, field names, and default values
static char omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__stroke_mm[] = "stroke_mm";
static char omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__use_speed[] = "use_speed";
static char omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__speed_rpm[] = "speed_rpm";
static char omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__use_current[] = "use_current";
static char omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__current_ma[] = "current_ma";

static rosidl_runtime_c__type_description__Field omnipicker_3_node_msgs__msg__StrokeCommand__FIELDS[] = {
  {
    {omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__stroke_mm, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__use_speed, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__speed_rpm, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__use_current, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnipicker_3_node_msgs__msg__StrokeCommand__FIELD_NAME__current_ma, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
omnipicker_3_node_msgs__msg__StrokeCommand__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {omnipicker_3_node_msgs__msg__StrokeCommand__TYPE_NAME, 40, 40},
      {omnipicker_3_node_msgs__msg__StrokeCommand__FIELDS, 5, 5},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# OP3 distance control. stroke_mm is millimetres; speed/current use SDK units.\n"
  "float64 stroke_mm\n"
  "bool use_speed\n"
  "int16 speed_rpm\n"
  "bool use_current\n"
  "int16 current_ma";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
omnipicker_3_node_msgs__msg__StrokeCommand__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {omnipicker_3_node_msgs__msg__StrokeCommand__TYPE_NAME, 40, 40},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 162, 162},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnipicker_3_node_msgs__msg__StrokeCommand__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *omnipicker_3_node_msgs__msg__StrokeCommand__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
