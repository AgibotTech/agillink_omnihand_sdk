// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnipicker_3_node_msgs:msg/StrokeState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/stroke_state.hpp"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__STRUCT_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__STRUCT_HPP_

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
# define DEPRECATED__omnipicker_3_node_msgs__msg__StrokeState __attribute__((deprecated))
#else
# define DEPRECATED__omnipicker_3_node_msgs__msg__StrokeState __declspec(deprecated)
#endif

namespace omnipicker_3_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct StrokeState_
{
  using Type = StrokeState_<ContainerAllocator>;

  explicit StrokeState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->stroke_mm = 0.0;
    }
  }

  explicit StrokeState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->stroke_mm = 0.0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _stroke_mm_type =
    double;
  _stroke_mm_type stroke_mm;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__stroke_mm(
    const double & _arg)
  {
    this->stroke_mm = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__StrokeState
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__StrokeState
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const StrokeState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->stroke_mm != other.stroke_mm) {
      return false;
    }
    return true;
  }
  bool operator!=(const StrokeState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct StrokeState_

// alias to use template instance with default allocator
using StrokeState =
  omnipicker_3_node_msgs::msg::StrokeState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__STRUCT_HPP_
