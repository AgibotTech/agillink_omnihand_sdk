# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target omnihand_pro_2025_node_msgs::omnihand_pro_2025_node_msgs
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${omnihand_pro_2025_node_msgs_TARGETS}.
if(omnihand_pro_2025_node_msgs_TARGETS AND NOT TARGET omnihand_pro_2025_node_msgs::omnihand_pro_2025_node_msgs)
  add_library(omnihand_pro_2025_node_msgs::omnihand_pro_2025_node_msgs INTERFACE IMPORTED)
  set_target_properties(omnihand_pro_2025_node_msgs::omnihand_pro_2025_node_msgs PROPERTIES
    INTERFACE_LINK_LIBRARIES "${omnihand_pro_2025_node_msgs_TARGETS}")
endif()
