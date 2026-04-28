# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHandPro2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Monitor error for OmniHand Pro 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(hand_type=HandType.LEFT)
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)
    
    hand.show_data_details(True)
    
    # get error report for joint 1
    error = hand.get_error_report(1)
    print(f"Joint 1 error info: motor_except={error.motor_except}")

    # get error report for all joints
    all_errors = hand.get_all_error_reports()
    for i, error in enumerate(all_errors):
        print(f"Joint {i+1} error info: motor_except={error.motor_except}")


if __name__ == "__main__":
    main()
