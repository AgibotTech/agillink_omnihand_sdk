// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnipicker_3_node_msgs:msg/GentleGripCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__BUILDER_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace builder
{

class Init_GentleGripCommand_current_limit_ma
{
public:
  explicit Init_GentleGripCommand_current_limit_ma(::omnipicker_3_node_msgs::msg::GentleGripCommand & msg)
  : msg_(msg)
  {}
  ::omnipicker_3_node_msgs::msg::GentleGripCommand current_limit_ma(::omnipicker_3_node_msgs::msg::GentleGripCommand::_current_limit_ma_type arg)
  {
    msg_.current_limit_ma = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::GentleGripCommand msg_;
};

class Init_GentleGripCommand_use_current_limit
{
public:
  Init_GentleGripCommand_use_current_limit()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GentleGripCommand_current_limit_ma use_current_limit(::omnipicker_3_node_msgs::msg::GentleGripCommand::_use_current_limit_type arg)
  {
    msg_.use_current_limit = std::move(arg);
    return Init_GentleGripCommand_current_limit_ma(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::GentleGripCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnipicker_3_node_msgs::msg::GentleGripCommand>()
{
  return omnipicker_3_node_msgs::msg::builder::Init_GentleGripCommand_use_current_limit();
}

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_COMMAND__BUILDER_HPP_
