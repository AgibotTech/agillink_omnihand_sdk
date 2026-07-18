cmake_minimum_required(VERSION 3.8)
project(omnihand_ros2_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(omnihand_2025_node_msgs REQUIRED)
find_package(omnihand_pro_2025_node_msgs REQUIRED)

set(DEMO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

macro(omnihand_ros2_demo_target target_name source_file)
  add_executable(${target_name} ${source_file})
  target_include_directories(${target_name} PRIVATE ${DEMO_INCLUDE_DIR})
  ament_target_dependencies(${target_name} rclcpp sensor_msgs std_msgs omnihand_2025_node_msgs omnihand_pro_2025_node_msgs)
  install(TARGETS ${target_name} DESTINATION lib/${PROJECT_NAME})
endmacro()

omnihand_ros2_demo_target(ros2_joint_cmd_demo ros2_joint_cmd_demo.cpp)
omnihand_ros2_demo_target(ros2_mix_ctrl_pos_torque_demo ros2_mix_ctrl_pos_torque_demo.cpp)
omnihand_ros2_demo_target(ros2_mix_ctrl_pos_vel_torque_demo ros2_mix_ctrl_pos_vel_torque_demo.cpp)

ament_package()
