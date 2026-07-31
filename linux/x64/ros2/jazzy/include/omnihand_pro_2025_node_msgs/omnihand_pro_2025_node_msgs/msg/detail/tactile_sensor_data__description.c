// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice

#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_omnihand_pro_2025_node_msgs
const rosidl_type_hash_t *
omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xc0, 0x5d, 0x17, 0x96, 0x4c, 0x85, 0xe3, 0x33,
      0xe5, 0x34, 0x78, 0x31, 0x88, 0x34, 0x2c, 0x25,
      0xd1, 0xa2, 0x41, 0xb7, 0x7a, 0xec, 0xd8, 0x5d,
      0xfc, 0x36, 0x77, 0x88, 0xce, 0xc9, 0x88, 0x19,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME[] = "omnihand_pro_2025_node_msgs/msg/TactileSensorData";

// Define type names, field names, and default values
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__online_state[] = "online_state";
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__channel_value[] = "channel_value";
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__normal_force[] = "normal_force";
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__tangent_force[] = "tangent_force";
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__tangent_force_angle[] = "tangent_force_angle";
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__capa_approach[] = "capa_approach";

static rosidl_runtime_c__type_description__Field omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELDS[] = {
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__online_state, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__channel_value, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__normal_force, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__tangent_force, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__tangent_force_angle, 19, 19},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELD_NAME__capa_approach, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__FIELDS, 6, 6},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint8 online_state         # 1=sensor online; 0=sensor offline\n"
  "uint32[] channel_value     # 6-channel 24-bit value\n"
  "uint16 normal_force        # normal force (0.1 N, 0 ~ 3000)\n"
  "uint16 tangent_force       # tangent force (0.1 N, 0 ~ 3000)\n"
  "uint16 tangent_force_angle # tangent force angle, fingertip-up = 0\\xc2\\xb0, clockwise (0~359)\n"
  "uint8[] capa_approach      # 4 self-capacitance proximity values";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 388, 388},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
