# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
Tests for OmniHand 2025 (O10) via ZLG CAN over TCP.

Run when ZLG WiFi转CANFD is available as TCP server (e.g. 192.168.0.178:8000):

  OMNIHAND_TCP_HOST=192.168.0.178 OMNIHAND_TCP_PORT=8000 python test_omnihand_2025_zlgcan_tcp.py
  pytest test_omnihand_2025_zlgcan_tcp.py -v
"""

import os
import sys

try:
    import pytest
except ImportError:
    print("Python tests require pytest. Install with: pip install pytest")
    sys.exit(1)

from omnihand import OmniHand2025, HandType, Finger

# Default TCP server (ZLG WiFi adapter as server)
TCP_HOST = os.environ.get("OMNIHAND_TCP_HOST", "192.168.0.178")
TCP_PORT = int(os.environ.get("OMNIHAND_TCP_PORT", "8000"))


@pytest.fixture
def hand():
    """Create OmniHand 2025 via ZLG CAN over TCP."""
    hand = OmniHand2025.create_hand_by_zlgcan_tcp(
        hand_type=HandType.LEFT,
        hand_device_id=OmniHand2025.kDefaultHandDeviceId,
        host=TCP_HOST,
        port=TCP_PORT,
        canfd_channel_id=0,
    )
    if hand is None:
        pytest.skip(f"Cannot connect to TCP server {TCP_HOST}:{TCP_PORT}")
    yield hand


def test_create_hand_tcp(hand):
    """Test factory create_hand_by_zlgcan_tcp."""
    assert hand is not None


def test_init_tcp(hand):
    """Test init (requires TCP server and hand on CAN bus)."""
    result = hand.init()
    if not result:
        pytest.skip("Init failed (no device or TCP server unreachable)")


def test_get_vendor_info_tcp(hand):
    """Test get_vendor_info via TCP."""
    if not hand.init():
        pytest.skip("Device not initialized")
    vendor_info = hand.get_vendor_info()
    print(f"\nVendor: {vendor_info}")
    assert vendor_info.dof == 10


def test_get_device_info_tcp(hand):
    """Test get_device_info via TCP."""
    if not hand.init():
        pytest.skip("Device not initialized")
    device_info = hand.get_device_info()
    print(f"\nDevice: hand_device_id={device_info.hand_device_id}")


def test_get_all_joint_positions_tcp(hand):
    """Test get_all_joint_positions via TCP."""
    if not hand.init():
        pytest.skip("Device not initialized")
    positions = hand.get_all_joint_positions()
    if not positions:
        pytest.skip("Failed to get positions")
    print(f"\nPositions: {positions}")
    assert len(positions) == 10


def test_set_get_angles_tcp(hand):
    """Test set_all_active_joint_angles / get_all_active_joint_angles via TCP."""
    if not hand.init():
        pytest.skip("Device not initialized")
    angles = [0.0] * 10
    hand.set_all_active_joint_angles(angles)
    import time
    time.sleep(0.3)
    read_back = hand.get_all_active_joint_angles()
    if not read_back:
        pytest.skip("Failed to get angles")
    assert len(read_back) == 10


def test_tactile_sensor_tcp(hand):
    """Test tactile sensor read via TCP (optional)."""
    if not hand.init():
        pytest.skip("Device not initialized")
    data = hand.get_tactile_sensor_data(Finger.THUMB)
    print(f"\nThumb tactile (len={len(data)}): {data[:8]}...")
    assert isinstance(data, list)


if __name__ == "__main__":
    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    pytest.main([__file__] + args)
