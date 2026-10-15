#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "omnipicker_3_node_msgs::omnipicker_3_node_msgs__rosidl_generator_c" for configuration "Release"
set_property(TARGET omnipicker_3_node_msgs::omnipicker_3_node_msgs__rosidl_generator_c APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(omnipicker_3_node_msgs::omnipicker_3_node_msgs__rosidl_generator_c PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libomnipicker_3_node_msgs__rosidl_generator_c.so"
  IMPORTED_SONAME_RELEASE "libomnipicker_3_node_msgs__rosidl_generator_c.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS omnipicker_3_node_msgs::omnipicker_3_node_msgs__rosidl_generator_c )
list(APPEND _IMPORT_CHECK_FILES_FOR_omnipicker_3_node_msgs::omnipicker_3_node_msgs__rosidl_generator_c "${_IMPORT_PREFIX}/lib/libomnipicker_3_node_msgs__rosidl_generator_c.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
