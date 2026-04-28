# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHandPro2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Set joint velocities for OmniHand Pro 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(hand_type=HandType.RIGHT)
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT)
    
    if not hand.init():
        print("Failed to initialize device")
        return
    
    # Set all joint velocities to 0 (stop)
    velocities = [0] * 12
    hand.set_all_joint_velocities(velocities)
    print(f"Set all joint velocities to 0: {velocities}")
    time.sleep(1)
    
    # Get current velocities
    current_velocities = hand.get_all_joint_velocities()
    print(f"Current joint velocities: {current_velocities}")
    
    # Set individual joint velocity
    hand.set_joint_velocity(1, 100)
    print("Set joint 1 velocity to 100")
    time.sleep(1)
    
    # Get individual joint velocity
    joint1_velocity = hand.get_joint_velocity(1)
    print(f"Joint 1 velocity: {joint1_velocity}")
    
    # Set all joint velocities
    velocities = [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 0]
    hand.set_all_joint_velocities(velocities)
    print(f"Set all joint velocities: {velocities}")
    time.sleep(1)
    
    # Get all joint velocities
    all_velocities = hand.get_all_joint_velocities()
    print(f"All joint velocities: {all_velocities}")
    
    # Reset to 0
    velocities = [0] * 12
    hand.set_all_joint_velocities(velocities)
    print(f"Reset all joint velocities to 0: {velocities}")


if __name__ == "__main__":
    main()
