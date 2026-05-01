// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnihand_msgs:msg/JointStateInt8.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT8__STRUCT_HPP_
#define OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT8__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__omnihand_msgs__msg__JointStateInt8 __attribute__((deprecated))
#else
# define DEPRECATED__omnihand_msgs__msg__JointStateInt8 __declspec(deprecated)
#endif

namespace omnihand_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JointStateInt8_
{
  using Type = JointStateInt8_<ContainerAllocator>;

  explicit JointStateInt8_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit JointStateInt8_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _name_type =
    std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>>;
  _name_type name;
  using _data_type =
    std::vector<int8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int8_t>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__name(
    const std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>> & _arg)
  {
    this->name = _arg;
    return *this;
  }
  Type & set__data(
    const std::vector<int8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int8_t>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnihand_msgs::msg::JointStateInt8_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnihand_msgs::msg::JointStateInt8_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnihand_msgs::msg::JointStateInt8_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnihand_msgs::msg::JointStateInt8_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnihand_msgs__msg__JointStateInt8
    std::shared_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnihand_msgs__msg__JointStateInt8
    std::shared_ptr<omnihand_msgs::msg::JointStateInt8_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JointStateInt8_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->name != other.name) {
      return false;
    }
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const JointStateInt8_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JointStateInt8_

// alias to use template instance with default allocator
using JointStateInt8 =
  omnihand_msgs::msg::JointStateInt8_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnihand_msgs

#endif  // OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT8__STRUCT_HPP_
