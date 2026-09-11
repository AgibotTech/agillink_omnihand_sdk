#include <omnihand/omnipicker_2025.h>

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <thread>
#include "agilink_logger.h"

using agilink::AgilinkLogger;
static constexpr const char* TAG = "OmniPicker2025Demo";

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
    AgilinkLogger::get().errorf(TAG, "failed to create USB picker");
    return 1;
  }

  picker->ShowDataDetails(true);

  const auto info = picker->ShowDeviceInfo();
  AgilinkLogger::get().infof(TAG, "%s", ToString(info).c_str());

  if (!info.motor.calib_valid) {
    AgilinkLogger::get().errorf(TAG, "motor calibration is invalid; run StartMotorCalibration() first");
    return 2;
  }

  if (info.motor.error_code != 0) {
    AgilinkLogger::get().errorf(TAG, "motor error_code is not zero: %d", info.motor.error_code);
    return 3;
  }

  const auto range = picker->GetMitFrameRange();
  AgilinkLogger::get().infof(TAG,
      "MIT range: pos=[%f, %f], vel=[%f, %f], tor=[%f, %f], kp=[%f, %f], kd=[%f, %f]",
      range.pos_min, range.pos_max,
      range.vel_min, range.vel_max,
      range.tor_min, range.tor_max,
      range.kp_min, range.kp_max,
      range.kd_min, range.kd_max);

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

  AgilinkLogger::get().infof(TAG,
      "send MIT target A: pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
      mit.pos, mit.vel, mit.tor, mit.kp, mit.kd);
  picker->SendMitFrameAsync(mit);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  mit.pos = pos_mid - pos_span;
  AgilinkLogger::get().infof(TAG,
      "send MIT target B: pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
      mit.pos, mit.vel, mit.tor, mit.kp, mit.kd);
  picker->SendMitFrameAsync(mit);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  mit.pos = pos_mid;
  AgilinkLogger::get().infof(TAG,
      "send MIT target hold: pos=%f, vel=%f, tor=%f, kp=%f, kd=%f",
      mit.pos, mit.vel, mit.tor, mit.kp, mit.kd);
  picker->SendMitFrameAsync(mit);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  if (input_path == "")
    return 0;

  std::filesystem::path otaFilePath(input_path);
  if (!std::filesystem::exists(otaFilePath)) {
    throw std::runtime_error("input file does not exist");
  }
  AgilinkLogger::get().infof(TAG, "[INFO][UPDATE_FIRMWARE] update firmware: %s", otaFilePath.string().c_str());
  picker->UpdateFirmware(otaFilePath.string());
  AgilinkLogger::get().infof(TAG, "[INFO][UPDATE_FIRMWARE] update firmware success");

  // std::cout << "[INFO][UPDATE_FIRMWARE_VIA_FLASH] update firmware: " << otaFilePath.string() << std::endl;
  // picker->UpdateFirmwareViaFlash(otaFilePath.string());
  // std::cout << "[INFO][UPDATE_FIRMWARE_VIA_FLASH] update firmware success" << std::endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  AgilinkLogger::get().infof(TAG, "[INFO][START_MOTOR_CALIBRATION] start ...");
  if (!picker->StartMotorCalibration()) {
    AgilinkLogger::get().infof(TAG, "[ERROR][START_MOTOR_CALIBRATION] error start motor calibration");
    return -1;
  }
  AgilinkLogger::get().infof(TAG, "[INFO][START_MOTOR_CALIBRATION] completed");
  return 0;
}
