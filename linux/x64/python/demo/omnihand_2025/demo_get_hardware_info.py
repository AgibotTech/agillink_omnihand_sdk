# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

import argparse
from omnihand import OmniHand2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Get hardware info from OmniHand 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(
            hand_type=HandType.RIGHT,
            uart_port='COM6'
        )
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=HandType.RIGHT,
            host='192.168.0.178', 
            port=8000
        )
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            canfd_device_id=0,
            canfd_channel_id=0
        )

    hand.show_data_details(True)
    
    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(vendor_info)

    device_info = hand.get_device_info() 
    print("Device Info:")
    print(device_info)


if __name__ == "__main__":
    main()
