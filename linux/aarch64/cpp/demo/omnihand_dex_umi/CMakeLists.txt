# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2

# OmniHand Dex UMI (O10 UMI) Demo Examples
# Each example supports left/right/both modes
# Note: UMI protocol is read-only, position/velocity/torque control is not supported

# Variables are inherited from parent scope automatically

# CANFD (by canfd_id)
add_executable(demo_omnihand_dex_umi_canfd_id UMI_demo_canfd_id.cc)
target_link_libraries(demo_omnihand_dex_umi_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_dex_umi_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# CANFD (by serial_number)
add_executable(demo_omnihand_dex_umi_canfd_serial UMI_demo_canfd_serial.cc)
target_link_libraries(demo_omnihand_dex_umi_canfd_serial PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_dex_umi_canfd_serial PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# SocketCAN (Linux only)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(demo_omnihand_dex_umi_socketcan UMI_demo_socketcan.cc)
  target_link_libraries(demo_omnihand_dex_umi_socketcan PRIVATE ${OMNIHAND_TARGET})
  set_target_properties(demo_omnihand_dex_umi_socketcan PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
  )
endif()
