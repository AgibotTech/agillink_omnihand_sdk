# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2

# OmniHand 2025 (O10) Demo Examples
# Each example supports left/right/both modes

# Variables are inherited from parent scope automatically

if(NOT DEFINED OMNIHAND_ZLG_TCP_SUPPORTED)
  if(WIN32)
    set(OMNIHAND_ZLG_TCP_SUPPORTED 1)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64)$")
    set(OMNIHAND_ZLG_TCP_SUPPORTED 1)
  else()
    set(OMNIHAND_ZLG_TCP_SUPPORTED 0)
  endif()
endif()

# CANFD (by canfd_id)
add_executable(demo_omnihand_2025_canfd_id O10_demo_canfd_id.cc)
target_link_libraries(demo_omnihand_2025_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_2025_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# CANFD (by serial_number)
add_executable(demo_omnihand_2025_canfd_serial O10_demo_canfd_serial.cc)
target_link_libraries(demo_omnihand_2025_canfd_serial PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_2025_canfd_serial PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# ZLG CAN over TCP (e.g. WiFi/Ethernet adapter as server)
if(OMNIHAND_ZLG_TCP_SUPPORTED)
  add_executable(demo_omnihand_2025_zlgcan_tcp O10_demo_zlgcan_tcp.cc)
  target_link_libraries(demo_omnihand_2025_zlgcan_tcp PRIVATE ${OMNIHAND_TARGET})
  set_target_properties(demo_omnihand_2025_zlgcan_tcp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
  )
else()
  message(STATUS "Skip demo_omnihand_2025_zlgcan_tcp: OMNIHAND_ZLG_TCP_SUPPORTED=0 on this platform")
endif()

# RS485
add_executable(demo_omnihand_2025_rs485 O10_demo_rs485.cc)
target_link_libraries(demo_omnihand_2025_rs485 PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_2025_rs485 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# SocketCAN (Linux only)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(demo_omnihand_2025_socketcan O10_demo_socketcan.cc)
  target_link_libraries(demo_omnihand_2025_socketcan PRIVATE ${OMNIHAND_TARGET})
  set_target_properties(demo_omnihand_2025_socketcan PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
  )
endif()

# OTA Firmware Upgrade (CANFD only)
add_executable(demo_omnihand_2025_ota O10_demo_ota.cc)
target_link_libraries(demo_omnihand_2025_ota PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_2025_ota PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)
