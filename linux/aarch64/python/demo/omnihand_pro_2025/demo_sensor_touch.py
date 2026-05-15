# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 - Touch Sensor Demo

This demo shows how to read 3D tactile sensor data from OmniHand Pro 2025.
Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).

Run with -h or --help to see all available options and usage examples.
"""

import argparse
from omnihand import OmniHandPro2025, Finger, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_sensor_touch.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_sensor_touch.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_sensor_touch.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand Pro 2025 - Touch Sensor Demo',
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
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'socketcan':
        hand = OmniHandPro2025.create_hand_socketcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            can_interface=args.can_interface
        )
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )

    # O12 uses get_tactile_sensor_3d_data (returns TactileSensor3DData object)
    # O10 uses get_tactile_sensor_data (returns List[int])
    touch_data = hand.get_tactile_sensor_3d_data(Finger.THUMB)
    print(f"Thumb touch sensor data:")
    print(f"  Online state: {touch_data.online_state}")
    print(f"  Normal force: {touch_data.normal_force}")
    print(f"  Tangent force: {touch_data.tangent_force}")
    print(f"  Tangent force angle: {touch_data.tangent_force_angle}")
    print(f"  Channel values: {touch_data.channel_values}")
    print(f"  Capacitive approach: {touch_data.capacitive_approach}")

if __name__ == "__main__":
    main()