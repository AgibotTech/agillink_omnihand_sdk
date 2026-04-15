// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file proto.h
 * @brief Unified protocol definitions for OmniHand 2025 SDK
 * @author agiuser
 * @date 25-8-7
 **/

#ifndef AGILINK_PROTO_H
#define AGILINK_PROTO_H

#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {

#pragma pack(push, 1)

/**
 * @brief Hand type enumeration
 */
enum class AGIBOT_EXPORT HandType : uint8_t {
  LEFT = 0,      // Left hand
  RIGHT = 1,     // Right hand
  UNKNOWN = 255  // Unknown hand type
};

/**
 * @brief Convert HandType enum to string
 * @param hand_type Hand type enum value
 * @return String representation of the hand type
 */
inline std::string ToString(HandType hand_type) {
  switch (hand_type) {
    case HandType::LEFT: return "Left";
    case HandType::RIGHT: return "Right";
    default: return "Unknown";
  }
}

/**
 * @brief Product type enumeration
 */
enum class AGIBOT_EXPORT ProductType : unsigned char {
  OMNIHAND_2025 = 0,        // OmniHand 2025 (O10, 10 DOF)
  OMNIHAND_PRO_2025 = 1,    // OmniHand Pro 2025 (O12, 12 DOF)
  OMNIHAND_DEX_UMI = 2,     // OmniHand Dex UMI (O10 UMI, 10 DOF)
  OMNIHAND_3_LITE = 3,      // OmniHand 3 Lite S (O4, 4 DOF)
  OMNIHAND_3_ULTRA = 4,     // OmniHand 3 Ultra (O20, 20 DOF)
  OMNI_PICKER_3 = 5,        // OmniPicker 3 (1 DOF)
  UNKNOWN = 255             // Unknown product type
};

/**
 * @brief Convert ProductType enum to string
 * @param product_type Product type enum value
 * @return String representation of the product type
 */
inline std::string ToString(ProductType product_type) {
  switch (product_type) {
    case ProductType::OMNIHAND_2025: return "OmniHand 2025";
    case ProductType::OMNIHAND_PRO_2025: return "OmniHand Pro 2025";
    case ProductType::OMNIHAND_DEX_UMI: return "OmniHand Dex UMI";
    case ProductType::OMNIHAND_3_LITE: return "OmniHand 3 Lite";
    case ProductType::OMNIHAND_3_ULTRA: return "OmniHand 3 Ultra";
    default: return "Unknown";
  }
}

/**
 * @brief Finger enumeration (Unified for O10 and O12)
 * @note O12 does not support PALM and DORSUM sensors, using these will result in runtime error
 */
enum class AGIBOT_EXPORT Finger : unsigned char {
  THUMB = 0x01,    // Thumb
  INDEX = 0x02,    // Index finger
  MIDDLE = 0x03,   // Middle finger
  RING = 0x04,     // Ring finger
  LITTLE = 0x05,   // Little finger
  PALM = 0x06,     // Palm (O10 only, not supported by O12)
  DORSUM = 0x07,   // Dorsum (O10 only, not supported by O12)
  UNKNOWN = 0xff   // Unknown finger
};

/**
 * @brief Convert Finger enum to string
 * @param finger Finger enum value
 * @return String representation of the finger
 */
inline std::string ToString(Finger finger) {
  switch (finger) {
    case Finger::THUMB: return "Thumb";
    case Finger::INDEX: return "Index";
    case Finger::MIDDLE: return "Middle";
    case Finger::RING: return "Ring";
    case Finger::LITTLE: return "Little";
    case Finger::PALM: return "Palm";
    case Finger::DORSUM: return "Dorsum";
    default: return "Unknown";
  }
}

/**
 * @brief Control mode enumeration (Unified for O10 and O12)
 * @note O10 and O12 use the same control mode protocol (Pn16):
 *       POSITION=0, SERVO=1, VELOCITY=2, TORQUE=3, POSITION_TORQUE=4, VELOCITY_TORQUE=5, POSITION_VELOCITY_TORQUE=6
 * @note According to protocol specification:
 *       - POSITION (0): Position control mode - supported
 *       - SERVO (1): Servo control mode - supported
 *       - VELOCITY (2): Velocity control mode - not yet supported in protocol
 *       - TORQUE (3): Torque control mode - defined in protocol but may not be fully supported
 *       - POSITION_TORQUE (4): Position-Torque mixed control - not yet supported in protocol
 *       - VELOCITY_TORQUE (5): Velocity-Torque mixed control - not yet supported in protocol
 *       - POSITION_VELOCITY_TORQUE (6): Position-Velocity-Torque mixed control - not yet supported in protocol
 */
