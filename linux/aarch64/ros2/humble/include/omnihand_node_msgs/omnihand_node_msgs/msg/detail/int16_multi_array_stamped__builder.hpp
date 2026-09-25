// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnihand_node_msgs:msg/Int16MultiArrayStamped.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_NODE_MSGS__MSG__DETAIL__INT16_MULTI_ARRAY_STAMPED__BUILDER_HPP_
#define OMNIHAND_NODE_MSGS__MSG__DETAIL__INT16_MULTI_ARRAY_STAMPED__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnihand_node_msgs/msg/detail/int16_multi_array_stamped__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnihand_node_msgs
{

namespace msg
{

namespace builder
{

class Init_Int16MultiArrayStamped_data
{
public:
  explicit Init_Int16MultiArrayStamped_data(::omnihand_node_msgs::msg::Int16MultiArrayStamped & msg)
  : msg_(msg)
  {}
  ::omnihand_node_msgs::msg::Int16MultiArrayStamped data(::omnihand_node_msgs::msg::Int16MultiArrayStamped::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnihand_node_msgs::msg::Int16MultiArrayStamped msg_;
};

class Init_Int16MultiArrayStamped_layout
{
public:
  explicit Init_Int16MultiArrayStamped_layout(::omnihand_node_msgs::msg::Int16MultiArrayStamped & msg)
  : msg_(msg)
  {}
  Init_Int16MultiArrayStamped_data layout(::omnihand_node_msgs::msg::Int16MultiArrayStamped::_layout_type arg)
  {
    msg_.layout = std::move(arg);
    return Init_Int16MultiArrayStamped_data(msg_);
  }

private:
  ::omnihand_node_msgs::msg::Int16MultiArrayStamped msg_;
};

class Init_Int16MultiArrayStamped_header
{
public:
  Init_Int16MultiArrayStamped_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Int16MultiArrayStamped_layout header(::omnihand_node_msgs::msg::Int16MultiArrayStamped::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Int16MultiArrayStamped_layout(msg_);
  }

private:
  ::omnihand_node_msgs::msg::Int16MultiArrayStamped msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnihand_node_msgs::msg::Int16MultiArrayStamped>()
{
  return omnihand_node_msgs::msg::builder::Init_Int16MultiArrayStamped_header();
}

}  // namespace omnihand_node_msgs

#endif  // OMNIHAND_NODE_MSGS__MSG__DETAIL__INT16_MULTI_ARRAY_STAMPED__BUILDER_HPP_
