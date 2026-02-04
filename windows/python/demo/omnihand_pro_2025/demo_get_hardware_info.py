# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHandPro2025, EHandType
import time
def main():
    hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=EHandType.LEFT)
    
    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(vendor_info)

    time.sleep(2)

    device_info = hand.get_device_info() 
    print("Device Info:")
    print(device_info)


if __name__ == "__main__":
    main()
