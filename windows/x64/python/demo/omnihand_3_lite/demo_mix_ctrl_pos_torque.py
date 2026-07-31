# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) - Mix Control Demo (Position + Torque)

Demonstrates position+torque mixed control via mix_control_by_pt(),
alternating between two sets of position/torque parameters.

Supports connection types: ZLG CANFD, HCAN, ZLG TCP, SocketCAN.
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD
  python demo_mix_ctrl_pos_torque.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_mix_ctrl_pos_torque.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_mix_ctrl_pos_torque.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand 3 Lite S (O4) - Mix Control Demo (Position + Torque)',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp'], default='zlgcan',
                        help='Device type: zlgcan, hcan, zlgcan_tcp, socketcan (Linux only), default: zlgcan')
    parser.add_argument('--canfd-device-id', type=int, default=0,
                        help='CANFD device index, default: 0')
    parser.add_argument('--canfd-channel-id', type=int, default=0,
                        help='CANFD channel index, default: 0')
    parser.add_argument('--can-interface', type=str, default='can0',
                        help='SocketCAN interface (Linux only), default: can0')
    parser.add_argument('--host', type=str, default='192.168.0.178',
                        help='ZLG CAN TCP host address, default: 192.168.0.178')
    parser.add_argument('--port', type=int, default=8000,
                        help='ZLG CAN TCP port, default: 8000')
    args = parser.parse_args()

    print("=" * 60)
    print("OmniHand 3 Lite S (O4) Mix Control Demo - POSITION_TORQUE")
    print("=" * 60)

    if args.device == 'hcan':
        hand = OmniHand3Lite.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand3Lite.create_hand_by_zlgcan_tcp(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
            host=args.host,
            port=args.port
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

    if not hand.init():
        print("[Error]: Failed to initialize OmniHand 3 Lite S hand!")
        return

    hand.show_data_details(True)
    print("[OK]: OmniHand 3 Lite S hand initialized successfully!\n")

    NUM_JOINTS = 4

    for cycle in range(6):
        if cycle % 2 == 0:
            positions = [2000] * NUM_JOINTS
            torques = [50] * NUM_JOINTS
        else:
            positions = [1000] * NUM_JOINTS
            torques = [150] * NUM_JOINTS

        print(f"[Cycle {cycle}] mix_control_by_pt pos[0]={positions[0]} torque[0]={torques[0]}")
        hand.mix_control_by_pt(positions, torques)
        time.sleep(1.5)

        print(f"[Cycle {cycle}] mix_control_by_pt single-joint: joint=1 pos={positions[0]} torque={torques[0]}")
        hand.mix_control_by_pt(1, positions[0], torques[0])
        time.sleep(0.5)

    print("\n[Done]: Mix control demo completed!")

if __name__ == "__main__":
    main()