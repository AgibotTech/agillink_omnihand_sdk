// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from omnihand_msgs:msg/JointStateInt8.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "omnihand_msgs/msg/detail/joint_state_int8__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace omnihand_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void JointStateInt8_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) omnihand_msgs::msg::JointStateInt8(_init);
}

void JointStateInt8_fini_function(void * message_memory)
{
  auto typed_message = static_cast<omnihand_msgs::msg::JointStateInt8 *>(message_memory);
  typed_message->~JointStateInt8();
}

size_t size_function__JointStateInt8__name(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return member->size();
}

const void * get_const_function__JointStateInt8__name(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void * get_function__JointStateInt8__name(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointStateInt8__name(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const std::string *>(
    get_const_function__JointStateInt8__name(untyped_member, index));
  auto & value = *reinterpret_cast<std::string *>(untyped_value);
  value = item;
}

void assign_function__JointStateInt8__name(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<std::string *>(
    get_function__JointStateInt8__name(untyped_member, index));
  const auto & value = *reinterpret_cast<const std::string *>(untyped_value);
  item = value;
}

void resize_function__JointStateInt8__name(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<std::string> *>(untyped_member);
  member->resize(size);
}

size_t size_function__JointStateInt8__data(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<int8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__JointStateInt8__data(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<int8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__JointStateInt8__data(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<int8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointStateInt8__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const int8_t *>(
    get_const_function__JointStateInt8__data(untyped_member, index));
  auto & value = *reinterpret_cast<int8_t *>(untyped_value);
  value = item;
}

void assign_function__JointStateInt8__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<int8_t *>(
    get_function__JointStateInt8__data(untyped_member, index));
  const auto & value = *reinterpret_cast<const int8_t *>(untyped_value);
  item = value;
}

void resize_function__JointStateInt8__data(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<int8_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember JointStateInt8_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_msgs::msg::JointStateInt8, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "name",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_msgs::msg::JointStateInt8, name),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointStateInt8__name,  // size() function pointer
    get_const_function__JointStateInt8__name,  // get_const(index) function pointer
    get_function__JointStateInt8__name,  // get(index) function pointer
    fetch_function__JointStateInt8__name,  // fetch(index, &value) function pointer
    assign_function__JointStateInt8__name,  // assign(index, value) function pointer
    resize_function__JointStateInt8__name  // resize(index) function pointer
  },
  {
    "data",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_msgs::msg::JointStateInt8, data),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointStateInt8__data,  // size() function pointer
    get_const_function__JointStateInt8__data,  // get_const(index) function pointer
    get_function__JointStateInt8__data,  // get(index) function pointer
    fetch_function__JointStateInt8__data,  // fetch(index, &value) function pointer
    assign_function__JointStateInt8__data,  // assign(index, value) function pointer
    resize_function__JointStateInt8__data  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers JointStateInt8_message_members = {
  "omnihand_msgs::msg",  // message namespace
  "JointStateInt8",  // message name
  3,  // number of fields
  sizeof(omnihand_msgs::msg::JointStateInt8),
  JointStateInt8_message_member_array,  // message members
  JointStateInt8_init_function,  // function to initialize message memory (memory has to be allocated)
  JointStateInt8_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t JointStateInt8_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &JointStateInt8_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace omnihand_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<omnihand_msgs::msg::JointStateInt8>()
{
  return &::omnihand_msgs::msg::rosidl_typesupport_introspection_cpp::JointStateInt8_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, omnihand_msgs, msg, JointStateInt8)() {
  return &::omnihand_msgs::msg::rosidl_typesupport_introspection_cpp::JointStateInt8_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
