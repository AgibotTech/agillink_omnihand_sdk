// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnipicker_3_node_msgs:msg/GentleGripState.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__STRUCT_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__STRUCT_HPP_

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
# define DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripState __attribute__((deprecated))
#else
# define DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripState __declspec(deprecated)
#endif

namespace omnipicker_3_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct GentleGripState_
{
  using Type = GentleGripState_<ContainerAllocator>;

  explicit GentleGripState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit GentleGripState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _raw_response_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _raw_response_type raw_response;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__raw_response(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->raw_response = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripState
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripState
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GentleGripState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->raw_response != other.raw_response) {
      return false;
    }
    return true;
  }
  bool operator!=(const GentleGripState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GentleGripState_

// alias to use template instance with default allocator
using GentleGripState =
  omnipicker_3_node_msgs::msg::GentleGripState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__STRUCT_HPP_
