// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/gentle_grip_command.hpp"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__STRUCT_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripCommand __attribute__((deprecated))
#else
# define DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripCommand __declspec(deprecated)
#endif

namespace omnipicker_3_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct GentleGripCommand_
{
  using Type = GentleGripCommand_<ContainerAllocator>;

  explicit GentleGripCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->use_current_limit = false;
      this->current_limit_ma = 0;
    }
  }

  explicit GentleGripCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->use_current_limit = false;
      this->current_limit_ma = 0;
    }
  }

  // field types and members
  using _use_current_limit_type =
    bool;
  _use_current_limit_type use_current_limit;
  using _current_limit_ma_type =
    uint16_t;
  _current_limit_ma_type current_limit_ma;

  // setters for named parameter idiom
  Type & set__use_current_limit(
    const bool & _arg)
  {
    this->use_current_limit = _arg;
    return *this;
  }
  Type & set__current_limit_ma(
    const uint16_t & _arg)
  {
    this->current_limit_ma = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripCommand
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__GentleGripCommand
    std::shared_ptr<omnipicker_3_node_msgs::msg::GentleGripCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GentleGripCommand_ & other) const
  {
    if (this->use_current_limit != other.use_current_limit) {
      return false;
    }
    if (this->current_limit_ma != other.current_limit_ma) {
      return false;
    }
    return true;
  }
  bool operator!=(const GentleGripCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GentleGripCommand_

// alias to use template instance with default allocator
using GentleGripCommand =
  omnipicker_3_node_msgs::msg::GentleGripCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__STRUCT_HPP_
