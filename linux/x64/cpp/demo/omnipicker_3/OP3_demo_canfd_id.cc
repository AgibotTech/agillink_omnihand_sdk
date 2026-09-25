// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file OP3_demo_canfd_id.cc
 * @brief OmniPicker 3 control demo - CANFD communication (via canfd_id)
 */

#include <omnihand/omnipicker_3.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

namespace oh = agilink::omnihand;

enum class CreateType {
  ZLGCAN,
  HCAN,
};

enum class ParseResult {
  OK,
  HELP,
  ERROR,
};

struct Options {
  CreateType create_type = CreateType::ZLGCAN;
  uint8_t hand_device_id = oh::OmniPicker3::kDefaultHandDeviceId;
  uint8_t canfd_device_id = 0;
  uint8_t canfd_channel_id = 0;
  bool verbose = false;
};

std::string GetProgramName(const char* argv0) {
  const std::string path = argv0 == nullptr ? "" : argv0;
  const size_t pos = path.find_last_of("/\\");
  return pos == std::string::npos ? path : path.substr(pos + 1);
}

void PrintUsage(const std::string& program_name) {
  std::cout << "Usage: " << program_name
            << " [-d|--device <device>] [-i <canfd_index>] [-c <channel>]"
               " [--id <device_id>] [-v|--verbose] [-h|--help]"
            << std::endl;
  std::cout << "  -d, --device\t\t[hcan|zlgcan] (default zlgcan)" << std::endl;
  std::cout << "  -i\t\t\tUSB CANFD adapter index, zlgcan/hcan (default 0)" << std::endl;
  std::cout << "  -c\t\t\tCAN channel index (default 0)" << std::endl;
  std::cout << "  --id\t\t\thand device id to address (default 1; --device-id accepted too)"
            << std::endl;
  std::cout << "  -v, --verbose\t\tdump raw CAN frames (TX/RX hex)" << std::endl;
  std::cout << "  -h, --help\t\tshow this help" << std::endl;
  std::cout << "\nExample (runs the full demo against a gripper on adapter 0, channel 0, id 1):"
            << std::endl;
  std::cout << "  " << program_name << " -d zlgcan -i 0 -c 0 --id 1 -v" << std::endl;
  std::cout << "    -d zlgcan  use the ZLG USBCANFD adapter" << std::endl;
  std::cout << "    -i 0       adapter index 0" << std::endl;
  std::cout << "    -c 0       CAN channel 0" << std::endl;
  std::cout << "    --id 1     address the gripper as device id 1" << std::endl;
  std::cout << "    -v         dump raw CAN frames while running" << std::endl;
  std::cout << "  Drop -v for quieter output; drop -d/-i/-c/--id to accept the defaults."
            << std::endl;
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
    if (arg == "-d" || arg == "--device") {
      if (value == "hcan") {
        options->create_type = CreateType::HCAN;
      } else if (value == "zlgcan") {
        options->create_type = CreateType::ZLGCAN;
      } else {
        std::cout << "error: unsupported device: " << value << std::endl;
        PrintUsage(program_name);
        return ParseResult::ERROR;
      }
    } else if (arg == "--id" || arg == "--device-id") {
      options->hand_device_id = static_cast<uint8_t>(std::atoi(value.c_str()));
    } else if (arg == "-i") {
      options->canfd_device_id = static_cast<uint8_t>(std::atoi(value.c_str()));
    } else if (arg == "-c") {
      options->canfd_channel_id = static_cast<uint8_t>(std::atoi(value.c_str()));
    } else {
      std::cout << "error: unsupported option: " << arg << std::endl;
      PrintUsage(program_name);
      return ParseResult::ERROR;
    }
  }

  return ParseResult::OK;
}

