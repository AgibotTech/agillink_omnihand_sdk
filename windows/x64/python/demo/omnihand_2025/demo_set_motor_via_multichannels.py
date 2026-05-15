# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, Finger, ControlMode, HandType, OmniHand2025Gesture

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
    
    # Verbose CAN log
    left_hand.show_data_details(True)
    right_hand.show_data_details(True)

    # Step 1: fist both hands
    print("Step 1: Making both hands into fist position...")
    # Use SetHandGesture (solver handles left/right differences)
    left_hand.set_hand_gesture(OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST2)
    right_hand.set_hand_gesture(OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST2)
    time.sleep(2)  # wait for fist motion
    print("Fist position set for both hands")
    
    # Step 2: open palm
    print("\nStep 2: Opening both hands (reset position)...")
    left_hand.set_hand_gesture(OmniHand2025Gesture.OMNIHAND_2025_GESTURE_RESET)
    right_hand.set_hand_gesture(OmniHand2025Gesture.OMNIHAND_2025_GESTURE_RESET)
    time.sleep(2)  # wait for open palm
    print("Hands opened (reset position)")
    
    # Step 3: thumb and little-finger demo
    print("\nStep 3: Setting thumb and pinky positions...")
    left_hand.set_joint_position(2, 200)  # left joint 2 (thumb)
    right_hand.set_joint_position(10, 200)  # right joint 10 (little)
    time.sleep(1)

    # Read and print positions
    id2_joint_posi = left_hand.get_joint_position(2)
    print("Joint 2 position of left hand (thumb): ", id2_joint_posi)
    id2_joint_posi_right = right_hand.get_joint_position(10)
    print("Joint 10 position of right hand (pinky): ", id2_joint_posi_right)

    # Read all joint positions
    real_positions = left_hand.get_all_joint_positions()
    print("All joint positions of left hand: ", real_positions)

    real_positions_right = right_hand.get_all_joint_positions()
    print("All joint positions of right hand: ", real_positions_right)


if __name__ == "__main__":
    main()
