// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnipicker_3_node_msgs:msg/GentleGripState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/gentle_grip_state.hpp"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__BUILDER_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnipicker_3_node_msgs/msg/detail/gentle_grip_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace builder
{

class Init_GentleGripState_raw_response
{
public:
  explicit Init_GentleGripState_raw_response(::omnipicker_3_node_msgs::msg::GentleGripState & msg)
  : msg_(msg)
  {}
  ::omnipicker_3_node_msgs::msg::GentleGripState raw_response(::omnipicker_3_node_msgs::msg::GentleGripState::_raw_response_type arg)
  {
    msg_.raw_response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::GentleGripState msg_;
};

class Init_GentleGripState_header
{
public:
  Init_GentleGripState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GentleGripState_raw_response header(::omnipicker_3_node_msgs::msg::GentleGripState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_GentleGripState_raw_response(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::GentleGripState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnipicker_3_node_msgs::msg::GentleGripState>()
{
  return omnipicker_3_node_msgs::msg::builder::Init_GentleGripState_header();
}

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__GENTLE_GRIP_STATE__BUILDER_HPP_
