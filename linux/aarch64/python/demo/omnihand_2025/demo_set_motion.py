# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
Interactive gesture demo for OmniHand 2025 (O10).

Supports multiple connection types: ZLG CANFD, HCAN, RS485, ZLG TCP, USB CDC serial,
SocketCAN (Linux only), TJ Marvin. Run with -h or --help for options and examples.
"""

import argparse
import time

from omnihand import OmniHand2025, HandType, OmniHand2025Gesture

EXAMPLES = """\
examples:
  # ZLG CANFD, right hand
  python demo_set_motion.py -d zlgcan -t right --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_set_motion.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_set_motion.py -d socketcan --can-interface can0

  # RS485, specify serial port (e.g. COM3 or /dev/ttyUSB0)
  python demo_set_motion.py -d rs485 --uart-port COM3

  # ZLG TCP
  python demo_set_motion.py -d zlgcan_tcp --host 192.168.0.178 --port 8000

  # USB CDC serial (e.g. COM3 or /dev/ttyACM0)
  python demo_set_motion.py -d usb --uart-port COM3

  # TJ Marvin controller
  python demo_set_motion.py -d tj --tj-ip 192.168.10.190
"""

# Menu index matches OmniHand2025Gesture numeric value (0=ALL_ZERO … 17=CLASPING)
GESTURE_MENU = (
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ALL_ZERO, "All zero"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_PAPER, "Open palm"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST1, "Fist style 1"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST2, "Fist style 2"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_OK, "OK gesture"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ONE_HANDED_FINGER_HEART, "One-hand heart"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_LIKE, "Thumbs up"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ILY, "ILY gesture"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM1, "Digit 1"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM2, "Digit 2"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM3, "Digit 3"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM4, "Digit 4"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM6, "Digit 6"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM8, "Digit 8"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART1, "Two-hand heart 1"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART2, "Two-hand heart 2"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART3, "Two-hand heart 3"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_CLASPING, "Prayer hands"),
)


def create_hand(args):
    hand_type = HandType.LEFT if args.hand_type == 'left' else HandType.RIGHT
    hand_device_id = args.hand_id if args.hand_id is not None else OmniHand2025.kDefaultHandDeviceId

    if args.device == 'hcan':
        return OmniHand2025.create_hand_by_hcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id,
        )
    if args.device == 'rs485':
        return OmniHand2025.create_hand_by_rs485(
            hand_type=hand_type,
            uart_port=args.uart_port,
        )
    if args.device == 'zlgcan_tcp':
        return OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=hand_type,
            host=args.host,
            port=args.port,
        )
    if args.device == 'usb':
        return OmniHand2025.create_hand_by_usb(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            uart_port=args.uart_port,
        )
    if args.device == 'socketcan':
        return OmniHand2025.create_hand_socketcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            can_interface=args.can_interface,
        )
    if args.device == 'tj':
        return OmniHand2025.create_hand_by_tj(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            marvin_controller_ip=args.tj_ip,
        )
    return OmniHand2025.create_hand_by_zlgcan(
        hand_type=hand_type,
        hand_device_id=hand_device_id,
        canfd_device_id=args.canfd_device_id,
        canfd_channel_id=args.canfd_channel_id,
    )


def print_menu():
    print("\n=== OmniHand gesture menu (index = OmniHand2025Gesture value) ===")
    for gesture, label in GESTURE_MENU:
        print(f"{int(gesture)}. {label}")
    print("99. Exit")
    print("Enter gesture index: ")


def main():
    parser = argparse.ArgumentParser(
        description='OmniHand 2025 interactive gesture demo',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        '-d', '--device',
        choices=['zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp', 'rs485', 'usb', 'tj'],
        default='zlgcan',
        help='Device type: zlgcan, hcan, zlgcan_tcp, rs485, usb (CDC serial), socketcan (Linux only), tj, default: zlgcan',
    )
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

    hand = create_hand(args)
    if hand is None:
        print("Failed to create hand instance")
        return 1

    if args.device == 'zlgcan_tcp':
        hand.set_frame_recv_timeout(200)

    if not hand.init():
        print("Failed to initialize hand")
        return 1

    while True:
        print_menu()
        try:
            choice = int(input())

            if choice == 99:
                print("Exiting")
                break

            try:
                gesture = OmniHand2025Gesture(choice)
            except ValueError:
                print("Invalid input; enter a gesture index 0–17, or 99 to exit")
                continue

            print(f"\nRunning gesture: {gesture.name}")
            hand.set_hand_gesture(gesture)
            time.sleep(1)

            real_positions = hand.get_all_active_joint_angles()
            print("Current joint angles: ", real_positions)

        except ValueError:
            print("Invalid input; enter a valid number")
        except Exception as e:
            print(f"Error: {e}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
