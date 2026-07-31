# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
UDP receives Protobuf (RetargetInfoArray) from `hand_send_data`, controlling hand only via
`OmniHand2025.create_hand_by_zlgcan_tcp()` (O10, 10 active joints, radians).

For ZLG CANFD over TCP (e.g. USBCANFD-100U Ethernet, WiFi-to-CAN).

Python binding args: hand_type, hand_device_id, host, port, canfd_channel_id.
Dual-hand on single-channel box usually needs **two TCP endpoints** (--left-tcp-* / --right-tcp-*).
"""

from __future__ import annotations

import argparse
import math
import signal
import socket
import sys
from pathlib import Path
from typing import Any, List, Optional, Tuple

from omnihand import OmniHand2025

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


def _tcp_endpoint(
    args: argparse.Namespace,
    side: str,
) -> Tuple[str, int, int, int]:
    """
    One TCP side: host, port, hand_device_id, canfd_channel_id.
    Falls back to --tcp-host / --tcp-port if omitted.
    """
    if side == "left":
        host = args.left_tcp_host or args.tcp_host
        port = args.left_tcp_port if args.left_tcp_port is not None else args.tcp_port
        dev_id = args.left_hand_device_id
        ch = args.left_canfd_channel
    else:
        host = args.right_tcp_host or args.tcp_host
        port = args.right_tcp_port if args.right_tcp_port is not None else args.tcp_port
        dev_id = args.right_hand_device_id
        ch = args.right_canfd_channel
    return host, int(port), int(dev_id), int(ch)


def _create_hands_tcp(
    args: argparse.Namespace,
) -> Tuple[Optional[Any], Optional[Any]]:
    from omnihand import OmniHand2025, HandType

    mode = args.mode
    dual = mode == "dual"

    if dual:
        lh, lp, ldev, lch = _tcp_endpoint(args, "left")
        rh, rp, rdev, rch = _tcp_endpoint(args, "right")
        if (lh, lp) == (rh, rp):
            print(
                "[Warn] left and right share one TCP endpoint "
f"{lh}:{lp}. Single-channel 100U usually needs two boxes/addresses/ports;"
"If only one CAN path, use --mode left or --mode right, "
"or set --left-tcp-host/--right-tcp-host separately."
            )
        left = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=HandType.LEFT,
            hand_device_id=ldev,
            host=lh,
            port=lp,
            canfd_channel_id=lch,
        )
        right = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=HandType.RIGHT,
            hand_device_id=rdev,
            host=rh,
            port=rp,
            canfd_channel_id=rch,
        )
        return left, right

    if mode == "left":
        h, p, dev, ch = _tcp_endpoint(args, "left")
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=HandType.LEFT,
            hand_device_id=dev,
            host=h,
            port=p,
            canfd_channel_id=ch,
        )
        return hand, None

    h, p, dev, ch = _tcp_endpoint(args, "right")
    hand = OmniHand2025.create_hand_by_zlgcan_tcp(
        hand_type=HandType.RIGHT,
        hand_device_id=dev,
        host=h,
        port=p,
        canfd_channel_id=ch,
    )
    return None, hand


def _apply_tcp_timeouts(left_hand: Optional[Any], right_hand: Optional[Any]) -> None:
    for h in (left_hand, right_hand):
        if h is None:
            continue
        if hasattr(h, "set_frame_recv_timeout"):
            try:
                h.set_frame_recv_timeout(200)
            except Exception:
                pass


def main() -> None:
    parser = argparse.ArgumentParser(
        description="UDP RetargetInfo -> OmniHand 2025 (ZLG CAN over TCP only)"
    )
    parser.add_argument(
        "--udp-host",
        default="0.0.0.0",
        help="UDP bind address",
    )
    parser.add_argument(
        "--udp-port",
        type=int,
        default=5000,
        help="UDP port (match hand_send_data, default 5000)",
    )
    parser.add_argument(
        "--mode",
        choices=["dual", "left", "right"],
        default="dual",
        help="dual: both hands; left/right: one hand only",
    )
    parser.add_argument(
        "--tcp-host",
        default="192.168.0.178",
        help="default TCP server IP",
    )
    parser.add_argument(
        "--tcp-port",
        type=int,
        default=8000,
        help="default TCP server port",
    )
    parser.add_argument(
        "--left-tcp-host",
        default=None,
        help="left TCP IP (defaults to --tcp-host)",
    )
    parser.add_argument(
        "--left-tcp-port",
        type=int,
        default=None,
        help="left TCP port (defaults to --tcp-port)",
    )
    parser.add_argument(
        "--right-tcp-host",
        default=None,
        help="right TCP IP (defaults to --tcp-host)",
    )
    parser.add_argument(
        "--right-tcp-port",
        type=int,
        default=None,
        help="right TCP port (defaults to --tcp-port)",
    )
    parser.add_argument(
        "--left-hand-device-id",
        type=int,
        default=OmniHand2025.kDefaultHandDeviceId,
        help="left hand_device_id (default OmniHand2025.kDefaultHandDeviceId)",
    )
    parser.add_argument(
        "--right-hand-device-id",
        type=int,
        default=OmniHand2025.kDefaultHandDeviceId,
        help="right hand_device_id (default OmniHand2025.kDefaultHandDeviceId)",
    )
    parser.add_argument(
        "--left-canfd-channel",
        type=int,
        default=0,
        help="left canfd_channel_id (0 for single channel)",
    )
    parser.add_argument(
        "--right-canfd-channel",
        type=int,
        default=0,
        help="right canfd_channel_id",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="parse UDP only, no hardware",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="print joint command each frame",
    )
    args = parser.parse_args()

    signal.signal(signal.SIGINT, _on_signal)
    signal.signal(signal.SIGTERM, _on_signal)

    left_hand: Optional[Any] = None
    right_hand: Optional[Any] = None

    if not args.dry_run:
        left_hand, right_hand = _create_hands_tcp(args)
        _apply_tcp_timeouts(left_hand, right_hand)

        for name, h in (("left", left_hand), ("right", right_hand)):
            if h is None:
                continue
            if not h.init():
                print(f"[Error] Failed to initialize OmniHand 2025 ({name})")
                return
        print(
            f"[OK] zlgcan_tcp mode={args.mode} | "
            f"UDP {args.udp_host}:{args.udp_port}"
        )
    else:
        print("[Dry-run] No hardware; listening for UDP...")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((args.udp_host, int(args.udp_port)))
    sock.settimeout(0.5)

    frame = 0
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
                elif args.verbose:
                    print("[Skip] LEFT packet but no left hand instance")

            elif info.group_id == RetargetInfo.RetargetInfo.GROUP_RIGHT_TOOL:
                if args.dry_run:
                    print(f"[Dry-run] RIGHT {joints}")
                elif right_hand is not None:
                    right_hand.set_all_active_joint_angles(joints)
                    if args.verbose:
                        print(f"[{frame}] RIGHT from={addr} joints={joints}")
                elif args.verbose:
                    print("[Skip] RIGHT packet but no right hand instance")
            elif args.verbose:
                print(f"[Skip] unknown group_id={info.group_id}")

        if not args.verbose and frame % 200 == 0:
            print(f"... frames received: {frame} (last from {addr})")

    sock.close()
    print("Stopped.")


if __name__ == "__main__":
    main()
