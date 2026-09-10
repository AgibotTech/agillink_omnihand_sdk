// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_omnihand_pro_2025_node_msgs
const rosidl_type_hash_t *
omnihand_pro_2025_node_msgs__msg__TactileSensor__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x26, 0xf1, 0x89, 0x93, 0xec, 0xbf, 0x6d, 0x73,
      0xfa, 0x02, 0x5e, 0xa9, 0x85, 0x84, 0x40, 0xf6,
      0x64, 0x99, 0x1a, 0x2f, 0x96, 0xbc, 0x04, 0x6e,
      0x98, 0x11, 0xf4, 0xaf, 0x92, 0x0d, 0x49, 0x75,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "std_msgs/msg/detail/header__functions.h"
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t omnihand_pro_2025_node_msgs__msg__TactileSensorData__EXPECTED_HASH = {1, {
    0xc0, 0x5d, 0x17, 0x96, 0x4c, 0x85, 0xe3, 0x33,
    0xe5, 0x34, 0x78, 0x31, 0x88, 0x34, 0x2c, 0x25,
    0xd1, 0xa2, 0x41, 0xb7, 0x7a, 0xec, 0xd8, 0x5d,
    0xfc, 0x36, 0x77, 0x88, 0xce, 0xc9, 0x88, 0x19,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char omnihand_pro_2025_node_msgs__msg__TactileSensor__TYPE_NAME[] = "omnihand_pro_2025_node_msgs/msg/TactileSensor";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME[] = "omnihand_pro_2025_node_msgs/msg/TactileSensorData";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__header[] = "header";
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__thumb[] = "thumb";
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__index[] = "index";
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__middle[] = "middle";
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__ring[] = "ring";
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__little[] = "little";
static char omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__palm[] = "palm";

static rosidl_runtime_c__type_description__Field omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELDS[] = {
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__thumb, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__index, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__middle, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__ring, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__little, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELD_NAME__palm, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription omnihand_pro_2025_node_msgs__msg__TactileSensor__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {omnihand_pro_2025_node_msgs__msg__TactileSensorData__TYPE_NAME, 49, 49},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
omnihand_pro_2025_node_msgs__msg__TactileSensor__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {omnihand_pro_2025_node_msgs__msg__TactileSensor__TYPE_NAME, 45, 45},
      {omnihand_pro_2025_node_msgs__msg__TactileSensor__FIELDS, 7, 7},
    },
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__REFERENCED_TYPE_DESCRIPTIONS, 3, 3},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&omnihand_pro_2025_node_msgs__msg__TactileSensorData__EXPECTED_HASH, omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# OmniHand Pro 2025 (O12) 3D tactile: one TactileSensorData per finger (thumb \\xe2\\x80\\xa6 palm).\n"
  "std_msgs/Header header\n"
  "TactileSensorData thumb\n"
  "TactileSensorData index\n"
  "TactileSensorData middle\n"
  "TactileSensorData ring\n"
  "TactileSensorData little\n"
  "TactileSensorData palm";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
omnihand_pro_2025_node_msgs__msg__TactileSensor__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {omnihand_pro_2025_node_msgs__msg__TactileSensor__TYPE_NAME, 45, 45},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 254, 254},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnihand_pro_2025_node_msgs__msg__TactileSensor__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[4];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 4, 4};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *omnihand_pro_2025_node_msgs__msg__TactileSensor__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *omnihand_pro_2025_node_msgs__msg__TactileSensorData__get_individual_type_description_source(NULL);
    sources[3] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
