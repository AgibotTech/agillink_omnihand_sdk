// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnipicker_3_node_msgs:msg/StrokeCommand.idl
// generated code does not contain a copyright notice

#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__BUILDER_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnipicker_3_node_msgs/msg/detail/stroke_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace builder
{

class Init_StrokeCommand_current_ma
{
public:
  explicit Init_StrokeCommand_current_ma(::omnipicker_3_node_msgs::msg::StrokeCommand & msg)
  : msg_(msg)
  {}
  ::omnipicker_3_node_msgs::msg::StrokeCommand current_ma(::omnipicker_3_node_msgs::msg::StrokeCommand::_current_ma_type arg)
  {
    msg_.current_ma = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeCommand msg_;
};

class Init_StrokeCommand_use_current
{
public:
  explicit Init_StrokeCommand_use_current(::omnipicker_3_node_msgs::msg::StrokeCommand & msg)
  : msg_(msg)
  {}
  Init_StrokeCommand_current_ma use_current(::omnipicker_3_node_msgs::msg::StrokeCommand::_use_current_type arg)
  {
    msg_.use_current = std::move(arg);
    return Init_StrokeCommand_current_ma(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeCommand msg_;
};

class Init_StrokeCommand_speed_rpm
{
public:
  explicit Init_StrokeCommand_speed_rpm(::omnipicker_3_node_msgs::msg::StrokeCommand & msg)
  : msg_(msg)
  {}
  Init_StrokeCommand_use_current speed_rpm(::omnipicker_3_node_msgs::msg::StrokeCommand::_speed_rpm_type arg)
  {
    msg_.speed_rpm = std::move(arg);
    return Init_StrokeCommand_use_current(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeCommand msg_;
};

class Init_StrokeCommand_use_speed
{
public:
  explicit Init_StrokeCommand_use_speed(::omnipicker_3_node_msgs::msg::StrokeCommand & msg)
  : msg_(msg)
  {}
  Init_StrokeCommand_speed_rpm use_speed(::omnipicker_3_node_msgs::msg::StrokeCommand::_use_speed_type arg)
  {
    msg_.use_speed = std::move(arg);
    return Init_StrokeCommand_speed_rpm(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeCommand msg_;
};

class Init_StrokeCommand_stroke_mm
{
public:
  Init_StrokeCommand_stroke_mm()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StrokeCommand_use_speed stroke_mm(::omnipicker_3_node_msgs::msg::StrokeCommand::_stroke_mm_type arg)
  {
    msg_.stroke_mm = std::move(arg);
    return Init_StrokeCommand_use_speed(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnipicker_3_node_msgs::msg::StrokeCommand>()
{
  return omnipicker_3_node_msgs::msg::builder::Init_StrokeCommand_stroke_mm();
}

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_COMMAND__BUILDER_HPP_
