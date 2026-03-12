# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 Set + Get Position Reliability Test with CSV Logging

Loop test: set_all_joint_positions(), then get_all_joint_positions().
Records timestamps and response values to CSV.

Usage:
  python demo_set_get_reliability_csv.py -i 10 -n 1000 -o log.csv
  python demo_set_get_reliability_csv.py --interval_ms 5 --iterations 500 --output set_get_log.csv -d zlgcan
"""

from omnihand import OmniHand2025, HandType
import time
import csv
import argparse
from collections import defaultdict


def main():
    parser = argparse.ArgumentParser(
        description='OmniHand 2025 Set+Get reliability test (set position + get position), log results to CSV'
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type (default: zlgcan)')
    parser.add_argument('-i', '--interval_ms', type=int, default=1,
                        help='Request interval in ms (default: 1)')
    parser.add_argument('-n', '--iterations', type=int, default=100,
                        help='Number of set+get iterations (default: 100)')
    parser.add_argument('-o', '--output', type=str, default='set_get_reliability.csv',
                        help='Output CSV path (default: set_get_reliability.csv)')
    parser.add_argument('--timeout_ms', type=int, default=30,
                        help='Frame receive timeout in ms (default: 30)')
    parser.add_argument('--positions', type=str, default='2048,2048,2048,2048,2048,2048,2048,2048,2048,2048',
                        help='Comma-separated target motor positions for set, 0-4096 (default: 2048 for 10 joints)')
    args = parser.parse_args()

    interval_ms = max(0, args.interval_ms)
    total_iterations = max(1, args.iterations)
    frame_recv_timeout_ms = max(10, min(1000, args.timeout_ms))
    target_positions = [int(x.strip()) for x in args.positions.split(',')]
    if len(target_positions) < 10:
        target_positions.extend([2048] * (10 - len(target_positions)))
    target_positions = target_positions[:10]

    print("=" * 60)
    print("OmniHand 2025 Set + Get Reliability Test (CSV)")
    print("=" * 60)
    print()

    print("Creating hand instance (Left hand, channel 0)...")
    try:
        if args.device == 'hcan':
            hand = OmniHand2025.create_hand_by_hcan(
                hand_type=HandType.LEFT,
                hand_device_id=1,
                canfd_device_id=0,
                canfd_channel_id=0,
            )
        else:
            hand = OmniHand2025.create_hand_by_zlgcan(
                hand_type=HandType.LEFT,
                hand_device_id=1,
                canfd_device_id=0,
                canfd_channel_id=0,
            )
    except Exception as e:
        print(f"Failed to create hand: {e}")
        return

    print("Initializing hand...")
    if not hand.init():
        print("Failed to initialize hand")
        return

    hand.set_request_interval(interval_ms)
    hand.set_frame_recv_timeout(frame_recv_timeout_ms)
    print("Hand initialized successfully")
    print()

    csv_header = [
        "elapsed_ms", "iteration",
        "set_pos_ok", "set_pos_duration_ms",
        "get_pos_ok", "get_pos_duration_ms",
    ]
    csv_header.extend([f"pos_{i}" for i in range(len(target_positions))])

    rows = []
    start_time = time.time()
    last_update_time = start_time
    failure_counts = defaultdict(int)

    print(f"Config: interval_ms={interval_ms}, iterations={total_iterations}, output={args.output}")
    print(f"Target positions: {target_positions}")
    print()

    try:
        for iteration in range(total_iterations):
            row = {
                "elapsed_ms": (time.time() - start_time) * 1000.0,
                "iteration": iteration,
                "set_pos_ok": 0,
                "set_pos_duration_ms": 0.0,
                "get_pos_ok": 0,
                "get_pos_duration_ms": 0.0,
            }
            for i in range(len(target_positions)):
                row[f"pos_{i}"] = ""

            # --- Set position ---
            try:
                t0 = time.perf_counter()
                hand.set_all_joint_positions(target_positions)
                row["set_pos_duration_ms"] = (time.perf_counter() - t0) * 1000.0
                row["set_pos_ok"] = 1
            except Exception as e:
                failure_counts["set_pos"] += 1
                row["set_pos_ok"] = 0
                if iteration < 5:
                    print(f"\n[Iter {iteration}] set_pos error: {e}")

            # --- Get position ---
            try:
                t0 = time.perf_counter()
                positions = hand.get_all_joint_positions()
                row["get_pos_duration_ms"] = (time.perf_counter() - t0) * 1000.0
                row["get_pos_ok"] = 1
                if positions is not None:
                    for i, v in enumerate(positions):
                        if i < len(target_positions):
                            row[f"pos_{i}"] = v
            except Exception as e:
                failure_counts["get_pos"] += 1
                row["get_pos_ok"] = 0
                if iteration < 5:
                    print(f"\n[Iter {iteration}] get_pos error: {e}")

            rows.append(row)

            current_time = time.time()
            if (iteration + 1) % 100 == 0 or current_time - last_update_time >= 1.0:
                elapsed = current_time - start_time
                pct = (iteration + 1) * 100.0 / total_iterations
                eta = (elapsed / (iteration + 1)) * (total_iterations - iteration - 1) if iteration > 0 else 0
                print(f"\rProgress: {iteration + 1}/{total_iterations} ({pct:.1f}%) | "
                      f"Elapsed: {elapsed:.1f}s | ETA: {eta:.1f}s | "
                      f"Failures: set_pos={failure_counts['set_pos']} get_pos={failure_counts['get_pos']}",
                      end='', flush=True)
                last_update_time = current_time

    except KeyboardInterrupt:
        print("\n\nInterrupted by user")

    print()
    elapsed_total = time.time() - start_time

    with open(args.output, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=csv_header)
        writer.writeheader()
        for r in rows:
            writer.writerow(r)

    print(f"Wrote {len(rows)} rows to {args.output}")
    print()
    print("Summary:")
    print(f"  Total time: {elapsed_total:.2f}s")
    print(f"  Iterations: {len(rows)}")
    print(f"  set_pos failures: {failure_counts['set_pos']}")
    print(f"  get_pos failures: {failure_counts['get_pos']}")
    if rows:
        set_ok_count = sum(1 for r in rows if r["set_pos_ok"] == 1)
        get_ok_count = sum(1 for r in rows if r["get_pos_ok"] == 1)
        print(f"  set_pos success rate: {set_ok_count}/{len(rows)} ({100.0*set_ok_count/len(rows):.2f}%)")
        print(f"  get_pos success rate: {get_ok_count}/{len(rows)} ({100.0*get_ok_count/len(rows):.2f}%)")


if __name__ == "__main__":
    main()
