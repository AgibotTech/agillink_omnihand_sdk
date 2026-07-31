// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_pro_2025_node_msgs/msg/tactile_sensor.hpp"


#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_HPP_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_HPP_

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
// Member 'thumb'
// Member 'index'
// Member 'middle'
// Member 'ring'
// Member 'little'
// Member 'palm'
#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensor __attribute__((deprecated))
#else
# define DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensor __declspec(deprecated)
#endif

namespace omnihand_pro_2025_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TactileSensor_
{
  using Type = TactileSensor_<ContainerAllocator>;

  explicit TactileSensor_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    thumb(_init),
    index(_init),
    middle(_init),
    ring(_init),
    little(_init),
    palm(_init)
  {
    (void)_init;
  }

  explicit TactileSensor_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    thumb(_alloc, _init),
    index(_alloc, _init),
    middle(_alloc, _init),
    ring(_alloc, _init),
    little(_alloc, _init),
    palm(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _thumb_type =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>;
  _thumb_type thumb;
  using _index_type =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>;
  _index_type index;
  using _middle_type =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>;
  _middle_type middle;
  using _ring_type =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>;
  _ring_type ring;
  using _little_type =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>;
  _little_type little;
  using _palm_type =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>;
  _palm_type palm;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__thumb(
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> & _arg)
  {
    this->thumb = _arg;
    return *this;
  }
  Type & set__index(
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> & _arg)
  {
    this->index = _arg;
    return *this;
  }
  Type & set__middle(
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> & _arg)
  {
    this->middle = _arg;
    return *this;
  }
  Type & set__ring(
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> & _arg)
  {
    this->ring = _arg;
    return *this;
  }
  Type & set__little(
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> & _arg)
  {
    this->little = _arg;
    return *this;
  }
  Type & set__palm(
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> & _arg)
  {
    this->palm = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensor
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensor
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const TactileSensor_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->thumb != other.thumb) {
      return false;
    }
    if (this->index != other.index) {
      return false;
    }
    if (this->middle != other.middle) {
      return false;
    }
    if (this->ring != other.ring) {
      return false;
    }
    if (this->little != other.little) {
      return false;
    }
    if (this->palm != other.palm) {
      return false;
    }
    return true;
  }
  bool operator!=(const TactileSensor_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct TactileSensor_

// alias to use template instance with default allocator
using TactileSensor =
  omnihand_pro_2025_node_msgs::msg::TactileSensor_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnihand_pro_2025_node_msgs

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_HPP_
