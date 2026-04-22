# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Set motor positions for OmniHand 2025')
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
    # hand.show_data_details(True)

    hand.set_joint_position(2, 200)
    time.sleep(1)

    id2_joint_posi = hand.get_joint_position(2)
    print("Joint 2 position: ", id2_joint_posi)

    init_positions = [2048,2048,4096,2048,4096,4096,2048,4096,2048,4096]
    hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_positions = hand.get_all_joint_positions()
    print("All joint positions: ", real_positions)


if __name__ == "__main__":
    main()
