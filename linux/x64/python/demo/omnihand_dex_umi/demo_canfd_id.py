#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 综合控制示例 - CANFD 通信（通过 canfd_id）

此示例演示如何使用 canfd_id 创建和读取 OmniHand Dex UMI 灵巧手数据
支持单手（left/right）和双手（both）控制

注意：UMI 协议是只读的，不支持位置/速度/力矩控制
位置数据只能通过周期性位置报告获取

运行方式：
    python3 demo_canfd_id.py left    # 读取左手数据
    python3 demo_canfd_id.py right   # 读取右手数据
    python3 demo_canfd_id.py both    # 同时读取左右手数据
"""

import sys
import time
import threading
from omnihand import OmniHandDexUMI, HandType, Finger


def print_usage(program_name):
    """打印使用说明"""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Read left hand data only")
    print("  right  - Read right hand data only")
    print("  both   - Read both hands data simultaneously")
    print()
    print("Note: UMI protocol is read-only, position/velocity/torque control is not supported")


# 用于统计和显示的数据
position_report_count = {}
tactile_report_count = {}
lock = threading.Lock()


def position_report_callback(positions, hand_name="Unknown"):
    """位置周期上报回调函数"""
    global position_report_count
    with lock:
        if hand_name not in position_report_count:
            position_report_count[hand_name] = 0
        position_report_count[hand_name] += 1
        count = position_report_count[hand_name]
        if count % 100 == 0:  # 每100次打印一次
            print(f"\n[{hand_name} Position Report #{count}]")
            print(f"  Position data (0-4096): {positions[:5]}..." if len(positions) > 5 else f"  Position data: {positions}")


def tactile_report_callback(sensor_data, hand_name="Unknown"):
    """触觉传感器周期上报回调函数"""
    global tactile_report_count
    with lock:
        if hand_name not in tactile_report_count:
            tactile_report_count[hand_name] = 0
        tactile_report_count[hand_name] += 1
        count = tactile_report_count[hand_name]
        if count % 100 == 0:  # 每100次打印一次
            print(f"\n[{hand_name} Tactile Report #{count}]")
            print(f"  Sensor ID: {sensor_data.sensor_id}, Data length: {len(sensor_data.data) if sensor_data.data else 0}")


def read_single_hand(hand, hand_name):
    """读取单手的完整流程"""
    print(f"\n=== {hand_name} Hand Data Reading ===")

    # ============ 获取设备信息 ============
    print("\n--- Vendor Info ---")
    vendor_info = hand.get_vendor_info()
    print(f"  Model: {vendor_info.product_model}")
    print(f"  Serial: {vendor_info.product_seq_num}")
    print(f"  Hardware Version: {vendor_info.hardware_version.major}."
          f"{vendor_info.hardware_version.minor}."
          f"{vendor_info.hardware_version.patch}")
    print(f"  Software Version: {vendor_info.software_version.major}."
          f"{vendor_info.software_version.minor}."
          f"{vendor_info.software_version.patch}")
    print(f"  Voltage: {vendor_info.voltage} mV")
    print(f"  DOF: {vendor_info.dof}")

    print("\n--- Device Info ---")
    device_info = hand.get_device_info()
    print(f"  Device ID: {device_info.hand_device_id}")
    print(f"  Communication Parameters:")
    print(f"    Bitrate: {device_info.commu_params.bitrate}")
    print(f"    Sample Point: {device_info.commu_params.sample_point}")
    print(f"    D-Bitrate: {device_info.commu_params.dbitrate}")
    print(f"    D-Sample Point: {device_info.commu_params.dsample_point}")

    # ============ 读取传感器数据 ============
    print("\n=== Reading Sensor Data ===")

    # 注意：UMI 协议支持主动查询关节位置
    print("\nNote: UMI protocol supports active position query.")
    print("      Use get_joint_position() or get_all_joint_positions() to get position data.")

    # 读取触觉传感器数据（1D，使用 Raw API）
    print("\n--- 1D Tactile Sensor Data (Raw) ---")
    try:
        fingers = [
            (Finger.THUMB, "Thumb"),
            (Finger.INDEX, "Index"),
            (Finger.MIDDLE, "Middle"),
        ]
        
        for finger_enum, finger_name in fingers:
            try:
                sensor_data = hand.get_tactile_sensor_data_raw(finger_enum)
                if sensor_data and sensor_data.data:
                    data = sensor_data.data
                    print(f"  {finger_name}: [{', '.join(map(str, data[:10]))}...] (unit: 1g, max: 255g, {len(data)} points)")
                else:
                    print(f"  {finger_name}: No data available")
            except Exception as e:
                print(f"  {finger_name}: Error - {e}")

        # 读取所有传感器数据
        print("\n--- All Tactile Sensor Data ---")
        all_sensors = hand.get_all_tactile_sensor_data_raw()
        print(f"  Total sensors: {len(all_sensors)}")
        for sensor in all_sensors:
            finger_name = "Unknown"
            for finger_enum, name in fingers:
                if sensor.sensor_id == finger_enum:
                    finger_name = name
                    break
            data_len = len(sensor.data) if sensor.data else 0
            print(f"  {finger_name} (ID: {sensor.sensor_id}): {data_len} points")
    except Exception as e:
        print(f"  Warning: {e}")

    # ============ 主动查询位置数据 ============
    print("\n=== Active Position Query ===")
    
    # 主动查询所有关节位置
    try:
        positions = hand.get_all_joint_positions()
        print(f"  All joint positions: {positions}")
    except Exception as e:
        print(f"  Warning: Failed to get all joint positions - {e}")
    
    # 主动查询单个关节位置
    try:
        for i in range(1, 4):  # 查询前3个关节
            pos = hand.get_joint_position(i)
            print(f"  Joint {i} position: {pos}")
    except Exception as e:
        print(f"  Warning: Failed to get single joint position - {e}")


def main():
    """主函数"""
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand Dex UMI - CANFD Control (by canfd_id)')
    parser.add_argument('mode', nargs='?', choices=['left', 'right', 'both'], default='left',
                        help='Hand control mode: left, right, or both (default: left)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    mode = args.mode
    device_type = args.device

    print("=" * 60)
    print("OmniHand Dex UMI - CANFD Control (by canfd_id)")
    print(f"Mode: {mode}")
    print(f"Device: {device_type}")
    print("=" * 60)

    hand_device_id= OmniHandDexUMI.kDefaultHandDeviceId
    canfd_device_id= 0

    # Helper function to create hand instance
    def create_hand(hand_type, channel_id=0):
        if device_type == 'hcan':
            return OmniHandDexUMI.create_hand_by_hcan(
                hand_type=hand_type,
                hand_device_id=hand_device_id,
                canfd_device_id=canfd_device_id,
                canfd_channel_id=channel_id
            )
        else:  # default: zlgcan
            return OmniHandDexUMI.create_hand_by_zlgcan(
                hand_type=hand_type,
                hand_device_id=hand_device_id,
                canfd_device_id=canfd_device_id,
                canfd_channel_id=channel_id
            )

    if mode == "left":
        # 创建左手实例
        left_hand = create_hand(HandType.LEFT, 0)

        if left_hand is None:
            print("[Error]: Failed to create left hand instance")
            return 1

        if not left_hand.init():
            print("[Error]: Failed to initialize left hand")
            return 1

        print("[OK]: Left hand initialized successfully")
        read_single_hand(left_hand, "Left")

    elif mode == "right":
        # 创建右手实例
        right_hand = create_hand(HandType.RIGHT, 0)

        if right_hand is None:
            print("[Error]: Failed to create right hand instance")
            return 1

        if not right_hand.init():
            print("[Error]: Failed to initialize right hand")
            return 1

        print("[OK]: Right hand initialized successfully")
        read_single_hand(right_hand, "Right")

    elif mode == "both":
        # both 模式：同时读取两个手
        left_hand = create_hand(HandType.LEFT, 0)  # 第一个通道
        right_hand = create_hand(HandType.RIGHT, 1)  # 第二个通道（需要多通道适配器）

        if left_hand is None or right_hand is None:
            print("[Error]: Failed to create hand instances")
            return 1

        if not left_hand.init():
            print("[Error]: Failed to initialize left hand")
            return 1

        if not right_hand.init():
            print("[Error]: Failed to initialize right hand")
            return 1

        print("[OK]: Both hands initialized successfully")

        # 同时读取两个手的数据
        print("\n=== Dual Hand Data Reading ===")

        # 获取设备信息
        left_vendor = left_hand.get_vendor_info()
        right_vendor = right_hand.get_vendor_info()

        print("\n--- Left Hand Info ---")
        print(f"  Model: {left_vendor.product_model}")
        print(f"  Serial: {left_vendor.product_seq_num}")

        print("\n--- Right Hand Info ---")
        print(f"  Model: {right_vendor.product_model}")
        print(f"  Serial: {right_vendor.product_seq_num}")

        # 主动查询两个手的位置数据
        print("\n--- Active Position Query ---")
        try:
            left_positions = left_hand.get_all_joint_positions()
            print(f"  Left hand positions: {left_positions[:5]}...")
        except Exception as e:
            print(f"  Left hand: Failed to get positions - {e}")
        
        try:
            right_positions = right_hand.get_all_joint_positions()
            print(f"  Right hand positions: {right_positions[:5]}...")
        except Exception as e:
            print(f"  Right hand: Failed to get positions - {e}")

    print("\n[Done]: Example completed successfully!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
