# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

# OmniHand 3 Ultra M (O20) Demo Examples

# CANFD (by canfd_id)
add_executable(demo_omnihand_3_ultra_m_canfd_id h3u_m_demo_canfd_id.cc)
target_link_libraries(demo_omnihand_3_ultra_m_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_3_ultra_m_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# CANFD (by serial_number)
add_executable(demo_omnihand_3_ultra_m_canfd_serial h3u_m_demo_canfd_serial.cc)
target_link_libraries(demo_omnihand_3_ultra_m_canfd_serial PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_3_ultra_m_canfd_serial PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# SocketCAN (Linux only)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(demo_omnihand_3_ultra_m_socketcan h3u_m_demo_socketcan.cc)
  target_link_libraries(demo_omnihand_3_ultra_m_socketcan PRIVATE ${OMNIHAND_TARGET})
  set_target_properties(demo_omnihand_3_ultra_m_socketcan PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
  )
endif()

# Xense visual-tactile sensor demo
if(OMNIHAND_WITH_XENSE)
  add_executable(demo_omnihand_3_ultra_m_xense h3u_m_demo_xense.cc)
  target_link_libraries(demo_omnihand_3_ultra_m_xense PRIVATE ${OMNIHAND_TARGET})
  set_target_properties(demo_omnihand_3_ultra_m_xense PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
  )
endif()
