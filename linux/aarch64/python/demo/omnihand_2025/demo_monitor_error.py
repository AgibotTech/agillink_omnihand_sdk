# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Monitor error for OmniHand 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.LEFT)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(
            hand_type=HandType.RIGHT,
            uart_port='/dev/ttyACM0'
        )
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=HandType.RIGHT,
            host='192.168.0.178', 
            port=8000
        )
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)
    
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
