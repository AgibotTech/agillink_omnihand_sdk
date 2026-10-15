// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnipicker_3_node_msgs:msg/TactileSensor.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__BUILDER_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnipicker_3_node_msgs/msg/detail/tactile_sensor__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace builder
{

class Init_TactileSensor_index
{
public:
  explicit Init_TactileSensor_index(::omnipicker_3_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  ::omnipicker_3_node_msgs::msg::TactileSensor index(::omnipicker_3_node_msgs::msg::TactileSensor::_index_type arg)
  {
    msg_.index = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_thumb
{
public:
  explicit Init_TactileSensor_thumb(::omnipicker_3_node_msgs::msg::TactileSensor & msg)
  : msg_(msg)
  {}
  Init_TactileSensor_index thumb(::omnipicker_3_node_msgs::msg::TactileSensor::_thumb_type arg)
  {
    msg_.thumb = std::move(arg);
    return Init_TactileSensor_index(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::TactileSensor msg_;
};

class Init_TactileSensor_header
{
public:
  Init_TactileSensor_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TactileSensor_thumb header(::omnipicker_3_node_msgs::msg::TactileSensor::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_TactileSensor_thumb(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::TactileSensor msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnipicker_3_node_msgs::msg::TactileSensor>()
{
  return omnipicker_3_node_msgs::msg::builder::Init_TactileSensor_header();
}

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR__BUILDER_HPP_
