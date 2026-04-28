# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, Finger, ControlMode, HandType

def main():
    parser = argparse.ArgumentParser(description='Control multiple hands via multiple channels')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instances based on device type
    if args.device == 'hcan':
        left_hand = OmniHand2025.create_hand_by_hcan(canfd_device_id=0, hand_type=HandType.LEFT, canfd_channel_id=0)
        right_hand = OmniHand2025.create_hand_by_hcan(canfd_device_id=0, hand_type=HandType.RIGHT, canfd_channel_id=1)
    else:  # default: zlgcan
        left_hand = OmniHand2025.create_hand_by_zlgcan(canfd_device_id=0, hand_type=HandType.LEFT, canfd_channel_id=0)
        right_hand = OmniHand2025.create_hand_by_zlgcan(canfd_device_id=0, hand_type=HandType.RIGHT, canfd_channel_id=1)
    
    # 启用详细日志查看 CAN 通信
    left_hand.show_data_details(True)
    right_hand.show_data_details(True)

    # Step 1: 左右手先握拳
    print("Step 1: Making both hands into fist position...")
    # 使用新的 SetHandGesture 接口（solver会自动处理左右手差异）
    left_hand.set_hand_gesture(2)  # FIST2
    right_hand.set_hand_gesture(2)  # FIST2
    time.sleep(2)  # 等待握拳动作完成
    print("Fist position set for both hands")
    
    # Step 2: 摊开手掌
    print("\nStep 2: Opening both hands (reset position)...")
    reset_angles = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # 重置位置（摊开）
    left_hand.set_all_active_joint_angles(reset_angles)
    right_hand.set_all_active_joint_angles(reset_angles)
    time.sleep(2)  # 等待摊开动作完成
    print("Hands opened (reset position)")
    
    # Step 3: 做目前的大拇指和小拇指动作
    print("\nStep 3: Setting thumb and pinky positions...")
    left_hand.set_joint_position(2, 200)  # 左手关节2（大拇指）
    right_hand.set_joint_position(10, 200)  # 右手关节10（小拇指）
    time.sleep(1)

    # 读取并打印位置
    id2_joint_posi = left_hand.get_joint_position(2)
    print("Joint 2 position of left hand (thumb): ", id2_joint_posi)
    id2_joint_posi_right = right_hand.get_joint_position(10)
    print("Joint 10 position of right hand (pinky): ", id2_joint_posi_right)

    # 读取所有关节位置
    real_positions = left_hand.get_all_joint_positions()
    print("All joint positions of left hand: ", real_positions)

    real_positions_right = right_hand.get_all_joint_positions()
    print("All joint positions of right hand: ", real_positions_right)


if __name__ == "__main__":
    main()
