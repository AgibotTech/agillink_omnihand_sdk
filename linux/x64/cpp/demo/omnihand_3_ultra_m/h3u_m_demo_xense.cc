// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "omnihand/omnihand_3_ultra_m.h"

using namespace agilink::omnihand;

int main(int argc, char** argv) {
  std::string xense_sn;
  if (argc > 1) {
    xense_sn = argv[1];
  }

  std::cout << "OmniHand 3 Ultra (O20) with Xense Visual-Tactile Sensor Demo\n\n";

  uint8_t device_id = 9;
  uint8_t canfd_id = 0;
  uint8_t channel = 0;
  auto hand = OmniHand3UltraM::createHandByZlgcan(
      HandType::LEFT, device_id, canfd_id, channel, xense_sn);

  if (!hand) {
    std::cerr << "Failed to create OmniHand 3 Ultra!\n";
    return 1;
  }
  std::cout << "OmniHand 3 Ultra created successfully.\n";

  if (!hand->XenseInit()) {
    std::cerr << "Failed to initialize Xense sensor!\n";
    return 1;
  }
  std::cout << "Xense sensor initialized.\n";

  if (!hand->XenseStart()) {
    std::cerr << "Failed to start Xense streaming!\n";
    return 1;
  }
  std::cout << "Xense streaming started.\n";

  hand->XenseCalibrate();

  std::cout << "Collecting Xense data for 5 seconds...\n";
  XenseFrame frame;
  int frame_count = 0;

  for (int i = 0; i < 50; ++i) {
    if (hand->XenseGetFrame(frame)) {
      frame_count++;
      std::cout << "Frame " << frame_count << ": "
                << frame.rgb_width << "x" << frame.rgb_height
                << ", depth: " << frame.depth_width << "x" << frame.depth_height
                << ", ts: " << frame.timestamp << " ns\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << "Collected " << frame_count << " frames.\n";

  hand->XenseStop();
  hand->XenseExportRuntimeConfig("xense_runtime_config.json");

  std::cout << "Demo completed.\n";
  return 0;
}