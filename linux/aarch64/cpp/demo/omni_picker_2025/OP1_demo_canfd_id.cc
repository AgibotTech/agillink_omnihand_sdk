#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "omnihand/omni_picker_2025.h"
#include "omnihand/proto.h"
static std::unique_ptr<agilink::omnihand::OmniPicker2025> picker;

int main(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }
  picker = agilink::omnihand::OmniPicker2025::createHandByHcan(
      agilink::omnihand::HandType::LEFT,
      1,
      0,
      0);
  if (!picker->Init()) {
    std::cout << "[ERROR][INIT] error init omnipicker 2025" << std::endl;
    return -1;
  }
  std::cout << picker->GetDeviceInfo().ToString() << std::endl;

  agilink::omnihand::Op1ControlFrame frame{};
  frame.pos_cmd = 0x00;
  frame.vel_cmd = 0xFF;
  frame.force_cmd = 0xFF;
  frame.acc_cmd = 0xFF;
  frame.dec_cmd = 0xFF;
  picker->SendFrameSync(frame);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  frame.pos_cmd = 0xFF;
  picker->SendFrameSync(frame);

  return 0;
}
