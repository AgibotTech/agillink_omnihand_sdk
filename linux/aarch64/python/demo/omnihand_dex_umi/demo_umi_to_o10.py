#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
UMI to O10 control demo

This demo reads positions from UMI and applies them directly to O10.

Flow:
1. Read motor positions from UMI (0-4096)
2. Apply same values to O10 (no conversion)

Run:
    python3 demo_umi_to_o10.py [left|right]
    
Arguments:
    left  - left hand (default)
    right - right hand

Notes:
    - UMI is read-only; only positions
    - O10 must support position control
    - Match left/right between UMI and O10
    - Same motor position range 0-4096 on both
"""

import sys
import time
import argparse
from omnihand import OmniHandDexUMI, OmniHand2025, HandType


def print_usage(program_name):
    """Print usage help."""
    print(f"Usage: {program_name} [left|right]")
    print("  left  - Control left hand (default)")
    print("  right - Control right hand")
    print()
    print("Note: UMI device is read-only, O10 device needs position control support")
    print("Note: UMI and O10 use the same motor position range (0-4096), direct transfer is supported")


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description='UMI to O10 control demo',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument(
        'hand',
        nargs='?',
        choices=['left', 'right'],
        default='left',
        help='Hand type: left or right (default: left)'
    )
    parser.add_argument(
        '--umi-device-id',
        type=int,
        default=OmniHandDexUMI.kDefaultHandDeviceId,
        help='UMI device ID (default: OmniHandDexUMI.kDefaultHandDeviceId)'
    )
    parser.add_argument(
        '--umi-canfd-id',
        type=int,
        default=0,
        help='UMI CANFD device ID (default: 0)'
    )
    parser.add_argument(
        '--umi-channel-id',
        type=int,
        default=0,
        help='UMI CANFD channel ID (default: 0)'
    )
    parser.add_argument(
        '--o10-device-id',
        type=int,
        default=OmniHand2025.kDefaultHandDeviceId,
        help='O10 device ID (default: OmniHand2025.kDefaultHandDeviceId)'
    )
    parser.add_argument(
        '--o10-canfd-id',
        type=int,
        default=0,
        help='O10 CANFD device ID (default: 0)'
    )
    parser.add_argument(
        '--o10-channel-id',
        type=int,
        default=1,
        help='O10 CANFD channel ID (default: 0)'
    )
    parser.add_argument(
        '--update-interval',
        type=float,
        default=0.001,
        help='Update interval in seconds (default: 0.02, i.e., 50Hz)'
    )
    parser.add_argument(
        '-d', '--device',
        choices=['zlgcan', 'hcan'],
        default='zlgcan',
        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan'
    )
    
    args = parser.parse_args()
    
    # Resolve hand side
    is_left = (args.hand == 'left')
    hand_type = HandType.LEFT if is_left else HandType.RIGHT
    hand_name = "Left" if is_left else "Right"
    
    print("=" * 60)
    print("UMI to O10 Control Demo")
    print(f"Hand Type: {hand_name}")
    print("=" * 60)
    
    # ============ Init UMI ============
    print("\n[1/3] Initializing UMI device...")
    try:
        if args.device == 'hcan':
            umi_hand = OmniHandDexUMI.create_hand_by_hcan(
                hand_type=hand_type,
                hand_device_id=args.umi_device_id,
                canfd_device_id=args.umi_canfd_id,
                canfd_channel_id=args.umi_channel_id
            )
        else:  # default: zlgcan
            umi_hand = OmniHandDexUMI.create_hand_by_zlgcan(
                hand_type=hand_type,
                hand_device_id=args.umi_device_id,
                canfd_device_id=args.umi_canfd_id,
                canfd_channel_id=args.umi_channel_id
            )
        
        if umi_hand is None:
            print("[ERROR]: Failed to create UMI hand instance")
            print("Please check if UMI device is connected")
            return 1
        
        if not umi_hand.init():
            print("[ERROR]: Failed to initialize UMI hand")
            return 1
        
        print(f"[OK]: UMI {hand_name} hand initialized successfully")
        
        # Get device info
        vendor_info = umi_hand.get_vendor_info()
        print(f"  Model: {vendor_info.product_model}")
        print(f"  Serial: {vendor_info.product_seq_num}")
        print(f"  DOF: {vendor_info.dof}")
        
    except Exception as e:
        print(f"[ERROR]: Failed to initialize UMI device: {e}")
        return 1
    
    # ============ Init O10 ============
    print("\n[2/3] Initializing O10 device...")
    try:
        if args.device == 'hcan':
            o10_hand = OmniHand2025.create_hand_by_hcan(
                hand_type=hand_type,
                hand_device_id=args.o10_device_id,
                canfd_device_id=args.o10_canfd_id,
                canfd_channel_id=args.o10_channel_id
            )
        else:  # default: zlgcan
            o10_hand = OmniHand2025.create_hand_by_zlgcan(
                hand_type=hand_type,
                hand_device_id=args.o10_device_id,
                canfd_device_id=args.o10_canfd_id,
                canfd_channel_id=args.o10_channel_id
            )
        
        if o10_hand is None:
            print("[ERROR]: Failed to create O10 hand instance")
            print("Please check if O10 device is connected")
            return 1
        
        if not o10_hand.init():
            print("[ERROR]: Failed to initialize O10 hand")
            return 1
        
        print(f"[OK]: O10 {hand_name} hand initialized successfully")
        
        # Get device info
        vendor_info = o10_hand.get_vendor_info()
        print(f"  Model: {vendor_info.product_model}")
        print(f"  Serial: {vendor_info.product_seq_num}")
        print(f"  DOF: {vendor_info.dof}")
        
        # Set control mode to position
        # o10_hand.set_control_mode(0)  # 0 = POSI mode
        print("  Control mode: POSI (position control)")
        
    except Exception as e:
        print(f"[ERROR]: Failed to initialize O10 device: {e}")
        return 1

    update_period = args.update_interval
    request_interval = 0
    recv_timeout = 10
    loop_count = 0
    total_time = 0.0
    max_time = 0.0
    min_time = float('inf')
    umi_error_count = 0
    o10_error_count = 0
    consecutive_errors = 0
    max_consecutive_errors = 10  # warn after this many consecutive errors

    # ============ Main loop: UMI read -> O10 write ============
    print("\n[3/3] Starting control loop...")
    print("-" * 60)
    print("**Press Ctrl+C to stop**")
    print("-" * 60)
    print("Performance notes:")
    print(f"  - Update interval: {update_period} seconds")
    print("  - Serial execution: UMI read + O10 write (blocking)")
    print(f"  - UMI/O10 timeout: {recv_timeout}ms, request interval: {request_interval}ms")
    print("-" * 60)
    
    # Shorter timeout for faster response (less wait per call)
    # Note: too short a timeout may cause frequent timeouts
    # umi_hand.set_request_interval(request_interval)
    umi_hand.set_frame_recv_timeout(recv_timeout)
    # o10_hand.set_request_interval(request_interval)
    o10_hand.set_frame_recv_timeout(recv_timeout)

    # Uncomment to enable CAN traffic debug
    # umi_hand.show_data_details(True)
    # o10_hand.show_data_details(True)
    
    try:
        while True:
            loop_start_time = time.time()
            
            # Read positions from UMI
            try:
                umi_positions = umi_hand.get_all_joint_positions()
                
                if len(umi_positions) != 10:
                    umi_error_count += 1
                    consecutive_errors += 1
                    time.sleep(update_period)
                    continue
                
                # Write motor positions to O10 directly
                # set_all_joint_positions returns actual; empty means fail
                actual_positions = o10_hand.set_all_joint_positions(umi_positions)
                if len(actual_positions) > 0:
                    consecutive_errors = 0
                else:
                    o10_error_count += 1
                    consecutive_errors += 1
                    
            except Exception as e:
                umi_error_count += 1
                consecutive_errors += 1
            
            # Print stats (same line)
            loop_count += 1
            print(f"\r#{loop_count} | UMI err: {umi_error_count} | O10 err: {o10_error_count}", end="", flush=True)
            
            # Consecutive error warning
            if consecutive_errors == max_consecutive_errors:
                print(f"[WARNING] {consecutive_errors} consecutive errors! Check device connections.")
            
            # Loop rate
            elapsed = time.time() - loop_start_time
            sleep_time = max(0, update_period - elapsed)
            if sleep_time > 0:
                time.sleep(sleep_time)
                    
    except KeyboardInterrupt:
        print("\n\n[INFO]: Stopped by user")
    except Exception as e:
        print(f"\n[ERROR]: Unexpected error: {e}")
        return 1
    
    print("\n[Done]: Demo completed successfully!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
