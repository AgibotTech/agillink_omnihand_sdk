// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file OP3_demo_rs485.cc
 * @brief OmniPicker 3 control demo - RS485 communication (standard serial protocol)
 */

#include <omnihand/omnipicker_3.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

namespace oh = agilink::omnihand;

enum class ParseResult {
  OK,
  HELP,
  ERROR,
};

struct Options {
#ifdef _WIN32
  std::string uart_port = "COM3";
#else
  std::string uart_port = "/dev/ttyUSB0";
#endif
  int32_t baudrate = 460800;
  uint8_t hand_device_id = oh::OmniPicker3::kDefaultHandDeviceId;
  bool verbose = false;
};

std::string GetProgramName(const char* argv0) {
  const std::string path = argv0 == nullptr ? "" : argv0;
  const size_t pos = path.find_last_of("/\\");
  return pos == std::string::npos ? path : path.substr(pos + 1);
}

void PrintUsage(const std::string& program_name) {
  std::cout << "Usage: " << program_name
            << " [-h|--help] [-p|--port <port>] [-b|--baudrate <baudrate>] [-i|--id <device_id>]"
               " [-v|--verbose]"
            << std::endl;
  std::cout << "  -p, --port\t\t\tserial port (default /dev/ttyUSB0 on Linux, COM3 on Windows)"
            << std::endl;
  std::cout << "  -b, --baudrate\t\tbaud rate (default 460800)" << std::endl;
  std::cout << "  -i, --id\t\t\thand device id (default 1)" << std::endl;
  std::cout << "  -v, --verbose\t\t\tdump raw serial frames (TX/RX hex)" << std::endl;
  std::cout << "  -h, --help\t\t\tshow help of project" << std::endl;
}

ParseResult ParseArgs(int argc, char* argv[], Options* options) {
  const std::string program_name = GetProgramName(argc > 0 ? argv[0] : "");

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      PrintUsage(program_name);
      return ParseResult::HELP;
    }
    if (arg == "-v" || arg == "--verbose") {
      options->verbose = true;
      continue;
    }
    if (i + 1 == argc) {
      std::cout << "error: missing value for option: " << arg << std::endl;
      PrintUsage(program_name);
      return ParseResult::ERROR;
    }

    const std::string value = argv[++i];
    if (arg == "-p" || arg == "--port") {
      options->uart_port = value;
    } else if (arg == "-b" || arg == "--baudrate") {
      options->baudrate = static_cast<int32_t>(std::atol(value.c_str()));
    } else if (arg == "-i" || arg == "--id") {
      options->hand_device_id = static_cast<uint8_t>(std::atoi(value.c_str()));
    } else {
      std::cout << "error: unsupported option: " << arg << std::endl;
      PrintUsage(program_name);
      return ParseResult::ERROR;
    }
  }

  return ParseResult::OK;
}

template <typename T>
void PrintVector(const std::string& name, const std::vector<T>& values) {
  std::cout << name << ": [";
  for (size_t i = 0; i < values.size(); ++i) {
    if (i > 0) {
      std::cout << ", ";
    }
    std::cout << values[i];
  }
  std::cout << "]" << std::endl;
}

void PrintTactilePreview(const oh::TactileSensorDataU16& sensor) {
  constexpr size_t kPreviewSize = 16;
  const size_t preview_size = std::min(kPreviewSize, sensor.data_.size());

  std::cout << oh::ToString(sensor.sensor_id_) << " tactile: "
            << sensor.data_.size() << " points, preview: [";

  for (size_t i = 0; i < preview_size; ++i) {
    if (i > 0) {
      std::cout << ", ";
    }
    std::cout << sensor.data_[i];
  }
  if (preview_size < sensor.data_.size()) {
    std::cout << ", ...";
  }
  std::cout << "]" << std::endl;
}

void PrintAllErrorReports(const std::vector<oh::JointMotorErrorReport>& errors) {
  if (errors.empty()) {
    std::cout << "All error reports: []" << std::endl;
    return;
  }

  for (size_t i = 0; i < errors.size(); ++i) {
    std::cout << "Joint " << (i + 1) << " error: " << errors[i].ToString() << std::endl;
  }
}

