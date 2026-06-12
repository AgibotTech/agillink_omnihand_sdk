# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

# OmniPicker 3 Demo Examples - CAN FD only
# Each example supports left/right/both modes

# CANFD (by canfd_id)
add_executable(demo_omni_picker_3_canfd_id OP3_demo_canfd_id.cc)
target_link_libraries(demo_omni_picker_3_canfd_id PRIVATE ${OMNIHAND_TARGET})
set_target_properties(demo_omni_picker_3_canfd_id PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)
