# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - Get Hardware Info Demo

This demo shows how to get vendor and device information from O4.
"""

import argparse
from omnihand import OmniHand3Lite, HandType

def main():
    parser = argparse.ArgumentParser(description='Get hardware info from OmniHand 3 Lite S (O4)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create O4 hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand3Lite.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=1,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    else:  # default: zlgcan
        hand = OmniHand3Lite.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=1,
            canfd_device_id=0,
            canfd_channel_id=0
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
