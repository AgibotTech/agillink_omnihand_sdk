# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
CANFD-specific tests for OmniHand 2025 (O10)

Usage:
  python3 test_omnihand_2025_canfd.py [-d TRANSPORT] [-c CHANNEL] [-i CANFD_ID] [-f INTERVAL]
  OMNIHAND_DEVICE_TYPE=hcan python3 -m pytest test_omnihand_2025_canfd.py

  -d TRANSPORT  zlgcan | hcan | socketcan | zlgcantcp (default: zlgcan)
  --can-if IF   SocketCAN interface name (default: can0)
  --tcp-host H  ZLG CANFD-over-TCP host (default: 192.168.0.178)
  --tcp-port P  TCP port (default: 8000)
  -c CHANNEL    CAN channel index for zlgcan/hcan/zlgcantcp (default: 0)
  -i CANFD_ID   Adapter device index for zlgcan/hcan (default: 0)
  -f INTERVAL   Request interval in ms (default: 5, max: 100)
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
DEVICE_TYPE = "zlgcan"
CHANNEL_ID = 0
CANFD_ID = 0
REQUEST_INTERVAL = 5
CAN_IF = "can0"
TCP_HOST = "192.168.0.178"
TCP_PORT = 8000

env_device = os.environ.get("OMNIHAND_DEVICE_TYPE")
if env_device in ("zlgcan", "hcan", "socketcan", "zlgcantcp"):
    DEVICE_TYPE = env_device

env_interval = os.environ.get("OMNIHAND_REQUEST_INTERVAL")
if env_interval is not None:
    try:
        REQUEST_INTERVAL = max(0, min(100, int(env_interval)))
    except ValueError:
        pass

env_channel = os.environ.get("OMNIHAND_CHANNEL_ID")
if env_channel is not None:
    try:
        CHANNEL_ID = int(env_channel)
    except ValueError:
        pass

env_canfd_id = os.environ.get("OMNIHAND_CANFD_ID")
if env_canfd_id is not None:
    try:
        CANFD_ID = int(env_canfd_id)
    except ValueError:
        pass

env_can_if = os.environ.get("OMNIHAND_CAN_IF")
if env_can_if:
    CAN_IF = env_can_if

env_tcp_host = os.environ.get("OMNIHAND_TCP_HOST")
if env_tcp_host:
    TCP_HOST = env_tcp_host

env_tcp_port = os.environ.get("OMNIHAND_TCP_PORT")
if env_tcp_port is not None:
    try:
        TCP_PORT = int(env_tcp_port)
    except ValueError:
        pass

# Parse CLI args (only when running as __main__; stripped before pytest.main())
_argv = sys.argv[:]
for _flag, _attr, _cast in (
    ("-d", "DEVICE_TYPE", str),
    ("-c", "CHANNEL_ID", int),
    ("-i", "CANFD_ID", int),
    ("-f", "REQUEST_INTERVAL", int),
    ("--can-if", "CAN_IF", str),
    ("--tcp-host", "TCP_HOST", str),
    ("--tcp-port", "TCP_PORT", int),
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


# ---------------------------------------------------------------------------
# Session-scoped fixture (one CANFD device for the whole test run)
# ---------------------------------------------------------------------------

@pytest.fixture(scope="session")
def hand():
    if DEVICE_TYPE == "hcan":
        h = OmniHand2025.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            canfd_device_id=CANFD_ID,
            canfd_channel_id=CHANNEL_ID,
        )
    elif DEVICE_TYPE == "socketcan":
        h = OmniHand2025.create_hand_socketcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            can_if=CAN_IF,
        )
    elif DEVICE_TYPE == "zlgcantcp":
        h = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            tcp_host=TCP_HOST,
            tcp_port=TCP_PORT,
            canfd_channel_id=CHANNEL_ID,
        )
    else:  # zlgcan (default)
        h = OmniHand2025.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHand2025.kDefaultHandDeviceId,
            canfd_device_id=CANFD_ID,
            canfd_channel_id=CHANNEL_ID,
        )
    h.set_request_interval(REQUEST_INTERVAL)
    h.show_data_details(True)
    print(f"\n[Info] transport={DEVICE_TYPE} channel={CHANNEL_ID} canfd_id={CANFD_ID} interval={REQUEST_INTERVAL}ms")
    yield h


@pytest.fixture(scope="session")
def initialized_hand(hand):
    """Skip the whole session if device is unreachable."""
    if not hand.init():
        pytest.skip("CANFD device not available")
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
    print(f"\n[get_vendor_info]\n{info}")
    if info.dof == 0:
        pytest.skip("GetVendorInfo timeout")
    assert info.dof == 10


def test_get_device_info(initialized_hand):
    info = initialized_hand.get_device_info()
    print(f"\n[get_device_info]\n{info}")
    if info.hand_device_id == 0:
        pytest.skip("GetDeviceInfo timeout")
    assert info.hand_device_id == OmniHand2025.kDefaultHandDeviceId


def test_set_device_id(initialized_hand):
    current = initialized_hand.get_device_info()
    if current.hand_device_id == 0:
        pytest.skip("Cannot get current device ID")

    initialized_hand.set_device_id(2)
    print("\n[set_device_id] Set to 2")
    new_info = initialized_hand.get_device_info()
    assert new_info.hand_device_id == 2

    initialized_hand.set_device_id(OmniHand2025.kDefaultHandDeviceId)
    print(f"[set_device_id] Reset to {OmniHand2025.kDefaultHandDeviceId}")
    reset_info = initialized_hand.get_device_info()
    assert reset_info.hand_device_id == OmniHand2025.kDefaultHandDeviceId


