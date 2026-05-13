# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHand2025, Finger, ControlMode, HandType

def main():
    parser = argparse.ArgumentParser(description='Control multiple hands via multiple CAN devices')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'tj'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Two ways to create hand (pick by device type):
    # - canfd_device_id: by adapter index, no scan, single open; index may change after replug/reboot.
    # - usbcanfd_serial_number: by serial, scan then open, extra open/close; serial is stable.
    if args.device == 'tj':
        left_hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.LEFT, marvin_controller_ip="192.168.10.190")
        right_hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.RIGHT, marvin_controller_ip="192.168.10.190")
        # TjMarvinCanBusDevice defaults to 200ms; increase if timeouts persist (UDP + arm scheduling + CAN)
        for _h in (left_hand, right_hand):
            _h.set_frame_recv_timeout(200)
    elif args.device == 'hcan':
        left_hand = OmniHand2025.create_hand_by_hcan(HandType.LEFT, OmniHand2025.kDefaultHandDeviceId, "201BFF2AF01202D44690")
        right_hand = OmniHand2025.create_hand_by_hcan(HandType.RIGHT, OmniHand2025.kDefaultHandDeviceId, "A029A58630B30D14DBB")
    else:  # default: zlgcan
        left_hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.LEFT, hand_device_id=OmniHand2025.kDefaultHandDeviceId, usbcanfd_serial_number="201BFF2AF01202D44690", canfd_channel_id=0)
        right_hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT, hand_device_id=OmniHand2025.kDefaultHandDeviceId, usbcanfd_serial_number="201BFF2AF01202D44690", canfd_channel_id=1)
    
    if left_hand is None or right_hand is None:
        print("Cannot find CANFD devices by serial numbers!")
        return
    
    # Or if canfd_id is known, use it directly:
    # left_hand = OmniHand2025.create_hand_by_zlgcan(HandType.LEFT, OmniHand2025.kDefaultHandDeviceId, 0)  # canfd_device_id= 0
    # right_hand = OmniHand2025.create_hand_by_zlgcan(HandType.RIGHT, OmniHand2025.kDefaultHandDeviceId, 1)  # canfd_device_id= 1
    
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
