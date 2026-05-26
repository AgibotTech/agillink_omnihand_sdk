# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 (O12) - Mix Control Demo (Position + Force)

Demonstrates position+force mixed control via mix_control_by_pt(),
alternating between two sets of position/force parameters.

O12 and OP3 only support position+force mixed control (MixControlMode::POSITION_TORQUE /
mix_control_by_pt). mix_control_by_pv and mix_control_by_pvt are not available on these
products. Torque on O12/OP3 is int16 with unit 0.01 N (not motor current mA).

Array length must match active DOF (O12: 12). Index i maps to mix-control joint id i (0-based).

Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHandPro2025, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, device 0 channel 0
  python demo_mix_ctrl_pos_torque.py -d zlgcan --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_mix_ctrl_pos_torque.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_mix_ctrl_pos_torque.py -d socketcan --can-interface can0
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand Pro 2025 - Mix Control Demo',
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

    print("=" * 60)
    print("OmniHand Pro 2025 (O12) Mix Control Demo")
    print("  O12 / OP3: position + force only (mix_control_by_pt)")
    print("  Force unit: 0.01 N per joint")
    print("=" * 60)

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
    else:
        hand = OmniHandPro2025.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )

    if not hand.init():
        print("[Error]: Failed to initialize OmniHand Pro 2025 hand!")
        return

    hand.show_data_details(True)
    print("[OK]: OmniHand Pro 2025 hand initialized successfully!\n")

    NUM_JOINTS = 12

    for cycle in range(6):
        if cycle % 2 == 0:
            positions = [1000] * NUM_JOINTS
            torques = [50] * NUM_JOINTS
        else:
            positions = [500] * NUM_JOINTS
            torques = [150] * NUM_JOINTS

        print(f"[Cycle {cycle}] mix_control_by_pt pos[0]={positions[0]} force[0]={torques[0]} (0.01N)")
        hand.mix_control_by_pt(positions, torques)
        time.sleep(1.5)

    print("\n[Done]: Mix control demo completed!")

if __name__ == "__main__":
    main()