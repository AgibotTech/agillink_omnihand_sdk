// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnihand_2025_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__BUILDER_HPP_
#define OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnihand_2025_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnihand_2025_node_msgs
{

namespace msg
{

namespace builder
{

class Init_TactileSensor_dorsum
{
public:
  explicit Init_TactileSensor_dorsum(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  ::omnihand_2025_node_msgs::msg::TactileSensor dorsum(::omnihand_2025_node_msgs::msg::TactileSensor::_dorsum_type arg)
  {
    msg_.dorsum = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_palm
{
public:
  explicit Init_TactileSensor_palm(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_dorsum palm(::omnihand_2025_node_msgs::msg::TactileSensor::_palm_type arg)
  {
    msg_.palm = std::move(arg);
    return Init_TactileSensor_dorsum(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_little
{
public:
  explicit Init_TactileSensor_little(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_palm little(::omnihand_2025_node_msgs::msg::TactileSensor::_little_type arg)
  {
    msg_.little = std::move(arg);
    return Init_TactileSensor_palm(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_ring
{
public:
  explicit Init_TactileSensor_ring(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_little ring(::omnihand_2025_node_msgs::msg::TactileSensor::_ring_type arg)
  {
    msg_.ring = std::move(arg);
    return Init_TactileSensor_little(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_middle
{
public:
  explicit Init_TactileSensor_middle(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_ring middle(::omnihand_2025_node_msgs::msg::TactileSensor::_middle_type arg)
  {
    msg_.middle = std::move(arg);
    return Init_TactileSensor_ring(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_index
{
public:
  explicit Init_TactileSensor_index(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_middle index(::omnihand_2025_node_msgs::msg::TactileSensor::_index_type arg)
  {
    msg_.index = std::move(arg);
    return Init_TactileSensor_middle(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_thumb
{
public:
  explicit Init_TactileSensor_thumb(::omnihand_2025_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_index thumb(::omnihand_2025_node_msgs::msg::TactileSensor::_thumb_type arg)
  {
    msg_.thumb = std::move(arg);
    return Init_TactileSensor_index(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_header
{
public:
  Init_TactileSensor_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TactileSensor_thumb header(::omnihand_2025_node_msgs::msg::TactileSensor::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_TactileSensor_thumb(msg_);
  }

private:
  ::omnihand_2025_node_msgs::msg::TactileSensor msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnihand_2025_node_msgs::msg::TactileSensor>()
{
  return omnihand_2025_node_msgs::msg::builder::Init_TactileSensor_header();
}

}  // namespace omnihand_2025_node_msgs

#endif  // OMNIHAND_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__BUILDER_HPP_
