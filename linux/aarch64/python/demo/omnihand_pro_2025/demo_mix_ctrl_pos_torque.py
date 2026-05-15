# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 - Mix Control Demo (Position + Torque)

Demonstrates POSITION_TORQUE mode via mix_ctrl_joint_motor(),
alternating between two sets of position/torque parameters.

Note: O12 only supports POSITION_TORQUE. VELOCITY_TORQUE and
POSITION_VELOCITY_TORQUE are defined in the protocol but not supported.

Supports multiple connection types: ZLG CANFD, HCAN, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHandPro2025, HandType, MixCtrl, ControlMode

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
    print("OmniHand Pro 2025 Mix Control Demo")
    print("  Supported mode: POSITION_TORQUE only")
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
        mix_ctrls = []
        for i in range(NUM_JOINTS):
            mc = MixCtrl()
            mc.joint_index = i + 1
            mc.ctrl_mode = int(ControlMode.POSITION_TORQUE)
            if cycle % 2 == 0:
                mc.tgt_posi = 1000
                mc.tgt_torque = 50
            else:
                mc.tgt_posi = 500
                mc.tgt_torque = 150
            mix_ctrls.append(mc)

        print(f"[Cycle {cycle}] mode=POSITION_TORQUE, pos={mc.tgt_posi}, torque={mc.tgt_torque}")

        hand.mix_ctrl_joint_motor(mix_ctrls)
        time.sleep(1.5)

    print("\n[Done]: Mix control demo completed!")

if __name__ == "__main__":
    main()