// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

/**
 * @file OP3_demo_canfd_id.cc
 * @brief OmniPicker 3 control demo - CANFD communication (via canfd_id)
 */

#include <omnihand/omni_picker_3.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
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

std::string GetProgramName(const char* argv0) {
  const std::string path = argv0 == nullptr ? "" : argv0;
  const size_t pos = path.find_last_of("/\\");
  return pos == std::string::npos ? path : path.substr(pos + 1);
}

void PrintUsage(const std::string& program_name) {
  std::cout << "Usage: " << program_name << " [-h|--help] [-d|--device] <device>" << std::endl;
  std::cout << "  -d, --device\t\t[hcan|zlgcan]" << std::endl;
  std::cout << "  -h, --help\t\tshow help of project" << std::endl;
}

ParseResult ParseArgs(int argc, char* argv[], CreateType* create_type) {
  const std::string program_name = GetProgramName(argc > 0 ? argv[0] : "");

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "-d" || arg == "--device") {
      if (i + 1 == argc) {
        std::cout << "error: device name not provided!" << std::endl;
        return ParseResult::ERROR;
      }

      const std::string device = argv[++i];
      if (device == "hcan") {
        *create_type = CreateType::HCAN;
      } else if (device == "zlgcan") {
        *create_type = CreateType::ZLGCAN;
      } else {
        std::cout << "error: unsupported device: " << device << std::endl;
        PrintUsage(program_name);
        return ParseResult::ERROR;
      }
    } else if (arg == "-h" || arg == "--help") {
      PrintUsage(program_name);
      return ParseResult::HELP;
    } else {
      std::cout << "error: unsupported option: " << arg << std::endl;
      PrintUsage(program_name);
      return ParseResult::ERROR;
    }
  }

  return ParseResult::OK;
}

std::unique_ptr<oh::OmniPicker3> CreateHand(CreateType create_type) {
  // OmniPicker 3 is a gripper. Use UNKNOWN for the shared factory's HandType slot.
  constexpr oh::HandType kFactoryHandType = oh::HandType::UNKNOWN;
  constexpr uint8_t kHandDeviceId = oh::OmniPicker3::kDefaultHandDeviceId;
  constexpr uint8_t kCanfdDeviceId = 0;
  constexpr uint8_t kCanfdChannelId = 0;

  if (create_type == CreateType::HCAN) {
    return oh::OmniPicker3::createHandByHcan(
        kFactoryHandType,
        kHandDeviceId,
        kCanfdDeviceId,
        kCanfdChannelId);
  }

  return oh::OmniPicker3::createHandByZlgcan(
      kFactoryHandType,
      kHandDeviceId,
      kCanfdDeviceId,
      kCanfdChannelId);
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

  hand.ShowDataDetails(false);
  hand.SetRequestInterval(request_interval);
  hand.SetFrameRecvTimeout(recv_timeout);
  hand.SetFrameSendTimeout(send_timeout);
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
  PrintAllErrorReports(hand.GetAllErrorReport());
  PrintVector("GetAllTemperatureReport()", hand.GetAllTemperatureReport());
  PrintVector("GetAllCurrentReport()", hand.GetAllCurrentReport());
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
  std::cout << "GetSensorOrder(): [";
  for (size_t i = 0; i < sensor_order.size(); ++i) {
    if (i > 0) {
      std::cout << ", ";
    }
    std::cout << oh::ToString(sensor_order[i]);
  }
  std::cout << "]" << std::endl;

  for (const auto finger : sensor_order) {
    std::cout << "GetSensorDataLength(" << oh::ToString(finger)
              << "): " << hand.GetSensorDataLength(finger) << std::endl;
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
  CreateType create_type = CreateType::HCAN;
  const ParseResult parse_result = ParseArgs(argc, argv, &create_type);
  if (parse_result == ParseResult::HELP) {
    return 0;
  }
  if (parse_result == ParseResult::ERROR) {
    return 1;
  }

  auto hand = CreateHand(create_type);
  if (!hand) {
    std::cout << "error: device instance not created" << std::endl;
    return -1;
  }

  if (!hand->Init()) {
    std::cout << "error: device not opened" << std::endl;
    return -1;
  }

  DemoDeviceInfo(*hand);
  DemoCommunicationSettings(*hand);
  DemoPositionControl(*hand);
  DemoGesture(*hand);
  DemoReports(*hand);
  DemoMixControl(*hand);
  DemoTactileSensor(*hand);
  ReturnToZero(*hand);

  return 0;
}
