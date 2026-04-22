# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, Finger, HandType

def main():
    parser = argparse.ArgumentParser(description='Get tactile sensor data for OmniHand 2025')
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

    thumb_tactile_data = hand.get_tactile_sensor_data(Finger.THUMB)
    print("Thumb tactile data: {} g".format(sum(thumb_tactile_data)))

    index_tactile_data = hand.get_tactile_sensor_data(Finger.INDEX)
    print("Index tactile data: {} g".format(sum(index_tactile_data)))

    middle_tactile_data = hand.get_tactile_sensor_data(Finger.MIDDLE)
    print("Middle tactile data: {} g".format(sum(middle_tactile_data)))

    ring_tactile_data = hand.get_tactile_sensor_data(Finger.RING)
    print("Ring tactile data: {} g".format(sum(ring_tactile_data)))

    little_tactile_data = hand.get_tactile_sensor_data(Finger.LITTLE)
    print("Little tactile data: {} g".format(sum(little_tactile_data)))

    palm_tactile_data = hand.get_tactile_sensor_data(Finger.PALM)
    print("Palm tactile data: {} g".format(sum(palm_tactile_data)))

    dorsum_tactile_data = hand.get_tactile_sensor_data(Finger.DORSUM)
    print("Dorsum tactile data: {} g".format(sum(dorsum_tactile_data)))


if __name__ == "__main__":
    main()
