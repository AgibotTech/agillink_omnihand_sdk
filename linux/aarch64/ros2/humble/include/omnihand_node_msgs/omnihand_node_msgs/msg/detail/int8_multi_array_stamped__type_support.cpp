// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from omnihand_node_msgs:msg/Int8MultiArrayStamped.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "omnihand_node_msgs/msg/detail/int8_multi_array_stamped__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace omnihand_node_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void Int8MultiArrayStamped_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) omnihand_node_msgs::msg::Int8MultiArrayStamped(_init);
}

void Int8MultiArrayStamped_fini_function(void * message_memory)
{
  auto typed_message = static_cast<omnihand_node_msgs::msg::Int8MultiArrayStamped *>(message_memory);
  typed_message->~Int8MultiArrayStamped();
}

size_t size_function__Int8MultiArrayStamped__data(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<int8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__Int8MultiArrayStamped__data(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<int8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__Int8MultiArrayStamped__data(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<int8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__Int8MultiArrayStamped__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const int8_t *>(
    get_const_function__Int8MultiArrayStamped__data(untyped_member, index));
  auto & value = *reinterpret_cast<int8_t *>(untyped_value);
  value = item;
}

void assign_function__Int8MultiArrayStamped__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<int8_t *>(
    get_function__Int8MultiArrayStamped__data(untyped_member, index));
  const auto & value = *reinterpret_cast<const int8_t *>(untyped_value);
  item = value;
}

void resize_function__Int8MultiArrayStamped__data(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<int8_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Int8MultiArrayStamped_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_node_msgs::msg::Int8MultiArrayStamped, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "layout",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::MultiArrayLayout>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_node_msgs::msg::Int8MultiArrayStamped, layout),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "data",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(omnihand_node_msgs::msg::Int8MultiArrayStamped, data),  // bytes offset in struct
    nullptr,  // default value
    size_function__Int8MultiArrayStamped__data,  // size() function pointer
    get_const_function__Int8MultiArrayStamped__data,  // get_const(index) function pointer
    get_function__Int8MultiArrayStamped__data,  // get(index) function pointer
    fetch_function__Int8MultiArrayStamped__data,  // fetch(index, &value) function pointer
    assign_function__Int8MultiArrayStamped__data,  // assign(index, value) function pointer
    resize_function__Int8MultiArrayStamped__data  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Int8MultiArrayStamped_message_members = {
  "omnihand_node_msgs::msg",  // message namespace
  "Int8MultiArrayStamped",  // message name
  3,  // number of fields
  sizeof(omnihand_node_msgs::msg::Int8MultiArrayStamped),
  Int8MultiArrayStamped_message_member_array,  // message members
  Int8MultiArrayStamped_init_function,  // function to initialize message memory (memory has to be allocated)
  Int8MultiArrayStamped_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Int8MultiArrayStamped_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Int8MultiArrayStamped_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace omnihand_node_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<omnihand_node_msgs::msg::Int8MultiArrayStamped>()
{
  return &::omnihand_node_msgs::msg::rosidl_typesupport_introspection_cpp::Int8MultiArrayStamped_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, omnihand_node_msgs, msg, Int8MultiArrayStamped)() {
  return &::omnihand_node_msgs::msg::rosidl_typesupport_introspection_cpp::Int8MultiArrayStamped_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
