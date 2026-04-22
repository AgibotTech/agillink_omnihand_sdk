# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - SocketCAN Demo (Linux only)

This demo shows how to create O4 instance using SocketCAN on Linux.
"""

import sys
import time

# Check if running on Linux
if sys.platform != "linux":
    print("SocketCAN is only available on Linux")
    sys.exit(1)

from omnihand import OmniHand3Lite, HandType

def main():
    # Create O4 hand instance using SocketCAN
    hand = OmniHand3Lite.create_hand_socketcan(
        hand_type=HandType.LEFT,
        hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
        can_interface="can0"  # CAN interface name
    )
    
    if hand is None:
        print("Failed to create hand instance")
        return
    
    # Initialize hand
    if not hand.init():
        print("Failed to initialize hand")
        return
    
    print("O4 hand initialized successfully via SocketCAN")
    
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
