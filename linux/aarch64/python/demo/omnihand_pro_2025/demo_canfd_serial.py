#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 综合控制示例 - CANFD 通信（通过 serial_number）

此示例演示如何使用设备序列号创建和控制 OmniHand Pro 2025 灵巧手
支持单手（left/right）和双手（both）控制

运行方式：
    python3 demo_canfd_serial.py left    # 控制左手
    python3 demo_canfd_serial.py right   # 控制右手
    python3 demo_canfd_serial.py both    # 同时控制左右手

注意：代码中的序列号需要根据实际情况修改
"""

import sys
import time
from omnihand import OmniHandPro2025, HandType, Finger


def print_usage(program_name):
    """打印使用说明"""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Control left hand only")
    print("  right  - Control right hand only")
    print("  both   - Control both hands simultaneously")
    print()
    print("Note: Serial numbers in code need to be modified according to actual devices")


def control_single_hand(hand, hand_name):
    """控制单手的完整流程"""
    print(f"\n=== {hand_name} Hand Control ===")

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

    # 读取 3D 触觉传感器数据（O12 特有）
    print("\n--- 3D Tactile Sensor Data (O12 only) ---")
    try:
        thumb_sensor = hand.get_tactile_sensor_3d_data(Finger.THUMB)
        print(f"  Thumb:")
        print(f"    Online State: {'Online' if thumb_sensor.online_state else 'Offline'}")
        print(f"    Normal Force: {thumb_sensor.normal_force} (0.1N, max: 3000)")
        print(f"    Tangent Force: {thumb_sensor.tangent_force}")
        print(f"    Tangent Force Angle: {thumb_sensor.tangent_force_angle}°")

        index_sensor = hand.get_tactile_sensor_3d_data(Finger.INDEX)
        print(f"  Index:")
        print(f"    Online State: {'Online' if index_sensor.online_state else 'Offline'}")
        print(f"    Normal Force: {index_sensor.normal_force} (0.1N, max: 3000)")
        print(f"    Tangent Force: {index_sensor.tangent_force}")
        print(f"    Tangent Force Angle: {index_sensor.tangent_force_angle}°")
    except Exception as e:
        print(f"  Warning: {e}")

    # 读取温度报告
    print("\n--- Temperature Reports ---")
    try:
        periods = [500] * 12
        hand.set_all_temperature_report_periods(periods)
        time.sleep(0.5)

        temperatures = hand.get_all_temperature_reports()
        print(f"  All Joint Temperatures (°C): {temperatures}")
    except Exception as e:
        print(f"  Warning: {e}")

    # 读取电流报告
    print("\n--- Current Reports ---")
    try:
        periods = [500] * 12
        hand.set_all_current_report_periods(periods)
        time.sleep(0.5)

        currents = hand.get_all_current_reports()
        print(f"  All Joint Currents: {currents}")
    except Exception as e:
        print(f"  Warning: {e}")

    # 读取错误报告
    print("\n--- Error Reports ---")
    try:
        errors = hand.get_all_error_reports()
        has_error = False
        for i, error in enumerate(errors):
            error_flags = []
            if error.stalled:
                error_flags.append("Stalled")
            if error.overheat:
                error_flags.append("Overheat")
            if error.over_current:
                error_flags.append("OverCurrent")
            if error.motor_except:
                error_flags.append("MotorException")
            if error.commu_except:
                error_flags.append("CommException")
            
            if error_flags:
                print(f"  Joint {i + 1}: {' '.join(error_flags)}")
                has_error = True
        
        if not has_error:
            print("  No errors detected")
    except Exception as e:
        print(f"  Warning: {e}")

    # 读取速度
    print("\n--- Joint Velocities ---")
    try:
        velocities = hand.get_all_joint_motor_velo()
        print(f"  All Joint Velocities: {velocities}")
    except Exception as e:
        print(f"  Warning: {e}")

    # ============ 关节角度控制示例 ============
    print("\n=== Joint Angle Control ===")
    print("Setting joint angles...")
    angles = [0.0] * 12  # O12 有 12 个主动关节
    hand.set_all_active_joint_angles(angles)

    time.sleep(1.0)

    # 读取关节角度
    active_angles = hand.get_all_active_joint_angles()
    print(f"Active Joint Angles (rad): {[f'{a:.4f}' for a in active_angles]}")


def main():
    """主函数"""
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand Pro 2025 - CANFD Control (by serial_number)')
    parser.add_argument('mode', nargs='?', choices=['left', 'right', 'both'], default='left',
                        help='Hand control mode: left, right, or both (default: left)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    mode = args.mode
    device_type = args.device

    print("=" * 60)
    print("OmniHand Pro 2025 - CANFD Control (by serial_number)")
    print(f"Mode: {mode}")
    print(f"Device: {device_type}")
    print("=" * 60)

    hand_device_id= 1
    # 注意：序列号需要根据实际情况修改
    # 序列号支持部分匹配（例如 "201BFF2A" 可以匹配 "201BFF2AF01202D44690USBCANFD-200U"）
    left_serial = "201BFF2A"   # 左手适配器序列号（部分匹配）
    right_serial = "201BFF2B"  # 右手适配器序列号（部分匹配，请根据实际情况修改）

    # Helper function to create hand instance by serial number
    def create_hand_by_serial(hand_type, serial_number, channel_id=0):
        if device_type == 'hcan':
            return OmniHandPro2025.create_hand_by_hcan(
                hand_type=hand_type,
                hand_device_id=hand_device_id,
                hcan_serial_number=serial_number,
                canfd_channel_id=channel_id
            )
        else:  # default: zlgcan
            return OmniHandPro2025.create_hand_by_zlgcan(
                hand_type=hand_type,
                hand_device_id=hand_device_id,
                usbcanfd_serial_number=serial_number,
                canfd_channel_id=channel_id
            )

    if mode == "left" or mode == "both":
        left_hand = create_hand_by_serial(HandType.LEFT, left_serial, 0)

        if left_hand is None:
            print("[Error]: Failed to create left hand instance")
            print("Please check if device with serial number is connected")
            return 1

        if not left_hand.init():
            print("[Error]: Failed to initialize left hand")
            return 1

        print("[OK]: Left hand initialized successfully")
        if mode == "left":
            control_single_hand(left_hand, "Left")

    if mode == "right" or mode == "both":
        right_hand = create_hand_by_serial(HandType.RIGHT, right_serial, 0)

        if right_hand is None:
            print("[Error]: Failed to create right hand instance")
            print("Please check if device with serial number is connected")
            return 1

        if not right_hand.init():
            print("[Error]: Failed to initialize right hand")
            return 1

        print("[OK]: Right hand initialized successfully")

        if mode == "right":
            control_single_hand(right_hand, "Right")
        else:
            # both 模式：同时控制
            print("\n=== Dual Hand Control ===")

            if left_hand is None:
                # 如果之前没有创建左手，现在创建
                left_hand = create_hand_by_serial(HandType.LEFT, left_serial, 0)
                if left_hand is None or not left_hand.init():
                    print("[Error]: Failed to initialize left hand for dual mode")
                    return 1

            # 使用关节角度控制
            print("\nSetting joint angles for both hands...")
            left_angles = [0.0] * 12
            right_angles = [0.5] * 12

            left_hand.set_all_active_joint_angles(left_angles)
            right_hand.set_all_active_joint_angles(right_angles)

            time.sleep(1.0)

            left_angles_read = left_hand.get_all_active_joint_angles()
            right_angles_read = right_hand.get_all_active_joint_angles()

            print(f"Left Hand Angles (rad): {[f'{a:.4f}' for a in left_angles_read]}")
            print(f"Right Hand Angles (rad): {[f'{a:.4f}' for a in right_angles_read]}")

    print("\n[Done]: Example completed successfully!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
