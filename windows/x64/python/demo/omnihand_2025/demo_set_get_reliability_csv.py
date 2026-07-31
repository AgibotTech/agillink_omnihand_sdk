# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 Set + Get Position Reliability Test with CSV Logging

Loop test: set_all_joint_positions(), then get_all_joint_positions().
Records timestamps and response values to CSV.

Usage:
  python demo_set_get_reliability_csv.py -i 10 -n 1000 -o log.csv
  python demo_set_get_reliability_csv.py --interval_ms 5 --iterations 500 --output set_get_log.csv -d zlgcan
"""

from omnihand import OmniHand2025, HandType, OmniHand2025Gesture
import time
import csv
import argparse


def main():
    parser = argparse.ArgumentParser(
        description='OmniHand 2025 Set+Get reliability test (set position + get position), log results to CSV'
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp'], default='zlgcan',
                        help='CAN device type (default: zlgcan)')
    parser.add_argument('-i', '--interval_ms', type=int, default=0,
                        help='Request interval in ms (default: 10)')
    parser.add_argument('-n', '--iterations', type=int, default=1000,
                        help='Number of set+get iterations (default: 1000)')
    parser.add_argument('-o', '--output', type=str, default='set_get_reliability.csv',
                        help='Output CSV path (default: set_get_reliability.csv)')
    parser.add_argument('--timeout_ms', type=int, default=100,
                        help='Frame receive timeout in ms (default: 100; TJ recommended >= 200)')
    parser.add_argument('--positions', type=str, default='2048,0,0,0,0,0,0,0,0,4095',
                        help='Comma-separated target motor positions for set, 0-4096 (default: 0 for 10 joints)')
    args = parser.parse_args()

    interval_ms = max(0, args.interval_ms)
    total_iterations = max(1, args.iterations)
    frame_recv_timeout_ms = max(10, min(1000, args.timeout_ms))

    raw_positions = [int(x.strip()) for x in args.positions.split(',') if x.strip() != ""]
    # O10 fixed 10 joints: pad short lists, truncate long
    base_positions = (raw_positions + [2048] * 10)[:10]

    try:
        if args.device == 'hcan':
            hand = OmniHand2025.create_hand_by_hcan(
                hand_type=HandType.LEFT,
                hand_device_id=OmniHand2025.kDefaultHandDeviceId,
                canfd_device_id=0,
                canfd_channel_id=0,
            )
        elif args.device == 'rs485':
            hand = OmniHand2025.create_hand_by_rs485(
                hand_type=HandType.RIGHT,
                uart_port='/dev/ttyACM0'
            )
        elif args.device == 'zlgcan_tcp':
            hand = OmniHand2025.create_hand_by_zlgcan_tcp(
                hand_type=HandType.RIGHT,
                host='192.168.0.178', 
                port=8000
            )
        else:
            hand = OmniHand2025.create_hand_by_zlgcan(
                hand_type=HandType.LEFT,
                hand_device_id=OmniHand2025.kDefaultHandDeviceId,
                canfd_device_id=0,
                canfd_channel_id=0,
            )
    except Exception as e:
        print(f"Failed to create hand: {e}")
        return

    if not hand.init():
        print("Failed to initialize hand")
        return

    hand.set_request_interval(interval_ms)
    hand.set_frame_recv_timeout(frame_recv_timeout_ms)
    hand.set_hand_gesture(OmniHand2025Gesture.OMNIHAND_2025_GESTURE_PAPER)
    time.sleep(1)
    read_back_positions = hand.get_all_joint_positions()
    if read_back_positions is not None and len(read_back_positions) >= 10:
        target_positions = [int(x) for x in read_back_positions[:10]]
    else:
        target_positions = list(base_positions)
    time.sleep(1)

    csv_header = ["action", "elapsed_ms"] + [f"pos_{i}" for i in range(10)]
    # Monotonic clock avoids wall-clock skew; elapsed_ms may repeat but reflects real timing
    start_time = time.perf_counter()

    try:
        with open(args.output, 'w', newline='', encoding='utf-8') as f:
            writer = csv.DictWriter(f, fieldnames=csv_header)
            writer.writeheader()
            for iteration in range(total_iterations):
                # --- Set position ---
                try:
                    # Normalize length each round to avoid index errors on short/empty lists
                    target_positions = (list(target_positions) + [2048] * 10)[:10]
                    target_positions[9] = iteration % 4096

                    actual_positions = hand.set_all_joint_positions(target_positions)
                    elapsed_ms = (time.perf_counter() - start_time) * 1000.0
                    row_send = {"action": "set", "elapsed_ms": ""}
                    for i in range(10):
                        row_send[f"pos_{i}"] = target_positions[i] if i < len(target_positions) else ""
                    writer.writerow(row_send)
                    row_reply = {"action": "reply", "elapsed_ms": elapsed_ms}
                    for i in range(10):
                        row_reply[f"pos_{i}"] = actual_positions[i] if actual_positions and i < len(actual_positions) else ""
                    writer.writerow(row_reply)
                except Exception as ex:
                    print(f"Failed to set position: {ex}")

                # # --- Get position ---
                # try:
                #     positions = hand.get_all_joint_positions()
                #     if positions is not None:
                #         t_ms = (time.time() - start_time) * 1000.0
                #         writer.writerow({"elapsed_ms": t_ms, "action": "get"})
                # except Exception:
                #     pass

    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
