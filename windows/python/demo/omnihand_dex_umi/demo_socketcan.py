#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 综合控制示例 - SocketCAN 通信（仅 Linux）

此示例演示如何使用 SocketCAN 创建和读取 OmniHand Dex UMI 灵巧手数据
支持单手（left/right）和双手（both）控制

⚠️ 注意：此示例适用于已有 SocketCAN 环境的场景（如板载 CAN、其他 SocketCAN 设备）
⚠️ 对于 USB CANFD 设备，推荐使用 ZLG 库方式，无需配置驱动
⚠️ UMI 协议是只读的，不支持位置/速度/力矩控制

使用前需配置 CAN 接口:
    sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
    sudo ip link set can0 up
    sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
    sudo ip link set can1 up

运行方式：
    python3 demo_socketcan_comprehensive.py left    # 读取左手数据（使用 can0）
    python3 demo_socketcan_comprehensive.py right   # 读取右手数据（使用 can0）
    python3 demo_socketcan_comprehensive.py both    # 同时读取左右手数据（使用 can0 和 can1）
"""

import sys
import time
import threading
from omnihand import OmniHandDexUMI, HandType, Finger


def print_usage(program_name):
    """打印使用说明"""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Read left hand data only (uses can0)")
    print("  right  - Read right hand data only (uses can0)")
    print("  both   - Read both hands data simultaneously (uses can0 and can1)")
    print()
    print("Before running, configure CAN interfaces:")
    print("  sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on")
    print("  sudo ip link set can0 up")
    print("  sudo ip link set can1 type can bitrate 1000000 dbitrate 5000000 fd on")
    print("  sudo ip link set can1 up")


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
    print(f"  Hardware Version: {vendor_info.hardware_version.major_}."
          f"{vendor_info.hardware_version.minor_}."
          f"{vendor_info.hardware_version.patch_}")
    print(f"  Software Version: {vendor_info.software_version.major_}."
          f"{vendor_info.software_version.minor_}."
          f"{vendor_info.software_version.patch_}")
    print(f"  Voltage: {vendor_info.voltage} mV")
    print(f"  DOF: {vendor_info.dof}")

    print("\n--- Device Info ---")
    device_info = hand.get_device_info()
    print(f"  Device ID: {device_info.hand_device_id}")
    print(f"  Communication Parameters:")
    print(f"    Bitrate: {device_info.commu_params.bitrate_}")
    print(f"    Sample Point: {device_info.commu_params.sample_point_}")
    print(f"    D-Bitrate: {device_info.commu_params.dbitrate_}")
    print(f"    D-Sample Point: {device_info.commu_params.dsample_point_}")

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
    mode = "left"
    if len(sys.argv) > 1:
        arg = sys.argv[1]
        if arg in ["--help", "-h"]:
            print_usage(sys.argv[0])
            return 0
        elif arg in ["left", "right", "both"]:
            mode = arg
        else:
            print(f"[Error]: Invalid argument: {arg}")
            print_usage(sys.argv[0])
            return 1

    print("=" * 60)
    print("OmniHand Dex UMI - SocketCAN Control")
    print(f"Mode: {mode}")
    print("=" * 60)

    hand_device_id= 1
    left_interface = "can0"
    right_interface = "can1"

    if mode == "left" or mode == "both":
        left_hand = OmniHandDexUMI.create_hand_socketcan(
            hand_type=HandType.LEFT,
            hand_device_id=hand_device_id,
            can_interface=left_interface
        )

        if left_hand is None:
            print("[Error]: Failed to create left hand instance")
            return 1

        if not left_hand.init():
            print("[Error]: Failed to initialize left hand")
            print(f"Please check if {left_interface} is configured and up")
            return 1

        print(f"[OK]: Left hand initialized successfully ({left_interface})")
        if mode == "left":
            read_single_hand(left_hand, "Left")
            print("\nWaiting for periodic reports (5 seconds)...")
            time.sleep(5)

    if mode == "right" or mode == "both":
        interface = right_interface if mode == "both" else left_interface

        right_hand = OmniHandDexUMI.create_hand_socketcan(
            hand_type=HandType.RIGHT,
            hand_device_id=hand_device_id,
            can_interface=interface
        )

        if right_hand is None:
            print("[Error]: Failed to create right hand instance")
            return 1

        if not right_hand.init():
            print("[Error]: Failed to initialize right hand")
            print(f"Please check if {interface} is configured and up")
            return 1

        print(f"[OK]: Right hand initialized successfully ({interface})")

        if mode == "right":
            read_single_hand(right_hand, "Right")
            print("\nWaiting for periodic reports (5 seconds)...")
            time.sleep(5)
        else:
            # both 模式：同时读取
            print("\n=== Dual Hand Data Reading ===")

            if left_hand is None:
                # 如果之前没有创建左手，现在创建
                left_hand = OmniHandDexUMI.create_hand_socketcan(
                    hand_type=HandType.LEFT,
                    hand_device_id=hand_device_id,
                    can_interface=left_interface
                )
                if left_hand is None or not left_hand.init():
                    print("[Error]: Failed to initialize left hand for dual mode")
                    return 1

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
