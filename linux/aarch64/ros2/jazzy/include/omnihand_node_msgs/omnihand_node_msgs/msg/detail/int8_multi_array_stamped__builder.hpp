// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnihand_node_msgs:msg/Int8MultiArrayStamped.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnihand_node_msgs/msg/int8_multi_array_stamped.hpp"


#ifndef OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__BUILDER_HPP_
#define OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnihand_node_msgs/msg/detail/int8_multi_array_stamped__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnihand_node_msgs
{

namespace msg
{

namespace builder
{

class Init_Int8MultiArrayStamped_data
{
public:
  explicit Init_Int8MultiArrayStamped_data(::omnihand_node_msgs::msg::Int8MultiArrayStamped & msg)
  : msg_(msg)
  {}
  ::omnihand_node_msgs::msg::Int8MultiArrayStamped data(::omnihand_node_msgs::msg::Int8MultiArrayStamped::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnihand_node_msgs::msg::Int8MultiArrayStamped msg_;
};

class Init_Int8MultiArrayStamped_layout
{
public:
  explicit Init_Int8MultiArrayStamped_layout(::omnihand_node_msgs::msg::Int8MultiArrayStamped & msg)
  : msg_(msg)
  {}
  Init_Int8MultiArrayStamped_data layout(::omnihand_node_msgs::msg::Int8MultiArrayStamped::_layout_type arg)
  {
    msg_.layout = std::move(arg);
    return Init_Int8MultiArrayStamped_data(msg_);
  }

private:
  ::omnihand_node_msgs::msg::Int8MultiArrayStamped msg_;
};

class Init_Int8MultiArrayStamped_header
{
public:
  Init_Int8MultiArrayStamped_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Int8MultiArrayStamped_layout header(::omnihand_node_msgs::msg::Int8MultiArrayStamped::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Int8MultiArrayStamped_layout(msg_);
  }

private:
  ::omnihand_node_msgs::msg::Int8MultiArrayStamped msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnihand_node_msgs::msg::Int8MultiArrayStamped>()
{
  return omnihand_node_msgs::msg::builder::Init_Int8MultiArrayStamped_header();
}

}  // namespace omnihand_node_msgs

#endif  // OMNIHAND_NODE_MSGS__MSG__DETAIL__INT8_MULTI_ARRAY_STAMPED__BUILDER_HPP_
