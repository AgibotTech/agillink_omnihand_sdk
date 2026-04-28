# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

# OmniHand 3 Lite (O4) Demo Examples - CAN FD only
# Each example supports left/right/both modes

# CANFD (by canfd_id)
add_executable(demo_omnihand_3_lite_canfd_id O4_demo_canfd_id.cc)
target_link_libraries(demo_omnihand_3_lite_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_3_lite_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# CANFD (by serial_number)
add_executable(demo_omnihand_3_lite_canfd_serial O4_demo_canfd_serial.cc)
target_link_libraries(demo_omnihand_3_lite_canfd_serial PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnihand_3_lite_canfd_serial PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)
