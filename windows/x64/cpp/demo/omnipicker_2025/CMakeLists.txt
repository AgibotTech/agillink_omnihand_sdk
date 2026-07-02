# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

# OmniPicker 2025 Demo Examples - CAN FD only
# Each example supports left/right/both modes

# CANFD (by canfd_id)
add_executable(demo_omnipicker_2025_canfd_id OP1_demo_canfd_id.cc)
target_link_libraries(demo_omnipicker_2025_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnipicker_2025_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# USB
add_executable(demo_omnipicker_2025_usb OP1_demo_usb.cc)
target_link_libraries(demo_omnipicker_2025_usb PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omnipicker_2025_usb PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)
