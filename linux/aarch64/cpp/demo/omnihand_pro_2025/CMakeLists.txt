# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

# OmniHand Pro 2025 (O12) Demo Examples
# Each example supports left/right/both modes

# Variables are inherited from parent scope automatically

# CANFD (by canfd_id)
add_executable(demo_omnihand_pro_2025_canfd_id O12_demo_canfd_id.cc)
target_link_libraries(demo_omnihand_pro_2025_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_pro_2025_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# CANFD (by serial_number)
add_executable(demo_omnihand_pro_2025_canfd_serial O12_demo_canfd_serial.cc)
target_link_libraries(demo_omnihand_pro_2025_canfd_serial PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_pro_2025_canfd_serial PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# SocketCAN (Linux only)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(demo_omnihand_pro_2025_socketcan O12_demo_socketcan.cc)
  target_link_libraries(demo_omnihand_pro_2025_socketcan PRIVATE ${OMNIHAND_TARGET})
  set_target_properties(demo_omnihand_pro_2025_socketcan PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
  )
endif()
