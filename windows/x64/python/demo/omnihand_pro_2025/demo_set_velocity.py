# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 - Set Joint Velocities Demo

This demo shows how to set and read joint velocities for OmniHand Pro 2025.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).

Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHandPro2025, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_set_velocity.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_set_velocity.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_set_velocity.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand Pro 2025 - Set Joint Velocities Demo',
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
    
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(
            hand_type=HandType.RIGHT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'socketcan':
        hand = OmniHandPro2025.create_hand_socketcan(
            hand_type=HandType.RIGHT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            can_interface=args.can_interface
        )
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(
            hand_type=HandType.RIGHT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    
    if not hand.init():
        print("Failed to initialize device")
        return
    
    # Set all joint velocities to 0 (stop)
    velocities = [0] * 12
    hand.set_all_joint_velocities(velocities)
    print(f"Set all joint velocities to 0: {velocities}")
    time.sleep(1)
    
    # Get current velocities
    current_velocities = hand.get_all_joint_velocities()
    print(f"Current joint velocities: {current_velocities}")
    
    # Set individual joint velocity
    hand.set_joint_velocity(1, 100)
    print("Set joint 1 velocity to 100")
    time.sleep(1)
    
    # Get individual joint velocity
    joint1_velocity = hand.get_joint_velocity(1)
    print(f"Joint 1 velocity: {joint1_velocity}")
    
    # Set all joint velocities
    velocities = [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 0]
    hand.set_all_joint_velocities(velocities)
    print(f"Set all joint velocities: {velocities}")
    time.sleep(1)
    
    # Get all joint velocities
    all_velocities = hand.get_all_joint_velocities()
    print(f"All joint velocities: {all_velocities}")
    
    # Reset to 0
    velocities = [0] * 12
    hand.set_all_joint_velocities(velocities)
    print(f"Reset all joint velocities to 0: {velocities}")


if __name__ == "__main__":
    main()