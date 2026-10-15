# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target omnipicker_3_node_msgs::omnipicker_3_node_msgs
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${omnipicker_3_node_msgs_TARGETS}.
if(omnipicker_3_node_msgs_TARGETS AND NOT TARGET omnipicker_3_node_msgs::omnipicker_3_node_msgs)
  add_library(omnipicker_3_node_msgs::omnipicker_3_node_msgs INTERFACE IMPORTED)
  set_target_properties(omnipicker_3_node_msgs::omnipicker_3_node_msgs PROPERTIES
    INTERFACE_LINK_LIBRARIES "${omnipicker_3_node_msgs_TARGETS}")
endif()
