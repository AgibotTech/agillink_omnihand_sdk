#include <omnihand/omnipicker_2025.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

enum class Commands : uint8_t {
  PATH = 1,
  NONE = 0xff
};

int main(int argc, char** argv) {
  using namespace agilink::omnihand;

  std::string input_path = "";
  auto prev = Commands::NONE;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (prev != Commands::NONE) {
      if (prev == Commands::PATH) {
        input_path = arg;
      }
      prev = Commands::NONE;
      continue;
    }
    if (arg == "--path" || arg == "-p") {
      prev = Commands::PATH;
      continue;
    }
    throw std::runtime_error("error occurred when processing " + arg);
  }

  auto picker = OmniPicker2025::createHandByUSB();
  if (!picker || !picker->Init()) {
    std::cerr << "failed to create USB picker" << std::endl;
    return 1;
  }

  picker->ShowDataDetails(true);

  const auto info = picker->ShowDeviceInfo();
  std::cout << ToString(info) << std::endl;

  if (!info.motor.calib_valid) {
    std::cerr << "motor calibration is invalid; run StartMotorCalibration() first" << std::endl;
    return 2;
  }

  if (info.motor.error_code != 0) {
    std::cerr << "motor error_code is not zero: " << info.motor.error_code << std::endl;
    return 3;
  }

  const auto range = picker->GetMitFrameRange();
  std::cout << "MIT range: pos=[" << range.pos_min << ", " << range.pos_max
            << "], vel=[" << range.vel_min << ", " << range.vel_max << "], tor=["
            << range.tor_min << ", " << range.tor_max << "], kp=[" << range.kp_min
            << ", " << range.kp_max << "], kd=[" << range.kd_min << ", "
            << range.kd_max << "]" << std::endl;

  const float pos_mid = 0.5f * (range.pos_min + range.pos_max);
  const float pos_span = 0.15f * (range.pos_max - range.pos_min);
  const float vel_test = range.vel_max;
  const float tor_test = range.tor_max;
  const float kp_test = range.kp_min + 0.2f * (range.kp_max - range.kp_min);
  const float kd_test = range.kd_min + 0.2f * (range.kd_max - range.kd_min);

  Op1USBCtrlFrame mit{};
  mit.pos = pos_mid + pos_span;
  mit.vel = vel_test;
  mit.tor = tor_test;
  mit.kp = kp_test;
  mit.kd = kd_test;

  std::cout << "send MIT target A: pos=" << mit.pos << ", vel=" << mit.vel
            << ", tor=" << mit.tor << ", kp=" << mit.kp << ", kd=" << mit.kd
            << std::endl;
  picker->SendMitFrameAsync(mit);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  mit.pos = pos_mid - pos_span;
  std::cout << "send MIT target B: pos=" << mit.pos << ", vel=" << mit.vel
            << ", tor=" << mit.tor << ", kp=" << mit.kp << ", kd=" << mit.kd
            << std::endl;
  picker->SendMitFrameAsync(mit);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  mit.pos = pos_mid;
  std::cout << "send MIT target hold: pos=" << mit.pos << ", vel=" << mit.vel
            << ", tor=" << mit.tor << ", kp=" << mit.kp << ", kd=" << mit.kd
            << std::endl;
  picker->SendMitFrameAsync(mit);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  if (input_path == "")
    return 0;

  std::filesystem::path otaFilePath(input_path);
  if (!std::filesystem::exists(otaFilePath)) {
    throw std::runtime_error("input file does not exist");
  }
  std::cout << "[INFO][UPDATE_FIRMWARE] update firmware: " << otaFilePath.string() << std::endl;
  picker->UpdateFirmware(otaFilePath.string());
  std::cout << "[INFO][UPDATE_FIRMWARE] update firmware success" << std::endl;

  // std::cout << "[INFO][UPDATE_FIRMWARE_VIA_FLASH] update firmware: " << otaFilePath.string() << std::endl;
  // picker->UpdateFirmwareViaFlash(otaFilePath.string());
  // std::cout << "[INFO][UPDATE_FIRMWARE_VIA_FLASH] update firmware success" << std::endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << "[INFO][START_MOTOR_CALIBRATION] start ..." << std::endl;
  if (!picker->StartMotorCalibration()) {
    std::cout << "[ERROR][START_MOTOR_CALIBRATION] error start motor calibration" << std::endl;
    return -1;
  }
  std::cout << "[INFO][START_MOTOR_CALIBRATION] completed" << std::endl;
  return 0;
}