enum class AGIBOT_EXPORT ControlMode : unsigned char {
  POSITION = 0,                    // Position control mode (supported, default mode)
  SERVO = 1,                       // Servo control mode (supported)
  VELOCITY = 2,                    // Velocity control mode (not yet supported)
  TORQUE = 3,                      // Torque control mode (not yet supported)
  POSITION_TORQUE = 4,             // Position-Torque mixed control (not yet supported)
  VELOCITY_TORQUE = 5,             // Velocity-Torque mixed control (not yet supported)
  POSITION_VELOCITY_TORQUE = 6,    // Position-Velocity-Torque mixed control (not yet supported)
  UNKNOWN = 10                     // Unknown control mode
};

/**
 * @brief Convert ControlMode enum to string
 * @param mode Control mode enum value
 * @return String representation of the control mode
 */
inline std::string ToString(ControlMode mode) {
  switch (mode) {
    case ControlMode::POSITION: return "Position";
    case ControlMode::SERVO: return "Servo";
    case ControlMode::VELOCITY: return "Velocity";
    case ControlMode::TORQUE: return "Torque";
    case ControlMode::POSITION_TORQUE: return "Position-Torque";
    case ControlMode::VELOCITY_TORQUE: return "Velocity-Torque";
    case ControlMode::POSITION_VELOCITY_TORQUE: return "Position-Velocity-Torque";
    default: return "Unknown";
  }
}

/**
 * @brief 关节电机错误上报
 */
struct AGIBOT_EXPORT JointMotorErrorReport {
  // 使用命名 union（匿名 union 不能包含命名结构体），但使用简洁的访问路径
  union {
    struct {
      unsigned char stalled_ : 1;
      unsigned char overheat_ : 1;
      unsigned char over_current_ : 1;
      unsigned char motor_except_ : 1;
      unsigned char commu_except_ : 1;
      unsigned char res1_ : 3;
      unsigned char res2_;
    } bits;
    unsigned char res_[2];
  };

  std::string ToString() const {
    std::string s;
    if (bits.stalled_) s += "stalled,";
    if (bits.overheat_) s += "overheat,";
    if (bits.over_current_) s += "over_current,";
    if (bits.motor_except_) s += "motor_except,";
    if (bits.commu_except_) s += "commu_except,";
    if (s.empty()) return "0";
    s.pop_back();
    return s;
  }
};

/**
 * @brief data structure for 3D tactile sensor data (O12 only)
 */
struct AGIBOT_EXPORT TactileSensor3DData {
  static constexpr size_t kChannelCount = 6;
  uint8_t online_state;          // 1: online, 0: offline
  uint32_t channel_value[kChannelCount];  // 6 decoded 24-bit channel values
  uint16_t normal_force;         // force normal to the sensor surface (0.1N, max: 2000)
  uint16_t tangent_force;        // force tangential to the sensor surface (0.1N, max: 2000)
  uint16_t tangent_force_angle;  // angle of the tangent force in degrees, zero degrees is up (0-359)
  uint8_t capa_approach[4];      // self-capacitance approach

  std::string ToString() const {
    std::stringstream sstream;
    sstream << "\t[Online State: " << static_cast<unsigned int>(online_state) << "]\n";
    sstream << "\t[Channel Values: ";
    for (size_t i = 0; i < kChannelCount; ++i) {
      sstream << channel_value[i] << " ";
    }
    sstream << "]\n\t[Normal Force: " << static_cast<unsigned int>(normal_force) << "]\n";
    sstream << "\t[Tangent Force: " << static_cast<unsigned int>(tangent_force) << "]\n";
    sstream << "\t[Tangent Force Angle: " << static_cast<unsigned int>(tangent_force_angle) << " degrees]\n";
    sstream << "\t[Capacitance Approach: ";
    for (size_t i = 0; i < 4; ++i) {
      sstream << static_cast<unsigned int>(capa_approach[i]) << " ";
    }
    sstream << "]\n";
    return sstream.str();
  }
};

struct AGIBOT_EXPORT Version {
  uint8_t major{0};
  uint8_t minor{0};
  uint8_t patch{0};
  uint8_t res{0};

