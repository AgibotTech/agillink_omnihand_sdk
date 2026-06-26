#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 comprehensive control demo - CANFD communication (via serial_number)

This demo creates and controls OmniHand 2025 via device serial number
Supports single-hand (left/right) and dual-hand (both)

Run:
    python3 demo_canfd_serial.py left    # control left hand
    python3 demo_canfd_serial.py right   # control right hand
    python3 demo_canfd_serial.py both    # control both hands

Note: update serial numbers in code for your setup
"""

import sys
import time
from omnihand import OmniHand2025, HandType, Finger


def print_usage(program_name):
    """Print usage help."""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Control left hand only")
    print("  right  - Control right hand only")
    print("  both   - Control both hands simultaneously")
    print()
    print("Note: Serial numbers in code need to be modified according to actual devices")


def control_single_hand(hand, hand_name):
    """Full demo flow for one hand."""
    print(f"\n=== {hand_name} Hand Control ===")

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

    # Read tactile data (1D)
    print("\n--- Tactile Sensor Data (1D) ---")
    try:
        thumb_tactile = hand.get_tactile_sensor_data(Finger.THUMB)
        print(f"  Thumb: {thumb_tactile} (unit: 1g, max: 255g)")

        index_tactile = hand.get_tactile_sensor_data(Finger.INDEX)
        print(f"  Index: {index_tactile} (unit: 1g, max: 255g)")
    except Exception as e:
        print(f"  Warning: {e}")

    # Read error report
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

    # Read velocity
    print("\n--- Joint Velocities ---")
    try:
        velocities = hand.get_all_joint_motor_velo()
        print(f"  All Joint Velocities: {velocities}")
    except Exception as e:
        print(f"  Warning: {e}")

    # ============ Joint angle control demo ============
    print("\n=== Joint Angle Control ===")
    print("Setting joint angles...")
    angles = [0.0] * 10  # O10 has 10 active joints
    hand.set_all_active_joint_angles(angles)

    time.sleep(1.0)

    # Read joint angles
    active_angles = hand.get_all_active_joint_angles()
    print(f"Active Joint Angles (rad): {[f'{a:.4f}' for a in active_angles]}")


def main():
    """Main entry point."""
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand 2025 - CANFD Control (by serial_number)')
    parser.add_argument('mode', nargs='?', choices=['left', 'right', 'both'], default='left',
                        help='Hand control mode: left, right, or both (default: left)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    mode = args.mode
    device_type = args.device

    print("=" * 60)
    print("OmniHand 2025 - CANFD Control (by serial_number)")
    print(f"Mode: {mode}")
    print(f"Device: {device_type}")
    print("=" * 60)

    hand_device_id= OmniHand2025.kDefaultHandDeviceId
    # Note: update serial numbers for your setup
    # Serial numbers support partial match (e.g. "201BFF2A" matches longer strings)
    left_serial = "201BFF2A"   # left adapter serial (partial match)
    right_serial = "201BFF2B"  # right adapter serial (partial match; update for your setup)

    # Helper function to create hand instance by serial number
    def create_hand_by_serial(hand_type, serial_number, channel_id=0):
        if device_type == 'hcan':
            return OmniHand2025.create_hand_by_hcan(
                hand_type=hand_type,
                hand_device_id=hand_device_id,
                hcan_serial_number=serial_number,
                canfd_channel_id=channel_id
            )
        else:  # default: zlgcan
            return OmniHand2025.create_hand_by_zlgcan(
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
            # both mode: control both
            print("\n=== Dual Hand Control ===")

            if left_hand is None:
                # Create left hand if not created yet
                left_hand = create_hand_by_serial(HandType.LEFT, left_serial, 0)
                if left_hand is None or not left_hand.init():
                    print("[Error]: Failed to initialize left hand for dual mode")
                    return 1

            # Get device info
            left_vendor = left_hand.get_vendor_info()
            right_vendor = right_hand.get_vendor_info()

            print("\n--- Left Hand Info ---")
            print(f"  Model: {left_vendor.product_model}")
            print(f"  Serial: {left_vendor.product_seq_num}")

            print("\n--- Right Hand Info ---")
            print(f"  Model: {right_vendor.product_model}")
            print(f"  Serial: {right_vendor.product_seq_num}")

            # Joint-angle control
            print("\nSetting joint angles for both hands...")
            left_angles = [0.0] * 10
            right_angles = [0.5] * 10

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
