# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 demo - ZLG CAN over TCP (e.g. WiFi/Ethernet adapter as server).

Connect to ZLG WiFi转CANFD device as TCP client. Default: 192.168.0.178:8000.

Usage:
    python demo_zlgcan_tcp.py
    python demo_zlgcan_tcp.py --host 192.168.0.178 --port 8000
    python demo_zlgcan_tcp.py --hand right
"""

import argparse
import time
from omnihand import OmniHand2025, HandType


def main():
    parser = argparse.ArgumentParser(
        description="OmniHand 2025 via ZLG CAN over TCP (WiFi/Ethernet adapter as server)"
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
        default=OmniHand2025.kDefaultHandDeviceId,
        help="Hand device ID (default: OmniHand2025.kDefaultHandDeviceId)",
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
    print("OmniHand 2025 - ZLG CAN over TCP")
    print(f"Server: {args.host}:{args.port}")
    print(f"Hand: {args.hand}, device_id={args.hand_device_id}")
    print("============================================")

    hand = OmniHand2025.create_hand_by_zlgcan_tcp(
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
    hand.set_frame_recv_timeout(200)

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

        # Step 1: 左右手先握拳
    print("Step 1: Making hand into fist position...")
    # 使用新的 SetHandGesture 接口（solver会自动处理左右手差异）
    hand.set_hand_gesture(2)  # FIST2
    time.sleep(2)  # 等待握拳动作完成
    print("Fist position set for hand")

        # Step 2: 摊开手掌
    print("\nStep 2: Opening hand (reset position)...")
    reset_angles = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # 重置位置（摊开）
    hand.set_all_active_joint_angles(reset_angles)
    time.sleep(2)  # 等待摊开动作完成
    print("Hands opened (reset position)")

    # Step 3: 做目前的小拇指动作
    print("\nStep 3: Setting pinky positions...")
    hand.set_joint_position(10, 200) 
    time.sleep(1)

    # 读取所有关节位置
    real_positions = hand.get_all_joint_positions()
    print("All joint positions of hand: ", real_positions)

    print("\n[Done]: ZLG CAN TCP demo completed.")
    return 0


if __name__ == "__main__":
    exit(main())