  Version() = default;
  Version(uint8_t major, uint8_t minor, uint8_t patch, uint8_t res = 0)
      : major(major), minor(minor), patch(patch), res(res) {}

  bool operator>(const Version& other) const {
    if (major > other.major) return true;
    if (major < other.major) return false;
    if (minor > other.minor) return true;
    if (minor < other.minor) return false;
    if (patch > other.patch) return true;
    if (patch < other.patch) return false;
    return res > other.res;
  }

  bool operator>=(const Version& other) const {
    if (major < other.major) return false;
    if (major > other.major) return true;
    if (minor < other.minor) return false;
    if (minor > other.minor) return true;
    if (patch < other.patch) return false;
    if (patch > other.patch) return true;
    return res >= other.res;
  }

  bool operator==(const Version& other) const {
    return major == other.major && minor == other.minor && patch == other.patch && res == other.res;
  }

  bool operator!=(const Version& other) const {
    return !(*this == other);
  }

  std::string ToString() const {
    std::stringstream sstream;
    sstream << static_cast<unsigned int>(major) << "."
            << static_cast<unsigned int>(minor) << "."
            << static_cast<unsigned int>(patch);
    if (res != 0) {
      sstream << "." << static_cast<unsigned int>(res);
    }
    return sstream.str();
  }
};

/**
 * @brief Vendor Information
 */
struct AGIBOT_EXPORT VendorInfo {
  std::string productModel;   // product model
  std::string productSeqNum;  // product serial number
  Version hardwareVersion;    // hardware version
  Version softwareVersion;    // software version
  int16_t voltage;            // supply voltage (mV)
  unsigned char dof;          // active degrees of freedom

  std::string ToString() const {
    std::stringstream sstream;
    sstream << "\t[Product Model: " << productModel
            << "]\n\t[Serial Number: " << productSeqNum
            << "]\n\t[Hardware Version: " << hardwareVersion.ToString()
            << "]\n\t[Software Version: " << softwareVersion.ToString()
            << "]\n\t[Supply Voltage: " << voltage << "mV"
            << "]\n\t[Active Degrees of Freedom: " << static_cast<unsigned int>(dof) << "]\n";
    return sstream.str();
  }
};

/**
 * @brief Communication parameters for CANFD communication
 */
struct AGIBOT_EXPORT CommuParams {
  uint8_t bitrate{0};
  uint8_t sample_point{0};
  uint8_t dbitrate{0};
  uint8_t dsample_point{0};

  std::string ToString() const {
    static std::vector<std::string> vecBitrate = {"125Kbps", "500Kbps", "1Mbps", "5Mbps"};
    static std::vector<std::string> vecSamplePoint = {"75.0%", "80.0%", "87.5%"};

    std::stringstream sstream;
    sstream << "\t[Arbitration Bitrate: "
            << (bitrate < vecBitrate.size() ? vecBitrate[bitrate] : "Unknown")
            << "]\n\t[Arbitration Sample Point: "
            << (sample_point < vecSamplePoint.size() ? vecSamplePoint[sample_point] : "Unknown")
            << "]\n\t[Data Bitrate: "
            << (dbitrate < vecBitrate.size() ? vecBitrate[dbitrate] : "Unknown")
            << "]\n\t[Data Sample Point: "
            << (dsample_point < vecSamplePoint.size() ? vecSamplePoint[dsample_point] : "Unknown") << "]\n";
    return sstream.str();
  }
};

/**
 * @brief 设备信息
 */
struct AGIBOT_EXPORT DeviceInfo {
  uint8_t hand_device_id;
  CommuParams commu_params;

  std::string ToString() const {
    std::stringstream sstream;
    sstream << "\t[Hand Device ID: " << static_cast<unsigned int>(hand_device_id) << "]\n";
    sstream << commu_params.ToString();
    return sstream.str();
  }
};

/**
 * @brief 混合控制参数
 * @note 默认构造函数确保位域自动初始化为0，使用更安全
 */
struct AGIBOT_EXPORT MixCtrl {
  unsigned char joint_index_ : 5;
  unsigned char ctrl_mode_ : 3;
  std::optional<short> tgt_posi_;
  std::optional<short> tgt_velo_;
  std::optional<short> tgt_torque_;

  // 默认构造函数：确保位域初始化为0（C++17兼容）
  MixCtrl() : joint_index_(0), ctrl_mode_(0) {}
};

#pragma pack(pop)

}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_PROTO_H
