# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2

cmake_minimum_required(VERSION 3.24)

# Option to enable/disable tests
option(BUILD_CPP_TESTS "Build C++ unit tests" ON)

if(NOT BUILD_CPP_TESTS)
  return()
endif()

# Find GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Support both: building together (as subdirectory) and building separately (after installation)
if(TARGET omnihand)
  set(OMNIHAND_TARGET omnihand)
else()
  # Building separately: find installed omnihand package
  if(NOT CMAKE_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local")
  endif()
  foreach(_prefix ${CMAKE_PREFIX_PATH})
    list(APPEND CMAKE_MODULE_PATH "${_prefix}/share/cmake/omnihand")
  endforeach()
  find_package(omnihand REQUIRED)
  set(OMNIHAND_TARGET omnihand)
endif()

# Set output directory for test executables
# Always use the top-level binary directory to ensure consistent output location
get_filename_component(TOP_LEVEL_BINARY_DIR "${CMAKE_BINARY_DIR}" DIRECTORY)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # Standalone build - use current binary dir
  set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/omnihand/test")
else()
  # Subdirectory build - use top-level binary dir (parent of cpp/test build dir)
  # CMAKE_BINARY_DIR is the top-level build directory when building as subdirectory
  set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/omnihand/test")
endif()

# OmniHand 2025 (O10) Tests - Combined USB/CANFD (legacy)
add_executable(test_omnihand_2025 test_omnihand_2025.cc)
target_link_libraries(
  test_omnihand_2025
  PRIVATE
  ${OMNIHAND_TARGET}
  GTest::gtest
)
set_target_properties(test_omnihand_2025 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
)
add_test(NAME OmniHand2025Test COMMAND test_omnihand_2025)

# OmniHand 2025 USB-specific Tests
add_executable(test_omnihand_2025_usb test_omnihand_2025_usb.cc)
target_link_libraries(
  test_omnihand_2025_usb
  PRIVATE
  ${OMNIHAND_TARGET}
  GTest::gtest
)
set_target_properties(test_omnihand_2025_usb PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
)
add_test(NAME OmniHand2025UsbTest COMMAND test_omnihand_2025_usb)

# OmniHand 2025 CANFD-specific Tests
add_executable(test_omnihand_2025_canfd test_omnihand_2025_canfd.cc)
target_link_libraries(
  test_omnihand_2025_canfd
  PRIVATE
  ${OMNIHAND_TARGET}
  GTest::gtest
)
set_target_properties(test_omnihand_2025_canfd PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
)
add_test(NAME OmniHand2025CanfdTest COMMAND test_omnihand_2025_canfd)

# OmniHand Pro 2025 (O12) Tests
add_executable(test_omnihand_pro_2025 test_omnihand_pro_2025.cc)
target_link_libraries(
  test_omnihand_pro_2025
  PRIVATE
  ${OMNIHAND_TARGET}
  GTest::gtest
)
set_target_properties(test_omnihand_pro_2025 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
)
add_test(NAME OmniHandPro2025Test COMMAND test_omnihand_pro_2025)

# OmniHand Dex UMI Tests
add_executable(test_omnihand_dex_umi test_omnihand_dex_umi.cc)
target_link_libraries(
  test_omnihand_dex_umi
  PRIVATE
  ${OMNIHAND_TARGET}
  GTest::gtest
  GTest::gtest_main
)
set_target_properties(test_omnihand_dex_umi PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
)
add_test(NAME OmniHandDexUMITest COMMAND test_omnihand_dex_umi)

# OmniHand 3 Lite (O4) Tests (conditionally)
# Note: BUILD_OMNIHAND_3_LITE is defined in the main CMakeLists.txt
if(BUILD_OMNIHAND_3_LITE)
  add_executable(test_omnihand_3_lite test_omnihand_3_lite.cc)
  target_link_libraries(
    test_omnihand_3_lite
    PRIVATE
    ${OMNIHAND_TARGET}
    GTest::gtest
  )
  set_target_properties(test_omnihand_3_lite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
  )
  add_test(NAME OmniHand3LiteTest COMMAND test_omnihand_3_lite)
endif()
