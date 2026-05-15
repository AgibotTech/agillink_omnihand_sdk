# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - Get Hardware Info Demo

This demo shows how to get vendor and device information from O4.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
from omnihand import OmniHand3Lite, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_get_hardware_info.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_get_hardware_info.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_get_hardware_info.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='Get hardware info from OmniHand 3 Lite S (O4)',
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
    
    # Get vendor information
    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(vendor_info)
    print(f"  DOF: {vendor_info.dof} (should be 4 for O4)")

    # Get device information
    device_info = hand.get_device_info() 
    print("\nDevice Info:")
    print(device_info)


if __name__ == "__main__":
    main()
