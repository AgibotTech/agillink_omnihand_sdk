/**
 * @file omnihand_2025_solver.h
 * @brief O10灵巧手运动学求解器 (10自由度)
 * @author AgiBot-lishuang
 * @date 2025-03
 */

#ifndef OMNIHAND_2025_SOLVER_H
#define OMNIHAND_2025_SOLVER_H

#include <cassert>
#include <iostream>
#include <vector>

#include "omnihand/export_symbols.h"

/**
 * @brief O10灵巧手主动关节枚举
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

  ActiveJointCount  ///< 主动关节总数
};

/**
 * @brief O10灵巧手所有关节枚举 (主动+被动)
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

  JointCount  ///< 所有关节总数
};

/**
 * @brief O10灵巧手手势枚举
 */
enum OmnihandGesture {
  GesturePAPER = 0,
  GestureFIST1,
  GestureFIST2,
  GestureOK,
  GestureOneHandedFingerHeart,
  GestureLIKE,
  GestureILY,
  GestureNUM1,
  GestureNUM2,
  GestureNUM3,
  GestureNUM4,
  GestureNUM6,
  GestureNUM8,
  GestureHandHeart1,
  GestureHandHeart2,
  GestureHandHeart3,
  GestureClasping,
};

/**
 * @class OmniHand2025Solver
 * @brief OmniHand 2025 kinematics solver
 */
class AGIBOT_EXPORT OmniHand2025Solver {
 private:
  bool hand_type_;  ///< true=左手, false=右手
  std::vector<int> actuator_max_;
  std::vector<int> actuator_min_;
  int max_iput_ = 4096;
  int min_iput_ = 0;
  // 右手系数
  std::vector<double> active_joint_max_ = {1.12, 0.05, 0.8416, 0, 1.48,
                                           1.48, 0.17, 1.48, 0.19, 1.48};
  std::vector<double> motor_max_ = {1.12, 0.05, 1.33, 0, 1.43,
                                    1.43, 0.17, 1.43, 0.19, 1.43};
  std::vector<double> active_joint_min_ = {-0.03, -1.64, 0.0, -0.16, 0.0,
                                           0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<double> motor_min_ = {-0.03, -1.64, 0.0, -0.16, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<int> left_pos_direction_ = {-1, -1, -1, -1, 1, 1, -1, 1, -1, 1};
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
   * @brief 构造函数
   * @param hand_type true=左手, false=右手
   */
  OmniHand2025Solver(const bool &hand_type);

  ~OmniHand2025Solver();

  bool flag_ = false;
  void show_log(bool flag) {
    flag_ = flag;
  }

  /**
   * @brief 设置预定义手势
   * @param gesture_num 手势索引
   * @return 执行器命令向量
   */
  std::vector<int> SetHandGesture(const int &gesture_num);

  /**
   * @brief 主动关节位置转执行器输入
   * @param active_joint_pos 主动关节位置向量
   * @return 执行器命令向量 (范围: 0-4096)
   * @note O10 motor input range is 0-4096, different from O12 which is 0-2000
   */
  std::vector<int>
  ActiveJointPos2ActuatorInput(const std::vector<double> &active_joint_pos);

  /**
   * @brief 执行器输入转主动关节位置
   * @param actuator_input 执行器命令向量 (范围: 0-4096)
   * @return 主动关节位置向量
   * @note O10 motor input range is 0-4096, different from O12 which is 0-2000
   */
  std::vector<double>
  ActuatorInput2ActiveJointPos(const std::vector<int> &actuator_input);

  /**
   * @brief 获取所有关节位置 (主动+被动)
   * @param active_joint_pos 当前主动关节位置
   * @return 所有关节位置向量
   */
  std::vector<double>
  GetAllJointPos(const std::vector<double> &active_joint_pos);
};

#endif  // OMNIHAND_2025_SOLVER_H
