# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - Control multiple hands via multiple CAN devices

This demo shows how to control two O4 hands (left and right) connected to
two CANFD devices (e.g. two ZLG USB-CANFD or two HCAN USB-CANFD), by specifying
each device's serial number. O4 has 4 DOF; joint indices are 1-4.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, left on device 0, right on device 1, channel 0
  python demo_set_motor_via_multicans.py -d zlgcan --canfd-device-id-left 0 --canfd-device-id-right 1 --canfd-channel-id 0

  # HCAN, left on device 0, right on device 1, channel 0
  python demo_set_motor_via_multicans.py -d hcan --canfd-device-id-left 0 --canfd-device-id-right 1 --canfd-channel-id 0

  # SocketCAN (Linux only), two interfaces
  python demo_set_motor_via_multicans.py -d socketcan --can-interface-left can0 --can-interface-right can1
"""

def main():
    parser = argparse.ArgumentParser(
        description='Control multiple O4 hands via multiple CAN devices',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        '-d', '--device', choices=['zlgcan', 'hcan', 'socketcan'], default='zlgcan',
        help='CAN device type: zlgcan (ZLG USB CANFD), hcan (HCAN USB CANFD), socketcan (Linux only), default: zlgcan'
    )
    parser.add_argument('--canfd-device-id-left', type=int, default=0,
                        help='CANFD device index for left hand, default: 0')
    parser.add_argument('--canfd-device-id-right', type=int, default=1,
                        help='CANFD device index for right hand, default: 1')
    parser.add_argument('--canfd-channel-id', type=int, default=0,
                        help='CANFD channel index (shared), default: 0')
    parser.add_argument('--can-interface-left', type=str, default='can0',
                        help='SocketCAN interface for left hand (Linux only), default: can0')
    parser.add_argument('--can-interface-right', type=str, default='can1',
                        help='SocketCAN interface for right hand (Linux only), default: can1')
    args = parser.parse_args()

    # Create O4 hands by CAN device serial number (replace with your actual serials)
    # hand_device_id=kDefaultHandDeviceId, canfd_channel_id=0 per device
    if args.device == 'hcan':
        left_hand = OmniHand3Lite.create_hand_by_hcan(
            HandType.LEFT,  # hand_type
            OmniHand3Lite.kDefaultHandDeviceId,  # hand_device_id
            args.canfd_device_id_left,  # canfd_device_id
            args.canfd_channel_id  # canfd_channel_id
        )
        right_hand = OmniHand3Lite.create_hand_by_hcan(
            HandType.RIGHT,  # hand_type
            OmniHand3Lite.kDefaultHandDeviceId,  # hand_device_id
            args.canfd_device_id_right,  # canfd_device_id
            args.canfd_channel_id  # canfd_channel_id
        )
    elif args.device == 'socketcan':
        left_hand = OmniHand3Lite.create_hand_socketcan(
            HandType.LEFT,
            OmniHand3Lite.kDefaultHandDeviceId,
            args.can_interface_left
        )
        right_hand = OmniHand3Lite.create_hand_socketcan(
            HandType.RIGHT,
            OmniHand3Lite.kDefaultHandDeviceId,
            args.can_interface_right
        )
    else:  # default: zlgcan
        left_hand = OmniHand3Lite.create_hand_by_zlgcan(
            HandType.LEFT,  # hand_type
            OmniHand3Lite.kDefaultHandDeviceId,  # hand_device_id
            args.canfd_device_id_left,  # canfd_device_id
            args.canfd_channel_id  # canfd_channel_id
        )
        right_hand = OmniHand3Lite.create_hand_by_zlgcan(
            HandType.RIGHT,  # hand_type
            OmniHand3Lite.kDefaultHandDeviceId,  # hand_device_id
            args.canfd_device_id_right,  # canfd_device_id
            args.canfd_channel_id  # canfd_channel_id
        )

    if left_hand is None or right_hand is None:
        print("Cannot find CANFD devices by serial numbers!")
        print("Replace LEFT_*_SERIAL / RIGHT_*_SERIAL with your actual device serials.")
        return

    # Optional: enable detailed CAN log
    # left_hand.show_data_details(True)
    # right_hand.show_data_details(True)

    # Single joint: O4 has joints 1-4
    left_hand.set_joint_position(1, 200)
    right_hand.set_joint_position(1, 200)
    time.sleep(1)

    pos_left = left_hand.get_joint_position(1)
    pos_right = right_hand.get_joint_position(1)
    print("Joint 1 position of left hand: ", pos_left)
    print("Joint 1 position of right hand: ", pos_right)

    # All joints: O4 has 4 DOF
    init_positions = [2048, 2048, 2048, 2048]
    left_hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_left = left_hand.get_all_joint_positions()
    print("All joint positions of left hand (O4): ", real_left)

    right_hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_right = right_hand.get_all_joint_positions()
    print("All joint positions of right hand (O4): ", real_right)

    # Alternatively, create by canfd_device_id when you have two devices (e.g. id 0 and 1):
    # left_hand = OmniHand3Lite.create_hand_by_zlgcan(HandType.LEFT, OmniHand3Lite.kDefaultHandDeviceId, 0, 0)   # device 0, channel 0
    # right_hand = OmniHand3Lite.create_hand_by_zlgcan(HandType.RIGHT, OmniHand3Lite.kDefaultHandDeviceId, 1, 0)  # device 1, channel 0


if __name__ == "__main__":
    main()
