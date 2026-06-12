// Copyright (c) 2025, Agibot Co., Ltd.
// OmniHand Pro 2025 SDK is licensed under Mulan PSL v2.

/**
 * @file o12hand_ctrl.h
 * @author AgiBot-lishuang
 * @date 2025-08
 * @brief O12 Controller interface
 */

#ifndef OMNIHAND_PRO_2025_SOLVER_H
#define OMNIHAND_PRO_2025_SOLVER_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "omnihand/export_symbols.h"
#include "omnihand/utils.h"

namespace agilink {
namespace omnihand {
namespace o12 {

enum JointID {

  JThumbRoll,
  JThumbAbad,
  JThumbMCP,
  JThumbPIP,
  JThumbDIP,

  JIndexAbad,
  JIndexMCP,
  JIndexPIP,
  JIndexDIP,

  JMiddleAbad,
  JMiddleMCP,
  JMiddlePIP,
  JMiddleDIP,

  JRingMCP,
  JRingPIP,
  JRingDIP,

  JPinkyMCP,
  JPinkyPIP,
  JPinkyDIP,
  MaxJoint,
};

enum ActiveJointID {
  ActiveJointThumbRoll = 0,
  ActiveJointThumbAbAd,
  ActiveJointThumbMCP,
  ActiveJointThumbPIP,
  ActiveJointIndexAbAd,
  ActiveJointIndexMCP,
  ActiveJointIndexPIP,
  ActiveJointMiddleABAD,
  ActiveJointMiddleMCP,
  ActiveJointMiddlePIP,
  ActiveJointRingMCP,
  ActiveJointPinkyMCP,
  MaxActiveJoint,
};

enum O12handProActuator {
  ActuatorIndex1,  // Near thumb
  ActuatorIndex2,  // Away from thumb
  ActuatorMiddle1,
  ActuatorMiddle2,
  ActuatorThumbABAD,
  ActuatorThumbRoll,
  ActuatorIndex3,
  ActuatorMiddle3,
  ActuatorRing,
  ActuatorPinky,
  ActuatorThumbPIP,
  ActuatorThumbMCP,
  ActuatorCount
};

enum class OmniHandPro2025Gesture : int {
  OMNIHAND_PRO_2025_GESTURE_ALL_ZERO = 0,
  OMNIHAND_PRO_2025_GESTURE_PAPER,
  OMNIHAND_PRO_2025_GESTURE_FIST,
  OMNIHAND_PRO_2025_GESTURE_OK,
  OMNIHAND_PRO_2025_GESTURE_PACK,
};

class AGIBOT_EXPORT OmniHandPro2025Solver {
 private:
  bool is_left_hand_; ///< True for left-hand, false for right-hand
  std::vector<double> motor_max_ = {0.0, 0.0, 0.0, 0.0, 10e-3, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 10e-3, 10e-3};
  std::vector<double> motor_min_ = {-14.56e-3, -14.56e-3, -14.56e-3, -14.56e-3,
                                    0, -5.9e-3, -8.76e-3, -9.08e-3,
                                    -15.49e-3, -15.49e-3, 0.0, 0.0};

  static constexpr uint8_t kActuatorToActiveJoint[ActuatorCount] = {
      4, 5, 7, 8, 1, 0, 6, 9, 10, 11, 3, 2};
  static inline const std::vector<double> kActiveJointMin = {
      0.0, -1.39, -0.83, -1.29, -0.26, 0.0, 0.0, -0.26, 0.0, 0.0, 0.0, 0.0};
  static inline const std::vector<double> kActiveJointMax = {
      0.94, 0, 0, 0, 0.26, 1.35, 1.53, 0.26, 1.36, 1.82, 1.55, 1.54};

  std::vector<double> active_joint_max_ = kActiveJointMax;
  std::vector<double> active_joint_min_ = kActiveJointMin;

