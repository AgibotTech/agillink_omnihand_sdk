# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
Unit tests for OmniHand Dex UMI
"""

import sys
import os

# Ensure we use the installed omnihand package, not the source directory
# Remove parent directory from path to avoid importing from source
parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if parent_dir in sys.path:
    sys.path.remove(parent_dir)

import pytest
from omnihand import OmniHandDexUMI, EHandType, EFinger


@pytest.fixture
def hand():
    """Create and initialize OmniHand Dex UMI instance for testing"""
    hand = OmniHandDexUMI.create_hand_by_zlgcan(
        hand_type=EHandType.LEFT,
        hand_device_id=1,
        canfd_device_id=0,
        canfd_channel_id=0
    )
    yield hand


def test_create_hand(hand):
    """Test factory method"""
    assert hand is not None


def test_init(hand):
    """Test initialization"""
    assert hand.init(), "Failed to initialize device. Check hardware connection."


def test_get_vendor_info(hand):
    """Test vendor info"""
    assert hand.init(), "Failed to initialize device"
    vendor_info = hand.get_vendor_info()
    print(f"\n[get_vendor_info] Vendor Info:")
    print(vendor_info.to_string())
    assert vendor_info.dof == 10  # UMI has 10 DOF


def test_get_device_info(hand):
    """Test device info"""
    assert hand.init(), "Failed to initialize device"
    device_info = hand.get_device_info()
    print(f"\n[get_device_info] Device Info:")
    print(device_info.to_string())
    assert device_info.hand_device_id == 1


def test_tactile_sensor_raw(hand):
    """Test tactile sensor raw data (UMI specific)"""
    assert hand.init(), "Failed to initialize device"
    
    # Test all 6 sensors (UMI has: Thumb, Index, Middle, Ring, Little, Palm - no Dorsum)
    fingers = [
        (EFinger.THUMB, "Thumb"),
        (EFinger.INDEX, "Index"),
        (EFinger.MIDDLE, "Middle"),
        (EFinger.RING, "Ring"),
        (EFinger.LITTLE, "Little"),
        (EFinger.PALM, "Palm"),
    ]
    
    print(f"\n[get_tactile_sensor_data_raw] Reading individual sensors (unit: 1g, max: 255g):")
    for finger, name in fingers:
        tactile_data = hand.get_tactile_sensor_data_raw(finger)
        print(f"  {name} ({len(tactile_data.data)} values): {[int(x) for x in tactile_data.data]}")
        assert tactile_data.sensor_id == finger
    
    # Test getting all tactile sensor data at once
    all_tactile_data = hand.get_all_tactile_sensor_data_raw()
    print(f"\n[get_all_tactile_sensor_data_raw] All {len(all_tactile_data)} sensors:")
    finger_names = {
        EFinger.THUMB: "Thumb",
        EFinger.INDEX: "Index",
        EFinger.MIDDLE: "Middle",
        EFinger.RING: "Ring",
        EFinger.LITTLE: "Little",
        EFinger.PALM: "Palm",
    }
    for sensor in all_tactile_data:
        name = finger_names.get(sensor.sensor_id, "Unknown")
        print(f"  {name} ({len(sensor.data)} values): {[int(x) for x in sensor.data]}")
    assert len(all_tactile_data) >= 0


def test_get_joint_position(hand):
    """Test single joint motor position query (UMI Protocol Pn3=0x13)"""
    assert hand.init(), "Failed to initialize device"
    
    # Test single joint position query for all joints
    print(f"\n[get_joint_position] Testing single joint position query:")
    for joint_idx in range(1, 11):  # joints 1-10
        pos = hand.get_joint_position(joint_idx)
        print(f"  Joint {joint_idx} position: {pos}")


def test_get_all_joint_positions(hand):
    """Test all joint motor positions query (UMI Protocol Pn3=0x13)"""
    assert hand.init(), "Failed to initialize device"
    
    # Test all joint positions query
    positions = hand.get_all_joint_positions()
    print(f"\n[get_all_joint_positions] All joint positions ({len(positions)} values): {positions}")
    
    # Expect 10 joint positions (UMI has 10 DOF)
    assert len(positions) == 10


# Position calibration tests
# Warning: These tests perform actual calibration - use with caution
def test_min_position_calibration(hand):
    '''Test minimum position calibration (UMI Protocol Pn8=0x08)'''
    assert hand.init(), "Failed to initialize device"
    
    # Test all joints calibration (sub-register 0x00)
    hand.set_min_position_calibration()
    print(f"\n[set_min_position_calibration] Minimum position calibration set for all joints")
    
    # Test single joint calibration (sub-register 0x01-0x0A)
    for joint_idx in range(1, 11):
        hand.set_min_position_calibration(joint_idx)
        print(f"[set_min_position_calibration] Minimum position calibration set for joint {joint_idx}")


def test_max_position_calibration(hand):
    '''Test maximum position calibration (UMI Protocol Pn7=0x07)'''
    assert hand.init(), "Failed to initialize device"
    
    # Test all joints calibration (sub-register 0x00)
    hand.set_max_position_calibration()
    print(f"\n[set_max_position_calibration] Maximum position calibration set for all joints")
    
    # Test single joint calibration (sub-register 0x01-0x0A)
    for joint_idx in range(1, 11):
        hand.set_max_position_calibration(joint_idx)
        print(f"[set_max_position_calibration] Maximum position calibration set for joint {joint_idx}")


if __name__ == "__main__":
    # Default to verbose mode if -v is not already specified
    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    pytest.main([__file__] + args)
