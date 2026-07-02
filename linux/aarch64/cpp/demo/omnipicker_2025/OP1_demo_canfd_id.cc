#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>
#include "omnihand/omnipicker_2025.h"

int main() {
  using namespace agilink::omnihand;

  auto picker = OmniPicker2025::createHandByHcan(HandType::LEFT, 1, 0, 0);
  if (!picker || !picker->Init()) {
    std::cerr << "[ERROR][INIT] failed to init OmniPicker2025" << std::endl;
    return 1;
  }

  picker->ShowDataDetails(true);

  const auto info = picker->ShowDeviceInfo();
  std::cout << ToString(info) << std::endl;

  // ---- single-node sync control ----
  Op1CanfdCtrlFrame frame{};
  frame.vel_cmd   = 0xFF;
  frame.force_cmd = 0xFF;
  frame.acc_cmd   = 0xFF;
  frame.dec_cmd   = 0xFF;

  frame.pos_cmd = 0x00;
  picker->SendFrameSyncByCanfd(frame);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  frame.pos_cmd = 0xFF;
  picker->SendFrameSyncByCanfd(frame);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // ---- broadcast (CANFD FD 64-byte, CAN ID=0, no reply) ----
  Op1CanfdCtrlFrame f1{};
  f1.vel_cmd = f1.force_cmd = f1.acc_cmd = f1.dec_cmd = 0xFF;

  f1.pos_cmd = 0x00;
  picker->SendBroadcastFrameByCanfd({{1, f1}});
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  f1.pos_cmd = 0xFF;
  picker->SendBroadcastFrameByCanfd({{1, f1}});
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}