  static inline const std::vector<int> kMotorInputMax = {
    2000, 2000, 2000, 2000, 0, 2000, 2000, 2000, 2000, 2000, 0, 0};
  static inline const std::vector<int> kMotorInputMin = {
    0, 0, 0, 0, 2000, 0, 0, 0, 0, 0, 2000, 2000};
  static const int max_intput_ = 2000;
  static const int min_intput_ = 0;

  const std::vector<double> index_PIP_coeffs_ = {
      -0.003342057,  // (0,0,1)
      -0.003554716,  // (0,0,2)
      0.000605222,   // (0,1,0)
      -0.000922706,  // (0,1,1)
      0.000619337,   // (0,1,2)
      -0.000479029,  // (0,2,0)
      -5.98e-05,     // (0,2,1)
      5.92e-17,      // (1,0,0)
      -1.52e-16,     // (1,0,1)
      4.18e-17,      // (1,0,2)
      -1.36e-16,     // (1,1,0)
      9.22e-17,      // (1,1,1)
      3.75e-17,      // (1,2,0)
      -0.002522849,  // (2,0,0)
      0.001113772,   // (2,0,1)
      0.000426514,   // (2,1,0)
      0,             // (0,0,0)
      6.57e-16,      // (3,0,0)
      -0.000158924,  // (0,3,0)
      0.0013434      // (0,0,3)
  };               // 0-2000 min-0 // unit: m

  const std::vector<double> index_PIP_coeffs_Motor2Joint_ = {
      -239.38462,    // (0,0,1)
      -18084.14901,  // (0,0,2)
      0.071080981,   // (0,1,0)
      11.66347542,   // (0,1,1)
      1735.505584,   // (0,1,2)
      -0.074887112,  // (0,2,0)
      4.73,          // (0,2,1)
      -1.56e-14,     // (1,0,0)
      -1.49e-13,     // (1,0,1)
      4.91e-11,      // (1,0,2)
      1.38e-14,      // (1,1,0)
      2.15e-13,      // (1,1,1)
      -3.92e-15,     // (1,2,0)
      -0.463491891,  // (2,0,0)
      -38.51910042,  // (2,0,1)
      0.0687204,     // (2,1,0)
      0,             // (0,0,0)
      1.51e-13,      // (3,0,0)
      -0.023075136,  // (0,3,0)
      -1244934.545   // (0,0,3)
  };

  const std::vector<double> middle_PIP_coeffs_ = {
      -0.004504499,  // (0,0,1)
      -0.002341199,  // (0,0,2)
      0.000697489,   // (0,1,0)
      -0.000962223,  // (0,1,1)
      0.000512516,   // (0,1,2)
      -0.000609666,  // (0,2,0)
      1.08e-04,      // (0,2,1)
      9.45e-18,      // (1,0,0)
      1.05e-16,      // (1,0,1)
      -5.43e-17,     // (1,0,2)
      2.40e-17,      // (1,1,0)
      -1.31e-17,     // (1,1,1)
      -3.95e-17,     // (1,2,0)
      -0.002541165,  // (2,0,0)
      0.001239013,   // (2,0,1)
      0.000411984,   // (2,1,0)
      0,             // (0,0,0)
      -4.11e-16,     // (3,0,0)
      -0.000147334,  // (0,3,0)
      0.000961855    // (0,0,3)
  };                 // 0-2000 min-0 // unit: m

  const std::vector<double> middle_PIP_coeffs_Motor2Joint_ = {
      -206.3849098,  // (0,0,1)
      -10893.31803,  // (0,0,2)
      0.089069357,   // (0,1,0)
      16.01605453,   // (0,1,1)
      1535.395248,   // (0,1,2)
      -0.08634878,   // (0,2,0)
      0.933,         // (0,2,1)
      -8.18e-14,     // (1,0,0)
      -1.75e-12,     // (1,0,1)
      1.36e-11,      // (1,0,2)
      -9.64e-15,     // (1,1,0)
      1.50e-13,      // (1,1,1)
      4.65e-15,      // (1,2,0)
      -0.429062665,  // (2,0,0)
      -35.49983074,  // (2,0,1)
      0.059369174,   // (2,1,0)
      0,             // (0,0,0)
      -2.53e-13,     // (3,0,0)
      -0.021891079,  // (0,3,0)
      -768921.6366   // (0,0,3)
  };

