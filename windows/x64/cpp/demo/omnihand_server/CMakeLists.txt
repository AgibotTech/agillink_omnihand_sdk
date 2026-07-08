# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2

# OmniHand Server Client Demo Examples
# Variables (OMNIHAND_TARGET, DEMO_OUTPUT_DIR) are inherited from parent scope.

# WebSocket client demo (connects to a running omnihand_ws_server)
add_executable(demo_omnihand_ws_client demo_ws_client.cc)
target_link_libraries(demo_omnihand_ws_client PRIVATE ${OMNIHAND_TARGET})
if(WIN32)
  target_link_libraries(demo_omnihand_ws_client PRIVATE ws2_32)
endif()
set_target_properties(demo_omnihand_ws_client PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)

# REST client demo (connects to a running omnihand_rest_server)
add_executable(demo_omnihand_rest_client demo_rest_client.cc)
target_link_libraries(demo_omnihand_rest_client PRIVATE ${OMNIHAND_TARGET})
if(WIN32)
  target_link_libraries(demo_omnihand_rest_client PRIVATE ws2_32)
endif()
set_target_properties(demo_omnihand_rest_client PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${DEMO_OUTPUT_DIR}"
)
