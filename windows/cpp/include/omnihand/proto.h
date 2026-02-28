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
enum class AGIBOT_EXPORT HandType : unsigned char {
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
 * @brief CAN frame format for createHandByZlgcan / createHandByHcan (interface layer)
 * @see docs/REFACTOR_SDK_PROTOCOL.md
 */
enum class AGIBOT_EXPORT CanFrameFormat : unsigned char {
  Extended = 0,  ///< 29-bit ID, command in ID (default, backward compatible)
  Standard = 1   ///< 11-bit ID, command in D0, same data format as USB/RS485
};

inline std::string ToString(CanFrameFormat f) {
  return f == CanFrameFormat::Standard ? "Standard" : "Extended";
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
 *       - VELOCITY (2): Velocity control mode - marked as "暂不支持" (not yet supported) in protocol
 *       - TORQUE (3): Torque control mode - defined in protocol but may not be fully supported
 *       - POSITION_TORQUE (4): Position-Torque mixed control - marked as "暂不支持" (not yet supported) in protocol
 *       - VELOCITY_TORQUE (5): Velocity-Torque mixed control - marked as "暂不支持" (not yet supported) in protocol
 *       - POSITION_VELOCITY_TORQUE (6): Position-Velocity-Torque mixed control - marked as "暂不支持" (not yet supported) in protocol
 */
enum class AGIBOT_EXPORT ControlMode : unsigned char {
  POSITION = 0,                    // Position control mode
  SERVO = 1,                       // Servo control mode
  VELOCITY = 2,                    // Velocity control mode (not yet supported)
  TORQUE = 3,                      // Torque control mode
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
  union {
    struct {
      unsigned char stalled_ : 1;
      unsigned char overheat_ : 1;
      unsigned char over_current_ : 1;
      unsigned char motor_except_ : 1;
      unsigned char commu_except_ : 1;
      unsigned char res1_ : 3;
      unsigned char res2_;
    };
    unsigned char res_[2];
  };

  std::string ToString() const {
    std::string s;
    if (stalled_) s += "stalled,";
    if (overheat_) s += "overheat,";
    if (over_current_) s += "over_current,";
    if (motor_except_) s += "motor_except,";
    if (commu_except_) s += "commu_except,";
    if (s.empty()) return "0";
    s.pop_back();
    return s;
  }
};

/**
 * @brief 触觉传感器数据 (O12专用)
 */
struct AGIBOT_EXPORT TactileSensor3DData {
  unsigned char online_state_;          // 1~传感器在线; 0~传感器不在线
  unsigned short channel_value_[9];     // 各通道值
  unsigned short normal_force_;         // 法向力: (0-3000, 0.1N)
  unsigned short tangent_force_;        // 切向力
  unsigned short tangent_force_angle_;  // 切向力角度，指尖向上为0度，顺时针旋转: (0~359)
  unsigned char capa_approach_[4];      // 自电容接近
};

/**
 * @brief 单个传感器的1D触觉数据 (O10专用)
 */
struct AGIBOT_EXPORT TactileSensorData {
  Finger sensor_id_;                   // 传感器ID (手指/手心/手背)
  std::vector<uint8_t> data_;           // 传感器数据，单位: 1g, 最大值: 255g
};

/**
 * @brief 版本信息
 */
struct AGIBOT_EXPORT Version {
  unsigned char major_;
  unsigned char minor_;
  unsigned char patch_;
  unsigned char res_;
};

/**
 * @brief 厂商信息
 */
struct AGIBOT_EXPORT VendorInfo {
  std::string productModel;   // 产品型号
  std::string productSeqNum;  // 产品序列号
  Version hardwareVersion;    // 硬件版本
  Version softwareVersion;    // 软件版本
  int16_t voltage;            // 供电电压(mV)
  unsigned char dof;          // 主动自由度

  std::string toString() const {
    std::stringstream sstream;
    sstream << "Product Model: " << productModel
            << "\nSerial Number: " << productSeqNum
            << "\nHardware Version: " << static_cast<unsigned int>(hardwareVersion.major_)
            << "." << static_cast<unsigned int>(hardwareVersion.minor_)
            << "." << static_cast<unsigned int>(hardwareVersion.patch_)
            << "\nSoftware Version: " << static_cast<unsigned int>(softwareVersion.major_)
            << "." << static_cast<unsigned int>(softwareVersion.minor_)
            << "." << static_cast<unsigned int>(softwareVersion.patch_)
            << "\nSupply Voltage: " << voltage << "mV"
            << "\nActive Degrees of Freedom: " << static_cast<unsigned int>(dof);
    return sstream.str();
  }
};

/**
 * @brief 通信参数
 */
struct AGIBOT_EXPORT CommuParams {
  unsigned char bitrate_;
  unsigned char sample_point_;
  unsigned char dbitrate_;
  unsigned char dsample_point_;
};

/**
 * @brief 设备信息
 */
struct AGIBOT_EXPORT DeviceInfo {
  unsigned char hand_device_id;   // 手部设备ID
  CommuParams commu_params;  // 通信参数

  std::string toString() const {
    std::vector<std::string> vecBitrate = {"125Kbps", "500Kbps", "1Mbps", "5Mbps"};
    std::vector<std::string> vecSamplePoint = {"75.0%", "80.0%", "87.5%"};

    std::stringstream sstream;
    sstream << "Device ID: " << static_cast<unsigned int>(hand_device_id);
    
    // Add bounds checking to prevent array out-of-bounds access
    unsigned char bitrate_idx = commu_params.bitrate_;
    unsigned char sample_point_idx = commu_params.sample_point_;
    unsigned char dbitrate_idx = commu_params.dbitrate_;
    unsigned char dsample_point_idx = commu_params.dsample_point_;
    
    if (bitrate_idx < vecBitrate.size()) {
      sstream << "\nArbitration Bitrate: " << vecBitrate[bitrate_idx];
    } else {
      sstream << "\nArbitration Bitrate: Invalid(" << static_cast<unsigned int>(bitrate_idx) << ")";
    }
    
    if (sample_point_idx < vecSamplePoint.size()) {
      sstream << "\nArbitration Sample Point: " << vecSamplePoint[sample_point_idx];
    } else {
      sstream << "\nArbitration Sample Point: Invalid(" << static_cast<unsigned int>(sample_point_idx) << ")";
    }
    
    if (dbitrate_idx < vecBitrate.size()) {
      sstream << "\nData Bitrate: " << vecBitrate[dbitrate_idx];
    } else {
      sstream << "\nData Bitrate: Invalid(" << static_cast<unsigned int>(dbitrate_idx) << ")";
    }
    
    if (dsample_point_idx < vecSamplePoint.size()) {
      sstream << "\nData Sample Point: " << vecSamplePoint[dsample_point_idx];
    } else {
      sstream << "\nData Sample Point: Invalid(" << static_cast<unsigned int>(dsample_point_idx) << ")";
    }

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
