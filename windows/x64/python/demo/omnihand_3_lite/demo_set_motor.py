# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - Set Motor Position Demo

This demo shows how to set motor positions for O4 (4 DOF).
Note: O4 does not support angle-based control, use motor position control instead.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_set_motor.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_set_motor.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_set_motor.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='Set motor positions for OmniHand 3 Lite S (O4)',
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
    
    # Create O4 hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand3Lite.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'socketcan':
        hand = OmniHand3Lite.create_hand_socketcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            can_interface=args.can_interface
        )
    else:  # default: zlgcan
        hand = OmniHand3Lite.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    # hand.show_data_details(True)

    # Set position of a single joint motor (index 1, position 200)
    hand.set_joint_position(1, 200)
    time.sleep(1)

    # Get position of joint motor 1
    joint1_posi = hand.get_joint_position(1)
    print("Joint 1 position: ", joint1_posi)

    # Set positions of all 4 joint motors
    # O4 has 4 DOF, so we need 4 position values
    init_positions = [2048, 2048, 2048, 2048]  # 4 joints, all at middle position
    actual_positions = hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    # Get all joint positions
    real_positions = hand.get_all_joint_positions()
    print("All joint positions: ", real_positions)
    print(f"Number of joints: {len(real_positions)} (should be 4 for O4)")


if __name__ == "__main__":
    main()
