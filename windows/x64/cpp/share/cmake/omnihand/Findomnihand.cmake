# Findomnihand.cmake - Windows
#
# Usage:
#   list(APPEND CMAKE_MODULE_PATH "path/to/share/cmake/omnihand")
#   find_package(omnihand REQUIRED)
#   target_link_libraries(your_target omnihand)

get_filename_component(_cmake_dir "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
# For installed path: C:/omnihand/share/cmake/omnihand -> C:/omnihand
# For release package: windows/x64/cpp/share/cmake/omnihand -> windows/x64
get_filename_component(_possible_root "${_cmake_dir}/../../.." ABSOLUTE)
if(EXISTS "${_possible_root}/cpp/include/omnihand")
  # Release package structure
  get_filename_component(OMNIHAND_ROOT "${_cmake_dir}/../../../.." ABSOLUTE)
else()
  # Installed structure
  get_filename_component(OMNIHAND_ROOT "${_cmake_dir}/../../.." ABSOLUTE)
endif()

# Try installed path first, then fallback to release package path
if(EXISTS "${OMNIHAND_ROOT}/include/omnihand")
  set(omnihand_INCLUDE_DIRS "${OMNIHAND_ROOT}/include")
  set(_lib_dir "${OMNIHAND_ROOT}/lib")
else()
  set(omnihand_INCLUDE_DIRS "${OMNIHAND_ROOT}/cpp/include")
  set(_lib_dir "${OMNIHAND_ROOT}/cpp/lib")
endif()

find_library(OMNIHAND_LIBRARY NAMES omnihand HINTS "${_lib_dir}" NO_DEFAULT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(omnihand REQUIRED_VARS omnihand_INCLUDE_DIRS OMNIHAND_LIBRARY)

if(omnihand_FOUND)
  if(NOT TARGET omnihand)
    add_library(omnihand SHARED IMPORTED)
    set_target_properties(omnihand PROPERTIES
      IMPORTED_IMPLIB "${OMNIHAND_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${omnihand_INCLUDE_DIRS}")
  endif()
  find_library(AGILINK_LOGGER_LIBRARY NAMES agilink_logger HINTS "${_lib_dir}" NO_DEFAULT_PATH)
  if(AGILINK_LOGGER_LIBRARY)
    if(NOT TARGET agilink_logger)
      add_library(agilink_logger SHARED IMPORTED)
      set_target_properties(agilink_logger PROPERTIES
        IMPORTED_IMPLIB "${AGILINK_LOGGER_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${omnihand_INCLUDE_DIRS}")
    endif()
    set_property(TARGET omnihand APPEND PROPERTY
      INTERFACE_LINK_LIBRARIES agilink_logger)
  endif()
endif()
