/**
 * @file omnihand_2025_solver.h
 * @brief O10 dexterous hand kinematics solver (10 DOF)
 * @author AgiBot-lishuang
 * @date 2025-03
 */

#ifndef OMNIHAND_2025_SOLVER_H
#define OMNIHAND_2025_SOLVER_H

#include <cassert>
#include <iostream>
#include <vector>

#include "omnihand/export_symbols.h"

namespace agilink {
namespace omnihand {
namespace o10 {

/**
 * @brief O10 dexterous hand active joint enumeration
 */
enum OmnihandActiveJoint {
  ActiveJointhumbRoll = 0,
  ActiveJointThumbAbAd,
  ActiveJointThumbMCP,
  ActiveJointIndexAbAd,
  ActiveJointIndexPIP,
  ActiveJointMiddlePIP,
  ActiveJointRingAbAd,
  ActiveJointRingPIP,
  ActiveJointPinkyAbAd,
  ActiveJointPinkyPIP,

  ActiveJointCount  ///< Total number of active joints
};

/**
 * @brief O10 dexterous hand full joint enumeration (active + passive)
 */
enum OmnihandJoint {
  JointThumbRoll = 0,
  JointThumbAbAd,
  JointThumbMCP,
  JointThumbPIP,
  JointThumbDIP,
  JointIndexAbAd,
  JointIndexPIP,
  JointIndexDIP,
  JointMiddlePIP,
  JointMiddleDIP,
  JointRingAbAd,
  JointRingPIP,
  JointRingDIP,
  JointPinkyAbAd,
  JointPinkyPIP,
  JointPinkyDIP,

