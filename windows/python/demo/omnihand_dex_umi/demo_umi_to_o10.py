#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
UMI 到 O10 控制示例

此示例演示如何从 UMI 设备获取位置数据，并将其直接设置到 O10 灵巧手。

工作流程：
1. 从 UMI 获取电机位置值（范围：0-4096）
2. 直接将电机位置值设置到 O10 灵巧手（无需转换）

运行方式：
    python3 demo_umi_to_o10.py [left|right]
    
参数说明：
    left  - 控制左手（默认）
    right - 控制右手

注意：
    - UMI 设备是只读的，只能获取位置数据
    - O10 设备需要支持位置控制
    - 需要确保 UMI 和 O10 的手型（左右手）匹配
    - UMI 和 O10 都使用相同的电机位置值范围（0-4096），可直接使用
"""

import sys
import time
import argparse
from omnihand import OmniHandDexUMI, OmniHand2025, HandType


def print_usage(program_name):
    """打印使用说明"""
    print(f"Usage: {program_name} [left|right]")
    print("  left  - Control left hand (default)")
    print("  right - Control right hand")
    print()
    print("Note: UMI device is read-only, O10 device needs position control support")
    print("Note: UMI and O10 use the same motor position range (0-4096), direct transfer is supported")


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='UMI to O10 control demo',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument(
        'hand',
        nargs='?',
        choices=['left', 'right'],
        default='right',
        help='Hand type: left or right (default: left)'
    )
    parser.add_argument(
        '--umi-device-id',
        type=int,
        default=1,
        help='UMI device ID (default: 1)'
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
        default=1,
        help='O10 device ID (default: 1)'
    )
    parser.add_argument(
        '--o10-canfd-id',
        type=int,
        default=1,
        help='O10 CANFD device ID (default: 0)'
    )
    parser.add_argument(
        '--o10-channel-id',
        type=int,
        default=0,
        help='O10 CANFD channel ID (default: 0)'
    )
    parser.add_argument(
        '--update-interval',
        type=float,
        default=0.001,
        help='Update interval in seconds (default: 0.02, i.e., 50Hz)'
    )
    
    args = parser.parse_args()
    
    # 确定手型
    is_left = (args.hand == 'left')
    hand_type = HandType.LEFT if is_left else HandType.RIGHT
    hand_name = "Left" if is_left else "Right"
    
    print("=" * 60)
    print("UMI to O10 Control Demo")
    print(f"Hand Type: {hand_name}")
    print("=" * 60)
    
    # ============ 初始化 UMI 设备 ============
    print("\n[1/3] Initializing UMI device...")
    try:
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
        
        # 获取设备信息
        vendor_info = umi_hand.get_vendor_info()
        print(f"  Model: {vendor_info.product_model}")
        print(f"  Serial: {vendor_info.product_seq_num}")
        print(f"  DOF: {vendor_info.dof}")
        
    except Exception as e:
        print(f"[ERROR]: Failed to initialize UMI device: {e}")
        return 1
    
    # ============ 初始化 O10 设备 ============
    print("\n[2/3] Initializing O10 device...")
    try:
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
        
        # 获取设备信息
        vendor_info = o10_hand.get_vendor_info()
        print(f"  Model: {vendor_info.product_model}")
        print(f"  Serial: {vendor_info.product_seq_num}")
        print(f"  DOF: {vendor_info.dof}")
        
        # 设置控制模式为位置控制
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
    max_consecutive_errors = 10  # 连续错误超过此数量时打印警告

    # ============ 主循环：从 UMI 读取位置并设置到 O10 ============
    print("\n[3/3] Starting control loop...")
    print("-" * 60)
    print("**Press Ctrl+C to stop**")
    print("-" * 60)
    print("Performance notes:")
    print(f"  - Update interval: {update_period} seconds")
    print("  - Serial execution: UMI read + O10 write (blocking)")
    print(f"  - UMI/O10 timeout: {recv_timeout}ms, request interval: {request_interval}ms")
    print("-" * 60)
    
    # 设置较短的超时以提高响应速度（降低单次超时等待时间）
    # 注意：如果设置太短可能导致频繁超时
    # umi_hand.set_request_interval(request_interval)
    umi_hand.set_frame_recv_timeout(recv_timeout)
    # o10_hand.set_request_interval(request_interval)
    o10_hand.set_frame_recv_timeout(recv_timeout)

    # 开启调试模式，查看 CAN 收发数据（出问题时取消注释）
    # umi_hand.show_data_details(True)
    # o10_hand.show_data_details(True)
    
    try:
        while True:
            loop_start_time = time.time()
            
            # 从 UMI 获取位置数据
            try:
                umi_positions = umi_hand.get_all_joint_positions()
                
                if len(umi_positions) != 10:
                    umi_error_count += 1
                    consecutive_errors += 1
                    time.sleep(update_period)
                    continue
                
                # 直接设置电机位置值到 O10（无需转换）
                # set_all_joint_positions 返回实际位置，空列表表示失败
                actual_positions = o10_hand.set_all_joint_positions(umi_positions)
                if len(actual_positions) > 0:
                    consecutive_errors = 0
                else:
                    o10_error_count += 1
                    consecutive_errors += 1
                    
            except Exception as e:
                umi_error_count += 1
                consecutive_errors += 1
            
            # 打印统计（同一行刷新）
            loop_count += 1
            print(f"\r#{loop_count} | UMI err: {umi_error_count} | O10 err: {o10_error_count}", end="", flush=True)
            
            # 连续错误警告
            if consecutive_errors == max_consecutive_errors:
                print(f"[WARNING] {consecutive_errors} consecutive errors! Check device connections.")
            
            # 控制循环频率
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
