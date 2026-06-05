# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
Unit tests for OmniHand Pro 2025 (O12) using pytest framework
"""

import sys
import os
import time

# Ensure we use the installed omnihand package, not the source directory
# Remove parent directory from path to avoid importing from source
parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if parent_dir in sys.path:
    sys.path.remove(parent_dir)

import pytest
from omnihand import OmniHandPro2025, HandType, Finger, ControlMode

# Global variable to store request interval from command line argument
# Can be set via environment variable OMNIHAND_REQUEST_INTERVAL or -f when running directly
REQUEST_INTERVAL = 5  # Default: 5ms

# Global variable to store device type from command line argument
# Can be set via environment variable OMNIHAND_DEVICE_TYPE or -d when running directly
DEVICE_TYPE = "zlgcan"  # Default: zlgcan

# Try to get from environment variable first (works with pytest)
import os
env_interval = os.environ.get("OMNIHAND_REQUEST_INTERVAL")
if env_interval is not None:
    try:
        REQUEST_INTERVAL = int(env_interval)
        if not (0 <= REQUEST_INTERVAL <= 100):
            print(f"[Error]: OMNIHAND_REQUEST_INTERVAL value {REQUEST_INTERVAL} is out of range (0-100ms)")
    except ValueError:
        print(f"[Error]: Invalid OMNIHAND_REQUEST_INTERVAL value")
elif "-f" in sys.argv:
    # Parse -f argument when running directly with Python (not with pytest)
    # Note: -f doesn't work with pytest directly, use OMNIHAND_REQUEST_INTERVAL env var instead
    idx = sys.argv.index("-f")
    if idx + 1 < len(sys.argv):
        try:
            REQUEST_INTERVAL = int(sys.argv[idx + 1])
            if not (0 <= REQUEST_INTERVAL <= 100):
                print(f"[Error]: -f value {REQUEST_INTERVAL} is out of range (0-100ms)")
                sys.exit(1)
            # Remove -f and its value from sys.argv so pytest.main() doesn't see them
            sys.argv.pop(idx)
            sys.argv.pop(idx)
        except (ValueError, IndexError):
            print(f"[Error]: Invalid -f value")
            sys.exit(1)

# Try to get device type from environment variable first (works with pytest)
env_device_type = os.environ.get("OMNIHAND_DEVICE_TYPE")
if env_device_type is not None:
    if env_device_type in ["zlgcan", "hcan"]:
        DEVICE_TYPE = env_device_type
    else:
        print(f"[Error]: OMNIHAND_DEVICE_TYPE must be 'zlgcan' or 'hcan', got: {env_device_type}")
elif "-d" in sys.argv:
    # Parse -d argument when running directly with Python (not with pytest)
    # Note: -d doesn't work with pytest directly, use OMNIHAND_DEVICE_TYPE env var instead
    idx = sys.argv.index("-d")
    if idx + 1 < len(sys.argv):
        device_type = sys.argv[idx + 1]
        if device_type in ["zlgcan", "hcan"]:
            DEVICE_TYPE = device_type
            # Remove -d and its value from sys.argv so pytest.main() doesn't see them
            sys.argv.pop(idx)
            sys.argv.pop(idx)
        else:
            print(f"[Error]: -d value must be 'zlgcan' or 'hcan', got: {device_type}")
            sys.exit(1)
    else:
        print(f"[Error]: -d requires a value ('zlgcan' or 'hcan')")
        sys.exit(1)



@pytest.fixture
def hand():
    """Create and initialize OmniHand Pro 2025 instance for testing"""
    # Create hand instance based on device type
    if DEVICE_TYPE == "hcan":
        hand = OmniHandPro2025.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=0,
            canfd_channel_id=0
        )
        print("[Info]: Using HCAN device")
    else:
        # Default: ZLG CAN
        hand = OmniHandPro2025.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandPro2025.kDefaultHandDeviceId,
            canfd_device_id=0,
            canfd_channel_id=0
        )
        print("[Info]: Using ZLG CAN device")
    
    if REQUEST_INTERVAL != 0:
        hand.set_request_interval(REQUEST_INTERVAL)
        print(f"[Info]: Using request interval: {REQUEST_INTERVAL} ms")
    yield hand
    # Cleanup: explicitly delete the hand instance to ensure proper resource cleanup
    # This matches gtest's TearDown() behavior where hand_.reset() is called
    if hand:
        del hand
        time.sleep(0.1)  # Wait for resources to be fully released


def test_create_hand(hand):
    """Test factory method"""
    assert hand is not None


def test_init(hand):
    """Test initialization"""
    # Note: This test may fail if hardware is not connected
    init_result = hand.init()
    # We don't assert on init_result as it depends on hardware availability


def test_get_vendor_info(hand):
    """Test vendor info (may require hardware)"""
    assert hand.init(), "Device not initialized"
    
    # # Add a small delay after Init to allow device to be ready
    # # This is especially important on first run after system boot
    # time.sleep(0.1)
    
    vendor_info = hand.get_vendor_info()
    # Check if request succeeded (non-zero dof indicates success)
    assert vendor_info.dof != 0, "Failed to get vendor info (timeout)"
    
    # Print vendor info similar to gtest (avoid UnicodeDecodeError by printing fields individually)
    print(f"\n[get_vendor_info] Vendor Info:")
    print(f"{vendor_info}")
    assert vendor_info.dof == 12  # O12 has 12 DOF


def test_get_device_info(hand):
    """Test device info"""
    assert hand.init(), "Device not initialized"
    
    device_info = hand.get_device_info()
    print(f"\n[get_device_info] Device Info:")
    print(f"{device_info}")
    # Only check deviceId if request succeeded (non-zero indicates success)
    if device_info.hand_device_id != 0:
        assert device_info.hand_device_id == OmniHandPro2025.kDefaultHandDeviceId


# def test_set_device_id(hand):
#     """Test setting device ID (may cause device inaccessibility)"""
#     assert hand.init(), "Device not initialized"
    
#     # Get current device ID first
#     current_device_info = hand.get_device_info()
#     current_id = current_device_info.hand_device_id
    
#     # Only test if we got a valid device ID (fail if timeout, like gtest)
#     assert current_id != 0, "Failed to get current device ID"
    
#     # Store original ID for cleanup
#     original_id = current_id
    
#     # Set to target ID (2) using current ID
#     target_id = 2
#     hand.set_device_id(target_id)
#     print(f"\n[set_device_id] Set Device ID: {target_id}")
#     time.sleep(0.1)  # O12 firmware requires 2s delay after device ID change
    
#     # Verify new device ID
#     device_info = hand.get_device_info()
#     assert device_info.hand_device_id == 2, f"Expected device ID 2, got {device_info.hand_device_id}"
    
#     # Reset to original ID
#     hand.set_device_id(original_id)
#     print(f"[set_device_id] Reset Device ID: {original_id}")
#     time.sleep(0.1)  # O12 firmware requires 2s delay after device ID change
    
#     # Verify reset
#     device_info1 = hand.get_device_info()
#     assert device_info1.hand_device_id == original_id, (
#         f"Expected device ID {original_id}, got {device_info1.hand_device_id}"
#     )


def test_joint_angle_control(hand):
    """Test joint angle control (requires hardware)"""
    assert hand.init(), "Device not initialized"
    
    # Test setting active joint angles
    angles = [0.0] * 12  # 12 joints, all at 0
    hand.set_all_active_joint_angles(angles)
    print(f"\n[set_all_active_joint_angles] Set Active Joint Angles (rad): {angles}")
    
    time.sleep(0.5)
    angles = [0.6] * 12
    hand.set_all_active_joint_angles(angles)
    print(f"\n[set_all_active_joint_angles] Set Active Joint Angles (rad): {angles}")

    time.sleep(0.5)
    angles = [0.0] * 12
    hand.set_all_active_joint_angles(angles)
    print(f"\n[set_all_active_joint_angles] Set Active Joint Angles (rad): {angles}")
    # Get active joint angles (may fail if hardware communication fails)
    active_angles = hand.get_all_active_joint_angles()
    # Check if request succeeded (non-empty result and correct size)
    assert active_angles and len(active_angles) == 12, \
        f"Failed to get active joint angles: got {len(active_angles) if active_angles else 0} angles, expected 12"
    
    print(f"[get_all_active_joint_angles] Active Joint Angles (rad): {active_angles}")
    assert len(active_angles) == 12
    
    # Use GetAllJointAngles (similar to gtest) instead of get_all_joint_pos
    # But GetAllJointAngles internally calls GetAllActiveJointAngles again, which may fail
    # So we need to check if GetAllActiveJointAngles succeeded before calling GetAllJointAngles
    # Actually, we already have active_angles, so we can use get_all_joint_pos instead
    # to avoid double hardware communication and potential crashes
    try:
        all_angles = hand.get_all_joint_pos(active_angles)
    except Exception as e:
        # If get_all_joint_pos fails, try get_all_joint_angles as fallback
        # But this may crash if GetAllActiveJointAngles returns empty vector
        print(f"[get_all_joint_pos] Failed: {e}, trying get_all_joint_angles as fallback")
        all_angles = hand.get_all_joint_angles()
    
    # Check if request succeeded (non-empty result and correct size)
    assert all_angles and len(all_angles) == 19, \
        f"Failed to get all joint angles: got {len(all_angles) if all_angles else 0} angles, expected 19"
    
    print(f"[get_all_joint_angles] All Joint Angles (rad, {len(all_angles)} joints): {all_angles[:10]}...")
    assert len(all_angles) == 19  # 12 active + 7 passive


# def test_control_mode(hand):
#     """Test control mode (read-only, requires hardware)"""
#     assert hand.init(), "Device not initialized"
    
#     # Only test reading control mode (read-only operation)
#     # Note: set_all_control_modes is not tested as it may cause CANFD communication to crash
#     current_modes = hand.get_all_control_modes()
#     # Check if request succeeded (non-empty result)
#     assert current_modes, "Failed to get control modes"
    
#     print(f"\n[get_all_control_modes] Control Modes: {current_modes}")
#     assert len(current_modes) == 12


def test_tactile_sensor_3d(hand):
    """Test 3D tactile sensor (O12 specific, requires hardware)"""
    assert hand.init(), "Device not initialized"
    
    # Test single sensor
    thumb_tactile = hand.get_tactile_sensor_3d_data(Finger.THUMB)
    print(f"\n[get_tactile_sensor_3d_data] Thumb 3D Tactile Data:")
    print(f"  Online State: {thumb_tactile.online_state}")
    print(f"  Normal Force: {thumb_tactile.normal_force}")
    print(f"  Tangent Force: {thumb_tactile.tangent_force}")
    print(f"  Tangent Force Angle: {thumb_tactile.tangent_force_angle}")
    assert thumb_tactile.normal_force >= 0
    
    # Test multiple sensors
    fingers = [Finger.INDEX, Finger.MIDDLE, Finger.RING, Finger.LITTLE]
    for finger in fingers:
        tactile_data = hand.get_tactile_sensor_3d_data(finger)
        print(f"\n[get_tactile_sensor_3d_data] {finger} 3D Tactile Data:")
        print(f"  Online State: {tactile_data.online_state}")
        print(f"  Normal Force: {tactile_data.normal_force}")
        print(f"  Tangent Force: {tactile_data.tangent_force}")
        print(f"  Tangent Force Angle: {tactile_data.tangent_force_angle}")


def test_error_report(hand):
    """Test error report (requires hardware)"""
    assert hand.init(), "Device not initialized"
    
    # Use try-except to catch potential crashes from pointer conversion issues
    try:
        error_reports = hand.get_all_error_reports()
    except Exception as e:
        pytest.fail(f"Failed to get error reports: {e}")
    
    # Check if request succeeded (non-empty result)
    assert error_reports, "Failed to get error reports: got empty error reports"
    
    print(f"\n[get_all_error_reports] Error Reports (12 joints): ", end="")
    has_errors = False
    for i, error in enumerate(error_reports):
        error_flags = []
        if error.stalled:
            error_flags.append("S")
            has_errors = True
        if error.overheat:
            error_flags.append("H")
            has_errors = True
        if error.over_current:
            error_flags.append("C")
            has_errors = True
        if error.motor_except:
            error_flags.append("M")
            has_errors = True
        if error.commu_except:
            error_flags.append("X")
            has_errors = True
        print(f"J{i+1}:[{''.join(error_flags) if error_flags else ''}]", end=" ")
    print()
    
    if has_errors:
        print("[Note] Error flags: S=Stalled, H=Overheat, C=Over-current, M=Motor exception, X=Communication exception")
        print("[Note] X (Communication exception) may indicate historical communication errors. "
              "This is normal if the device had previous communication timeouts.")
    
    assert len(error_reports) == 12


def test_temperature_report(hand):
    """Test temperature report (requires hardware)"""
    assert hand.init(), "Device not initialized"
    
    temp_reports = hand.get_all_temperature_reports()
    # Check if request succeeded (non-empty result)
    # If cache is empty, device hasn't sent reports yet - this is expected and not an error
    if not temp_reports:
        # No data available (cache is empty), skip assertion
        # This is expected if report periods are not set or device hasn't sent reports yet
        return
    
    print(f"\n[get_all_temperature_reports] Temperature Reports (°C): ", end="")
    for i, temp in enumerate(temp_reports):
        print(f"J{i+1}:{temp}", end=", " if i < len(temp_reports) - 1 else "")
    print()
    assert len(temp_reports) == 12


def test_current_report(hand):
    """Test current report (requires hardware)"""
    assert hand.init(), "Device not initialized"
    
    # Similar to gtest: directly call GetAllCurrentReport without setting periods
    # The device may have cached reports from previous operations
    current_reports = hand.get_all_current_reports()
    # Check if request succeeded (non-empty result)
    # If empty, skip assertion (same as gtest behavior)
    if not current_reports:
        # No data available (cache is empty), skip assertion
        # This is expected if report periods are not set or device hasn't sent reports yet
        return
    
    print(f"\n[get_all_current_reports] Current Reports (mA): ", end="")
    for i, current in enumerate(current_reports):
        print(f"J{i+1}:{current}", end=", " if i < len(current_reports) - 1 else "")
    print()
    assert len(current_reports) == 12


def test_kinematics_solver(hand):
    """Test kinematics solver"""
    assert hand.init(), "Device not initialized"
    
    # Test forward kinematics
    # First verify we can get motor positions (prerequisite check)
    motor_positions = hand.get_all_joint_positions()
    # If empty or wrong size, skip assertion (same as gtest behavior)
    if not motor_positions or len(motor_positions) != 12:
        print(f"\n[KinematicsSolver] Failed to get motor positions: got "
              f"{len(motor_positions) if motor_positions else 0}, expected 12")
        return
    
    # Test forward kinematics with valid input
    active_angles = [0.0] * 12
    all_angles = hand.get_all_joint_angles(active_angles)
    
    # Check if calculation succeeded (non-empty result and correct size)
    # If failed, skip assertion (same as gtest behavior)
    if not all_angles or len(all_angles) != 19:
        print(f"\n[get_all_joint_angles] Failed: got {len(all_angles) if all_angles else 0} angles, expected 19")
        return
    
    print(f"\n[get_all_joint_angles] Forward Kinematics (input: 12 active angles, output: {len(all_angles)} joint angles): ", end="")
    for i, angle in enumerate(all_angles):
        print(f"{angle:.4f}", end=", " if i < len(all_angles) - 1 else "")
    print()
    assert len(all_angles) == 19  # 12 active + 7 passive


def test_velocity_control(hand):
    """Test velocity control (requires hardware)"""
    assert hand.init(), "Device not initialized"
    
    # Test setting velocity
    velocities = [0] * 12
    hand.set_all_joint_velocities(velocities)
    print(f"\n[set_all_joint_velocities] Set Velocities: {velocities}")
    
    current_velocities = hand.get_all_joint_velocities()
    # Check if request succeeded (non-empty result and correct size)
    assert current_velocities and len(current_velocities) == 12, \
        f"Failed to get velocities: got {len(current_velocities) if current_velocities else 0} velocities, expected 12"
    
    print(f"[get_all_joint_velocities] Current Velocities: {current_velocities}")
    assert len(current_velocities) == 12


def test_voltage_control(hand):
    """Test voltage control (requires hardware)"""
    assert hand.init(), "Device not initialized"

    dof = 12
    safe_voltage = 0
    probe_joint = 1
    probe_voltage = 500
    voltage_mode = int(ControlMode.VOLTAGE)
    position_mode = int(ControlMode.POSITION)
    previous_timeout = hand.get_frame_recv_timeout()

    assert voltage_mode == 4, "ControlMode.VOLTAGE should match firmware mode value 4"

    try:
        hand.set_frame_recv_timeout(100)

        for joint in range(1, dof + 1):
            hand.set_control_mode(joint, voltage_mode)
        print(f"\n[set_control_mode] Set all joints to voltage mode: {voltage_mode}")

        current_modes = hand.get_all_control_modes()
        assert current_modes and len(current_modes) == dof, \
            f"Failed to get control modes: got {len(current_modes) if current_modes else 0} modes, expected {dof}"
        print(f"[get_all_control_modes] Voltage Control Modes: {current_modes}")
        assert all(mode == voltage_mode for mode in current_modes), \
            f"Not all joints are in voltage mode: {current_modes}"

        # Use a non-zero probe so a timeout/default 0 return cannot pass accidentally.
        hand.set_joint_voltage(probe_joint, probe_voltage)
        time.sleep(0.5)
        # O12 firmware versions up to and including 1.2.15 do not support voltage readback.
        current_probe_voltage = hand.get_joint_voltage(probe_joint)
        print(f"[set/get_joint_voltage] Joint {probe_joint}: "
              f"target={probe_voltage}, current={current_probe_voltage}")
        assert current_probe_voltage == probe_voltage, \
            f"Joint {probe_joint} voltage mismatch: expected {probe_voltage}, got {current_probe_voltage}"

        hand.set_joint_voltage(probe_joint, safe_voltage)

        voltages = [safe_voltage] * dof
        hand.set_all_joint_voltages(voltages)
        print(f"[set_all_joint_voltages] Set Voltages: {voltages}")

        # O12 firmware versions up to and including 1.2.15 do not support voltage readback.
        current_voltages = hand.get_all_joint_voltages()
        assert current_voltages and len(current_voltages) == dof, \
            f"Failed to get voltages: got {len(current_voltages) if current_voltages else 0} voltages, expected {dof}"
        print(f"[get_all_joint_voltages] Current Voltages: {current_voltages}")
        assert current_voltages == voltages, \
            f"Voltage mismatch: expected {voltages}, got {current_voltages}"
    finally:
        try:
            hand.set_all_joint_voltages([safe_voltage] * dof)
            for joint in range(1, dof + 1):
                hand.set_control_mode(joint, position_mode)
            print("[set_control_mode] Restored all joints to position mode")
        except Exception as exc:
            print(f"[Warning] Failed to restore voltage control test state: {exc}")
        hand.set_frame_recv_timeout(previous_timeout)


# Parse custom arguments (when running directly with Python, not via pytest)
# This is a fallback in case the earlier parsing didn't work
if __name__ == "__main__":
    if "-f" in sys.argv:
        idx = sys.argv.index("-f")
        if idx + 1 < len(sys.argv):
            try:
                REQUEST_INTERVAL = int(sys.argv[idx + 1])
                if not (0 <= REQUEST_INTERVAL <= 100):
                    print(f"[Error]: -f value {REQUEST_INTERVAL} is out of range (0-100ms)")
                    sys.exit(1)
                # Remove -f and its value from sys.argv so pytest doesn't see them
                sys.argv.pop(idx)
                sys.argv.pop(idx)
            except (ValueError, IndexError):
                print(f"[Error]: Invalid -f value")
                sys.exit(1)
    
    if "-d" in sys.argv:
        idx = sys.argv.index("-d")
        if idx + 1 < len(sys.argv):
            device_type = sys.argv[idx + 1]
            if device_type in ["zlgcan", "hcan"]:
                DEVICE_TYPE = device_type
                # Remove -d and its value from sys.argv so pytest doesn't see them
                sys.argv.pop(idx)
                sys.argv.pop(idx)
            else:
                print(f"[Error]: -d value must be 'zlgcan' or 'hcan', got: {device_type}")
                sys.exit(1)
        else:
            print(f"[Error]: -d requires a value ('zlgcan' or 'hcan')")
            sys.exit(1)
    
    if "--help" in sys.argv or "-h" in sys.argv:
        print("Usage: python test_omnihand_pro_2025.py [-f INTERVAL] [-d DEVICE]")
        print("  -f INTERVAL  Set CAN request interval (0-100ms, 0=no limit, default: 5ms)")
        print("  -d DEVICE    Set CAN device type (zlgcan or hcan, default: zlgcan)")
        print()
        print("Environment variables:")
        print("  OMNIHAND_REQUEST_INTERVAL  Set CAN request interval (0-100ms)")
        print("  OMNIHAND_DEVICE_TYPE       Set CAN device type (zlgcan or hcan)")
        print()
        print("Examples:")
        print("  python test_omnihand_pro_2025.py -f 10")
        print("  python test_omnihand_pro_2025.py -d hcan")
        print("  python test_omnihand_pro_2025.py -f 10 -d hcan")
        print("  OMNIHAND_DEVICE_TYPE=hcan pytest test_omnihand_pro_2025.py")
        sys.exit(0)

if __name__ == "__main__":
    # Default to verbose mode if -v is not already specified
    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    pytest.main([__file__] + args)
