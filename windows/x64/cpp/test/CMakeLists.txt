# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

cmake_minimum_required(VERSION 3.22)

# Option to enable/disable tests
option(BUILD_CPP_TESTS "Build C++ unit tests" ON)

if(NOT BUILD_CPP_TESTS)
  return()
endif()

# GoogleTest: prefer cpp/third_party/googletest-1.14.0 in repo and use add_subdirectory directly,
# to avoid FetchContent generating googletest-subbuild; after project path migration, absolute paths in _deps CMakeCache may mismatch and fail.
get_filename_component(_gtest_vendor_default "${CMAKE_CURRENT_LIST_DIR}/../third_party/googletest-1.14.0" ABSOLUTE)
if(NOT googletest_LOCAL_SOURCE AND EXISTS "${_gtest_vendor_default}/CMakeLists.txt")
  set(googletest_LOCAL_SOURCE "${_gtest_vendor_default}" CACHE PATH "Local googletest source (skips FetchContent)")
endif()

include(FetchContent)
# For Windows: Prevent overriding the parent project's compiler settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(googletest_LOCAL_SOURCE)
  get_filename_component(_gtest_src "${googletest_LOCAL_SOURCE}" ABSOLUTE)
  add_subdirectory("${_gtest_src}" "${CMAKE_BINARY_DIR}/_deps/googletest-build" EXCLUDE_FROM_ALL)
else()
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

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
  target_compile_definitions(test_omnihand_3_lite PRIVATE BUILD_OMNIHAND_3_LITE)
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

# OmniHand 3 Ultra M (O20) Tests
if(BUILD_OMNIHAND_3_ULTRA_M)
  add_executable(test_omnihand_3_ultra_m test_omnihand_3_ultra_m.cc)
  target_link_libraries(
    test_omnihand_3_ultra_m
    PRIVATE
    ${OMNIHAND_TARGET}
    GTest::gtest
  )
  set_target_properties(test_omnihand_3_ultra_m PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
  )
  add_test(NAME OmniHand3UltraMTest COMMAND test_omnihand_3_ultra_m)
endif()

# OmniHand Server Tests (HandService, HandWsServer, HandRestServer)
# HandWsServerTest / HandRestServerTest: no device needed.
# HandServiceTest: requires a real hand; select device with -d zlgcan|hcan|socketcan.
add_executable(test_omnihand_server test_omnihand_server.cc)
target_link_libraries(
  test_omnihand_server
  PRIVATE
  ${OMNIHAND_TARGET}
  GTest::gtest
)
if(WIN32)
  target_link_libraries(test_omnihand_server PRIVATE ws2_32)
endif()
set_target_properties(test_omnihand_server PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}"
)
add_test(NAME OmniHandServerTest COMMAND test_omnihand_server)