  JointCount  ///< Total number of all joints
};

/**
 * @brief O10 predefined hand gestures for SetHandGesture.
 */
enum class OmniHand2025Gesture : int {
  OMNIHAND_2025_GESTURE_ALL_ZERO = 0,
  OMNIHAND_2025_GESTURE_PAPER,
  OMNIHAND_2025_GESTURE_FIST1,
  OMNIHAND_2025_GESTURE_FIST2,
  OMNIHAND_2025_GESTURE_OK,
  OMNIHAND_2025_GESTURE_ONE_HANDED_FINGER_HEART,
  OMNIHAND_2025_GESTURE_LIKE,
  OMNIHAND_2025_GESTURE_ILY,
  OMNIHAND_2025_GESTURE_NUM1,
  OMNIHAND_2025_GESTURE_NUM2,
  OMNIHAND_2025_GESTURE_NUM3,
  OMNIHAND_2025_GESTURE_NUM4,
  OMNIHAND_2025_GESTURE_NUM6,
  OMNIHAND_2025_GESTURE_NUM8,
  OMNIHAND_2025_GESTURE_HAND_HEART1,
  OMNIHAND_2025_GESTURE_HAND_HEART2,
  OMNIHAND_2025_GESTURE_HAND_HEART3,
  OMNIHAND_2025_GESTURE_CLASPING,
};

/**
 * @class OmniHand2025Solver
 * @brief OmniHand 2025 kinematics solver
 */
class AGIBOT_EXPORT OmniHand2025Solver {
 private:
  bool hand_type_;  ///< true=left hand, false=right hand
  std::vector<int> actuator_max_;
  std::vector<int> actuator_min_;
  static constexpr int kMaxInput = 4096;
  static constexpr int kMinInput = 0;
  static inline const std::vector<double> kActiveJointMin = {
      -0.03, -1.64, 0.0, -0.16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  static inline const std::vector<double> kActiveJointMax = {
      1.12, 0.05, 0.8416, 0, 1.48, 1.48, 0.17, 1.48, 0.19, 1.48};
  static inline const std::vector<int> kLeftDirection = {
      -1, -1, -1, -1, 1, 1, -1, 1, -1, 1};
  // Right-hand coefficients
  std::vector<double> active_joint_max_ = kActiveJointMax;
  std::vector<double> active_joint_min_ = kActiveJointMin;
  std::vector<double> motor_max_ = {1.12, 0.05, 1.33, 0, 1.43,
                                    1.43, 0.17, 1.43, 0.19, 1.43};
  std::vector<double> motor_min_ = {-0.03, -1.64, 0.0, -0.16, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<double> finger_pip2dip_poly_ = {0.0, 2.192, -1.425, 0.747,
                                              -0.167};
  std::vector<double> finger_mcp2motor_poly_ = {
      0.00944480234881967, 0.455882677008572, 0.683758090072141,
      -0.916673507519311, 0.459387725400186};
  std::vector<double> finger_motor2mcp_poly_ = {
      -0.000257594494466942, 1.57144033291557, 0.217395210463076,
      -0.768328304426314, 0.248168989312469};

  std::vector<double> thumb_mcp2pip_poly_ = {0.0, 1.33};
  std::vector<double> thumb_mcp2dip_poly_ = {0.0, 1.846, -0.853, 0.280};
  std::vector<double> thumb_pip2mcp_poly_ = {0.0, 1 / 1.33};
  std::vector<double> right_thumb_mcp2motor_poly_ = {
      0.00126371020922368, 0.919140692758276, 0.550958572722048,
      -0.785384985903032, 1.25635285116862};
  std::vector<double> left_thumb_mcp2motor_poly_ = {
      -0.00126371020922368, 0.919140692758276, -0.550958572722048,
      -0.785384985903032, -1.25635285116862};

  std::vector<double> right_thumb_motor2mcp_poly_ = {
      -0.000677604838762652, 1.05175893483608, -0.280133575638901,
      -0.115384415912668, 0.0676128925382166};
  std::vector<double> left_thumb_motor2mcp_poly_ = {
      0.000677604838762652, 1.05175893483608, 0.280133575638901,
      -0.115384415912668, -0.0676128925382166};

  double CalculatePower(const double x, const std::vector<double> &v);
  void ClampJointPos(std::vector<double> &active_joint_pos);
  bool CheckJointPos(const std::vector<double> &active_joint_pos);
  bool CheckActuatorInput(const std::vector<int> &actuator_input);

 public:
  /**
   * @brief Constructor
   * @param hand_type true=left hand, false=right hand
   */
  OmniHand2025Solver(const bool &hand_type);

  ~OmniHand2025Solver();

  bool flag_ = false;
  void show_log(bool flag) {
    flag_ = flag;
  }

  /**
   * @brief Set a predefined hand gesture
   * @param gesture See OmniHand2025Gesture
   * @return Actuator command vector
   */
  std::vector<int> SetHandGesture(OmniHand2025Gesture gesture);

  /**
   * @brief Convert active joint positions to actuator inputs
   * @param active_joint_pos Active joint position vector
   * @return Actuator command vector (range: 0-4096)
   * @note O10 motor input range is 0-4096, different from O12 which is 0-2000
   */
  std::vector<int>
  ActiveJointPos2ActuatorInput(const std::vector<double> &active_joint_pos);

  /**
   * @brief Convert actuator inputs to active joint positions
   * @param actuator_input Actuator command vector (range: 0-4096)
   * @return Active joint position vector
   * @note O10 motor input range is 0-4096, different from O12 which is 0-2000
   */
  std::vector<double>
  ActuatorInput2ActiveJointPos(const std::vector<int> &actuator_input);

  /**
   * @brief Get all joint positions (active + passive)
   * @param active_joint_pos Current active joint positions
   * @return All joint position vector
   */
  std::vector<double>
  GetAllJointPos(const std::vector<double> &active_joint_pos);

  static std::pair<int, int> GetMotorPositionRange(uint8_t actuator_index);

  static std::pair<double, double> GetJointAngleRange(uint8_t actuator_index, bool is_left_hand);
};

}  // namespace o10
}  // namespace omnihand
}  // namespace agilink

#endif  // OMNIHAND_2025_SOLVER_H
