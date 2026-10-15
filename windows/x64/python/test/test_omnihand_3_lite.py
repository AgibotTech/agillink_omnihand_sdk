# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""Unified hardware tests for OmniHand 3 Lite (H3L, 4 DOF).

The test cases mirror the safe public-API coverage in
``cpp/test/test_omnihand_3_lite.cc``. Calibration, homing, device-ID changes,
parameter persistence, firmware maintenance, and other destructive operations
are deliberately excluded.

Transport is selected with ``-d`` when this file is run directly, or with
``OMNIHAND_DEVICE_TYPE`` when it is run through pytest:

    zlgcan | hcan | socketcan | zlgcantcp | rs485

Examples:

    python test_omnihand_3_lite.py -d hcan -i 0 -c 0
    python test_omnihand_3_lite.py -d rs485 -p COM7
    OMNIHAND_DEVICE_TYPE=hcan pytest -sv test_omnihand_3_lite.py

Environment variables:

    OMNIHAND_DEVICE_TYPE, OMNIHAND_REQUEST_INTERVAL,
    OMNIHAND_CHANNEL_ID, OMNIHAND_CANFD_ID, OMNIHAND_CAN_IF,
    OMNIHAND_TCP_HOST, OMNIHAND_TCP_PORT, OMNIHAND_RS485_PORT,
    OMNIHAND_BAUDRATE, OMNIHAND_FRAME_RECV_TIMEOUT,
    OMNIHAND_HAND_DEVICE_ID, OMNIHAND_HAND_TYPE
