# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) demo - ZLG CAN over TCP (e.g. WiFi/Ethernet adapter as server).

Connect to ZLG WiFi-to-CANFD device as TCP client. Default: 192.168.0.178:8000.

Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand3Lite, HandType, OmniHand3LiteGesture

EXAMPLES = """\
examples:
  # Default server (192.168.0.178:8000), right hand
  python demo_zlgcan_tcp.py

  # Custom server, left hand
  python demo_zlgcan_tcp.py --host 192.168.1.100 --port 8000 --hand left
"""


def main():
    parser = argparse.ArgumentParser(
        description="OmniHand 3 Lite S (O4) via ZLG CAN over TCP (WiFi/Ethernet adapter as server)",
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--host",
        default="192.168.0.178",
        help="TCP server IP (default: 192.168.0.178)",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=8000,
        help="TCP server port (default: 8000)",
    )
    parser.add_argument(
        "--hand",
        choices=["left", "right"],
        default="right",
        help="Hand type: left or right (default: right)",
    )
    parser.add_argument(
        "--hand_device_id",
        type=int,
        default=OmniHand3Lite.kDefaultHandDeviceId,
        help="Hand device ID (default: OmniHand3Lite.kDefaultHandDeviceId)",
    )
    parser.add_argument(
        "--canfd_channel_id",
        type=int,
        default=0,
        help="CAN channel id (default: 0)",
    )
    args = parser.parse_args()

    hand_type = HandType.RIGHT if args.hand == "right" else HandType.LEFT

    print("============================================")
    print("OmniHand 3 Lite S (O4) - ZLG CAN over TCP")
    print(f"Server: {args.host}:{args.port}")
    print(f"Hand: {args.hand}, device_id={args.hand_device_id}")
    print("============================================")

    hand = OmniHand3Lite.create_hand_by_zlgcan_tcp(
        hand_type=hand_type,
        hand_device_id=args.hand_device_id,
        host=args.host,
        port=args.port,
        canfd_channel_id=args.canfd_channel_id,
    )

    if hand is None:
        print("[Error]: Failed to create hand (check TCP connection)")
        return 1

    # TCP + gateway + CAN is slower than USB; increase timeout to avoid request timeout
    hand.set_frame_recv_timeout(500)

    if not hand.init():
        print("[Error]: Failed to initialize hand")
        return 1

    print("[OK]: Hand initialized via ZLG CAN TCP\n")

    # Vendor info
    vendor_info = hand.get_vendor_info()
    print("Vendor Info:")
    print(f"  Model: {vendor_info.product_model}")
    print(f"  Serial: {vendor_info.product_seq_num}")
    print(f"  DOF: {vendor_info.dof}\n")

    # Device info
    device_info = hand.get_device_info()
    print(f"Device Info: hand_device_id={device_info.hand_device_id}\n")

    # Step 0: Test gesture control
    print("Step 0: Testing gesture control...")
    print("  Setting gesture: FIST...")
    hand.set_hand_gesture(OmniHand3LiteGesture.OMNI_HAND_3_LITE_GESTURE_FIST)
    time.sleep(2)
    fist_positions = hand.get_all_joint_positions()
    print(f"  FIST positions: {fist_positions}")

    print("  Setting gesture: OPEN...")
    hand.set_hand_gesture(OmniHand3LiteGesture.OMNI_HAND_3_LITE_GESTURE_OPEN)
    time.sleep(2)
    open_positions = hand.get_all_joint_positions()
    print(f"  OPEN positions: {open_positions}\n")

    # Step 1: Set all joints to middle position (2048)
    print("Step 1: Setting all joints to middle position...")
    init_positions = [2048, 2048, 2048, 2048]  # O4 has 4 DOF
    actual_positions = hand.set_all_joint_positions(init_positions)
    time.sleep(2)
    print(f"Set positions: {actual_positions}")

    # Step 2: Get all joint positions
    print("\nStep 2: Getting all joint positions...")
    real_positions = hand.get_all_joint_positions()
    print(f"Current positions: {real_positions}")
    print(f"Number of joints: {len(real_positions)} (should be 4 for O4)")

    # Step 3: Set individual joint positions
    print("\nStep 3: Setting individual joint positions...")
    hand.set_joint_position(4, 0)
    time.sleep(1)
    hand.set_joint_position(3, 4095)
    time.sleep(1)
    hand.set_joint_position(2, 4095)
    time.sleep(1)
    hand.set_joint_position(1, 4095)
    time.sleep(1)
    
    # Get updated positions
    real_positions = hand.get_all_joint_positions()
    print(f"Updated positions: {real_positions}")

    print("\n[Done]: ZLG CAN TCP demo completed.")
    return 0


if __name__ == "__main__":
    exit(main())
