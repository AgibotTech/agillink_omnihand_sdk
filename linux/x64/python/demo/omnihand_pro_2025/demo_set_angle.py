# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 - Set Joint Angles Demo

This demo shows how to set and read joint angles for OmniHand Pro 2025.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).

Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHandPro2025, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_set_angle.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_set_angle.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_set_angle.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand Pro 2025 - Set Joint Angles Demo',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'socketcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD), hcan (HCAN USB CANFD), socketcan (Linux only), default: zlgcan')
    parser.add_argument('--canfd-device-id', type=int, default=0,
                        help='CANFD device index, default: 0')
    parser.add_argument('--canfd-channel-id', type=int, default=0,
                        help='CANFD channel index, default: 0')
    parser.add_argument('--can-interface', type=str, default='can0',
                        help='SocketCAN interface (Linux only), default: can0')
    args = parser.parse_args()
    
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(
            hand_type=HandType.RIGHT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'socketcan':
        hand = OmniHandPro2025.create_hand_socketcan(
            hand_type=HandType.RIGHT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            can_interface=args.can_interface
        )
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(
            hand_type=HandType.RIGHT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    
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