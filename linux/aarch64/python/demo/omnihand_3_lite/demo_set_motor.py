# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - Set Motor Position Demo

This demo shows how to set motor positions for O4 (4 DOF).
Note: O4 does not support angle-based control, use motor position control instead.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType

def main():
    parser = argparse.ArgumentParser(description='Set motor positions for OmniHand 3 Lite S (O4)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create O4 hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand3Lite.create_hand_by_hcan(hand_type=HandType.LEFT)
    else:  # default: zlgcan
        hand = OmniHand3Lite.create_hand_by_zlgcan(hand_type=HandType.LEFT)
    # hand.show_data_details(True)

    # Set position of a single joint motor (index 1, position 200)
    hand.set_joint_position(1, 200)
    time.sleep(1)

    # Get position of joint motor 1
    joint1_posi = hand.get_joint_position(1)
    print("Joint 1 position: ", joint1_posi)

    # Set positions of all 4 joint motors
    # O4 has 4 DOF, so we need 4 position values
    init_positions = [2048, 2048, 2048, 2048]  # 4 joints, all at middle position
    actual_positions = hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    # Get all joint positions
    real_positions = hand.get_all_joint_positions()
    print("All joint positions: ", real_positions)
    print(f"Number of joints: {len(real_positions)} (should be 4 for O4)")


if __name__ == "__main__":
    main()
