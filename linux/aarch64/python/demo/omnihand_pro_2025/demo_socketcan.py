#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Pro 2025 comprehensive control demo - SocketCAN communication (Linux only)

This demo shows how to create and control OmniHand Pro 2025 via SocketCAN
Supports single-hand (left/right) and dual-hand (both)

Warning: for environments with SocketCAN already set up (onboard CAN, etc.)
Warning: for USB CANFD, prefer ZLG library without extra driver setup

Configure CAN interface before use:
    sudo ip link set can0 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
    sudo ip link set can0 up
    sudo ip link set can1 type can bitrate 1000000 sample-point 0.8 dbitrate 5000000 dsample-point 0.8 fd on
    sudo ip link set can1 up

Run:
    python3 demo_socketcan_comprehensive.py left    # control left (using can0)
    python3 demo_socketcan_comprehensive.py right   # control right (using can0)
    python3 demo_socketcan_comprehensive.py both    # both hands (can0 and can1)
"""

import sys
import time
from omnihand import OmniHandPro2025, HandType, Finger


def print_usage(program_name):
    """Print usage help."""
    print(f"Usage: {program_name} [left|right|both]")
    print("  left   - Control left hand only (uses can0)")
    print("  right  - Control right hand only (uses can0)")
    print("  both   - Control both hands simultaneously (uses can0 and can1)")
    print()
    print("Before running, configure CAN interfaces:")
    print("  sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on")
    print("  sudo ip link set can0 up")
    print("  sudo ip link set can1 type can bitrate 1000000 dbitrate 5000000 fd on")
    print("  sudo ip link set can1 up")


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
        periods = [500] * 12
        hand.set_all_temperature_report_periods(periods)
        time.sleep(0.5)

        temperatures = hand.get_all_temperature_reports()
        print(f"  All Joint Temperatures (°C): {temperatures}")
    except Exception as e:
        print(f"  Warning: {e}")

    # Read current report
    print("\n--- Current Reports ---")
    try:
        periods = [500] * 12
        hand.set_all_current_report_periods(periods)
        time.sleep(0.5)

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


def main():
    """Main entry point."""
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
    print("OmniHand Pro 2025 - SocketCAN Control")
    print(f"Mode: {mode}")
    print("=" * 60)

    hand_device_id= OmniHandPro2025.kDefaultHandDeviceId
    left_interface = "can0"
    right_interface = "can1"

    if mode == "left" or mode == "both":
        left_hand = OmniHandPro2025.create_hand_socketcan(
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
            control_single_hand(left_hand, "Left")

    if mode == "right" or mode == "both":
        interface = right_interface if mode == "both" else left_interface

        right_hand = OmniHandPro2025.create_hand_socketcan(
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
            control_single_hand(right_hand, "Right")
        else:
            # both mode: control both
            print("\n=== Dual Hand Control ===")

            if left_hand is None:
                # Create left hand if not created yet
                left_hand = OmniHandPro2025.create_hand_socketcan(
                    hand_type=HandType.LEFT,
                    hand_device_id=hand_device_id,
                    can_interface=left_interface
                )
                if left_hand is None or not left_hand.init():
                    print("[Error]: Failed to initialize left hand for dual mode")
                    return 1

            # Joint-angle control
            print("\nSetting joint angles for both hands...")
            left_angles = [0.0] * 12
            right_angles = [0.5] * 12

            left_hand.set_all_active_joint_angles(left_angles)
            right_hand.set_all_active_joint_angles(right_angles)

            time.sleep(1.0)

            left_angles_read = left_hand.get_all_active_joint_angles()
            right_angles_read = right_hand.get_all_active_joint_angles()

            print(f"Left Hand Angles (rad, {left_interface}): {[f'{a:.4f}' for a in left_angles_read]}")
            print(f"Right Hand Angles (rad, {right_interface}): {[f'{a:.4f}' for a in right_angles_read]}")

    print("\n[Done]: Example completed successfully!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
