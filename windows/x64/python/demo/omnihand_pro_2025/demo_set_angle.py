# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHandPro2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Set joint angles for OmniHand Pro 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(hand_type=HandType.RIGHT)
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT)
    
    # reset
    aim_positions = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    hand.set_all_active_joint_angles(aim_positions)
    time.sleep(1)
    
    # FIST
    aim_positions = [0.5, -0.2, 0.0, -1.2, 0.0, 1.35, 1.53, 0.0, 1.36, 1.82, 1.55, 1.54]
    hand.set_all_active_joint_angles(aim_positions)
    time.sleep(1)

    # reset
    aim_positions = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    hand.set_all_active_joint_angles(aim_positions)
    time.sleep(1)

    all_active_angles = hand.get_all_active_joint_angles()
    print("All active joint positions: ", all_active_angles)

    all_angles = hand.get_all_joint_angles()
    print("All joint positions: ", all_angles)


if __name__ == "__main__":
    main()
