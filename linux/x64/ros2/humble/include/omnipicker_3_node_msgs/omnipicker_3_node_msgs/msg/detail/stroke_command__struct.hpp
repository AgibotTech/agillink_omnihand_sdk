// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__STRUCT_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__omnipicker_3_node_msgs__msg__StrokeCommand __attribute__((deprecated))
#else
# define DEPRECATED__omnipicker_3_node_msgs__msg__StrokeCommand __declspec(deprecated)
#endif

namespace omnipicker_3_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct StrokeCommand_
{
  using Type = StrokeCommand_<ContainerAllocator>;

  explicit StrokeCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->stroke_mm = 0.0;
      this->use_speed = false;
      this->speed_rpm = 0;
      this->use_current = false;
      this->current_ma = 0;
    }
  }

  explicit StrokeCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->stroke_mm = 0.0;
      this->use_speed = false;
      this->speed_rpm = 0;
      this->use_current = false;
      this->current_ma = 0;
    }
  }

  // field types and members
  using _stroke_mm_type =
    double;
  _stroke_mm_type stroke_mm;
  using _use_speed_type =
    bool;
  _use_speed_type use_speed;
  using _speed_rpm_type =
    int16_t;
  _speed_rpm_type speed_rpm;
  using _use_current_type =
    bool;
  _use_current_type use_current;
  using _current_ma_type =
    int16_t;
  _current_ma_type current_ma;

  // setters for named parameter idiom
  Type & set__stroke_mm(
    const double & _arg)
  {
    this->stroke_mm = _arg;
    return *this;
  }
  Type & set__use_speed(
    const bool & _arg)
  {
    this->use_speed = _arg;
    return *this;
  }
  Type & set__speed_rpm(
    const int16_t & _arg)
  {
    this->speed_rpm = _arg;
    return *this;
  }
  Type & set__use_current(
    const bool & _arg)
  {
    this->use_current = _arg;
    return *this;
  }
  Type & set__current_ma(
    const int16_t & _arg)
  {
    this->current_ma = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__StrokeCommand
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnipicker_3_node_msgs__msg__StrokeCommand
    std::shared_ptr<omnipicker_3_node_msgs::msg::StrokeCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const StrokeCommand_ & other) const
  {
    if (this->stroke_mm != other.stroke_mm) {
      return false;
    }
    if (this->use_speed != other.use_speed) {
      return false;
    }
    if (this->speed_rpm != other.speed_rpm) {
      return false;
    }
    if (this->use_current != other.use_current) {
      return false;
    }
    if (this->current_ma != other.current_ma) {
      return false;
    }
    return true;
  }
  bool operator!=(const StrokeCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct StrokeCommand_

// alias to use template instance with default allocator
using StrokeCommand =
  omnipicker_3_node_msgs::msg::StrokeCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__STRUCT_HPP_