std::unique_ptr<oh::OmniPicker3> CreateHand(const Options& options) {
  // hand_type carries no meaning for a gripper: OP3 stores it but never uses it, so any value works.
  constexpr oh::HandType kFactoryHandType = oh::HandType::UNKNOWN;

  if (options.create_type == CreateType::HCAN) {
    return oh::OmniPicker3::createHandByHcan(
        kFactoryHandType,
        options.hand_device_id,
        options.canfd_device_id,
        options.canfd_channel_id);
  }

  return oh::OmniPicker3::createHandByZlgcan(
      kFactoryHandType,
      options.hand_device_id,
      options.canfd_device_id,
      options.canfd_channel_id);
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
    if (ctrl.tgt_velo_) {
      std::cout << ", velo=" << *ctrl.tgt_velo_;
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
  std::cout << "Device info:\n"
            << hand.GetDeviceInfo().ToString();
  std::cout << "Vendor info:\n"
            << hand.GetVendorInfo().ToString();
  PrintVector("Joint names", hand.GetJointNames());
  auto range = oh::OmniPicker3::GetMinMaxMotorPosition();
  std::cout << "minal motor range: [" << range.min_value << ", " << range.max_value << "]" << std::endl;
}

void DemoCommunicationSettings(oh::OmniPicker3& hand) {
  std::cout << "\n========== Communication Settings ==========" << std::endl;
  const int request_interval = hand.GetRequestInterval();
  const int recv_timeout = hand.GetFrameRecvTimeout();
  const int send_timeout = hand.GetFrameSendTimeout();

  std::cout << "Request interval: " << request_interval << " ms" << std::endl;
  std::cout << "Frame receive timeout: " << recv_timeout << " ms" << std::endl;
  std::cout << "Frame send timeout: " << send_timeout << " ms" << std::endl;

  // Write the values straight back: exercises the setters without changing behavior.
  // ShowDataDetails is deliberately left alone here -- main() owns it via -v/--verbose.
  hand.SetRequestInterval(request_interval);
  hand.SetFrameRecvTimeout(recv_timeout);
  hand.SetFrameSendTimeout(send_timeout);
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

  std::cout << "SetHandGesture(0)" << std::endl;
  hand.SetHandGesture(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  PrintVector("Position readback", hand.GetAllJointMotorPosi());
}

void DemoReports(oh::OmniPicker3& hand) {
  std::cout << "\n========== Reports ==========" << std::endl;
  std::cout << "GetErrorReport(1): " << hand.GetErrorReport(1).ToString() << std::endl;
  PrintAllErrorReports(hand.GetAllErrorReport());
  std::cout << "GetTemperatureReport(1): " << hand.GetTemperatureReport(1) << std::endl;
  PrintVector("GetAllTemperatureReport()", hand.GetAllTemperatureReport());
  std::cout << "GetCurrentReport(1): " << hand.GetCurrentReport(1) << std::endl;
  PrintVector("GetAllCurrentReport()", hand.GetAllCurrentReport());
  // Register 0x12 (VELOCITY_CTRL) is absent from the current protocol spec's register
  // table and times out on current firmware; see BACKLOG.md. Shown for evidence.
  std::cout << "GetJointMotorVelo(1): " << hand.GetJointMotorVelo(1) << std::endl;
  PrintVector("GetAllJointMotorVelo()", hand.GetAllJointMotorVelo());
}

// Register 0x03 (CURRENT_THRESHOLD) is likewise absent from the current spec's register
// table and times out on current firmware (BACKLOG.md). The write is still demoed as a
// read/modify/restore round-trip so it can never leave a changed value behind.
void DemoCurrentThreshold(oh::OmniPicker3& hand) {
  std::cout << "\n========== Current Threshold (0x03) ==========" << std::endl;
  const auto original = hand.GetAllCurrentThreshold();
  PrintVector("GetAllCurrentThreshold()", original);
  std::cout << "GetCurrentThreshold(1): " << hand.GetCurrentThreshold(1) << std::endl;

  if (original.empty()) {
    std::cout << "read returned nothing, skipping the write round-trip" << std::endl;
    return;
  }

  std::cout << "SetCurrentThreshold(1, " << original[0] << ") (same value, no-op write)"
            << std::endl;
  hand.SetCurrentThreshold(1, original[0]);
  std::cout << "readback: " << hand.GetCurrentThreshold(1) << std::endl;

  std::cout << "SetAllCurrentThreshold(original) (restore)" << std::endl;
  hand.SetAllCurrentThreshold(original);
  PrintVector("readback", hand.GetAllCurrentThreshold());
}

void DemoMixControl(oh::OmniPicker3& hand) {
  std::cout << "\n========== Mixed Control ==========" << std::endl;
  std::cout << "MixControlByPT({2048}, {0})" << std::endl;
  PrintMixControlReply(hand.MixControlByPT({2048}, {0}));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Neither OP3 impl overrides the PV/PVT forms, so these hit the OmniHand base-class
  // no-ops: they send no frame and return empty. Demoed to document that.
  std::cout << "\nMixControlByPV / MixControlByPVT are not implemented on OP3 "
               "(base-class no-ops, no frame sent)"
            << std::endl;
  PrintMixControlReply(hand.MixControlByPV({0}, {0}));
  PrintMixControlReply(hand.MixControlByPVT({2048}, {0}, {0}));
}

void DemoTactileSensor(oh::OmniPicker3& hand) {
  std::cout << "\n========== Tactile Sensor ==========" << std::endl;

  if (!hand.InitTactilePointsMap()) {
    std::cerr << "InitTactilePointsMap failed" << std::endl;
    return;
  }

  const auto& sensor_order = hand.GetSensorOrder();
  std::cout << "GetSensorOrder(): [";
  for (size_t i = 0; i < sensor_order.size(); ++i) {
    if (i > 0) {
      std::cout << ", ";
    }
    std::cout << oh::ToString(sensor_order[i]);
  }
  std::cout << "]" << std::endl;

  std::cout << "GetNumOfTactileSensors(): " << hand.GetNumOfTactileSensors() << std::endl;

  for (const auto finger : sensor_order) {
    const std::string name = oh::ToString(finger);
    std::cout << "GetSensorDataLength(" << name << "): " << hand.GetSensorDataLength(finger)
              << ", GetNumOfTactilePoints(): " << hand.GetNumOfTactilePoints(finger)
              << ", GetLenOfTactileDatum(): " << hand.GetLenOfTactileDatum(finger)
              << ", GetNumOfRepliedTactileFrames(): "
              << hand.GetNumOfRepliedTactileFrames(finger)
              << ", GetSNOfTactileSensor(): \"" << hand.GetSNOfTactileSensor(finger) << "\""
              << std::endl;
    // Register 0x05 (single-sensor downsampled query) times out on current OP3 firmware
    // while 0x06 (the *Raw multi-frame path) works; see BACKLOG.md.
    PrintTactilePreview(hand.GetTactileSensorData(finger));
    PrintTactilePreview(hand.GetTactileSensorDataRaw(finger));
  }

  const auto all_tactile = hand.GetAllTactileSensorDataRaw();
  std::cout << "GetAllTactileSensorDataRaw() sensor count: " << all_tactile.size()
            << std::endl;
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

  std::cout << "Opening "
            << (options.create_type == CreateType::HCAN ? "hcan" : "zlgcan")
            << " adapter " << static_cast<unsigned int>(options.canfd_device_id)
            << " channel " << static_cast<unsigned int>(options.canfd_channel_id)
            << ", hand device id " << static_cast<unsigned int>(options.hand_device_id)
            << std::endl;

  auto hand = CreateHand(options);
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
  DemoKinematicsStubs(*hand);
  DemoGesture(*hand);
  DemoReports(*hand);
  DemoCurrentThreshold(*hand);
  DemoMixControl(*hand);
  DemoTactileSensor(*hand);
  ReturnToZero(*hand);

  return 0;
}
