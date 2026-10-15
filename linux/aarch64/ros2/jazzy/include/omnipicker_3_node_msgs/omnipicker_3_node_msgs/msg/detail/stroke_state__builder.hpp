// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnipicker_3_node_msgs:msg/StrokeState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "omnipicker_3_node_msgs/msg/stroke_state.hpp"


#ifndef OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__BUILDER_HPP_
#define OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnipicker_3_node_msgs/msg/detail/stroke_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnipicker_3_node_msgs
{

namespace msg
{

namespace builder
{

class Init_StrokeState_stroke_mm
{
public:
  explicit Init_StrokeState_stroke_mm(::omnipicker_3_node_msgs::msg::StrokeState & msg)
  : msg_(msg)
  {}
  ::omnipicker_3_node_msgs::msg::StrokeState stroke_mm(::omnipicker_3_node_msgs::msg::StrokeState::_stroke_mm_type arg)
  {
    msg_.stroke_mm = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeState msg_;
};

class Init_StrokeState_header
{
public:
  Init_StrokeState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StrokeState_stroke_mm header(::omnipicker_3_node_msgs::msg::StrokeState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_StrokeState_stroke_mm(msg_);
  }

private:
  ::omnipicker_3_node_msgs::msg::StrokeState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnipicker_3_node_msgs::msg::StrokeState>()
{
  return omnipicker_3_node_msgs::msg::builder::Init_StrokeState_header();
}

}  // namespace omnipicker_3_node_msgs

#endif  // OMNIPICKER_3_NODE_MSGS__MSG__DETAIL__STROKE_STATE__BUILDER_HPP_