  std::vector<double> coeffs_index1_ = {
      0,           // p00
      0.006081,    // p10
      -0.008884,   // p01
      -0.003837,   // p20
      0.001554,    // p11
      -0.00396,    // p02
      -0.0009369,  // p30
      0.005173,    // p21
      -0.0004128,  // p12
      0.001921     // p03
  };               // 0-2000 min-0 // unit: m

  std::vector<double> coeffs_abad_Motor2Joint_ = {
      0,           // p00
      -81.62,      // p10
      81.62,       // p01
      -852.2,      // p20
      -1.366,      // p11
      853.7,       // p02
      -7.296e+04,  // p30
      1.686e+05,   // p21
      -1.686e+05,  // p12
      7.301e+04    // p03
  };

  std::vector<double> coeffs_index2_ = {
      0,          // p00
      -0.006083,  // p10
      -0.008884,  // p01
      -0.003836,  // p20
      -0.001553,  // p11
      -0.003959,  // p02
      0.0009361,  // p30
      0.005173,   // p21
      0.0004146,  // p12
      0.001921    // p03
  };              // 0-2000 min-0 // unit: m

  std::vector<double> coeffs_mcp_Motor2Joint_ = {
      0,           // p00
      -54.01,      // p10
      -54.03,      // p01
      -2737.0,     // p20
      2464.0,      // p11
      -2738.0,     // p02
      -1.378e+05,  // p30
      6.981e+04,   // p21
      7.0e+04,     // p12
      -1.378e+05   // p03
  };

  std::vector<double> coeffs_thumb_roll_ = {0, -4.414, -2.493, 0.4605,
                                            0.1868};  // 0-2000 min-0 // unit: mm
  std::vector<double> coeffs_thumb_abad_ = {0, 2.265, 5.312, -1.507,
                                            -0.01794};  // 0-2000 max-0 // unit: mm
  std::vector<double> coeffs_thumb_mcp_ = {0, 13.68, -5.975, 4.724,
                                           -0.4232};  // 0-2000 max-0 // unit: mm
  std::vector<double> coeffs_thumb_pip_ = {0, 6.451, 2.701, -1.323,
                                           -0.00933};  // 0-2000 max-0 // unit: mm

  std::vector<double> coeffs_thumb_roll_Motor2Joint_ = {0, -0.2217, -0.02095, -0.002611, -0.0001367};
  std::vector<double> coeffs_thumb_abad_Motor2Joint_ = {0, 0.291, -0.04408, 0.005031, -0.0002107};
  std::vector<double> coeffs_thumb_mcp_Motor2Joint_ = {0, 0.07162, 0.003203, -0.0001568, -3.044e-06};
  std::vector<double> coeffs_thumb_pip_Motor2Joint_ = {0, 0.1504, -0.006036, 0.0004235, -2.954e-06};

  std::vector<double> coeffs_ring_pinky_ = {0, -6.13, -4.317, 0.5921, 0.306};  // 0-2000 min-0 // unit: mm
  std::vector<double> coeffs_ring_pinky_Motor2Joint_ = {0, -0.1488, -0.008172, -0.0004489, -8.805e-06};

  // Passive joints
  std::vector<double> thumb_dip_coeff_ = {0, 0.6359, -0.3539, -0.3066, -0.124};
  std::vector<double> index_dip_coeff_ = {0, 1.063, 0.08942, 0.1845, -0.2169};
  std::vector<double> middle_dip_coeff_ = {0, 1.149, -0.2581, 0.6033, -0.3371};
  std::vector<double> ring_pip_coeff_ = {0, 0.7869, 0.3884, -0.4545, 0.1578};
  std::vector<double> ring_dip_coeff_ = {0, 0.899, 0.3138, -0.1728, -0.03666};

  void ClampAbadMcpJoint(double &x, double &y);

