# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

cmake_minimum_required(VERSION 3.22)
project(omnihand_demo)

# Support both: building together (as subdirectory) and building separately (after installation)
if(TARGET omnihand)
  set(OMNIHAND_TARGET omnihand)
else()
  # Building separately: find installed omnihand package
  # Users can specify: cmake -DCMAKE_PREFIX_PATH=/custom/path
  # Default: /usr/local
  if(NOT CMAKE_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local")
  endif()
  foreach(_prefix ${CMAKE_PREFIX_PATH})
    list(APPEND CMAKE_MODULE_PATH "${_prefix}/share/cmake/omnihand")
  endforeach()
  find_package(omnihand REQUIRED)
  set(OMNIHAND_TARGET omnihand)
endif()

# Set output directory for demo executables
# Always use the top-level binary directory to ensure consistent output location
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # Standalone build - use current binary dir
  set(DEMO_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/omnihand/demo")
else()
  # Subdirectory build - use top-level binary dir (parent of cpp/demo build dir)
  # CMAKE_BINARY_DIR is the top-level build directory when building as subdirectory
  set(DEMO_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/omnihand/demo")
endif()

# Add subdirectories for each product
add_subdirectory(omnihand_2025)
add_subdirectory(omnihand_pro_2025)
add_subdirectory(omnihand_dex_umi)
add_subdirectory(omni_picker_3)
# Add OmniPicker 2025 demo conditionally
if(BUILD_OMNI_PICKER_2025)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/omnipicker_2025")
    add_subdirectory(omnipicker_2025)
  endif()
endif()
# Add OmniHand 3 Lite demo conditionally
if(BUILD_OMNIHAND_3_LITE)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/omnihand_3_lite")
    add_subdirectory(omnihand_3_lite)
  endif()
endif()
# Add OmniHand 3 Ultra M demo conditionally
if(BUILD_OMNIHAND_3_ULTRA_M)
  add_subdirectory(omnihand_3_ultra_m)
endif()

# OmniHand Server demos (WebSocket, REST, combined)
add_subdirectory(omnihand_server)