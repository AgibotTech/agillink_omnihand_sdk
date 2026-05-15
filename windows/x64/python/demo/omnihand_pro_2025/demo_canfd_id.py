#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 comprehensive control demo - CANFD communication (via canfd_id)

This demo shows how to create and control OmniHand Pro 2025 via canfd_id
Supports single-hand (left/right) and dual-hand (both)

Run:
    python3 demo_canfd_id.py left    # control left hand
    python3 demo_canfd_id.py right   # control right hand
    python3 demo_canfd_id.py both    # control both hands
"""

import sys
import time
from omnihand import OmniHandPro2025, HandType, Finger


def print_usage(program_name):
    """Print usage help."""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Control left hand only")
    print("  right  - Control right hand only")
    print("  both   - Control both hands simultaneously")
    print()
    print("Example:")
    print(f"  {program_name} left")
    print(f"  {program_name} both")


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

    # Read 3D tactile sensor data (O12-specific)
    print("\n--- 3D Tactile Sensor Data (O12 only) ---")
    try:
        thumb_sensor = hand.get_tactile_sensor_3d_data(Finger.THUMB)
        print(f"  Thumb:")
        print(f"    Online State: {'Online' if thumb_sensor.online_state else 'Offline'}")
        print(f"    Normal Force: {thumb_sensor.normal_force} (0.1N, max: 3000)")
        print(f"    Tangent Force: {thumb_sensor.tangent_force}")
        print(f"    Tangent Force Angle: {thumb_sensor.tangent_force_angle}°")
        print(f"    Channel Values: {thumb_sensor.channel_values}")

        index_sensor = hand.get_tactile_sensor_3d_data(Finger.INDEX)
        print(f"  Index:")
        print(f"    Online State: {'Online' if index_sensor.online_state else 'Offline'}")
        print(f"    Normal Force: {index_sensor.normal_force} (0.1N, max: 3000)")
        print(f"    Tangent Force: {index_sensor.tangent_force}")
        print(f"    Tangent Force Angle: {index_sensor.tangent_force_angle}°")
    except Exception as e:
        print(f"  Warning: {e}")

    # Read temperature report
    print("\n--- Temperature Reports ---")
    try:
        # Set temperature report period
        periods = [500] * 12  # 500 ms period
        hand.set_all_temperature_report_periods(periods)
        time.sleep(0.5)  # Wait for data update

        temperatures = hand.get_all_temperature_reports()
        print(f"  All Joint Temperatures (°C): {temperatures}")
    except Exception as e:
        print(f"  Warning: {e}")

    # Read current report
    print("\n--- Current Reports ---")
    try:
        # Set current report period
        periods = [500] * 12  # 500 ms period
        hand.set_all_current_report_periods(periods)
        time.sleep(0.5)  # Wait for data update

        currents = hand.get_all_current_reports()
        print(f"  All Joint Currents: {currents}")
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
    angles = [0.0] * 12  # O12 has 12 active joints
    hand.set_all_active_joint_angles(angles)

    time.sleep(1.0)

    # Read joint angles
    active_angles = hand.get_all_active_joint_angles()
    print(f"Active Joint Angles (rad): {[f'{a:.4f}' for a in active_angles]}")

    # Read all joint angles
    all_angles = hand.get_all_joint_angles()
    print(f"All Joint Angles (rad, {len(all_angles)} joints): {[f'{a:.4f}' for a in all_angles]}")


def main():
    """Main entry point."""
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand Pro 2025 - CANFD Control (by canfd_id)')
    parser.add_argument('mode', nargs='?', choices=['left', 'right', 'both'], default='left',
                        help='Hand control mode: left, right, or both (default: left)')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    mode = args.mode
    device_type = args.device

    print("=" * 60)
    print("OmniHand Pro 2025 - CANFD Control (by canfd_id)")
    print(f"Mode: {mode}")
    print(f"Device: {device_type}")
    print("=" * 60)

    hand_device_id= OmniHandPro2025.kDefaultHandDeviceId
    canfd_device_id= 0

    # Helper function to create hand instance
    def create_hand(hand_type, channel_id=0):
        if device_type == 'hcan':
            return OmniHandPro2025.create_hand_by_hcan(
                hand_type=hand_type,
                hand_device_id=hand_device_id,
                canfd_device_id=canfd_device_id,
                canfd_channel_id=channel_id
            )
        else:  # default: zlgcan
            return OmniHandPro2025.create_hand_by_zlgcan(
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
        control_single_hand(left_hand, "Left")

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
        control_single_hand(right_hand, "Right")

    elif mode == "both":
        # both mode: create both hands
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

        # Control both hands
        print("\n=== Dual Hand Control ===")

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
        left_angles = [0.0] * 12
        right_angles = [0.5] * 12

        left_hand.set_all_active_joint_angles(left_angles)
        right_hand.set_all_active_joint_angles(right_angles)

        time.sleep(1.0)

        # Read joint angles
        left_angles_read = left_hand.get_all_active_joint_angles()
        right_angles_read = right_hand.get_all_active_joint_angles()

        print(f"Left Hand Angles (rad): {[f'{a:.4f}' for a in left_angles_read]}")
        print(f"Right Hand Angles (rad): {[f'{a:.4f}' for a in right_angles_read]}")

    print("\n[Done]: Example completed successfully!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
