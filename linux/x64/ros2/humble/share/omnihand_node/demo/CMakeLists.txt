cmake_minimum_required(VERSION 3.8)
project(omnihand_ros2_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(omnihand_msgs REQUIRED)

add_executable(ros2_joint_cmd_demo ros2_joint_cmd_demo.cpp)
ament_target_dependencies(ros2_joint_cmd_demo rclcpp sensor_msgs omnihand_msgs)

install(TARGETS ros2_joint_cmd_demo DESTINATION lib/${PROJECT_NAME})

ament_package()
