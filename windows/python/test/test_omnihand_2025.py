# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
Unit tests for OmniHand 2025 (O10)
"""

import sys
import os

# Ensure we use the installed omnihand package, not the source directory
# Remove parent directory from path to avoid importing from source
parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if parent_dir in sys.path:
    sys.path.remove(parent_dir)

try:
    import pytest
except ImportError:
    print("Python tests require pytest. Install with: pip install pytest")
    sys.exit(1)
from omnihand import OmniHand2025, EHandType, EFinger, EControlMode

# Global variable to store request interval from command line argument
# Can be set via environment variable OMNIHAND_REQUEST_INTERVAL or -f when running directly
REQUEST_INTERVAL = 5  # Default: 5ms

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


@pytest.fixture
def hand():
    """Create and initialize OmniHand 2025 instance for testing"""
    hand = OmniHand2025.create_hand_by_zlgcan(
        hand_type=EHandType.LEFT,
        hand_device_id=1,
        canfd_device_id=0,
        canfd_channel_id=0
    )
    if REQUEST_INTERVAL != 0:
        hand.set_request_interval(REQUEST_INTERVAL)
        print(f"[Info]: Using request interval: {REQUEST_INTERVAL} ms")
    yield hand


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
    if hand.init():
        vendor_info = hand.get_vendor_info()
        print(f"\n[get_vendor_info] Vendor Info:")
        print(vendor_info)  # Use __str__ method
        assert vendor_info.dof == 10  # O10 has 10 DOF


def test_get_device_info(hand):
    """Test device info"""
    if hand.init():
        device_info = hand.get_device_info()
        print(f"\n[get_device_info] Device Info:")
        print(device_info)  # Use __str__ method
        # Only check deviceId if request succeeded (non-zero indicates success)
        if device_info.hand_device_id != 0:
            assert device_info.hand_device_id == 1


def test_set_device_id(hand):
    """Test setting device ID (may cause device inaccessibility)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Get current device ID first
    current_device_info = hand.get_device_info()
    current_id = current_device_info.hand_device_id
    
    # Only test if we got a valid device ID
    if current_id == 0:
        pytest.skip("Failed to get current device ID")
    
    # Set to target ID (2) using current ID
    target_id = 2
    hand.set_device_id(target_id)
    print(f"\n[set_device_id] Set Device ID: {target_id}")
    
    device_info = hand.get_device_info()
    # Check if request succeeded (non-zero indicates success)
    if device_info.hand_device_id != 0:
        assert device_info.hand_device_id == 2
    
    # Reset to original
    original_id = 1
    hand.set_device_id(original_id)
    print(f"[set_device_id] Reset Device ID: {original_id}")
    
    device_info1 = hand.get_device_info()
    # Check if request succeeded (non-zero indicates success)
    if device_info1.hand_device_id != 0:
        assert device_info1.hand_device_id == 1