void PrintMixControlReply(const std::vector<oh::MixCtrl>& reply) {
  if (reply.empty()) {
    std::cout << "MixControlByPT reply: []" << std::endl;
    return;
  }

  for (const auto& ctrl : reply) {
    std::cout << "MixControlByPT reply joint_field="
              << static_cast<unsigned int>(ctrl.joint_index_)
              << ", mode=" << oh::ToString(static_cast<oh::MixControlMode>(ctrl.ctrl_mode_));
    if (ctrl.tgt_posi_) {
      std::cout << ", pos=" << *ctrl.tgt_posi_;
    }
    if (ctrl.tgt_torque_) {
      std::cout << ", torque=" << *ctrl.tgt_torque_;
    }
    std::cout << std::endl;
  }
}

void DemoDeviceInfo(oh::OmniPicker3& hand) {
  std::cout << "\n========== Device Info ==========" << std::endl;
  std::cout << "Product type: " << oh::ToString(hand.GetProductType()) << std::endl;
  std::cout << "Device info:\n" << hand.GetDeviceInfo().ToString();
  std::cout << "Vendor info:\n" << hand.GetVendorInfo().ToString();
  PrintVector("Joint names", hand.GetJointNames());
  auto range = oh::OmniPicker3::GetMinMaxMotorPosition();
  std::cout << "motor position range: [" << range.min_value << ", " << range.max_value << "]"
            << std::endl;
}

void DemoCommunicationSettings(oh::OmniPicker3& hand) {
  std::cout << "\n========== Communication Settings ==========" << std::endl;
  std::cout << "Request interval: " << hand.GetRequestInterval() << " ms" << std::endl;
  std::cout << "Frame receive timeout: " << hand.GetFrameRecvTimeout() << " ms" << std::endl;
}

// OP3 has no kinematics solver; these are expected to log a warning and return without
// sending any protocol frame. Demoed here to document that behavior with evidence.
void DemoKinematicsStubs(oh::OmniPicker3& hand) {
  std::cout << "\n========== Joint Angle APIs (no kinematics solver on OP3) ==========" << std::endl;
  PrintVector("SetAllActiveJointAngles({0.5})", hand.SetAllActiveJointAngles({0.5}));
  PrintVector("GetAllActiveJointAngles()", hand.GetAllActiveJointAngles());
  PrintVector("GetAllJointAngles()", hand.GetAllJointAngles());
  PrintVector("GetAllJointAngles({0.5})", hand.GetAllJointAngles({0.5}));
}

void DemoPositionControl(oh::OmniPicker3& hand) {
  std::cout << "\n========== Position Control ==========" << std::endl;
  std::cout << "Joint 1 position: " << hand.GetJointMotorPosi(1) << std::endl;
  PrintVector("All joint positions", hand.GetAllJointMotorPosi());

  std::cout << "\nSetJointMotorPosi(1, 1024)" << std::endl;
  const int16_t actual_single = hand.SetJointMotorPosi(1, 1024);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "Actual position from reply: " << actual_single << std::endl;
  std::cout << "Joint 1 readback: " << hand.GetJointMotorPosi(1) << std::endl;

  std::cout << "\nSetAllJointMotorPosi({2048})" << std::endl;
  PrintVector("Actual positions from reply", hand.SetAllJointMotorPosi({2048}));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  PrintVector("All joint positions readback", hand.GetAllJointMotorPosi());
}

void DemoPositionWithStatus(oh::OmniPicker3& hand) {
  std::cout << "\n========== Position Control With Status (0x24) / Joint Detail Info (0x26) =========="
            << std::endl;

  std::cout << "SetJointMotorPosiWithStatus(1, 2048): "
            << hand.SetJointMotorPosiWithStatus(1, 2048).ToString() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << "GetJointDetailInfo(0): " << hand.GetJointDetailInfo(0).ToString() << std::endl;
}

