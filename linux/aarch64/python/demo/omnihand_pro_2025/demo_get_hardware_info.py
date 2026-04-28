# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHandPro2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Get hardware info from OmniHand Pro 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(hand_type=HandType.LEFT)
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)
    
    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(vendor_info)

    time.sleep(2)

    device_info = hand.get_device_info() 
    print("Device Info:")
    print(device_info)


if __name__ == "__main__":
    main()
