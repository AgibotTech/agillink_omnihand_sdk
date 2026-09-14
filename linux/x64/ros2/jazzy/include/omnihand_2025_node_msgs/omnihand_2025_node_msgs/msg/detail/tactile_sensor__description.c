// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_omnihand_2025_node_msgs
const rosidl_type_hash_t *
omnihand_2025_node_msgs__msg__TactileSensor__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x42, 0xb0, 0x0c, 0x81, 0x06, 0xf9, 0x2a, 0xa8,
      0x50, 0x1d, 0xa0, 0x28, 0x8f, 0x6c, 0xb2, 0x46,
      0x68, 0x47, 0x58, 0x17, 0x94, 0x7f, 0xcc, 0x71,
      0x64, 0xa6, 0x8a, 0x6f, 0xbb, 0xd3, 0xcb, 0x9b,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "std_msgs/msg/detail/header__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char omnihand_2025_node_msgs__msg__TactileSensor__TYPE_NAME[] = "omnihand_2025_node_msgs/msg/TactileSensor";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__header[] = "header";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__thumb[] = "thumb";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__index[] = "index";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__middle[] = "middle";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__ring[] = "ring";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__little[] = "little";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__palm[] = "palm";
static char omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__dorsum[] = "dorsum";

static rosidl_runtime_c__type_description__Field omnihand_2025_node_msgs__msg__TactileSensor__FIELDS[] = {
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__thumb, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__index, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__middle, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__ring, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__little, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__palm, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {omnihand_2025_node_msgs__msg__TactileSensor__FIELD_NAME__dorsum, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription omnihand_2025_node_msgs__msg__TactileSensor__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
omnihand_2025_node_msgs__msg__TactileSensor__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {omnihand_2025_node_msgs__msg__TactileSensor__TYPE_NAME, 41, 41},
      {omnihand_2025_node_msgs__msg__TactileSensor__FIELDS, 8, 8},
    },
    {omnihand_2025_node_msgs__msg__TactileSensor__REFERENCED_TYPE_DESCRIPTIONS, 2, 2},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# OmniHand 2025 (O10) 1D tactile: one uint8[] per region; 1g per point, max 255g. \n"
  "std_msgs/Header header\n"
  "uint8[] thumb   #  16 points\n"
  "uint8[] index   #  18 points\n"
  "uint8[] middle  #  18 points\n"
  "uint8[] ring    #  18 points\n"
  "uint8[] little  #  18 points\n"
  "uint8[] palm    #  78 points\n"
  "uint8[] dorsum  # 102 points";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
omnihand_2025_node_msgs__msg__TactileSensor__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {omnihand_2025_node_msgs__msg__TactileSensor__TYPE_NAME, 41, 41},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 309, 309},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
omnihand_2025_node_msgs__msg__TactileSensor__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[3];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 3, 3};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *omnihand_2025_node_msgs__msg__TactileSensor__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
