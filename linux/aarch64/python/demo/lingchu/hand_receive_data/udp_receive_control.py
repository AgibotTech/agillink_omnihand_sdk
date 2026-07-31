# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
UDP receiver for Protobuf (RetargetInfoArray) from `hand_send_data`, calling AGILINK OmniHand SDK
`set_all_active_joint_angles()` to control the hand (O10, 10 active joints, radians).

Matches default config of `hand_send_data/test_ik_genie_msg_UDP.py`:
  - UDP port defaults to 5000
  - Each message may include left/right RetargetInfo; group_id selects side; ABS_JOINT + 10 joint angles

Install/use omnihand SDK from parent dirs (see python README), and connect CAN before use.
"""

from __future__ import annotations

import argparse
import math
import signal
import socket
import sys
from pathlib import Path
from typing import Any, List, Optional

# genie_msgs_pb in this dir (compatible with hand_send_data)
_SCRIPT_DIR = Path(__file__).resolve().parent
if str(_SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(_SCRIPT_DIR))

from genie_msgs_pb.msg import RetargetInfoArray_pb2 as RetargetInfoArray
from genie_msgs_pb.msg import RetargetInfo_pb2 as RetargetInfo

_EXPECTED_JOINTS = 10
_running = True


def _on_signal(_sig, _frame):
    global _running
    _running = False


def _normalize_joints(raw: List[float]) -> List[float]:
    """Pad/truncate to 10 floats; replace non-finite with 0."""
    out: List[float] = []
    for i in range(_EXPECTED_JOINTS):
        if i < len(raw):
            v = float(raw[i])
            out.append(0.0 if not math.isfinite(v) else v)
        else:
            out.append(0.0)
    return out


def _create_hands(
    device: str,
    dual: bool,
    single_hand: str,
    canfd_device_id: int,
    left_channel: int,
    right_channel: int,
    single_channel: int,
):
    from omnihand import OmniHand2025, HandType

    if device == "hcan":
        factory = OmniHand2025.create_hand_by_hcan
    else:
        factory = OmniHand2025.create_hand_by_zlgcan

    if dual:
        left = factory(
            canfd_device_id=canfd_device_id,
            hand_type=HandType.LEFT,
            canfd_channel_id=left_channel,
        )
        right = factory(
            canfd_device_id=canfd_device_id,
            hand_type=HandType.RIGHT,
            canfd_channel_id=right_channel,
        )
        return left, right

    ht = HandType.LEFT if single_hand == "left" else HandType.RIGHT
    hand = factory(
        canfd_device_id=canfd_device_id,
        hand_type=ht,
        canfd_channel_id=single_channel,
    )
    return (hand, None) if ht == HandType.LEFT else (None, hand)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Receive hand_send_data UDP protobuf and control OmniHand 2025"
    )
    parser.add_argument(
        "--udp-host",
        default="0.0.0.0",
        help="Bind address (default: all interfaces)",
    )
    parser.add_argument(
        "--udp-port",
        type=int,
        default=5000,
        help="UDP port (must match hand_send_data sender, default 5000)",
    )
    parser.add_argument(
        "-d",
        "--device",
        choices=["zlgcan", "hcan"],
        default="zlgcan",
        help="CAN device type (default: zlgcan)",
    )
    parser.add_argument(
        "--mode",
        choices=["dual", "left", "right"],
        default="dual",
        help="dual: left+right on two CAN channels; left/right: single hand only",
    )
    parser.add_argument(
        "--canfd-device-id",
        type=int,
        default=0,
        help="CANFD device index (default: 0)",
    )
    parser.add_argument(
        "--left-channel",
        type=int,
        default=0,
        help="Dual mode: CANFD channel for left hand (default: 0)",
    )
    parser.add_argument(
        "--right-channel",
        type=int,
        default=1,
        help="Dual mode: CANFD channel for right hand (default: 1)",
    )
    parser.add_argument(
        "--single-channel",
        type=int,
        default=0,
        help="Single-hand mode: CANFD channel (default: 0)",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Parse UDP and print angles only; do not touch hardware",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print each applied frame (can be noisy)",
    )
    args = parser.parse_args()

    dual = args.mode == "dual"

    signal.signal(signal.SIGINT, _on_signal)
    signal.signal(signal.SIGTERM, _on_signal)

    left_hand: Optional[Any] = None
    right_hand: Optional[Any] = None

    if not args.dry_run:
        single_hand = "left" if args.mode == "left" else "right"
        left_hand, right_hand = _create_hands(
            device=args.device,
            dual=dual,
            single_hand=single_hand,
            canfd_device_id=args.canfd_device_id,
            left_channel=args.left_channel,
            right_channel=args.right_channel,
            single_channel=args.single_channel,
        )
        for name, h in (("left", left_hand), ("right", right_hand)):
            if h is None:
                continue
            if not h.init():
                print(f"[Error] Failed to initialize OmniHand 2025 ({name})")
                return
        print("[OK] Hand(s) initialized. Listening for UDP...")
    else:
        print("[Dry-run] No hardware; listening for UDP...")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((args.udp_host, int(args.udp_port)))
    sock.settimeout(0.5)

    frame = 0
    print(
        f"UDP bind {args.udp_host}:{args.udp_port} | dual={dual} | device={args.device}"
    )

    while _running:
        try:
            data, addr = sock.recvfrom(65535)
        except socket.timeout:
            continue
        except OSError:
            break

        msg = RetargetInfoArray.RetargetInfoArray()
        try:
            msg.ParseFromString(data)
        except Exception as e:
            print(f"[Warn] ParseFromString failed from {addr}: {e}")
            continue

        frame += 1
        for info in msg.retarget_infos:
            if info.control_type != RetargetInfo.RetargetInfo.ABS_JOINT:
                if args.verbose:
                    print(
                        f"[Skip] non-ABS_JOINT control_type={info.control_type}"
                    )
                continue

            joints = _normalize_joints(list(info.target_joint_positions))

            if info.group_id == RetargetInfo.RetargetInfo.GROUP_LEFT_TOOL:
                if args.dry_run:
                    print(f"[Dry-run] LEFT  {joints}")
                elif left_hand is not None:
                    left_hand.set_all_active_joint_angles(joints)
                    if args.verbose:
                        print(f"[{frame}] LEFT  from={addr} joints={joints}")
                else:
                    if args.verbose:
                        print("[Skip] LEFT packet but no left hand opened")

            elif info.group_id == RetargetInfo.RetargetInfo.GROUP_RIGHT_TOOL:
                if args.dry_run:
                    print(f"[Dry-run] RIGHT {joints}")
                elif right_hand is not None:
                    right_hand.set_all_active_joint_angles(joints)
                    if args.verbose:
                        print(f"[{frame}] RIGHT from={addr} joints={joints}")
                else:
                    if args.verbose:
                        print("[Skip] RIGHT packet but no right hand opened")
            else:
                if args.verbose:
                    print(f"[Skip] unknown group_id={info.group_id}")

        if not args.verbose and frame % 200 == 0:
            print(f"... frames received: {frame} (last from {addr})")

    sock.close()
    print("Stopped.")


if __name__ == "__main__":
    main()
