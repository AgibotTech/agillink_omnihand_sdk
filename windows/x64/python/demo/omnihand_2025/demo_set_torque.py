# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, Finger, ControlMode, HandType

def main():
    parser = argparse.ArgumentParser(description='Set control mode for OmniHand 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.LEFT)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(hand_type=HandType.RIGHT, uart_port='/dev/ttyACM0')
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)

    hand.show_data_details(True)

    # for i in range(10):
    #     print("Control mode of joint motor {}: ".format(i+1), hand.get_control_mode(i+1))

    
    
    all_control_modes = hand.get_all_control_modes()
    print("All control modes: ", all_control_modes)


if __name__ == "__main__":
    main()
