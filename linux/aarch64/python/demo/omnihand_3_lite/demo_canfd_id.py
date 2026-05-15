# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - CANFD Device ID Demo

This demo shows how to create O4 instance using CANFD device ID.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_canfd_id.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_canfd_id.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_canfd_id.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand 3 Lite S (O4) - CANFD Device ID Demo',
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
    
    # Create O4 hand instance using CANFD device ID based on device type
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
    
    if hand is None:
        print("Failed to create hand instance")
        return
    
    # Initialize hand
    if not hand.init():
        print("Failed to initialize hand")
        return
    
    print("O4 hand initialized successfully")
    
    # Get vendor info
    vendor_info = hand.get_vendor_info()
    print(f"Model: {vendor_info.product_model}")
    print(f"DOF: {vendor_info.dof}")
    
    # Test setting motor positions
    positions = [2048, 2048, 2048, 2048]
    actual_positions = hand.set_all_joint_positions(positions)
    time.sleep(1)
    
    if len(actual_positions) == 4:
        print("Motor positions set successfully")
        print(f"Actual positions: {actual_positions}")
    else:
        print("Failed to set motor positions")


if __name__ == "__main__":
    main()
