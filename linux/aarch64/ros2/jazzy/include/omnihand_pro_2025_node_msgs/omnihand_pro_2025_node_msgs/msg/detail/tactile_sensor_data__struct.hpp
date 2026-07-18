// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_pro_2025_node_msgs/msg/tactile_sensor_data.hpp"


#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__STRUCT_HPP_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensorData __attribute__((deprecated))
#else
# define DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensorData __declspec(deprecated)
#endif

namespace omnihand_pro_2025_node_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TactileSensorData_
{
  using Type = TactileSensorData_<ContainerAllocator>;

  explicit TactileSensorData_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->online_state = 0;
      this->normal_force = 0;
      this->tangent_force = 0;
      this->tangent_force_angle = 0;
    }
  }

  explicit TactileSensorData_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->online_state = 0;
      this->normal_force = 0;
      this->tangent_force = 0;
      this->tangent_force_angle = 0;
    }
  }

  // field types and members
  using _online_state_type =
    uint8_t;
  _online_state_type online_state;
  using _channel_value_type =
    std::vector<uint32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint32_t>>;
  _channel_value_type channel_value;
  using _normal_force_type =
    uint16_t;
  _normal_force_type normal_force;
  using _tangent_force_type =
    uint16_t;
  _tangent_force_type tangent_force;
  using _tangent_force_angle_type =
    uint16_t;
  _tangent_force_angle_type tangent_force_angle;
  using _capa_approach_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _capa_approach_type capa_approach;

  // setters for named parameter idiom
  Type & set__online_state(
    const uint8_t & _arg)
  {
    this->online_state = _arg;
    return *this;
  }
  Type & set__channel_value(
    const std::vector<uint32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint32_t>> & _arg)
  {
    this->channel_value = _arg;
    return *this;
  }
  Type & set__normal_force(
    const uint16_t & _arg)
  {
    this->normal_force = _arg;
    return *this;
  }
  Type & set__tangent_force(
    const uint16_t & _arg)
  {
    this->tangent_force = _arg;
    return *this;
  }
  Type & set__tangent_force_angle(
    const uint16_t & _arg)
  {
    this->tangent_force_angle = _arg;
    return *this;
  }
  Type & set__capa_approach(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->capa_approach = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> *;
  using ConstRawPtr =
    const omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensorData
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__omnihand_pro_2025_node_msgs__msg__TactileSensorData
    std::shared_ptr<omnihand_pro_2025_node_msgs::msg::TactileSensorData_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const TactileSensorData_ & other) const
  {
    if (this->online_state != other.online_state) {
      return false;
    }
    if (this->channel_value != other.channel_value) {
      return false;
    }
    if (this->normal_force != other.normal_force) {
      return false;
    }
    if (this->tangent_force != other.tangent_force) {
      return false;
    }
    if (this->tangent_force_angle != other.tangent_force_angle) {
      return false;
    }
    if (this->capa_approach != other.capa_approach) {
      return false;
    }
    return true;
  }
  bool operator!=(const TactileSensorData_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct TactileSensorData_

// alias to use template instance with default allocator
using TactileSensorData =
  omnihand_pro_2025_node_msgs::msg::TactileSensorData_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace omnihand_pro_2025_node_msgs

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__STRUCT_HPP_