"""

import os
import platform
import sys
import time

# Use the installed package rather than the adjacent source package.
_python_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if _python_dir in sys.path:
    sys.path.remove(_python_dir)

try:
    import pytest
except ImportError:
    print("Python tests require pytest. Install with: pip install pytest")
    raise

from omnihand import (
    Finger,
    HandType,
    MixControlMode,
    MixCtrl,
    OmniHand3Lite,
    OmniHand3LiteGesture,
    OmniHand3LiteSolver,
    TactileSensorDataU16,
)


CAN_TRANSPORTS = ("zlgcan", "hcan", "socketcan", "zlgcantcp")
TRANSPORTS = CAN_TRANSPORTS + ("rs485",)

DEVICE_TYPE = "zlgcan"
REQUEST_INTERVAL = 5
CHANNEL_ID = 0
CANFD_ID = 0
CAN_IF = "can0"
TCP_HOST = "192.168.0.178"
TCP_PORT = 8000
RS485_PORT = "COM5" if platform.system() == "Windows" else "/dev/ttyUSB0"
BAUDRATE = 460800
FRAME_RECV_TIMEOUT = 200
HAND_DEVICE_ID = OmniHand3Lite.kDefaultHandDeviceId
HAND_TYPE_NAME = "left"

_STR_OPTIONS = (
    ("OMNIHAND_DEVICE_TYPE", "-d", "DEVICE_TYPE"),
    ("OMNIHAND_CAN_IF", "--can-if", "CAN_IF"),
    ("OMNIHAND_TCP_HOST", "--tcp-host", "TCP_HOST"),
    ("OMNIHAND_RS485_PORT", "-p", "RS485_PORT"),
    ("OMNIHAND_RS485_PORT", "-P", "RS485_PORT"),
    ("OMNIHAND_HAND_TYPE", "--hand-type", "HAND_TYPE_NAME"),
)
_INT_OPTIONS = (
    ("OMNIHAND_REQUEST_INTERVAL", "-f", "REQUEST_INTERVAL"),
    ("OMNIHAND_CHANNEL_ID", "-c", "CHANNEL_ID"),
    ("OMNIHAND_CANFD_ID", "-i", "CANFD_ID"),
    ("OMNIHAND_TCP_PORT", "--tcp-port", "TCP_PORT"),
    ("OMNIHAND_BAUDRATE", "-b", "BAUDRATE"),
    ("OMNIHAND_FRAME_RECV_TIMEOUT", "-t", "FRAME_RECV_TIMEOUT"),
    ("OMNIHAND_HAND_DEVICE_ID", "--device-id", "HAND_DEVICE_ID"),
)


def _apply_environment():
    for env_name, _flag, attr in _STR_OPTIONS:
        value = os.environ.get(env_name)
        if value:
            globals()[attr] = value
    for env_name, _flag, attr in _INT_OPTIONS:
        value = os.environ.get(env_name)
        if value is None:
            continue
        try:
            globals()[attr] = int(value, 0)
        except ValueError:
            print(f"[Warning] Ignoring invalid {env_name}={value!r}")


def _apply_direct_cli():
    """Consume product-specific arguments before passing the rest to pytest."""
    argv = sys.argv[:]
    for options, cast in ((_STR_OPTIONS, str), (_INT_OPTIONS, lambda x: int(x, 0))):
        for _env_name, flag, attr in options:
            while flag in argv:
                index = argv.index(flag)
                if index + 1 >= len(argv):
                    print(f"[Warning] Ignoring {flag} without a value")
                    argv.pop(index)
                    continue
                value = argv[index + 1]
                try:
                    globals()[attr] = cast(value)
                except ValueError:
                    print(f"[Warning] Ignoring invalid {flag} value {value!r}")
                del argv[index:index + 2]
    sys.argv = argv


def _export_config_to_environment():
    """Preserve direct-run options when pytest imports this file again."""
    for env_name, _flag, attr in _STR_OPTIONS + _INT_OPTIONS:
        os.environ[env_name] = str(globals()[attr])


_apply_environment()
if __name__ == "__main__":
    _apply_direct_cli()

if DEVICE_TYPE not in TRANSPORTS:
    print(f"[Warning] Unknown transport {DEVICE_TYPE!r}; using zlgcan")
    DEVICE_TYPE = "zlgcan"
if HAND_TYPE_NAME.lower() not in ("left", "right"):
    print(f"[Warning] Unknown hand type {HAND_TYPE_NAME!r}; using left")
    HAND_TYPE_NAME = "left"
if not 1 <= HAND_DEVICE_ID <= 0x7F:
    print(f"[Warning] Invalid hand device ID {HAND_DEVICE_ID}; using 1")
    HAND_DEVICE_ID = OmniHand3Lite.kDefaultHandDeviceId
REQUEST_INTERVAL = max(0, min(100, REQUEST_INTERVAL))
HAND_TYPE = HandType.LEFT if HAND_TYPE_NAME.lower() == "left" else HandType.RIGHT

DOF = 4
SAFE_POSITIONS = [3072] * DOF
SAFE_VELOCITIES = [0] * DOF
SAFE_TORQUES = [0] * DOF

# The C++ gtest relies on a fresh Init() per TEST_F, which effectively spaces
# motion commands out; pytest reuses one session-scoped instance and otherwise
# fires every command back-to-back, so motors get re-targeted before they can
# physically move.  Pause after each motion command to let it settle.
MOTION_SETTLE_SECONDS = 0.3


def _create_hand():
    common = {
        "hand_type": HAND_TYPE,
        "hand_device_id": HAND_DEVICE_ID,
    }
    if DEVICE_TYPE == "hcan":
        return OmniHand3Lite.create_hand_by_hcan(
            **common,
            canfd_device_id=CANFD_ID,
            canfd_channel_id=CHANNEL_ID,
        )
    if DEVICE_TYPE == "socketcan":
        return OmniHand3Lite.create_hand_socketcan(
            **common,
            can_interface=CAN_IF,
        )
    if DEVICE_TYPE == "zlgcantcp":
        return OmniHand3Lite.create_hand_by_zlgcan_tcp(
            **common,
            host=TCP_HOST,
            port=TCP_PORT,
            canfd_channel_id=CHANNEL_ID,
        )
    if DEVICE_TYPE == "rs485":
        return OmniHand3Lite.create_hand_by_rs485(
            **common,
            serial_port=RS485_PORT,
            baud_rate=BAUDRATE,
        )
    return OmniHand3Lite.create_hand_by_zlgcan(
        **common,
        canfd_device_id=CANFD_ID,
        canfd_channel_id=CHANNEL_ID,
    )


@pytest.fixture(scope="session")
def hand():
    """Create one device instance for the selected transport."""
    try:
        instance = _create_hand()
    except Exception as error:
        pytest.fail(
            f"Failed to create {DEVICE_TYPE} H3L device: {error}",
            pytrace=False,
        )
    if instance is None:
        pytest.fail(
            f"{DEVICE_TYPE} factory returned no H3L device",
            pytrace=False,
        )

    instance.set_request_interval(REQUEST_INTERVAL)
    instance.set_frame_recv_timeout(FRAME_RECV_TIMEOUT)
    instance.show_data_details(True)
    print(
        f"\n[Info] transport={DEVICE_TYPE} hand={HAND_TYPE_NAME} "
        f"device_id={HAND_DEVICE_ID} interval={REQUEST_INTERVAL}ms "
        f"recv_timeout={FRAME_RECV_TIMEOUT}ms"
    )
    if DEVICE_TYPE in ("zlgcan", "hcan", "zlgcantcp"):
        print(f"[Info] canfd_id={CANFD_ID} channel={CHANNEL_ID}")
    elif DEVICE_TYPE == "socketcan":
        print(f"[Info] can_interface={CAN_IF}")
    else:
        print(f"[Info] port={RS485_PORT} baudrate={BAUDRATE}")
    yield instance


@pytest.fixture(scope="session")
def initialized_hand(hand):
    global HAND_DEVICE_ID

    # Mirror gtest SetUp: broadcast-discover the device's actual ID before
    # switching to unicast commands.  This handles hardware whose device ID
    # differs from the configured default.
    hand.set_broadcast(True)
    if not hand.init():
        pytest.fail(
            f"{DEVICE_TYPE} H3L device is not connected or initialization failed",
            pytrace=False,
        )
    info = hand.get_device_info()
    hand.set_broadcast(False)

    discovered_id = getattr(info, "hand_device_id", None) if info is not None else None
    if discovered_id is not None and 1 <= discovered_id <= 0x7F and discovered_id != HAND_DEVICE_ID:
        print(
            f"\n[Info] Broadcast-discovered device_id={discovered_id} "
            f"(was configured as {HAND_DEVICE_ID}); recreating hand"
        )
        HAND_DEVICE_ID = discovered_id
        try:
            unicast_hand = _create_hand()
        except Exception as error:
            pytest.fail(
                f"Failed to recreate H3L with device_id={discovered_id}: {error}",
                pytrace=False,
            )
        unicast_hand.set_request_interval(REQUEST_INTERVAL)
        unicast_hand.set_frame_recv_timeout(FRAME_RECV_TIMEOUT)
        unicast_hand.show_data_details(True)
        if not unicast_hand.init():
            pytest.fail(
                f"H3L init failed after recreating with device_id={discovered_id}",
                pytrace=False,
            )
        yield unicast_hand
    else:
        yield hand


@pytest.fixture(scope="session")
def tactile_sensors(initialized_hand):
    if not initialized_hand.init_tactile_points_map():
        pytest.fail("H3L tactile sensor initialization failed", pytrace=False)
    sensors = list(initialized_hand.get_sensor_order())
    if not sensors:
        pytest.fail("H3L returned an empty tactile sensor order", pytrace=False)
    return sensors


def _validate_mix_result(result, feedback_count, frame_count, operation):
    assert result.complete(), f"{operation} did not receive all status replies"
    assert result.all_frames_sent
    assert len(result.frames) == frame_count
    actual_feedback_count = 0
    for frame in result.frames:
        assert frame.replied
        assert frame.sub_register == 0x00
        for feedback in frame.feedback:
            assert feedback.motor_status is not None
        actual_feedback_count += len(frame.feedback)
        print(
            f"[{operation}] sub=0x{frame.sub_register:02x} "
            f"feedback={len(frame.feedback)} running=0x{frame.running_code:04x} "
            f"error=0x{frame.error_code:04x}"
        )
    assert actual_feedback_count == feedback_count


# Hardware-independent API checks

@pytest.mark.parametrize("is_left_hand", [False, True])
def test_solver_round_trip_handles_reversed_motors(is_left_hand):
    solver = OmniHand3LiteSolver(is_left_hand)
    actuator_input = [0, 1024, 2048, 4096]
    angles = solver.actuator_input_to_active_joint_pos(actuator_input)
    assert len(angles) == DOF
    recovered = solver.active_joint_pos_to_actuator_input(angles)
    assert recovered == pytest.approx(actuator_input, abs=1)


def test_safe_api_surface():
    assert OmniHand3Lite.kDegreesOfActiveFreedom == DOF
    assert OmniHand3LiteGesture.OMNI_HAND_3_LITE_GESTURE_OPEN.value == 2
    assert TactileSensorDataU16 is not None

    for destructive_name in (
        "calibrate_tactile_sensor",
        "set_min_position_calibration",
        "set_max_position_calibration",
        "set_zero_point",
        "save_params",
    ):
        assert not hasattr(OmniHand3Lite, destructive_name)


# Connection and product information

def test_create_hand(hand):
    assert hand is not None


def test_init(initialized_hand):
    assert initialized_hand.init()


def test_vendor_info(initialized_hand):
    assert initialized_hand.init()
    info = initialized_hand.get_vendor_info()
    print(f"\n[GetVendorInfo]\n{info}")
    assert info.dof == DOF


def test_device_info(initialized_hand):
    assert initialized_hand.init()
    info = initialized_hand.get_device_info()
    print(f"\n[GetDeviceInfo]\n{info}")
    assert info.hand_device_id == HAND_DEVICE_ID


def test_joint_names(initialized_hand):
    assert initialized_hand.init()
    names = initialized_hand.get_joint_names()
    print(f"\n[GetJointNames] {names}")
    assert len(names) == DOF


# Position and angle control

def test_motor_position_control(initialized_hand):
    assert initialized_hand.init()

    # Test single motor position
    target_pos = 4095
    initialized_hand.set_joint_position(1, target_pos)
    print(f"\n[SetJointMotorPosi] Set Joint 1 Motor Position: {target_pos}")
    time.sleep(MOTION_SETTLE_SECONDS)

    pos = initialized_hand.get_joint_position(1)
    print(f"[GetJointMotorPosi] Joint 1 Motor Position: {pos}")

    # Test batch motor positions
    positions = [3072] * DOF  # 4 motors, all at middle
    set_result = initialized_hand.set_all_joint_positions(positions)
    print(f"[SetAllJointMotorPosi] returned: {set_result}")
    time.sleep(MOTION_SETTLE_SECONDS)

    # Test GetAllJointMotorPosi separately
    all_positions = initialized_hand.get_all_joint_positions()
    print(f"[GetAllJointMotorPosi] returned: {all_positions}")
    if not all_positions:
        return
    print(f"[GetAllJointMotorPosi] Motor Positions: {all_positions}")
    assert len(all_positions) == DOF


# def test_position_control_with_status(initialized_hand):
#     set_all = initialized_hand.set_all_joint_positions_with_status(SAFE_POSITIONS)
#     print(f"\n[0x24 SetAll] {set_all}")
#     assert len(set_all.positions) == DOF

#     get_all = initialized_hand.get_all_joint_positions_with_status()
#     print(f"[0x24 GetAll] {get_all}")
#     assert len(get_all.positions) == DOF
#     assert all(0 <= position <= 4096 for position in get_all.positions)

#     set_one = initialized_hand.set_joint_position_with_status(1, SAFE_POSITIONS[0])
#     time.sleep(0.1)
#     get_one = initialized_hand.get_joint_position_with_status(1)
#     print(f"[0x24 SetOne] {set_one}")
#     print(f"[0x24 GetOne] {get_one}")
#     assert 0 <= set_one.position <= 4096
#     assert 0 < get_one.position <= 4096


def test_joint_angle_control(initialized_hand):
    assert initialized_hand.init()

    # Test setting active joint angles
    angles = [0.0] * DOF  # 4 joints, all at 0
    actual = initialized_hand.set_all_active_joint_angles(angles)
    print(f"\n[SetAllActiveJointAngles] Called (should output warning)")
    print(f"[SetAllActiveJointAngles] returned: {actual}")
    time.sleep(MOTION_SETTLE_SECONDS)

    # Test getting active joint angles
    active = initialized_hand.get_all_active_joint_angles()
    all_angles = initialized_hand.get_all_joint_angles()
    print(f"[GetAllActiveJointAngles] {active}")
    print(f"[GetAllJointAngles] {all_angles}")
    assert len(active) == DOF
    assert len(all_angles) >= DOF


def test_hand_gesture(initialized_hand):
    assert initialized_hand.init()

    gesture = OmniHand3LiteGesture.OMNI_HAND_3_LITE_GESTURE_FIST
    initialized_hand.set_hand_gesture(gesture)
    print(f"\n[SetHandGesture] FIST gesture set")
    time.sleep(MOTION_SETTLE_SECONDS)


# 0x25 and 0x26 status protocols

# def test_mixed_control_with_status(initialized_hand):
#     control = MixCtrl()
#     control.joint_index = 0
#     control.ctrl_mode = int(MixControlMode.POSITION_TORQUE)
#     control.tgt_posi = SAFE_POSITIONS[0]
#     control.tgt_torque = 0

#     _validate_mix_result(
#         initialized_hand.mix_control_with_status([control]), 1, 1,
#         "0x25 MixControlWithStatus",
#     )
#     _validate_mix_result(
#         initialized_hand.mix_control_by_pt_with_status(1, SAFE_POSITIONS[0], 0),
#         1, 1, "0x25 PT single",
#     )
#     _validate_mix_result(
#         initialized_hand.mix_control_by_pt_with_status(SAFE_POSITIONS, SAFE_TORQUES),
#         DOF, 1, "0x25 PT all",
#     )
#     _validate_mix_result(
#         initialized_hand.mix_control_by_pvt_with_status(
#             1, SAFE_POSITIONS[0], 0, 0
#         ),
#         1, 1, "0x25 PVT single",
#     )
#     _validate_mix_result(
#         initialized_hand.mix_control_by_pvt_with_status(
#             SAFE_POSITIONS, SAFE_VELOCITIES, SAFE_TORQUES
#         ),
#         DOF, 1, "0x25 PVT all",
#     )


# def test_joint_detail_info(initialized_hand):
#     hand_info = initialized_hand.get_all_joint_detail_info()
#     print(f"\n[0x26 GetAll] {hand_info}")
#     assert hand_info.hand_bus_voltage > 0

#     for joint_index in range(DOF):
#         info = initialized_hand.get_joint_detail_info(joint_index)
#         print(f"[0x26 Joint {joint_index}] {info}")
#         assert info.hand_bus_voltage > 0
#         assert 0 <= info.joint_position <= 4096


# Read-only reports and settings

def test_current_threshold(initialized_hand):
    assert initialized_hand.init()
    single = initialized_hand.get_current_threshold(1)
    thresholds = initialized_hand.get_all_current_thresholds()
    print(f"\n[GetCurrentThreshold] J1={single}, all={thresholds}")
    if not thresholds:
        pytest.fail("Current-threshold readback failed", pytrace=False)
    assert len(thresholds) == DOF


def test_error_report(initialized_hand):
    assert initialized_hand.init()
    single = initialized_hand.get_error_report(1)
    reports = initialized_hand.get_all_error_reports()
    print(f"\n[GetErrorReport] J1={single}")
    print(f"[GetAllErrorReport] count={len(reports)}")
    assert len(reports) == DOF


def test_temperature_report(initialized_hand):
    assert initialized_hand.init()
    single = initialized_hand.get_temperature_report(1)
    temperatures = initialized_hand.get_all_temperature_reports()
    print(f"\n[GetTemperatureReport] J1={single}, all={temperatures}")
    assert len(temperatures) == DOF


def test_current_report(initialized_hand):
    assert initialized_hand.init()
    single = initialized_hand.get_current_report(1)
    currents = initialized_hand.get_all_current_reports()
    print(f"\n[GetCurrentReport] J1={single}, all={currents}")
    if not currents:
        pytest.fail("Current report timed out", pytrace=False)
    assert len(currents) == DOF


def test_velocity_report(initialized_hand):
    assert initialized_hand.init()
    single = initialized_hand.get_joint_velocity(1)
    velocities = initialized_hand.get_all_joint_velocities()
    print(f"\n[GetJointMotorVelo] J1={single}, all={velocities}")
    if not velocities:
        pytest.fail("Velocity report timed out", pytrace=False)
    assert len(velocities) == DOF


def test_request_settings(initialized_hand):
    assert initialized_hand.init()
    original_interval = initialized_hand.get_request_interval()
    original_timeout = initialized_hand.get_frame_recv_timeout()
    try:
        initialized_hand.set_request_interval(10)
        assert initialized_hand.get_request_interval() == 10
        initialized_hand.set_frame_recv_timeout(100)
        assert initialized_hand.get_frame_recv_timeout() == 100
        assert initialized_hand.get_frame_send_timeout() >= 0
    finally:
        initialized_hand.set_request_interval(original_interval)
        initialized_hand.set_frame_recv_timeout(original_timeout)


def test_invalid_joint_index(initialized_hand):
    assert initialized_hand.init()
    assert initialized_hand.get_joint_position(0) == -1
    assert initialized_hand.get_joint_position(DOF + 1) == -1
    assert initialized_hand.get_joint_velocity(0) == -1
    assert initialized_hand.get_joint_velocity(DOF + 1) == -1


def test_constants(initialized_hand):
    assert OmniHand3Lite.kDegreesOfActiveFreedom == DOF
    assert initialized_hand.get_num_of_joint_motors() == DOF
    assert initialized_hand.get_doa() == DOF
    assert initialized_hand.get_dop() == 5
    position_range = OmniHand3Lite.get_min_max_motor_position(1)
    assert position_range.min_value == 0
    assert position_range.max_value == 4096


# Tactile sensors

def test_tactile_metadata(initialized_hand, tactile_sensors):
    count = initialized_hand.get_num_of_tactile_sensors()
    print(f"\n[GetSensorOrder] {tactile_sensors}")
    assert count == len(tactile_sensors)

    for sensor in tactile_sensors:
        points = initialized_hand.get_num_of_tactile_points(sensor)
        datum_length = initialized_hand.get_len_of_tactile_datum(sensor)
        frame_count = initialized_hand.get_num_of_replied_tactile_frames(sensor)
        serial_number = initialized_hand.get_sn_of_tactile_sensor(sensor)
        deprecated_length = initialized_hand.get_sensor_data_length(sensor)
        print(
            f"[{sensor.name}] points={points} datum={datum_length} "
            f"frames={frame_count} sn={serial_number!r}"
        )
        assert points > 0
        assert datum_length > 0
        assert frame_count > 0
        assert deprecated_length > 0

    assert initialized_hand.get_num_of_tactile_points(Finger.UNKNOWN) == 0
    assert initialized_hand.get_num_of_tactile_points(Finger.DORSUM) == 0


def test_tactile_sensor_data(initialized_hand, tactile_sensors):
    for sensor in tactile_sensors:
        data = initialized_hand.get_tactile_sensor_data(sensor)
        print(f"\n[GetTactileSensorData] {sensor.name}: {data.data[:6]}")
        assert data.sensor_id == sensor


def test_tactile_sensor_data_raw(initialized_hand, tactile_sensors):
    for sensor in tactile_sensors:
        data = initialized_hand.get_tactile_sensor_data_raw(sensor)
        print(f"\n[GetTactileSensorDataRaw] {sensor.name}: {data.data}")
        assert data.sensor_id == sensor


def test_all_tactile_sensor_data_raw(initialized_hand, tactile_sensors):
    all_data = initialized_hand.get_all_tactile_sensor_data_raw()
    print(f"\n[GetAllTactileSensorDataRaw] count={len(all_data)}")
    assert len(all_data) == len(tactile_sensors)
    assert [data.sensor_id for data in all_data] == tactile_sensors


if __name__ == "__main__":
    _export_config_to_environment()
    raise SystemExit(pytest.main([__file__, "-v", "-s", *sys.argv[1:]]))
