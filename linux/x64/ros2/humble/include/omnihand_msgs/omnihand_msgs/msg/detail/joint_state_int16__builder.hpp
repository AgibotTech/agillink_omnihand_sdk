// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnihand_msgs:msg/JointStateInt16.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__BUILDER_HPP_
#define OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnihand_msgs/msg/detail/joint_state_int16__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnihand_msgs
{

namespace msg
{

namespace builder
{

class Init_JointStateInt16_data
{
public:
  explicit Init_JointStateInt16_data(::omnihand_msgs::msg::JointStateInt16 & msg)
  : msg_(msg)
  {}
  ::omnihand_msgs::msg::JointStateInt16 data(::omnihand_msgs::msg::JointStateInt16::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnihand_msgs::msg::JointStateInt16 msg_;
};

class Init_JointStateInt16_name
{
public:
  explicit Init_JointStateInt16_name(::omnihand_msgs::msg::JointStateInt16 & msg)
  : msg_(msg)
  {}
  Init_JointStateInt16_data name(::omnihand_msgs::msg::JointStateInt16::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_JointStateInt16_data(msg_);
  }

private:
  ::omnihand_msgs::msg::JointStateInt16 msg_;
};

class Init_JointStateInt16_header
{
public:
  Init_JointStateInt16_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_JointStateInt16_name header(::omnihand_msgs::msg::JointStateInt16::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_JointStateInt16_name(msg_);
  }

private:
  ::omnihand_msgs::msg::JointStateInt16 msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnihand_msgs::msg::JointStateInt16>()
{
  return omnihand_msgs::msg::builder::Init_JointStateInt16_header();
}

}  // namespace omnihand_msgs

#endif  // OMNIHAND_MSGS__MSG__DETAIL__JOINT_STATE_INT16__BUILDER_HPP_