void DemoGesture(oh::OmniPicker3& hand) {
  std::cout << "\n========== Gesture ==========" << std::endl;

  const std::vector<std::pair<oh::OmniPicker3Gesture, std::string>> gestures = {
      {oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_ZERO, "ZERO"},
      {oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_HALF_OPEN, "HALF_OPEN"},
      {oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_FULL_OPEN, "FULL_OPEN"},
  };

  for (const auto& gesture : gestures) {
    PrintVector("GetHandGesture(" + gesture.second + ")", hand.GetHandGesture(gesture.first));
    std::cout << "SetHandGesture(" << gesture.second << ")" << std::endl;
    hand.SetHandGesture(gesture.first);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    PrintVector("Position readback", hand.GetAllJointMotorPosi());
  }
}

void DemoReports(oh::OmniPicker3& hand) {
  std::cout << "\n========== Reports ==========" << std::endl;
  PrintAllErrorReports(hand.GetAllErrorReport());
  PrintVector("GetAllTemperatureReport()", hand.GetAllTemperatureReport());
  PrintVector("GetAllCurrentReport()", hand.GetAllCurrentReport());
  std::cout << "GetJointMotorVelo(1): " << hand.GetJointMotorVelo(1) << std::endl;
  PrintVector("GetAllJointMotorVelo()", hand.GetAllJointMotorVelo());
  PrintVector("GetAllCurrentThreshold()", hand.GetAllCurrentThreshold());
}

void DemoMixControl(oh::OmniPicker3& hand) {
  std::cout << "\n========== Mixed Control ==========" << std::endl;
  std::cout << "MixControlByPT({2048}, {0})" << std::endl;
  PrintMixControlReply(hand.MixControlByPT({2048}, {0}));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void DemoTactileSensor(oh::OmniPicker3& hand) {
  std::cout << "\n========== Tactile Sensor ==========" << std::endl;

  if (!hand.InitTactilePointsMap()) {
    std::cerr << "InitTactilePointsMap failed" << std::endl;
    return;
  }

  const auto& sensor_order = hand.GetSensorOrder();
  for (const auto finger : sensor_order) {
    std::cout << "GetSensorDataLength(" << oh::ToString(finger)
              << "): " << hand.GetSensorDataLength(finger) << std::endl;
    PrintTactilePreview(hand.GetTactileSensorData(finger));
    PrintTactilePreview(hand.GetTactileSensorDataRaw(finger));
  }

  const auto all_tactile = hand.GetAllTactileSensorDataRaw();
  std::cout << "GetAllTactileSensorDataRaw() sensor count: " << all_tactile.size() << std::endl;
  for (const auto& sensor : all_tactile) {
    PrintTactilePreview(sensor);
  }
}

void ReturnToZero(oh::OmniPicker3& hand) {
  std::cout << "\nReturn to zero position" << std::endl;
  hand.SetHandGesture(oh::OmniPicker3Gesture::OMNIPICKER_3_GESTURE_ZERO);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  PrintVector("Final position", hand.GetAllJointMotorPosi());
}

}  // namespace

int main(int argc, char* argv[]) {
  Options options;
  const ParseResult parse_result = ParseArgs(argc, argv, &options);
  if (parse_result == ParseResult::HELP) {
    return 0;
  }
  if (parse_result == ParseResult::ERROR) {
    return 1;
  }

  std::cout << "Opening " << options.uart_port << " @ " << options.baudrate
            << ", hand device id " << static_cast<unsigned int>(options.hand_device_id)
            << std::endl;

  // hand_type carries no meaning for a gripper: OP3 stores it but never uses it, so any value works.
  auto hand = oh::OmniPicker3::createHandByRs485(oh::HandType::UNKNOWN,
                                                 options.hand_device_id,
                                                 options.uart_port,
                                                 options.baudrate);
  if (!hand) {
    std::cout << "error: device instance not created" << std::endl;
    return -1;
  }

  if (!hand->Init()) {
    std::cout << "error: device not opened" << std::endl;
    return -1;
  }

  if (options.verbose) {
    hand->ShowDataDetails(true);
  }

  DemoDeviceInfo(*hand);
  DemoCommunicationSettings(*hand);
  DemoPositionControl(*hand);
  DemoPositionWithStatus(*hand);
  DemoKinematicsStubs(*hand);
  DemoGesture(*hand);
  DemoReports(*hand);
  DemoMixControl(*hand);
  DemoTactileSensor(*hand);
  ReturnToZero(*hand);

  return 0;
}
