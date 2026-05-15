# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 mix control demo (position + torque).

Demonstrates POSITION_TORQUE mode via mix_ctrl_joint_motor(),
alternating between two sets of position/torque parameters.

Supports multiple connection types: ZLG CANFD, HCAN, RS485, ZLG TCP, USB CDC serial, SocketCAN, TJ.
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand2025, HandType, MixCtrl, ControlMode

EXAMPLES = """\
examples:
  # ZLG CANFD, right hand
  python demo_mix_ctrl_pos_torque.py -d zlgcan -t right --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_mix_ctrl_pos_torque.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_mix_ctrl_pos_torque.py -d socketcan --can-interface can0

  # RS485, specify serial port (e.g. COM3 or /dev/ttyUSB0)
  python demo_mix_ctrl_pos_torque.py -d rs485 --uart-port COM3

  # USB CDC serial (e.g. COM3 or /dev/ttyACM0)
  python demo_mix_ctrl_pos_torque.py -d usb --uart-port COM3

  # TJ Marvin controller
  python demo_mix_ctrl_pos_torque.py -d tj --tj-ip 192.168.10.190
"""

def main():
    parser = argparse.ArgumentParser(
        description='OmniHand 2025 Mix Control Demo (Position + Torque)',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp', 'rs485', 'usb', 'tj'], default='zlgcan',
                        help='Device type: zlgcan, hcan, zlgcan_tcp, rs485, usb (CDC serial), socketcan (Linux only), tj, default: zlgcan')
    parser.add_argument('-t', '--hand-type', choices=['left', 'right'], default='right',
                        help='Hand type: left or right, default: right')
    parser.add_argument('--hand-id', type=int, default=None,
                        help='Hand device ID, default: kDefaultHandDeviceId')
    parser.add_argument('--canfd-device-id', type=int, default=0,
                        help='CANFD device index, default: 0')
    parser.add_argument('--canfd-channel-id', type=int, default=0,
                        help='CANFD channel index, default: 0')
    parser.add_argument('--can-interface', type=str, default='can0',
                        help='SocketCAN interface (Linux only), default: can0')
    parser.add_argument('--uart-port', type=str, default='COM6',
                        help='Serial port for rs485/usb mode, default: COM6')
    parser.add_argument('--host', type=str, default='192.168.0.178',
                        help='ZLG CAN TCP host address, default: 192.168.0.178')
    parser.add_argument('--port', type=int, default=8000,
                        help='ZLG CAN TCP port, default: 8000')
    parser.add_argument('--tj-ip', type=str, default='192.168.10.190',
                        help='TJ Marvin controller IP, default: 192.168.10.190')
    args = parser.parse_args()

    hand_type = HandType.LEFT if args.hand_type == 'left' else HandType.RIGHT
    hand_device_id = args.hand_id if args.hand_id is not None else OmniHand2025.kDefaultHandDeviceId

    print("=" * 60)
    print("OmniHand 2025 Mix Control Demo - POSITION_TORQUE")
    print("=" * 60)

    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(
            hand_type=hand_type,
            uart_port=args.uart_port
        )
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=hand_type,
            host=args.host,
            port=args.port
        )
    elif args.device == 'usb':
        hand = OmniHand2025.create_hand_by_usb(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            uart_port=args.uart_port
        )
    elif args.device == 'socketcan':
        hand = OmniHand2025.create_hand_socketcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            can_interface=args.can_interface
        )
    elif args.device == 'tj':
        hand = OmniHand2025.create_hand_by_tj(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            marvin_controller_ip=args.tj_ip
        )
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )

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
