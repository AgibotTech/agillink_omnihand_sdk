// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__functions.h"
#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace omnihand_2025_node_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void TactileSensor_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) omnihand_2025_node_msgs::msg::TactileSensor(_init);
}

void TactileSensor_fini_function(void * message_memory)
{
  auto typed_message = static_cast<omnihand_2025_node_msgs::msg::TactileSensor *>(message_memory);
  typed_message->~TactileSensor();
}

size_t size_function__TactileSensor__thumb(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__thumb(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__thumb(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__thumb(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__thumb(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__thumb(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__thumb(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__thumb(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__index(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__index(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__index(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__index(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__index(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__index(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__index(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__index(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__middle(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__middle(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__middle(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__middle(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__middle(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__middle(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__middle(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__middle(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__ring(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__ring(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__ring(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__ring(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__ring(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__ring(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__ring(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__ring(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__little(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__little(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__little(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__little(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__little(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__little(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__little(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__little(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__palm(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__palm(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__palm(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__palm(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__palm(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__palm(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__palm(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__palm(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__dorsum(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__dorsum(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__dorsum(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__dorsum(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__TactileSensor__dorsum(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__dorsum(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__TactileSensor__dorsum(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__dorsum(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember TactileSensor_message_member_array[8] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "thumb",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, thumb),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__thumb,  // size() function pointer
    get_const_function__TactileSensor__thumb,  // get_const(index) function pointer
    get_function__TactileSensor__thumb,  // get(index) function pointer
    fetch_function__TactileSensor__thumb,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__thumb,  // assign(index, value) function pointer
    resize_function__TactileSensor__thumb  // resize(index) function pointer
  },
  {
    "index",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, index),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__index,  // size() function pointer
    get_const_function__TactileSensor__index,  // get_const(index) function pointer
    get_function__TactileSensor__index,  // get(index) function pointer
    fetch_function__TactileSensor__index,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__index,  // assign(index, value) function pointer
    resize_function__TactileSensor__index  // resize(index) function pointer
  },
  {
    "middle",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, middle),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__middle,  // size() function pointer
    get_const_function__TactileSensor__middle,  // get_const(index) function pointer
    get_function__TactileSensor__middle,  // get(index) function pointer
    fetch_function__TactileSensor__middle,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__middle,  // assign(index, value) function pointer
    resize_function__TactileSensor__middle  // resize(index) function pointer
  },
  {
    "ring",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, ring),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__ring,  // size() function pointer
    get_const_function__TactileSensor__ring,  // get_const(index) function pointer
    get_function__TactileSensor__ring,  // get(index) function pointer
    fetch_function__TactileSensor__ring,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__ring,  // assign(index, value) function pointer
    resize_function__TactileSensor__ring  // resize(index) function pointer
  },
  {
    "little",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, little),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__little,  // size() function pointer
    get_const_function__TactileSensor__little,  // get_const(index) function pointer
    get_function__TactileSensor__little,  // get(index) function pointer
    fetch_function__TactileSensor__little,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__little,  // assign(index, value) function pointer
    resize_function__TactileSensor__little  // resize(index) function pointer
  },
  {
    "palm",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, palm),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__palm,  // size() function pointer
    get_const_function__TactileSensor__palm,  // get_const(index) function pointer
    get_function__TactileSensor__palm,  // get(index) function pointer
    fetch_function__TactileSensor__palm,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__palm,  // assign(index, value) function pointer
    resize_function__TactileSensor__palm  // resize(index) function pointer
  },
  {
    "dorsum",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_2025_node_msgs::msg::TactileSensor, dorsum),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__dorsum,  // size() function pointer
    get_const_function__TactileSensor__dorsum,  // get_const(index) function pointer
    get_function__TactileSensor__dorsum,  // get(index) function pointer
    fetch_function__TactileSensor__dorsum,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__dorsum,  // assign(index, value) function pointer
    resize_function__TactileSensor__dorsum  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers TactileSensor_message_members = {
  "omnihand_2025_node_msgs::msg",  // message namespace
  "TactileSensor",  // message name
  8,  // number of fields
  sizeof(omnihand_2025_node_msgs::msg::TactileSensor),
  false,  // has_any_key_member_
  TactileSensor_message_member_array,  // message members
  TactileSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  TactileSensor_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t TactileSensor_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &TactileSensor_message_members,
  get_message_typesupport_handle_function,
  &omnihand_2025_node_msgs__msg__TactileSensor__get_type_hash,
  &omnihand_2025_node_msgs__msg__TactileSensor__get_type_description,
  &omnihand_2025_node_msgs__msg__TactileSensor__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace omnihand_2025_node_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<omnihand_2025_node_msgs::msg::TactileSensor>()
{
  return &::omnihand_2025_node_msgs::msg::rosidl_typesupport_introspection_cpp::TactileSensor_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, omnihand_2025_node_msgs, msg, TactileSensor)() {
  return &::omnihand_2025_node_msgs::msg::rosidl_typesupport_introspection_cpp::TactileSensor_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
