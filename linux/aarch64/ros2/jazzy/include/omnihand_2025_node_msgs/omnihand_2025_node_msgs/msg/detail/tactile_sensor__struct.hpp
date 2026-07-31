// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_2025_node_msgs/msg/tactile_sensor.hpp"


#ifndef OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_HPP_
#define OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_HPP_

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
# define DEPRECATED__omnihand_2025_node_msgs__msg__TactileSensor __attribute__((deprecated))
#else
# define DEPRECATED__omnihand_2025_node_msgs__msg__TactileSensor __declspec(deprecated)
#endif

namespace omnihand_2025_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TactileSensor_
{
  using Type = TactileSensor_<ContainerAllocator>;

  explicit TactileSensor_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit TactileSensor_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _thumb_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _thumb_type thumb;
  using _index_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _index_type index;
  using _middle_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _middle_type middle;
  using _ring_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _ring_type ring;
  using _little_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _little_type little;
  using _palm_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _palm_type palm;
  using _dorsum_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _dorsum_type dorsum;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__thumb(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->thumb = _arg;
    return *this;
  }
  Type & set__index(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->index = _arg;
    return *this;
  }
  Type & set__middle(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->middle = _arg;
    return *this;
  }
  Type & set__ring(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->ring = _arg;
    return *this;
  }
  Type & set__little(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->little = _arg;
    return *this;
  }
  Type & set__palm(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->palm = _arg;
    return *this;
  }
  Type & set__dorsum(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->dorsum = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnihand_2025_node_msgs__msg__TactileSensor
    std::shared_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnihand_2025_node_msgs__msg__TactileSensor
    std::shared_ptr<omnihand_2025_node_msgs::msg::TactileSensor_<ContainerAllocator> const>
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
    if (this->dorsum != other.dorsum) {
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
  omnihand_2025_node_msgs::msg::TactileSensor_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnihand_2025_node_msgs

#endif  // OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__STRUCT_HPP_
