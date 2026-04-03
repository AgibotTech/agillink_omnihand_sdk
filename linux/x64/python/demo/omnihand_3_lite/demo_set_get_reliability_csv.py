#!/usr/bin/env python
# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) Set + Get Position Reliability Test with CSV Logging

Loop test for O4 (4 DOF):
1. set_all_joint_positions() for 4 joints
2. get_all_joint_positions()
3. Log both target and reply positions with timestamps to CSV.

Usage:
  python demo_set_get_reliability_csv_o4.py -i 3 -n 1000 -o o4_set_get_reliability.csv
  python demo_set_get_reliability_csv_o4.py --interval_ms 3 --iterations 5000 --output log_o4.csv -d zlgcan
"""

from omnihand import OmniHand3Lite, HandType
import time
import csv
import argparse


def main():
    parser = argparse.ArgumentParser(
        description="OmniHand 3 Lite S (O4) Set+Get reliability test (set position + get position), log results to CSV"
    )
    parser.add_argument(
        "-d",
        "--device",
        choices=["zlgcan", "hcan"],
        default="zlgcan",
        help="CAN device type (default: zlgcan)",
    )
    parser.add_argument(
        "-i",
        "--interval_ms",
        type=int,
        default=4,
        help="Request interval in ms (default: 3)",
    )
    parser.add_argument(
        "-n",
        "--iterations",
        type=int,
        default=4096,
        help="Number of set+get iterations (default: 1000)",
    )
    parser.add_argument(
        "-o",
        "--output",
        type=str,
        default="o4_set_get_reliability.csv",
        help="Output CSV path (default: o4_set_get_reliability.csv)",
    )
    parser.add_argument(
        "--timeout_ms",
        type=int,
        default=30,
        help="Frame receive timeout in ms (default: 30)",
    )
    parser.add_argument(
        "--positions",
        type=str,
        default="2048,2048,2048,2048",
        help="Comma-separated target motor positions for set, 0-4096 (default: 4 joints)",
    )
    args = parser.parse_args()

    interval_ms = max(0, args.interval_ms)
    total_iterations = max(1, args.iterations)
    frame_recv_timeout_ms = max(10, min(1000, args.timeout_ms))
    target_positions = [int(x.strip()) for x in args.positions.split(",")]

    # O4 has exactly 4 joints; normalize length
    num_joints = 4
    sweep_index = 3
    # if len(target_positions) < num_joints:
    #     target_positions.extend([2048] * (num_joints - len(target_positions)))
    # target_positions = target_positions[:num_joints]

    try:
        if args.device == "hcan":
            hand = OmniHand3Lite.create_hand_by_hcan(
                hand_type=HandType.LEFT,
                hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
                canfd_device_id=0,
                canfd_channel_id=0,
            )
        else:
            hand = OmniHand3Lite.create_hand_by_zlgcan(
                hand_type=HandType.LEFT,
                hand_device_id=OmniHand3Lite.kDefaultHandDeviceId,
                canfd_device_id=0,
                canfd_channel_id=0,
            )
    except Exception as e:
        print(f"Failed to create O4 hand: {e}")
        return

    if not hand.init():
        print("Failed to initialize O4 hand")
        return

    hand.set_request_interval(interval_ms)
    hand.set_frame_recv_timeout(frame_recv_timeout_ms)

    # Sync initial positions from device once (if possible), then start test
    try:
        target_positions = hand.get_all_joint_positions()
        print(f"Initial O4 positions from device: {target_positions}")
    except Exception as e:
        print(f"Failed to get initial positions: {e}")
        return

    target_positions[sweep_index] = 0
    hand.set_all_joint_positions(target_positions)
    time.sleep(1)

    csv_header = ["action", "elapsed_ms"] + [f"pos_{i}" for i in range(num_joints)]
    # Use high-resolution monotonic clock for elapsed_ms
    start_time = time.perf_counter()

    print(
        f"Start O4 set+get reliability test: iterations={total_iterations}, "
        f"interval_ms={interval_ms}, timeout_ms={frame_recv_timeout_ms}"
    )

    try:
        with open(args.output, "w", newline="", encoding="utf-8") as f:
            writer = csv.DictWriter(f, fieldnames=csv_header)
            writer.writeheader()

            for iteration in range(total_iterations):
                try:
                    target_positions[sweep_index] = iteration % 4096

                    actual_positions = hand.set_all_joint_positions(target_positions)
                    elapsed_ms = (time.perf_counter() - start_time) * 1000.0

                    row_set = {"action": "set", "elapsed_ms": ""}
                    for i in range(num_joints):
                        row_set[f"pos_{i}"] = target_positions[i]
                    writer.writerow(row_set)

                    row_reply = {"action": "reply", "elapsed_ms": elapsed_ms}
                    if actual_positions and len(actual_positions) >= num_joints:
                        for i in range(num_joints):
                            row_reply[f"pos_{i}"] = actual_positions[i]
                    else:
                        for i in range(num_joints):
                            row_reply[f"pos_{i}"] = ""
                    writer.writerow(row_reply)
                except Exception as e:
                    # Log a failure row if needed; for now just print the first few
                    if iteration < 10:
                        print(f"[Iteration {iteration}] error: {e}")

    except KeyboardInterrupt:
        print("Interrupted by user, stopping O4 test.")


if __name__ == "__main__":
    main()

