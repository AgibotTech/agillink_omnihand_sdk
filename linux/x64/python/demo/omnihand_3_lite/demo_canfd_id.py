# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - CANFD Device ID Demo

This demo shows how to create O4 instance using CANFD device ID.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType

def main():
    parser = argparse.ArgumentParser(description='OmniHand 3 Lite S (O4) - CANFD Device ID Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create O4 hand instance using CANFD device ID based on device type
    if args.device == 'hcan':
        hand = OmniHand3Lite.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            canfd_device_id=0,  # First CANFD device
            canfd_channel_id=0  # First channel
        )
    else:  # default: zlgcan
        hand = OmniHand3Lite.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            canfd_device_id=0,  # First CANFD device
            canfd_channel_id=0  # First channel
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
