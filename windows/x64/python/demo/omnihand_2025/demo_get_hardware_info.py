# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
Get hardware info (VendorInfo and DeviceInfo) from OmniHand 2025 (O10).

Supports multiple connection types: ZLG CANFD, HCAN, RS485, ZLG TCP, USB CDC serial, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
from omnihand import OmniHand2025, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, right hand
  python demo_get_hardware_info.py -d zlgcan -t right --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_get_hardware_info.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_get_hardware_info.py -d socketcan --can-interface can0

  # RS485, specify serial port (e.g. COM3 or /dev/ttyUSB0)
  python demo_get_hardware_info.py -d rs485 --uart-port COM3

  # ZLG TCP
  python demo_get_hardware_info.py -d zlgcan_tcp --host 192.168.0.178 --port 8000

  # USB CDC serial (e.g. COM3 or /dev/ttyACM0)
  python demo_get_hardware_info.py -d usb --uart-port COM3

  # TJ Marvin controller
  python demo_get_hardware_info.py -d tj --tj-ip 192.168.10.190
"""

def main():
    parser = argparse.ArgumentParser(
        description='Get hardware info from OmniHand 2025',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp', 'rs485', 'usb', 'tj'], default='zlgcan',
                        help='Device type: zlgcan, hcan, zlgcan_tcp, rs485, usb (CDC serial), socketcan (Linux only), tj, default: zlgcan')
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
    parser.add_argument('--tj-ip', type=str, default='192.168.10.190',
                        help='TJ Marvin controller IP, default: 192.168.10.190')
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

    hand.show_data_details(True)

    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(vendor_info)

    device_info = hand.get_device_info()
    print("Device Info:")
    print(device_info)


if __name__ == "__main__":
    main()
