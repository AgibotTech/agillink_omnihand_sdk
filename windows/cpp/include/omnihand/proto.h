// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file proto.h
 * @brief Unified protocol definitions for OmniHand 2025 SDK
 * @author agiuser
 * @date 25-8-7
 **/

#ifndef PROTO_H
#define PROTO_H

#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include "omnihand/export_symbols.h"

#pragma pack(push, 1)

/**
 * @brief 报文CanId
 */
struct AGIBOT_EXPORT CanId {
  unsigned char device_id_ : 7;
  unsigned char rw_flag_ : 1;
  unsigned char product_id_ : 7;
  unsigned char res1 : 1;
  unsigned char msg_type_;
  unsigned char msg_id_;
};

/**
 * @brief 联合体CanId
 */
union AGIBOT_EXPORT UnCanId {
  CanId st_can_Id_;
  unsigned int ui_can_id_;

  UnCanId() {
    ui_can_id_ = 0;
  }
};

/**
 * @brief 手型枚举
 */
enum class AGIBOT_EXPORT EHandType : unsigned char {
  eLeft = 0,
  eRight = 1,
  eUnknown = 10
};

/**
 * @brief 产品类型枚举
 */
enum class AGIBOT_EXPORT ProductType : unsigned char {
  OMNIHAND_2025 = 0,        // OmniHand 2025 (10 DOF)
  OMNIHAND_PRO_2025 = 1,    // OmniHand Pro 2025 (12 DOF)
  OMNIHAND_DEX_UMI = 2,     // OmniHand Dex UMI (10 DOF, UMI protocol)
  UNKNOWN_PRODUCT_TYPE = 255
};

/**
 * @brief Finger enumeration (Unified for O10 and O12)
 * @note O12 does not support ePalm and eDorsum sensors, using these will result in runtime error
 */
enum class AGIBOT_EXPORT EFinger : unsigned char {
  eThumb = 0x01,   // 拇指
  eIndex = 0x02,   // 食指
  eMiddle = 0x03,  // 中指
  eRing = 0x04,    // 无名指
  eLittle = 0x05,  // 小指
  ePalm = 0x06,    // 手心 (O10 only, not supported by O12)
  eDorsum = 0x07,  // 手背 (O10 only, not supported by O12)
  eUnknown = 0xff
};

/**
 * @brief Convert EFinger enum to string
 * @param finger Finger enum value
 * @return String representation of the finger
 */
inline std::string ToString(EFinger finger) {
  switch (finger) {
    case EFinger::eThumb: return "Thumb";
    case EFinger::eIndex: return "Index";
    case EFinger::eMiddle: return "Middle";
    case EFinger::eRing: return "Ring";
    case EFinger::eLittle: return "Little";
    case EFinger::ePalm: return "Palm";
    case EFinger::eDorsum: return "Dorsum";
    case EFinger::eUnknown: return "Unknown";
    default: return "Invalid";
  }
}

/**
 * @brief 控制模式枚举 (Unified for O10 and O12)
 * @note O10 and O12 use the same control mode protocol (Pn16):
 *       ePosi=0, eServo=1, eVelo=2, eTorque=3, ePosiTorque=4, eVeloTorque=5, ePosiVeloTorque=6
 * @note According to protocol specification:
 *       - ePosi (0): Position control mode - supported
 *       - eServo (1): Servo control mode - supported
 *       - eVelo (2): Velocity control mode - marked as "暂不支持" (not yet supported) in protocol
 *       - eTorque (3): Torque control mode - defined in protocol but may not be fully supported
 *       - ePosiTorque (4): Position-Torque mixed control - marked as "暂不支持" (not yet supported) in protocol
 *       - eVeloTorque (5): Velocity-Torque mixed control - marked as "暂不支持" (not yet supported) in protocol
 *       - ePosiVeloTorque (6): Position-Velocity-Torque mixed control - marked as "暂不支持" (not yet supported) in protocol
 */
enum class AGIBOT_EXPORT EControlMode : unsigned char {
  ePosi = 0,            // 位置模式
  eServo = 1,          // 伺服模式
  eVelo = 2,            // 速度模式 (协议标记为暂不支持)
  eTorque = 3,          // 力控模式
  ePosiTorque = 4,      // 位置力控模式 (混合控制：位置+力矩，协议标记为暂不支持)
  eVeloTorque = 5,      // 速度力控模式 (混合控制：速度+力矩，协议标记为暂不支持)
  ePosiVeloTorque = 6,  // 位置速度力控模式 (混合控制：位置+速度+力矩，协议标记为暂不支持)
  eUnknown = 10         // 未知模式
};

