#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI comprehensive control demo - CANFD communication (via canfd_id)

This demo creates and reads OmniHand Dex UMI data via canfd_id
Supports single-hand (left/right) and dual-hand (both)

Note: UMI is read-only; no position/velocity/torque control
Position can also come from periodic position reports

Run:
    python3 demo_canfd_id.py left    # read left
    python3 demo_canfd_id.py right   # read right
    python3 demo_canfd_id.py both    # read both
"""

import sys
import time
import threading
from omnihand import OmniHandDexUMI, HandType, Finger


def print_usage(program_name):
    """Print usage help."""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Read left hand data only")
    print("  right  - Read right hand data only")
    print("  both   - Read both hands data simultaneously")
    print()
    print("Note: UMI protocol is read-only, position/velocity/torque control is not supported")


# Stats for display
position_report_count = {}
tactile_report_count = {}
lock = threading.Lock()


def position_report_callback(positions, hand_name="Unknown"):
    """Periodic position report callback."""
    global position_report_count
    with lock:
        if hand_name not in position_report_count:
            position_report_count[hand_name] = 0
        position_report_count[hand_name] += 1
        count = position_report_count[hand_name]
        if count % 100 == 0:  # print every 100 callbacks
            print(f"\n[{hand_name} Position Report #{count}]")
            print(f"  Position data (0-4096): {positions[:5]}..." if len(positions) > 5 else f"  Position data: {positions}")


def tactile_report_callback(sensor_data, hand_name="Unknown"):
    """Periodic tactile report callback."""
    global tactile_report_count
    with lock:
        if hand_name not in tactile_report_count:
            tactile_report_count[hand_name] = 0
        tactile_report_count[hand_name] += 1
        count = tactile_report_count[hand_name]
        if count % 100 == 0:  # print every 100 callbacks
            print(f"\n[{hand_name} Tactile Report #{count}]")
            print(f"  Sensor ID: {sensor_data.sensor_id}, Data length: {len(sensor_data.data) if sensor_data.data else 0}")


def read_single_hand(hand, hand_name):
    """Read flow for one hand."""
    print(f"\n=== {hand_name} Hand Data Reading ===")

    # ============ Get device info ============
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

    # ============ Read sensor data ============
    print("\n=== Reading Sensor Data ===")

    # Note: UMI supports active joint position query
    print("\nNote: UMI protocol supports active position query.")
    print("      Use get_joint_position() or get_all_joint_positions() to get position data.")

    # Read tactile data (1D, Raw API)
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

        # Read all sensor data
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

    # ============ Active position query ============
    print("\n=== Active Position Query ===")
    
    # Query all joint positions
    try:
        positions = hand.get_all_joint_positions()
        print(f"  All joint positions: {positions}")
    except Exception as e:
        print(f"  Warning: Failed to get all joint positions - {e}")
    
    # Query single joint positions
    try:
        for i in range(1, 4):  # first 3 joints
            pos = hand.get_joint_position(i)
            print(f"  Joint {i} position: {pos}")
    except Exception as e:
        print(f"  Warning: Failed to get single joint position - {e}")


def main():
    """Main entry point."""
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
        # Create left-hand instance
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
        # Create right-hand instance
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
        # both mode: read both hands
        left_hand = create_hand(HandType.LEFT, 0)  # first channel
        right_hand = create_hand(HandType.RIGHT, 1)  # second channel (multi-channel adapter required)

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

        # Read data from both hands
        print("\n=== Dual Hand Data Reading ===")

        # Get device info
        left_vendor = left_hand.get_vendor_info()
        right_vendor = right_hand.get_vendor_info()

        print("\n--- Left Hand Info ---")
        print(f"  Model: {left_vendor.product_model}")
        print(f"  Serial: {left_vendor.product_seq_num}")

        print("\n--- Right Hand Info ---")
        print(f"  Model: {right_vendor.product_model}")
        print(f"  Serial: {right_vendor.product_seq_num}")

        # Active query positions for both hands
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
