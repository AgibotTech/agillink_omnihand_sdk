# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, EHandType

def main():
    hand = OmniHand2025.create_hand_by_zlgcan(
        hand_type=EHandType.LEFT,
        hand_device_id=1,
        canfd_device_id=0,
        canfd_channel_id=0
    )
    
    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(vendor_info)

    device_info = hand.get_device_info() 
    print("Device Info:")
    print(device_info)


if __name__ == "__main__":
    main()
