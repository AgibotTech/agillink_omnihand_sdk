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
import threading
import time
from omnihand import OmniHandDexUMI, EHandType, EFinger

# Callback test variables
position_report_count = 0
tactile_sensor_report_count = 0
callback_mutex = threading.Lock()
last_position_data = []
last_tactile_data = None
last_tactile_sensor_id = 0


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
    
    # Cleanup: unregister callbacks
    if hand:
        hand.set_position_report_callback(None)
        hand.set_tactile_sensor_report_callback(None)


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
        print(vendor_info.to_string())
        assert vendor_info.dof == 10  # UMI has 10 DOF


def test_get_device_info(hand):
    """Test device info"""
    if hand.init():
        device_info = hand.get_device_info()
        print(f"\n[get_device_info] Device Info:")
        print(device_info.to_string())
        # Only check deviceId if request succeeded (non-zero indicates success)
        if device_info.hand_device_id != 0:
            assert device_info.hand_device_id == 1
        
        # UMI-specific device info fields
        if device_info.position_report_frequency is not None:
            print(f"  Position Report Frequency: {device_info.position_report_frequency} Hz")
        if device_info.tactile_sensor_report_frequency is not None:
            print(f"  Tactile Sensor Report Frequency: {device_info.tactile_sensor_report_frequency} Hz")
        if device_info.adc_channel_count is not None:
            print(f"  ADC Channel Count: {device_info.adc_channel_count}")


def test_tactile_sensor_raw(hand):
    """Test tactile sensor raw data (UMI specific, requires hardware)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test single sensor (1D tactile sensor, Raw API)
    thumb_tactile = hand.get_tactile_sensor_data_raw(EFinger.THUMB)
    print(f"\n[get_tactile_sensor_data_raw] Thumb Tactile Data ({len(thumb_tactile.data)} values): "
          f"{[int(x) for x in thumb_tactile.data[:10]]}... (unit: 1g, max: 255g)")
    assert thumb_tactile.sensor_id == EFinger.THUMB
    
    # Test multiple sensors
    fingers = [EFinger.INDEX, EFinger.MIDDLE, EFinger.RING, EFinger.LITTLE]
    for finger in fingers:
        tactile_data = hand.get_tactile_sensor_data_raw(finger)
        print(f"\n[get_tactile_sensor_data_raw] {finger} Tactile Data ({len(tactile_data.data)} values): "
              f"{[int(x) for x in tactile_data.data[:5]]}...")
        assert tactile_data.sensor_id == finger
    
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


def test_position_report_frequency(hand):
    """Test position report frequency setting (UMI specific)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test setting position report frequency
    test_freq = 50
    hand.set_position_report_frequency(test_freq)
    print(f"\n[set_position_report_frequency] Set to: {test_freq} Hz")
    
    # Reset to default (100 Hz)
    hand.set_position_report_frequency(100)
    print(f"[set_position_report_frequency] Reset to default: 100 Hz")


def test_tactile_sensor_report_frequency(hand):
    """Test tactile sensor report frequency setting (UMI specific)"""
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Test setting tactile sensor report frequency
    test_freq = 50
    hand.set_tactile_sensor_report_frequency(test_freq)
    print(f"\n[set_tactile_sensor_report_frequency] Set to: {test_freq} Hz")
    
    # Reset to default (100 Hz)
    hand.set_tactile_sensor_report_frequency(100)
    print(f"[set_tactile_sensor_report_frequency] Reset to default: 100 Hz")


def test_position_report_callback(hand):
    """Test position report callback (UMI specific)"""
    global position_report_count, last_position_data
    
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Reset counters
    position_report_count = 0
    last_position_data = []
    
    # Register callback
    def position_callback(positions):
        global position_report_count, last_position_data
        with callback_mutex:
            position_report_count += 1
            last_position_data = positions
            print(f"[PositionReportCallback] Received position data ({len(positions)} values): "
                  f"{positions[:5]}... (mV)")
    
    # Set callback with frequency (50 Hz)
    hand.set_position_report_callback(position_callback, 50)
    print(f"\n[set_position_report_callback] Registered callback with frequency: 50 Hz")
    
    # Wait for some reports (2 seconds = ~100 reports at 50 Hz)
    time.sleep(2)
    
    print(f"[PositionReportCallback] Total reports received: {position_report_count}")
    assert position_report_count > 0
    
    # Unregister callback
    hand.set_position_report_callback(None)
    print(f"[set_position_report_callback] Unregistered callback")


def test_tactile_sensor_report_callback(hand):
    """Test tactile sensor report callback (UMI specific)"""
    global tactile_sensor_report_count, last_tactile_data, last_tactile_sensor_id
    
    if not hand.init():
        pytest.skip("Device not initialized")
    
    # Reset counters
    tactile_sensor_report_count = 0
    last_tactile_data = None
    last_tactile_sensor_id = 0
    
    # Register callback
    def tactile_callback(sensor_data, sensor_id):
        global tactile_sensor_report_count, last_tactile_data, last_tactile_sensor_id
        with callback_mutex:
            tactile_sensor_report_count += 1
            last_tactile_data = sensor_data
            last_tactile_sensor_id = sensor_id
            finger_name = {
                EFinger.THUMB: "Thumb",
                EFinger.INDEX: "Index",
                EFinger.MIDDLE: "Middle",
                EFinger.RING: "Ring",
                EFinger.LITTLE: "Little",
                EFinger.PALM: "Palm",
                EFinger.DORSUM: "Dorsum",
            }.get(sensor_data.sensor_id, "Unknown")
            print(f"[TactileSensorReportCallback] Received {finger_name} sensor data "
                  f"(sensor_id: {sensor_id}, data points: {len(sensor_data.data)})")
    
    # Set callback with frequency (50 Hz)
    hand.set_tactile_sensor_report_callback(tactile_callback, 50)
    print(f"\n[set_tactile_sensor_report_callback] Registered callback with frequency: 50 Hz")
    
    # Wait for some reports (2 seconds = ~100 reports at 50 Hz)
    time.sleep(2)
    
    print(f"[TactileSensorReportCallback] Total reports received: {tactile_sensor_report_count}")
    assert tactile_sensor_report_count > 0
    
    # Unregister callback
    hand.set_tactile_sensor_report_callback(None)
    print(f"[set_tactile_sensor_report_callback] Unregistered callback")


if __name__ == "__main__":
    # Default to verbose mode if -v is not already specified
    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    pytest.main([__file__] + args)
