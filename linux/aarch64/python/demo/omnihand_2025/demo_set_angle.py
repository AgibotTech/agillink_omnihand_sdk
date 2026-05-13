# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 joint angle control demo

This demo uses `set_all_active_joint_angles()` for all active joints.
Sequence: reset → FIST → reset
Quick test for joint-angle control.
"""

import argparse
import time
from omnihand import OmniHand2025, HandType

def main():
    parser = argparse.ArgumentParser(description='OmniHand 2025 Joint Angle Control Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp', 'tj'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    print("=" * 50)
    print("OmniHand 2025 Joint Angle Control Demo")
    print("=" * 50)
    
    # Create OmniHand 2025 instance by device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.RIGHT)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(hand_type=HandType.RIGHT, uart_port='COM7')
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    elif args.device == 'tj':
        hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.LEFT, marvin_controller_ip="192.168.10.190")
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT)
    
    # Check init status
    if not hand.init():
        print("[Error]: Failed to initialize OmniHand 2025 hand!")
        return
    
    hand.show_data_details(True)
    print("[OK]: OmniHand 2025 hand initialized successfully!\n")
    
    # reset - all joints to initial pose
    print("Setting all joints to reset position...")
    aim_positions = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # O10 has 10 joints
    hand.set_all_active_joint_angles(aim_positions)
    print(f"Set active joint angles: {aim_positions}")
    time.sleep(1)
    
    # read current angles
    all_active_angles = hand.get_all_active_joint_angles()
    print(f"Current active joint angles: {all_active_angles}")
    
    # FIST - closed fist
    print("\nSetting all joints to FIST position...")
    aim_positions = [0.5, -0.2, 0.0, -1.2, 0.0, 1.35, 1.53, 0.0, 1.36, 1.82]  # O10 has 10 joints
    hand.set_all_active_joint_angles(aim_positions)
    print(f"Set active joint angles: {aim_positions}")
    time.sleep(1)
    
    # read current angles
    all_active_angles = hand.get_all_active_joint_angles()
    print(f"Current active joint angles: {all_active_angles}")
    
    # reset - return to initial pose
    print("\nSetting all joints back to reset position...")
    aim_positions = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    hand.set_all_active_joint_angles(aim_positions)
    print(f"Set active joint angles: {aim_positions}")
    time.sleep(1)
    
    # read final angles
    all_active_angles = hand.get_all_active_joint_angles()
    print(f"Final active joint angles: {all_active_angles}")
    
    # Read all joint angles (including passive)
    all_angles = hand.get_all_joint_angles()
    print(f"\nAll joint angles (including passive joints): {all_angles}")
    print(f"Total joints: {len(all_angles)} (10 active + 6 passive)")
    
    print("\n[Done]: Joint angle control demo completed!")

if __name__ == "__main__":
    main()
