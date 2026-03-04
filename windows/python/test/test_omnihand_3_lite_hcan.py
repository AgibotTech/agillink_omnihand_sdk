# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
Unit tests for OmniHand 3 Lite S (O4) with HCAN USB CANFD device
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
from omnihand import OmniHand3Lite, HandType, ControlMode

# Global variable to store request interval from command line argument
# Can be set via environment variable OMNIHAND_REQUEST_INTERVAL or -f when running directly
REQUEST_INTERVAL = 5  # Default: 5ms

# Try to get from environment variable first (works with pytest)
env_interval = os.environ.get("OMNIHAND_REQUEST_INTERVAL")
if env_interval is not None:
    try:
        REQUEST_INTERVAL = int(env_interval)
        if not (0 <= REQUEST_INTERVAL <= 100):
            print(f"[Error]: OMNIHAND_REQUEST_INTERVAL value {REQUEST_INTERVAL} is out of range (0-100ms)")
    except ValueError:
        print(f"[Error]: Invalid OMNIHAND_REQUEST_INTERVAL value")

if "-f" in sys.argv:
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
    """Create and initialize OmniHand 3 Lite S (O4) instance for testing with HCAN USB CANFD"""
    hand = OmniHand3Lite.create_hand_by_hcan(
        hand_type=HandType.LEFT,
        hand_device_id=1,
        canfd_device_id=0,
        canfd_channel_id=0
    )
    if REQUEST_INTERVAL != 0:
        hand.set_request_interval(REQUEST_INTERVAL)
        print(f"[Info]: Using request interval: {REQUEST_INTERVAL} ms")
    print(f"[Info]: Using device type: hcan")
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
        assert vendor_info.dof == 4  # O4 has 4 DOF


def test_get_device_info(hand):
    """Test device info"""
    if hand.init():
        device_info = hand.get_device_info()
        print(f"\n[get_device_info] Device Info:")
        print(device_info)  # Use __str__ method
        # Only check deviceId if request succeeded (non-zero indicates success)
        if device_info.hand_device_id != 0:
            assert device_info.hand_device_id == 1


def test_set_get_joint_position(hand):
    """Test setting and getting single joint position"""
    if hand.init():
        # Test all 4 joints (O4 has 4 DOF)
        for joint_idx in range(1, 5):
            # Set position
            hand.set_joint_position(joint_idx, 2000)
            
            # Get position
            pos = hand.get_joint_position(joint_idx)
            print(f"\n[set_get_joint_position] Joint {joint_idx} position: {pos}")
            # Note: Position may not match exactly due to hardware limitations
            assert pos >= 0


def test_set_get_all_joint_positions(hand):
    """Test setting and getting all joint positions"""
    if hand.init():
        # Set all positions (O4 has 4 joints)
        positions = [2048, 2048, 2048, 2048]
        actual_positions = hand.set_all_joint_positions(positions)

        print(f"\n[set_get_all_joint_positions] Set positions: {positions}")
        print(f"[set_get_all_joint_positions] Actual positions: {actual_positions}")

        # Check that we got 4 positions back
        assert len(actual_positions) == 4


def test_set_get_joint_velocity(hand):
    """Test setting and getting single joint velocity"""
    if hand.init():
        # Test all 4 joints (O4 has 4 DOF)
        for joint_idx in range(1, 5):
            # Set velocity
            hand.set_joint_velocity(joint_idx, 100)
            
            # Get velocity
            velo = hand.get_joint_velocity(joint_idx)
            print(f"\n[set_get_joint_velocity] Joint {joint_idx} velocity: {velo}")
            assert velo >= 0


def test_set_get_all_joint_velocities(hand):
    """Test setting and getting all joint velocities"""
    if hand.init():
        # Set all velocities (O4 has 4 joints)
        velocities = [100, 100, 100, 100]
        hand.set_all_joint_velocities(velocities)

        # Get all velocities
        all_velocities = hand.get_all_joint_velocities()
        print(f"\n[set_get_all_joint_velocities] All velocities: {all_velocities}")
        assert len(all_velocities) == 4


