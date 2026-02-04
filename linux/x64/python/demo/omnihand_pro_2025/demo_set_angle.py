# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHandPro2025, EHandType
import time

def main():
    hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=EHandType.RIGHT)
    
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
