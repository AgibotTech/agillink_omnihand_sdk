// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from omnihand_pro_2025_node_msgs:msg/TactileSensorData.idl
// generated code does not contain a copyright notice

#ifndef OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__BUILDER_HPP_
#define OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "omnihand_pro_2025_node_msgs/msg/detail/tactile_sensor_data__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace omnihand_pro_2025_node_msgs
{

namespace msg
{

namespace builder
{

class Init_TactileSensorData_capa_approach
{
public:
  explicit Init_TactileSensorData_capa_approach(::omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg)
  : msg_(msg)
  {}
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData capa_approach(::omnihand_pro_2025_node_msgs::msg::TactileSensorData::_capa_approach_type arg)
  {
    msg_.capa_approach = std::move(arg);
    return std::move(msg_);
  }

private:
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData msg_;
};

class Init_TactileSensorData_tangent_force_angle
{
public:
  explicit Init_TactileSensorData_tangent_force_angle(::omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg)
  : msg_(msg)
  {}
  Init_TactileSensorData_capa_approach tangent_force_angle(::omnihand_pro_2025_node_msgs::msg::TactileSensorData::_tangent_force_angle_type arg)
  {
    msg_.tangent_force_angle = std::move(arg);
    return Init_TactileSensorData_capa_approach(msg_);
  }

private:
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData msg_;
};

class Init_TactileSensorData_tangent_force
{
public:
  explicit Init_TactileSensorData_tangent_force(::omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg)
  : msg_(msg)
  {}
  Init_TactileSensorData_tangent_force_angle tangent_force(::omnihand_pro_2025_node_msgs::msg::TactileSensorData::_tangent_force_type arg)
  {
    msg_.tangent_force = std::move(arg);
    return Init_TactileSensorData_tangent_force_angle(msg_);
  }

private:
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData msg_;
};

class Init_TactileSensorData_normal_force
{
public:
  explicit Init_TactileSensorData_normal_force(::omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg)
  : msg_(msg)
  {}
  Init_TactileSensorData_tangent_force normal_force(::omnihand_pro_2025_node_msgs::msg::TactileSensorData::_normal_force_type arg)
  {
    msg_.normal_force = std::move(arg);
    return Init_TactileSensorData_tangent_force(msg_);
  }

private:
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData msg_;
};

class Init_TactileSensorData_channel_value
{
public:
  explicit Init_TactileSensorData_channel_value(::omnihand_pro_2025_node_msgs::msg::TactileSensorData & msg)
  : msg_(msg)
  {}
  Init_TactileSensorData_normal_force channel_value(::omnihand_pro_2025_node_msgs::msg::TactileSensorData::_channel_value_type arg)
  {
    msg_.channel_value = std::move(arg);
    return Init_TactileSensorData_normal_force(msg_);
  }

private:
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData msg_;
};

class Init_TactileSensorData_online_state
{
public:
  Init_TactileSensorData_online_state()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TactileSensorData_channel_value online_state(::omnihand_pro_2025_node_msgs::msg::TactileSensorData::_online_state_type arg)
  {
    msg_.online_state = std::move(arg);
    return Init_TactileSensorData_channel_value(msg_);
  }

private:
  ::omnihand_pro_2025_node_msgs::msg::TactileSensorData msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::omnihand_pro_2025_node_msgs::msg::TactileSensorData>()
{
  return omnihand_pro_2025_node_msgs::msg::builder::Init_TactileSensorData_online_state();
}

}  // namespace omnihand_pro_2025_node_msgs

#endif  // OMNIHAND_PRO_2025_NODE_MSGS__MSG__DETAIL__TACTILE_SENSOR_DATA__BUILDER_HPP_