/**
 * @brief 关节电机错误上报
 */
struct AGIBOT_EXPORT JointMotorErrorReport {
  unsigned char stalled_ : 1;
  unsigned char overheat_ : 1;
  unsigned char over_current_ : 1;
  unsigned char motor_except_ : 1;
  unsigned char commu_except_ : 1;
  unsigned char res1_ : 3;
  unsigned char res2_;
};

/**
 * @brief 所有关节电机错误上报 (O10专用)
 */
struct AGIBOT_EXPORT JointMotorAllErrorReport {
  unsigned char res_[2];
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
  EFinger sensor_id_;                   // 传感器ID (手指/手心/手背)
  std::vector<uint8_t> data_;           // 传感器数据，单位: 1g, 最大值: 255g
};

/**
 * @brief 消息类型枚举
 */
enum class AGIBOT_EXPORT EMsgType : unsigned char {
  eVendorInfo = 0x01,
  eDeviceInfo = 0x02,
  eCurrentThreshold = 0x03,
  eTactileSensor = 0x05,
  eAllTactileSensor = 0x06,  // All tactile sensor data (1D sensors, O10 only)
  eMaxPositionCalibration = 0x07,  // UMI: Maximum position calibration (Pn7), sub-register 0x00=all 10 joints, 0x01-0x0A=individual joints
  eMinPositionCalibration = 0x08,  // UMI: Minimum position calibration (Pn8), sub-register 0x00=all 10 joints, 0x01-0x0A=individual joints
  eCtrlMode = 0x10,
  eTorqueCtrl = 0x11,
  eVeloCtrl = 0x12,
  ePosiCtrl = 0x13,
  eMixCtrl = 0x14,
  eErrorReport = 0x20,
  eTemperatureReport = 0x21,
  eCurrentReport = 0x22,
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
    sstream << "Device ID: " << static_cast<unsigned int>(hand_device_id)
            << "\nArbitration Bitrate: " << vecBitrate[commu_params.bitrate_]
            << "\nArbitration Sample Point: " << vecSamplePoint[commu_params.sample_point_]
            << "\nData Bitrate: " << vecBitrate[commu_params.dbitrate_]
            << "\nData Sample Point: " << vecSamplePoint[commu_params.dsample_point_];

    return sstream.str();
  }
};

// ============ OTA 相关结构体 ============

struct AGIBOT_EXPORT OTAUpgradeReq {
  unsigned int firmware_length_;
  unsigned short package_num_;
  unsigned short res1_;
  unsigned int res2_;
  unsigned int res3_;
};

struct AGIBOT_EXPORT OTAUpgradeRep {
  unsigned int result_;
};

struct AGIBOT_EXPORT OTATransmitReq {
  unsigned short package_index_;
  unsigned short crc_;
};

struct AGIBOT_EXPORT OTATransmitRep {
  unsigned int result_;
};

struct AGIBOT_EXPORT OTAFinishReq {
  unsigned int res_{};
};

struct AGIBOT_EXPORT OTAFinishRep {
  unsigned int result_{};
};

struct AGIBOT_EXPORT OTARestartReq {
  unsigned int delay_{};
};

struct AGIBOT_EXPORT OTARestartRep {
  unsigned int result_{};
};

struct AGIBOT_EXPORT OTAResultReq {
  unsigned int result_;
};

struct AGIBOT_EXPORT OTAResultRep {
  unsigned int result_;
};

struct AGIBOT_EXPORT OTAExitReq {
  unsigned int code_;
};

struct AGIBOT_EXPORT OTAExitRep {
  unsigned int result_;
};

#pragma pack(pop)

/**
 * @brief 混合控制参数
 */
struct AGIBOT_EXPORT MixCtrl {
  unsigned char joint_index_ : 5 {};
  unsigned char ctrl_mode_ : 3 {};
  std::optional<short> tgt_posi_;
  std::optional<short> tgt_velo_;
  std::optional<short> tgt_torque_;
};

#endif  // PROTO_H
