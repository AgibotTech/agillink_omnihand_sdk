// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from omnipicker_3_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__functions.h"
#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void TactileSensor_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) omnipicker_3_node_msgs::msg::TactileSensor(_init);
}

void TactileSensor_fini_function(void * message_memory)
{
  auto typed_message = static_cast<omnipicker_3_node_msgs::msg::TactileSensor *>(message_memory);
  typed_message->~TactileSensor();
}

size_t size_function__TactileSensor__thumb(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint16_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__thumb(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint16_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__thumb(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint16_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__thumb(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint16_t *>(
    get_const_function__TactileSensor__thumb(untyped_member, index));
  auto & value = *reinterpret_cast<uint16_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__thumb(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint16_t *>(
    get_function__TactileSensor__thumb(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint16_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__thumb(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint16_t> *>(untyped_member);
  member->resize(size);
}

size_t size_function__TactileSensor__index(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint16_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__TactileSensor__index(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint16_t> *>(untyped_member);
  return &member[index];
}

void * get_function__TactileSensor__index(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint16_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__TactileSensor__index(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint16_t *>(
    get_const_function__TactileSensor__index(untyped_member, index));
  auto & value = *reinterpret_cast<uint16_t *>(untyped_value);
  value = item;
}

void assign_function__TactileSensor__index(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint16_t *>(
    get_function__TactileSensor__index(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint16_t *>(untyped_value);
  item = value;
}

void resize_function__TactileSensor__index(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint16_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember TactileSensor_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs::msg::TactileSensor, header),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs::msg::TactileSensor, thumb),  // bytes offset in struct
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
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnipicker_3_node_msgs::msg::TactileSensor, index),  // bytes offset in struct
    nullptr,  // default value
    size_function__TactileSensor__index,  // size() function pointer
    get_const_function__TactileSensor__index,  // get_const(index) function pointer
    get_function__TactileSensor__index,  // get(index) function pointer
    fetch_function__TactileSensor__index,  // fetch(index, &value) function pointer
    assign_function__TactileSensor__index,  // assign(index, value) function pointer
    resize_function__TactileSensor__index  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers TactileSensor_message_members = {
  "omnipicker_3_node_msgs::msg",  // message namespace
  "TactileSensor",  // message name
  3,  // number of fields
  sizeof(omnipicker_3_node_msgs::msg::TactileSensor),
  false,  // has_any_key_member_
  TactileSensor_message_member_array,  // message members
  TactileSensor_init_function,  // function to initialize message memory (memory has to be allocated)
  TactileSensor_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t TactileSensor_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &TactileSensor_message_members,
  get_message_typesupport_handle_function,
  &omnipicker_3_node_msgs__msg__TactileSensor__get_type_hash,
  &omnipicker_3_node_msgs__msg__TactileSensor__get_type_description,
  &omnipicker_3_node_msgs__msg__TactileSensor__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace omnipicker_3_node_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<omnipicker_3_node_msgs::msg::TactileSensor>()
{
  return &::omnipicker_3_node_msgs::msg::rosidl_typesupport_introspection_cpp::TactileSensor_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, omnipicker_3_node_msgs, msg, TactileSensor)() {
  return &::omnipicker_3_node_msgs::msg::rosidl_typesupport_introspection_cpp::TactileSensor_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
