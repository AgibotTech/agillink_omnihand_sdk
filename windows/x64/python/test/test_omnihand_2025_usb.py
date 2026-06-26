# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
USB-specific tests for OmniHand 2025 (O10)

Usage:
  python3 test_omnihand_2025_usb.py [-p PORT] [-b BAUDRATE] [-f INTERVAL] [-t TIMEOUT]
  OMNIHAND_USB_PORT=/dev/ttyACM0 python3 -m pytest test_omnihand_2025_usb.py

  -p PORT      USB serial port (default: /dev/ttyACM0, Windows: COM3)
  -b BAUDRATE  Baudrate (default: 460800)
  -f INTERVAL  Request interval in ms (default: 500, max: 500)
  -t TIMEOUT   Frame receive timeout in ms (default: 200)
"""

import sys
import os

parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if parent_dir in sys.path:
    sys.path.remove(parent_dir)

try:
    import pytest
except ImportError:
    print("Python tests require pytest. Install with: pip install pytest")
    sys.exit(1)

from omnihand import OmniHand2025, HandType, Finger

# ---------------------------------------------------------------------------
# Configuration (env vars take priority, then CLI args parsed below)
# ---------------------------------------------------------------------------
import platform
USB_PORT = "COM3" if platform.system() == "Windows" else "/dev/ttyACM0"
BAUDRATE = 460800
REQUEST_INTERVAL = 500  # USB default: 500ms
FRAME_RECV_TIMEOUT = 200

env_port = os.environ.get("OMNIHAND_USB_PORT")
if env_port:
    USB_PORT = env_port

env_baud = os.environ.get("OMNIHAND_BAUDRATE")
if env_baud is not None:
    try:
        BAUDRATE = int(env_baud)
    except ValueError:
        pass

env_interval = os.environ.get("OMNIHAND_REQUEST_INTERVAL")
if env_interval is not None:
    try:
        REQUEST_INTERVAL = max(0, min(500, int(env_interval)))
    except ValueError:
        pass

env_timeout = os.environ.get("OMNIHAND_FRAME_RECV_TIMEOUT")
if env_timeout is not None:
    try:
        FRAME_RECV_TIMEOUT = int(env_timeout)
    except ValueError:
        pass

# Parse CLI args (only when running as __main__; stripped before pytest.main())
_argv = sys.argv[:]
for _flag, _attr, _cast in (
    ("-p", "USB_PORT", str),
    ("-b", "BAUDRATE", int),
    ("-f", "REQUEST_INTERVAL", int),
    ("-t", "FRAME_RECV_TIMEOUT", int),
):
    if _flag in _argv:
        _idx = _argv.index(_flag)
        if _idx + 1 < len(_argv):
            try:
                globals()[_attr] = _cast(_argv[_idx + 1])
            except ValueError:
                pass
            _argv.pop(_idx)
            _argv.pop(_idx)
sys.argv = _argv
del _argv

REQUEST_INTERVAL = max(0, min(500, REQUEST_INTERVAL))


# ---------------------------------------------------------------------------
# Session-scoped fixture (one USB device for the whole test run)
# ---------------------------------------------------------------------------

@pytest.fixture(scope="session")
def hand():
    try:
        h = OmniHand2025.create_hand_by_usb(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            uart_port=USB_PORT,
            baudrate=BAUDRATE,
        )
    except Exception as e:
        pytest.skip(f"Failed to open USB port {USB_PORT}: {e}")
        return
    h.set_request_interval(REQUEST_INTERVAL)
    h.set_frame_recv_timeout(FRAME_RECV_TIMEOUT)
    h.show_data_details(True)
    print(f"\n[Info] port={USB_PORT} baudrate={BAUDRATE} interval={REQUEST_INTERVAL}ms timeout={FRAME_RECV_TIMEOUT}ms")
    yield h


@pytest.fixture(scope="session")
def initialized_hand(hand):
    """Skip the whole session if device is unreachable."""
    if not hand.init():
        pytest.skip("USB device not available")
    yield hand


# ---------------------------------------------------------------------------
# Basic connection
# ---------------------------------------------------------------------------

def test_create_hand(hand):
    assert hand is not None


def test_init(initialized_hand):
    assert initialized_hand.init()


# ---------------------------------------------------------------------------
# Vendor / device info
# ---------------------------------------------------------------------------

def test_get_vendor_info(initialized_hand):
    info = initialized_hand.get_vendor_info()
    print(f"\n[GetVendorInfo]\n{info}")
    if info.dof == 0:
        pytest.skip("GetVendorInfo timeout")
    assert info.dof == 10


def test_get_device_info(initialized_hand):
    info = initialized_hand.get_device_info()
    print(f"\n[GetDeviceInfo]\n{info}")
    # USB learns device ID from first received frame
    assert info.hand_device_id != 0


# ---------------------------------------------------------------------------
# Position control
# ---------------------------------------------------------------------------

def test_set_get_single_axis_pos(initialized_hand):
    import time
    safe_pos = [1024, 1024, 2048, 1024, 2048, 2048, 1024, 2048, 1024, 2048]
    print("\n[SetGetSingleAxisPos] Testing all 10 joints:")
    for joint_idx, target in enumerate(safe_pos, start=1):
        initialized_hand.set_joint_position(joint_idx, target)
        pos = initialized_hand.get_joint_position(joint_idx)
        print(f"  J{joint_idx}: set={target}, get={pos}")
        assert 0 <= pos <= 4096


def test_set_get_all_axis_pos(initialized_hand):
    positions = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
    set_result = initialized_hand.set_all_joint_positions(positions)
    print(f"\n[SetAllJointMotorPosi] returned {len(set_result)} positions: {set_result}")
    assert len(set_result) == 10

    get_result = initialized_hand.get_all_joint_positions()
    print(f"[GetAllJointMotorPosi] returned {len(get_result)} positions: {get_result}")
    assert len(get_result) == 10


# ---------------------------------------------------------------------------
# Current / velocity / temperature / error
# ---------------------------------------------------------------------------

def test_get_all_current_report(initialized_hand):
    currents = initialized_hand.get_all_current_reports()
    if not currents:
        pytest.skip("GetAllCurrentReport timeout")
    print("\n[GetAllCurrentReport] " + ", ".join(f"J{i+1}:{v}mA" for i, v in enumerate(currents)))
    assert len(currents) == 10


def test_get_all_temperature_report(initialized_hand):
    temps = initialized_hand.get_all_temperature_reports()
    if not temps:
        pytest.skip("GetAllTemperatureReport timeout")
    print("\n[GetAllTemperatureReport] " + ", ".join(f"J{i+1}:{v}degC" for i, v in enumerate(temps)))
    assert len(temps) == 10
    for t in temps:
        assert -40 <= t <= 127


def test_get_all_error_report(initialized_hand):
    errors = initialized_hand.get_all_error_reports()
    if not errors:
        pytest.skip("GetAllErrorReport timeout")
    flags = {
        "stalled": "S", "overheat": "H",
        "over_current": "C", "motor_except": "M", "commu_except": "X",
    }
    parts = []
    for i, e in enumerate(errors):
        f = "".join(v for k, v in flags.items() if getattr(e, k, False))
        parts.append(f"J{i+1}:[{f}]")
    print("\n[GetAllErrorReport] " + " ".join(parts))
    assert len(errors) == 10


def test_get_all_joint_motor_velo(initialized_hand):
    velos = initialized_hand.get_all_joint_velocities()
    if not velos:
        pytest.skip("GetAllJointMotorVelo timeout")
    print("\n[GetAllJointMotorVelo] " + ", ".join(str(v) for v in velos))
    assert len(velos) == 10


def test_get_all_current_threshold(initialized_hand):
    # USB returns cached values (no GET command on wire)
    thresholds = initialized_hand.get_all_current_thresholds()
    print(f"\n[GetAllCurrentThreshold] (cached) {thresholds}")
    assert len(thresholds) == 10


# ---------------------------------------------------------------------------
# Tactile sensors
# ---------------------------------------------------------------------------

def test_get_tactile_sensor_data(initialized_hand):
    fingers = [Finger.THUMB, Finger.INDEX, Finger.MIDDLE, Finger.RING, Finger.LITTLE]
    print("\n[GetTactileSensorData] Fingers (16 values each):")
    for finger in fingers:
        data = initialized_hand.get_tactile_sensor_data(finger)
        print(f"  {finger.name}: {list(data)}")
        assert len(data) == 16

    print("[GetTactileSensorData] Palm/Dorsum (25 values each):")
    for sensor in (Finger.PALM, Finger.DORSUM):
        data = initialized_hand.get_tactile_sensor_data(sensor)
        print(f"  {sensor.name}: {list(data)}")
        assert len(data) == 25


# ---------------------------------------------------------------------------
# Mixed control
# ---------------------------------------------------------------------------

def test_mix_control_by_pvt(initialized_hand):
    safe_pos = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
    velocities = [50] * 10
    torques = [0] * 10
    initialized_hand.mix_control_by_pvt(safe_pos, velocities, torques)
    print("\n[MixControlByPVT] all 10 joints")
    feedback = initialized_hand.get_all_joint_positions()
    print(f"[GetAllJointMotorPosi] After mixed control: {feedback}")
    assert len(feedback) == 10


def test_mix_control_by_pt(initialized_hand):
    safe_pos = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
    torques = [0] * 10
    initialized_hand.mix_control_by_pt(safe_pos, torques)
    print("\n[MixControlByPT] all 10 joints")


# ---------------------------------------------------------------------------
# Joint angle control
# ---------------------------------------------------------------------------

def test_set_get_all_active_joint_angles(initialized_hand):
    angles = [0.0] * 10
    initialized_hand.set_all_active_joint_angles(angles)
    print("\n[SetAllActiveJointAngles] All joints -> 0.0 rad")
    current = initialized_hand.get_all_active_joint_angles()
    if not current:
        pytest.skip("GetAllActiveJointAngles timeout")
    print(f"[GetAllActiveJointAngles] {[f'{v:.4f}' for v in current]}")
    assert len(current) == 10


def test_get_all_joint_angles(initialized_hand):
    all_angles = initialized_hand.get_all_joint_angles()
    if not all_angles:
        pytest.skip("GetAllJointAngles timeout")
    print(f"\n[GetAllJointAngles] ({len(all_angles)} joints): {[f'{v:.4f}' for v in all_angles]}")
    assert len(all_angles) == 16


# ---------------------------------------------------------------------------
# Kinematics solver
# ---------------------------------------------------------------------------

def test_kinematics_solver(initialized_hand):
    active_angles = [0.0] * 10
    all_angles = initialized_hand.get_all_joint_angles(active_angles)
    print(f"\n[KinematicsSolver] Forward kinematics: {[f'{v:.4f}' for v in all_angles]}")
    assert len(all_angles) == 16


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    os.environ["OMNIHAND_USB_PORT"] = USB_PORT
    os.environ["OMNIHAND_BAUDRATE"] = str(BAUDRATE)
    os.environ["OMNIHAND_REQUEST_INTERVAL"] = str(REQUEST_INTERVAL)
    os.environ["OMNIHAND_FRAME_RECV_TIMEOUT"] = str(FRAME_RECV_TIMEOUT)

    print("=== OmniHand 2025 USB Test ===")
    print(f"Port:             {USB_PORT}")
    print(f"Baudrate:         {BAUDRATE}")
    print(f"Request Interval: {REQUEST_INTERVAL} ms")
    print(f"Frame Timeout:    {FRAME_RECV_TIMEOUT} ms")
    print("==============================")

    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    if "-s" not in args and "--capture=no" not in args:
        args = ["-s"] + args
    pytest.main([__file__] + args)
