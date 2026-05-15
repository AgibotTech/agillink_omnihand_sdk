# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
Set and get motor positions for OmniHand 2025 (O10).

Supports multiple connection types: ZLG CANFD, HCAN, RS485, ZLG TCP, USB CDC serial, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand2025, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, left hand
  python demo_set_motor.py -d zlgcan -t left --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_set_motor.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_set_motor.py -d socketcan --can-interface can0

  # RS485, specify serial port (e.g. COM3 or /dev/ttyUSB0)
  python demo_set_motor.py -d rs485 --uart-port COM3

  # USB CDC serial (e.g. COM3 or /dev/ttyACM0)
  python demo_set_motor.py -d usb --uart-port COM3
"""

def main():
    parser = argparse.ArgumentParser(
        description='Set motor positions for OmniHand 2025',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp', 'rs485', 'usb'], default='zlgcan',
                        help='Device type: zlgcan, hcan, zlgcan_tcp, rs485, usb (CDC serial), socketcan (Linux only), default: zlgcan')
    parser.add_argument('-t', '--hand-type', choices=['left', 'right'], default='left',
                        help='Hand type: left or right, default: left')
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
    args = parser.parse_args()

    hand_type = HandType.LEFT if args.hand_type == 'left' else HandType.RIGHT
    hand_device_id = args.hand_id if args.hand_id is not None else OmniHand2025.kDefaultHandDeviceId

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
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )

    hand.set_joint_position(2, 200)
    time.sleep(1)

    id2_joint_posi = hand.get_joint_position(2)
    print("Joint 2 position: ", id2_joint_posi)

    init_positions = [2048,2048,4096,2048,4096,4096,2048,4096,2048,4096]
    hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_positions = hand.get_all_joint_positions()
    print("All joint positions: ", real_positions)


if __name__ == "__main__":
    main()