 public:
  OmniHandPro2025Solver(const bool &is_left_hand);
  ~OmniHandPro2025Solver();

  // check if the joint position is within the defined limits,if not, modify.
  bool CheckJointPos(std::vector<double> &active_joint_pos);

  // check the actuator input
  bool CheckActuatorInput(const std::vector<int> &actuator_input);

  // set the hand gesture, output the actuator input
  std::vector<int> SetHandGesture(OmniHandPro2025Gesture gesture);

  // Convert the active joint position to actuator input
  // Returns motor input values in range 0-2000 (different from O10 which is 0-4096)
  std::vector<int> ConvertJoint2Actuator(const std::vector<double> &active_joint_pos);

  // Convert the actuator input to active joint position
  // Input motor input values should be in range 0-2000 (different from O10 which is 0-4096)
  std::vector<double> ConvertActuator2Joint(const std::vector<int> &actuator_input);

  // Convert the active joint position to motor length
  std::vector<double> ActiveJoint2MotorLength(const std::vector<double> &active_jont_pos);

  // Convert the motor length to active joint position
  std::vector<double> MotorLength2ActiveJoint(const std::vector<double> &motor_length_);

  // Convert the motor length to motor input value
  std::vector<int> MotorLength2MotorInput(const std::vector<double> &motor_length);

  // Convert the motor input value to motor length
  std::vector<double> MotorInput2MotorLength(const std::vector<int> &motor_input);

  double FingerPIPFitPredict(const double &abad, const double &mcp,
                             const double &pip,
                             const std::vector<double> &coeffs);
  double PredictPoly33(const double &abad, const double &mcp,
                       const std::vector<double> &coeffs);
  double PredictPoly(const double &x, const std::vector<double> &coeffs);

  std::vector<double> GetAllJointPos(const std::vector<double> &active_joint_pos);

  /**
   * @brief Get min/max motor input for a given actuator (0-based index, 0=ActuatorIndex1 … 11=ActuatorThumbMCP)
   */
  static Int16Bound GetMotorPositionRange(uint8_t actuator_index) {
    if (actuator_index >= ActuatorCount) return {0, 0};
    int16_t mn = static_cast<int16_t>(kMotorInputMin[actuator_index]);
    int16_t mx = static_cast<int16_t>(kMotorInputMax[actuator_index]);
    if (mn > mx) {
        return {mx, mn};
    }
    return {mn, mx};
  }

  /**
   * @brief Get min/max active joint angle (rad) for a given actuator, adjusted for handedness
   * @param actuator_index 0-based, per O12handProActuator (0=ActuatorIndex1 … 11=ActuatorThumbMCP)
   */
  static FloatBound GetJointAngleRange(uint8_t actuator_index, bool is_left_hand) {
    if (actuator_index >= ActuatorCount) return {0.0, 0.0};
    uint8_t aj = kActuatorToActiveJoint[actuator_index];
    float mn = static_cast<float>(kActiveJointMin[aj]);
    float mx = static_cast<float>(kActiveJointMax[aj]);
    if (is_left_hand) {
      if (aj == ActiveJointIndexAbAd ||
          aj == ActiveJointMiddleABAD ||
          aj == ActiveJointThumbMCP)
        return {-mx, -mn};
    } else {
      if (aj == ActiveJointThumbAbAd ||
          aj == ActiveJointThumbMCP ||
          aj == ActiveJointThumbPIP)
        return {-mx, -mn};
    }
    return {mn, mx};
  }

  template <typename T>
  void Clamp(const std::vector<T> &max, const std::vector<T> &min,
             std::vector<T> &value);

  template <typename InputT, typename OutputT>
  std::vector<OutputT> Scale(const std::vector<InputT> &max,
                             const std::vector<InputT> &min,
                             const std::vector<InputT> &value,
                             const std::vector<OutputT> &target_max,
                             const std::vector<OutputT> &target_min);
};
}  // namespace o12
}  // namespace omnihand
}  // namespace agilink

#endif  // OMNIHAND_PRO_2025_SOLVER_H
