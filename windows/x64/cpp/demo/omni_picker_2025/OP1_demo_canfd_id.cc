#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include "omnihand/omni_picker_2025.h"
#include "omnihand/proto.h"
static std::unique_ptr<agilink::omnihand::OmniPicker2025> picker;
enum class Commands : uint8_t {
  PATH = 1,
  NONE = 0xff
};
int main(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }
  std::string input_path = "";
  auto prev = Commands::NONE;
  for (auto& arg : args) {
    if (prev != Commands::NONE) {
      if (prev == Commands::PATH) {
        input_path = arg;
      }
      continue;
    }
    if (arg == "--path" || arg == "-p") {
      prev = Commands::PATH;
      continue;
    }
    throw std::runtime_error("error occupid when process " + arg);
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
  // std::cout << picker->GetDeviceInfo().ToString() << std::endl;
  picker->ShowDataDetails(true);
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
  if (input_path == "")
    return 0;
  std::filesystem::path otaFilePath(input_path);
  if (!std::filesystem::exists(otaFilePath)) {
    throw std::runtime_error("input file is not existed");
  }
  picker->UpdateFirmware(otaFilePath.string());
  std::cout << "[INFO][UPDATE_FIRMWARE] update firmware success" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  frame.pos_cmd = 0x00;
  picker->SendFrameSync(frame);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  frame.pos_cmd = 0xFF;
  picker->SendFrameSync(frame);
  return 0;
}