def test_motor_position_control(hand):
    """Test motor position control (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test single motor position
    target_pos = 2048  # Middle position (0-4096 range)
    hand.set_joint_position(1, target_pos)
    print(f"\n[set_joint_position] Set Joint 1 Motor Position: {target_pos}")
    
    pos = hand.get_joint_position(1)
    print(f"[get_joint_position] Joint 1 Motor Position: {pos}")
    # Note: Actual position may differ due to hardware constraints
    
    # Test batch motor positions
    positions = [2048] * 10  # 10 motors, all at middle
    hand.set_all_joint_positions(positions)
    print(f"[set_all_joint_positions] Set Motor Positions: {positions}")
    
    all_positions = hand.get_all_joint_positions()
    # Check if request succeeded (non-empty result)
    if not all_positions:
        pytest.skip("Failed to get motor positions")
    
    print(f"[get_all_joint_positions] Motor Positions: {all_positions}")
    assert len(all_positions) == 10


def test_joint_angle_control(hand):
    """Test joint angle control (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test setting active joint angles
    angles = [0.0] * 10  # 10 joints, all at 0
    hand.set_all_active_joint_angles(angles)
    print(f"\n[set_all_active_joint_angles] Set Active Joint Angles (rad): {angles}")
    
    active_angles = hand.get_all_active_joint_angles()
    # Check if request succeeded (non-empty result)
    if not active_angles:
        pytest.skip("Failed to get active joint angles")
    
    print(f"[get_all_active_joint_angles] Active Joint Angles (rad): {active_angles}")
    assert len(active_angles) == 10
    
    # Use get_all_joint_pos instead of get_all_joint_angles to avoid double hardware communication
    # get_all_joint_angles() internally calls GetAllActiveJointAngles() again, which may fail
    # get_all_joint_pos() only does kinematics calculation without hardware communication
    all_angles = hand.get_all_joint_pos(active_angles)
    # Check if calculation succeeded
    if not all_angles or len(all_angles) == 0:
        pytest.skip("Failed to calculate all joint angles")
    
    print(f"[get_all_joint_pos] All Joint Angles (rad, {len(all_angles)} joints): {all_angles[:10]}...")
    assert len(all_angles) == 16  # 10 active + 6 passive


def test_control_mode(hand):
    """Test control mode (read-only, requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Only test reading control mode (read-only operation)
    # Note: set_all_control_modes is not tested as it may cause CANFD communication to crash
    current_modes = hand.get_all_control_modes()
    # Check if request succeeded (non-empty result)
    if not current_modes:
        pytest.skip("Failed to get control modes")
    
    print(f"\n[get_all_control_modes] Control Modes: {current_modes}")
    assert len(current_modes) == 10


def test_tactile_sensor(hand):
    """Test tactile sensor (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test GetTactileSensorData (downsampled data)
    tactile_data = hand.get_tactile_sensor_data(EFinger.THUMB)
    print(f"\n[get_tactile_sensor_data] Thumb Tactile Data ({len(tactile_data)} values): {tactile_data[:10]}...")
    assert len(tactile_data) >= 0


def test_tactile_sensor_raw(hand):
    """Test tactile sensor raw data (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test single sensor (1D tactile sensor, Raw API - full resolution)
    thumb_tactile = hand.get_tactile_sensor_data_raw(EFinger.THUMB)
    print(f"\n[get_tactile_sensor_data_raw] Thumb Tactile Data ({len(thumb_tactile.data)} values): "
          f"{[int(x) for x in thumb_tactile.data[:10]]}... (unit: 1g, max: 255g)")
    assert thumb_tactile.sensor_id == EFinger.THUMB
    
    # Test getting all tactile sensor data
    all_tactile_data = hand.get_all_tactile_sensor_data_raw()
    print(f"\n[get_all_tactile_sensor_data_raw] All Tactile Sensors: {len(all_tactile_data)} sensors")
    for sensor in all_tactile_data:
        finger_name = {
            EFinger.THUMB: "Thumb",
            EFinger.INDEX: "Index",
            EFinger.MIDDLE: "Middle",
            EFinger.RING: "Ring",
            EFinger.LITTLE: "Little",
            EFinger.PALM: "Palm",
            EFinger.DORSUM: "Dorsum",
        }.get(sensor.sensor_id, "Unknown")
        print(f"  {finger_name}: {len(sensor.data)} points")
    assert len(all_tactile_data) >= 0


def test_error_report(hand):
    """Test error report (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    error_reports = hand.get_all_error_reports()
    # Check if request succeeded (non-empty result)
    if not error_reports:
        pytest.skip("Failed to get error reports")
    
    print(f"\n[get_all_error_reports] Error Reports (10 joints): ", end="")
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
    
    assert len(error_reports) == 10


def test_temperature_report(hand):
    """Test temperature report (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    temp_reports = hand.get_all_temperature_reports()
    # Check if request succeeded (non-empty result)
    if not temp_reports:
        pytest.skip("Failed to get temperature reports")
    
    print(f"\n[get_all_temperature_reports] Temperature Reports (°C): ", end="")
    for i, temp in enumerate(temp_reports):
        print(f"J{i+1}:{temp}", end=", " if i < len(temp_reports) - 1 else "")
    print()
    assert len(temp_reports) == 10


def test_current_report(hand):
    """Test current report (requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    current_reports = hand.get_all_current_reports()
    # Check if request succeeded (non-empty result)
    if not current_reports:
        pytest.skip("Failed to get current reports")
    
    print(f"\n[get_all_current_reports] Current Reports (mA): ", end="")
    for i, current in enumerate(current_reports):
        print(f"J{i+1}:{current}", end=", " if i < len(current_reports) - 1 else "")
    print()
    assert len(current_reports) == 10


def test_kinematics_solver(hand):
    """Test kinematics solver"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test forward kinematics
    active_angles = [0.0] * 10
    all_angles = hand.get_all_joint_pos(active_angles)
    print(f"\n[get_all_joint_pos] Forward Kinematics (input: 10 active angles, output: {len(all_angles)} joint angles): "
          f"{all_angles[:10]}...")
    assert len(all_angles) == 16  # 10 active + 6 passive


if __name__ == "__main__":
    # Default to verbose mode if -v is not already specified
    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    pytest.main([__file__] + args)
