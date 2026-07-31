// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "omnihand/omnihand_3_ultra_m.h"

using namespace agilink::omnihand;

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  std::cout << "OmniHand 3 Ultra (O20) with Xense Visual-Tactile Sensor Demo\n\n";

  uint8_t device_id = 9;
  uint8_t canfd_id = 0;
  uint8_t channel = 0;
  auto hand = OmniHand3UltraM::createHandByZlgcan(
      HandType::LEFT, device_id, canfd_id, channel);

  if (!hand) {
    std::cerr << "Failed to create OmniHand 3 Ultra!\n";
    return 1;
  }
  std::cout << "OmniHand 3 Ultra created (Xense sensors auto-initialized).\n\n";

  std::cout << "Collecting tactile data for 5 seconds...\n";
  XenseFrame frame;
  int frame_count = 0;

  const char* finger_names[] = {"thumb", "index", "middle", "ring", "pinky"};
  for (int i = 0; i < 50; ++i) {
    for (int finger = 0; finger < 5; ++finger) {
      if (hand->GetFingerTactile(finger, frame)) {
        frame_count++;
        std::cout << "Finger[" << finger << "] (" << finger_names[finger] << ") "
                  << "Frame " << frame_count << ": "
                  << frame.rectify_width << "x" << frame.rectify_height
                  << ", force: " << frame.force_width << "x" << frame.force_height
                  << ", depth: " << frame.depth_width << "x" << frame.depth_height
                  << ", ts: " << frame.timestamp << " ns\n";
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << "Collected " << frame_count << " frames.\n\n";

  // Get palm tactile
  PalmFrame palm_frame;
  if (hand->GetPalmTactile(palm_frame)) {
    std::cout << "Palm tactile: force size=" << palm_frame.force.size() << "\n";
  }

  // Get all tactile at once
  AllTactileFrame all;
  if (hand->GetAllTactile(all)) {
    std::cout << "GetAllTactile: 5 fingers + palm OK\n";
  }

  std::cout << "Demo completed.\n";
  return 0;
}