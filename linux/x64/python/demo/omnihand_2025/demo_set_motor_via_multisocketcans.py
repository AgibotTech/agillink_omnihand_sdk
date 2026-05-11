# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, Finger, ControlMode, HandType
import time

def main():
    left_hand = OmniHand2025.create_hand_socketcan(hand_type=HandType.LEFT, can_interface="can0")
    right_hand = OmniHand2025.create_hand_socketcan(hand_type=HandType.RIGHT, can_interface="can1")

    # Verbose CAN log
    left_hand.show_data_details(True)
    right_hand.show_data_details(True)

    left_hand.set_joint_position(2, 200)
    # time.sleep(1)
    right_hand.set_joint_position(10, 200)
    time.sleep(1)

    id2_joint_posi = left_hand.get_joint_position(2)
    print("Joint 2 position of left hand: ", id2_joint_posi)
    id2_joint_posi_right = right_hand.get_joint_position(10)
    print("Joint 10 position of right hand: ", id2_joint_posi_right)

    init_positions = [2048,2048,4096,2048,4096,4096,2048,4096,2048,4096]
    left_hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_positions = left_hand.get_all_joint_positions()
    print("All joint positions of left hand: ", real_positions)

    right_hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_positions_right = right_hand.get_all_joint_positions()
    print("All joint positions of right hand: ", real_positions_right)


if __name__ == "__main__":
    main()
