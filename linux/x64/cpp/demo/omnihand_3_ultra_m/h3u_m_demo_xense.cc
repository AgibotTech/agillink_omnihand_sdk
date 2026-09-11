// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2

#include <cstdio>
#include <memory>
#include <thread>
#include <chrono>
#include "agilink_logger.h"
#include "omnihand/omnihand_3_ultra_m.h"

using namespace agilink::omnihand;
using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniHand3UltraMDemo";

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  AgilinkLogger::get().infof(TAG, "OmniHand 3 Ultra (O20) with Xense Visual-Tactile Sensor Demo");

  uint8_t device_id = 9;
  uint8_t canfd_id = 0;
  uint8_t channel = 0;
  auto hand = OmniHand3UltraM::createHandByZlgcan(
      HandType::LEFT, device_id, canfd_id, channel);

  if (!hand) {
    AgilinkLogger::get().errorf(TAG, "Failed to create OmniHand 3 Ultra!");
    return 1;
  }
  AgilinkLogger::get().infof(TAG, "OmniHand 3 Ultra created (Xense sensors auto-initialized).");

  AgilinkLogger::get().infof(TAG, "Collecting tactile data for 5 seconds...");
  XenseFrame frame;
  int frame_count = 0;

  const char* finger_names[] = {"thumb", "index", "middle", "ring", "pinky"};
  for (int i = 0; i < 50; ++i) {
    for (int finger = 0; finger < 5; ++finger) {
      if (hand->GetFingerTactile(finger, frame)) {
        frame_count++;
        char buf[256];
        snprintf(buf, sizeof(buf),
                 "Finger[%d] (%s) Frame %d: %dx%d, force: %dx%d, depth: %dx%d, ts: %llu ns",
                 finger, finger_names[finger], frame_count,
                 (int)frame.rectify_width, (int)frame.rectify_height,
                 (int)frame.force_width, (int)frame.force_height,
                 (int)frame.depth_width, (int)frame.depth_height,
                 (unsigned long long)frame.timestamp);
        AgilinkLogger::get().infof(TAG, "%s", buf);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  AgilinkLogger::get().infof(TAG, "Collected %d frames.", frame_count);

  // Get palm tactile
  PalmFrame palm_frame;
  if (hand->GetPalmTactile(palm_frame)) {
    AgilinkLogger::get().infof(TAG, "Palm tactile: force size=%zu", palm_frame.force.size());
  }

  // Get all tactile at once
  AllTactileFrame all;
  if (hand->GetAllTactile(all)) {
    AgilinkLogger::get().infof(TAG, "GetAllTactile: 5 fingers + palm OK");
  }

  AgilinkLogger::get().infof(TAG, "Demo completed.");
  return 0;
}