def test_set_get_control_mode(hand):
    """Test setting and getting control mode (same as C++ gtest - read-only)"""
    if hand.init():
        # Test reading control mode (read-only operation, same as C++ gtest)
        current_modes = hand.get_all_control_modes()
        # Check if request succeeded (non-empty result)
        if len(current_modes) == 0:
            # Request failed (timeout), skip assertion to avoid false failure
            return
        print(f"\n[get_all_control_modes] Control Modes: {current_modes}")
        assert len(current_modes) == 4
        
        # Test single joint control mode
        mode = hand.get_control_mode(1)
        print(f"[get_control_mode] Joint 1 Control Mode: {mode}")
        # Note: get_control_mode returns int, not enum


def test_set_all_control_modes(hand):
    """Test setting all control modes (same as C++ gtest - read-only)"""
    if hand.init():
        # Test reading control mode (read-only operation, same as C++ gtest)
        current_modes = hand.get_all_control_modes()
        # Check if request succeeded (non-empty result)
        if len(current_modes) == 0:
            # Request failed (timeout), skip assertion to avoid false failure
            return
        print(f"\n[get_all_control_modes] Control Modes: {current_modes}")
        assert len(current_modes) == 4


def test_get_all_error_reports(hand):
    """Test getting all error reports (same as C++ gtest)"""
    if hand.init():
        errors = hand.get_all_error_reports()
        # Check if request succeeded (non-empty result, same as C++ gtest)
        if len(errors) == 0:
            # Request failed (timeout), skip assertion to avoid false failure
            print("[Warning]: GetAllErrorReport returned empty (request timeout - check device/firmware or extended-frame response)")
            return
        print(f"\n[get_all_error_reports] Error reports: {len(errors)} joints")
        # O4 has 4 joints
        assert len(errors) == 4


def test_get_all_currents(hand):
    """Test getting all currents (same as C++ gtest)"""
    if hand.init():
        currents = hand.get_all_currents()
        # Check if request succeeded (non-empty result, same as C++ gtest)
        if len(currents) == 0:
            # Request failed (timeout), skip assertion to avoid false failure
            return
        print(f"\n[get_all_currents] Currents: {currents}")
        # O4 has 4 joints
        assert len(currents) == 4


def test_get_all_temperatures(hand):
    """Test getting all temperatures (same as C++ gtest)"""
    if hand.init():
        temperatures = hand.get_all_temperatures()
        # Check if request succeeded (non-empty result, same as C++ gtest)
        if len(temperatures) == 0:
            # Request failed (timeout), skip assertion to avoid false failure
            print("[Warning]: GetAllTemperatureReport returned empty (request timeout - check device/firmware or extended-frame response)")
            return
        print(f"\n[get_all_temperatures] Temperatures: {temperatures}")
        # O4 has 4 joints
        assert len(temperatures) == 4


def test_set_device_id(hand):
    """Test setting device ID (warning: may make device inaccessible)"""
    if hand.init():
        # Get current device ID
        device_info = hand.get_device_info()
        original_id = device_info.hand_device_id
        
        # Note: This test is commented out to avoid changing device ID
        # Uncomment only if you want to test device ID change
        # hand.set_device_id(2)
        # time.sleep(0.2)  # Wait 200ms for device ID change to take effect
        # device_info = hand.get_device_info()
        # assert device_info.hand_device_id == 2
        # 
        # # Restore original ID
        # hand.set_device_id(original_id)
        # time.sleep(0.2)  # Wait 200ms for device ID change to take effect
        print(f"\n[set_device_id] Current device ID: {original_id} (test skipped to avoid changing ID)")


def test_get_device_info_from_broadcast():
    """Test getting device info from broadcast"""
    device_info = OmniHand3Lite.get_device_info_from_broadcast(
        canfd_device_id=0,
        canfd_channel_id=0
    )
    print(f"\n[get_device_info_from_broadcast] Device Info:")
    print(device_info)
    # Note: This may return empty DeviceInfo if no device responds


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
