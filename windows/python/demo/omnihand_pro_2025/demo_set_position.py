# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHandPro2025, EHandType
import time

def main():
    hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=EHandType.LEFT)
    
    hand.set_joint_position(8, 1000)
    time.sleep(1)

    real_position = hand.get_joint_position(8)
    print("Joint 8 position: ", real_position)
    time.sleep(3)

    aim_positions = [2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 0]
    hand.set_all_joint_positions(aim_positions)
    time.sleep(1)

    real_positions = hand.get_all_joint_positions()
    print("All joint positions: ", real_positions)


if __name__ == "__main__":
    main()