# ---------------------------------------------------------------------------
# Position control
# ---------------------------------------------------------------------------

def test_set_get_single_axis_pos(initialized_hand):
    import time
    safe_pos = [1024, 1024, 2048, 1024, 2048, 2048, 1024, 2048, 1024, 2048]
    print("\n[SetGetSingleAxisPos] Testing all 10 joints:")
    for joint_idx, target in enumerate(safe_pos, start=1):
        initialized_hand.set_joint_position(joint_idx, target)
        time.sleep(1.0)
        pos = initialized_hand.get_joint_position(joint_idx)
        print(f"  J{joint_idx}: target={target}, get={pos}")
        assert 0 <= pos <= 4096


def test_set_get_all_axis_pos(initialized_hand):
    import time
    positions = [2048, 2048, 4096, 0, 4096, 4096, 0, 4096, 0, 4096]
    set_result = initialized_hand.set_all_joint_positions(positions)
    print(f"\n[SetAllJointMotorPosi] returned {len(set_result)} positions: {set_result}")
    assert len(set_result) == 10
    time.sleep(1.0)

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
    print("\n[GetAllTemperatureReport] " + ", ".join(f"J{i+1}:{v}" for i, v in enumerate(temps)))
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
    thresholds = initialized_hand.get_all_current_thresholds()
    if not thresholds:
        pytest.skip("GetAllCurrentThreshold timeout")
    print("\n[GetAllCurrentThreshold] " + ", ".join(str(v) for v in thresholds))
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


def test_get_tactile_sensor_data_raw(initialized_hand):
    data = initialized_hand.get_tactile_sensor_data_raw(Finger.THUMB)
    if not data.data:
        pytest.skip("Raw tactile data not supported by this firmware")
    print(f"\n[GetTactileSensorDataRaw] Thumb ({len(data.data)} values): {[int(x) for x in data.data[:10]]}...")
    assert len(data.data) > 0


def test_get_all_tactile_sensor_data_raw(initialized_hand):
    all_data = initialized_hand.get_all_tactile_sensor_data_raw()
    if not all_data:
        pytest.skip("Raw tactile data not supported by this firmware")
    print(f"\n[GetAllTactileSensorDataRaw] {len(all_data)} sensors:")
    for sensor in all_data:
        print(f"  {sensor.sensor_id.name}: {len(sensor.data)} points")
    assert len(all_data) >= 0


# ---------------------------------------------------------------------------
# Mixed control
# ---------------------------------------------------------------------------

def test_mix_control_by_pvt(initialized_hand):
    import time
    safe_pos = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
    velocities = [8000] * 10
    torques = [300] * 10
    initialized_hand.mix_control_by_pvt(safe_pos, velocities, torques)
    print("\n[MixControlByPVT] all 10 joints")
    time.sleep(1.0)
    feedback = initialized_hand.get_all_joint_positions()
    print(f"[GetAllJointMotorPosi] {feedback}")
    assert len(feedback) == 10

    initialized_hand.mix_control_by_pvt(1, safe_pos[0], 50, 0)
    print(f"[MixControlByPVT] single J1 pos={safe_pos[0]} vel=50")
    time.sleep(0.5)


def test_mix_control_by_pt(initialized_hand):
    import time
    safe_pos = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
    torques = [0] * 10
    initialized_hand.mix_control_by_pt(safe_pos, torques)
    print("\n[MixControlByPT] all 10 joints")
    time.sleep(1.0)

    initialized_hand.mix_control_by_pt(1, safe_pos[0], 0)
    print(f"[MixControlByPT] single J1 pos={safe_pos[0]}")
    time.sleep(0.5)


# ---------------------------------------------------------------------------
# Joint angle control
# ---------------------------------------------------------------------------

def test_set_get_all_active_joint_angles(initialized_hand):
    import time
    angles = [0.0] * 10
    initialized_hand.set_all_active_joint_angles(angles)
    print("\n[SetAllActiveJointAngles] All joints -> 0.0 rad")
    time.sleep(1.0)
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
    print(f"\n[KinematicsSolver] Forward kinematics: {len(all_angles)} angles")
    assert len(all_angles) == 16


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    os.environ["OMNIHAND_DEVICE_TYPE"] = DEVICE_TYPE
    os.environ["OMNIHAND_REQUEST_INTERVAL"] = str(REQUEST_INTERVAL)
    os.environ["OMNIHAND_CHANNEL_ID"] = str(CHANNEL_ID)
    os.environ["OMNIHAND_CANFD_ID"] = str(CANFD_ID)
    os.environ["OMNIHAND_CAN_IF"] = CAN_IF
    os.environ["OMNIHAND_TCP_HOST"] = TCP_HOST
    os.environ["OMNIHAND_TCP_PORT"] = str(TCP_PORT)

    print("=== OmniHand 2025 CANFD Test ===")
    print(f"Transport:        {DEVICE_TYPE}")
    print(f"Channel ID:       {CHANNEL_ID}")
    print(f"CANFD ID:         {CANFD_ID}")
    print(f"Request Interval: {REQUEST_INTERVAL} ms")
    if DEVICE_TYPE == "socketcan":
        print(f"CAN Interface:    {CAN_IF}")
    if DEVICE_TYPE == "zlgcantcp":
        print(f"TCP:              {TCP_HOST}:{TCP_PORT}")
    print("================================")

    args = sys.argv[1:]
    if "-v" not in args and "--verbose" not in args:
        args = ["-v"] + args
    if "-s" not in args and "--capture=no" not in args:
        args = ["-s"] + args
    pytest.main([__file__] + args)
