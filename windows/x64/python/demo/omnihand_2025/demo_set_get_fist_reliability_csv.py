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

from omnihand import OmniHand2025, HandType
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
    parser.add_argument('-o', '--output', type=str, default='same_set_get_reliability.csv',
                        help='Output CSV path (default: same_set_get_reliability.csv)')
    parser.add_argument('--timeout_ms', type=int, default=30,
                        help='Frame receive timeout in ms (default: 30)')
    parser.add_argument('--positions', type=str, default='2048,0,0,0,0,0,0,0,0,4095',
                        help='Comma-separated target motor positions for set, 0-4096 (default: 0 for 10 joints)')
    args = parser.parse_args()

    interval_ms = max(0, args.interval_ms)
    total_iterations = max(1, args.iterations)
    frame_recv_timeout_ms = max(10, min(1000, args.timeout_ms))
    target_positions = [int(x.strip()) for x in args.positions.split(',')]
    # if len(target_positions) < 10:
    #     target_positions.extend([2048] * (10 - len(target_positions)))
    # target_positions = target_positions[:10]

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
    # hand.show_data_details(True)
    open_target_positions = [4087, 18, 3963, 2026, 4094, 4094, 2072, 4094, 39, 4093]
    fist_target_positions = [4087, 4085, 1634, 2040, 3, 3, 2035, 2, 9, 3]
    hand.set_all_joint_positions(open_target_positions)
    time.sleep(2)
    init_positions = hand.get_all_joint_positions()
    print("init_positions:", init_positions)
    # time.sleep(1)

    csv_header = ["action", "elapsed_ms"] + [f"pos_{i}" for i in range(10)]
    # Monotonic clock avoids wall-clock skew; elapsed_ms may repeat but reflects real timing
    # start_time = time.perf_counter()

    try:
        with open(args.output, 'w', newline='', encoding='utf-8') as f:
            writer = csv.DictWriter(f, fieldnames=csv_header)
            writer.writeheader()
            start_time = time.perf_counter()
            for iteration in range(total_iterations):
                # --- Set position ---
                try:
                    acutual_fist_positions = hand.set_all_joint_positions(fist_target_positions)
                    elapsed_ms = (time.perf_counter() - start_time) * 1000.0
                    row_send = {"action": "set", "elapsed_ms": ""}
                    for i in range(10):
                        row_send[f"pos_{i}"] = fist_target_positions[i] if i < len(fist_target_positions) else ""
                    writer.writerow(row_send)
                    row_reply = {"action": "reply", "elapsed_ms": elapsed_ms}
                    for i in range(10):
                        row_reply[f"pos_{i}"] = acutual_fist_positions[i] if acutual_fist_positions and i < len(acutual_fist_positions) else ""
                    writer.writerow(row_reply)
                except Exception:
                    pass

            row_send = {"action": "rest", "elapsed_ms": ""   }
            writer.writerow(row_send)
            start_time = time.perf_counter()
            for iteration in range(total_iterations):
                # --- Set position ---
                try:
                    # actual_positions = hand.set_all_joint_positions(target_positions)
                    acutual_open_positions = hand.set_all_joint_positions(open_target_positions)
                    elapsed_ms = (time.perf_counter() - start_time) * 1000.0
                    row_send = {"action": "set", "elapsed_ms": ""}
                    for i in range(10):
                        row_send[f"pos_{i}"] = open_target_positions[i] if i < len(open_target_positions) else ""
                    writer.writerow(row_send)
                    row_reply = {"action": "reply", "elapsed_ms": elapsed_ms}
                    for i in range(10):
                        row_reply[f"pos_{i}"] = acutual_open_positions[i] if acutual_open_positions and i < len(acutual_open_positions) else ""
                    writer.writerow(row_reply)
                except Exception:
                    pass

    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
