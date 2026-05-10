# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 mix control demo (position + torque)

Demonstrates POSITION_TORQUE mode via mix_ctrl_joint_motor(),
alternating between two sets of position/torque parameters.
"""

import argparse
import time
from omnihand import OmniHand2025, HandType, MixCtrl, ControlMode

def main():
    parser = argparse.ArgumentParser(description='OmniHand 2025 Mix Control Demo (Position + Torque)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp', 'tj'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()

    print("=" * 60)
    print("OmniHand 2025 Mix Control Demo - POSITION_TORQUE")
    print("=" * 60)

    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.RIGHT)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(hand_type=HandType.RIGHT, uart_port='COM7')
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    elif args.device == 'tj':
        hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.LEFT, marvin_controller_ip="192.168.10.190")
    else:
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT)

    if not hand.init():
        print("[Error]: Failed to initialize OmniHand 2025 hand!")
        return

    hand.show_data_details(True)
    print("[OK]: OmniHand 2025 hand initialized successfully!\n")

    NUM_JOINTS = 10

    for cycle in range(6):
        mix_ctrls = []
        for i in range(NUM_JOINTS):
            mc = MixCtrl()
            mc.joint_index = i + 1
            mc.ctrl_mode = int(ControlMode.POSITION_TORQUE)
            if cycle % 2 == 0:
                mc.tgt_posi = 2000
                mc.tgt_torque = 50
            else:
                mc.tgt_posi = 1000
                mc.tgt_torque = 150
            mix_ctrls.append(mc)

        print(f"[Cycle {cycle}] mode=POSITION_TORQUE, pos={mc.tgt_posi}, torque={mc.tgt_torque}")

        hand.mix_ctrl_joint_motor(mix_ctrls)
        time.sleep(1.5)

    print("\n[Done]: Mix control demo completed!")

if __name__ == "__main__":
    main()