# Python Examples

## OmniHand 2025 (10 DOF) Examples
- `omnihand_2025/demo_get_hardware_info.py` - Get hardware information
- `omnihand_2025/demo_monitor_current.py` - Monitor motor current
- `omnihand_2025/demo_monitor_error.py` - Monitor errors
- `omnihand_2025/demo_monitor_temperature.py` - Monitor temperature
- `omnihand_2025/demo_set_angle.py` - Set finger angles
- `omnihand_2025/demo_set_motion.py` - Set motion parameters
- `omnihand_2025/demo_set_motor.py` - Direct motor control
- `omnihand_2025/demo_set_motor_via_multicans.py` - Control via multiple CANFD adapters
- `omnihand_2025/demo_set_motor_via_multichannels.py` - Control via multiple CAN channels
- `omnihand_2025/demo_set_motor_via_multisocketcans.py` - Control via multiple SocketCAN interfaces
- `omnihand_2025/demo_set_torque.py` - Set torque
- `omnihand_2025/demo_tactile_sensor.py` - Read tactile sensors
- `omnihand_2025/demo_tactile_sensor_raw.py` - Read tactile sensor raw data
- `omnihand_2025/demo_tactile_sensor_raw_reliability.py` - Tactile sensor reliability test
- `omnihand_2025/demo_all_tactile_sensor_raw_reliability.py` - All tactile sensors reliability test
- `omnihand_2025/demo_set_angle_and_tactile_sensor_reliability.py` - Angle and tactile sensor reliability test
- `omnihand_2025/demo_100set_and_20tactile_reliability.py` - High-frequency control and sensor test
- `omnihand_2025/demo_canfd_id.py` - CANFD communication example (by device ID)
- `omnihand_2025/demo_canfd_serial.py` - CANFD communication example (by serial number)
- `omnihand_2025/demo_socketcan.py` - SocketCAN example (Linux only)

## OmniHand Pro 2025 (12 DOF) Examples
- `omnihand_pro_2025/demo_get_hardware_info.py` - Get hardware information
- `omnihand_pro_2025/demo_monitor_current.py` - Monitor motor current
- `omnihand_pro_2025/demo_monitor_error.py` - Monitor errors
- `omnihand_pro_2025/demo_monitor_temperature.py` - Monitor temperature
- `omnihand_pro_2025/demo_sensor_touch.py` - Read touch sensors
- `omnihand_pro_2025/demo_set_angle.py` - Set finger angles
- `omnihand_pro_2025/demo_set_position.py` - Set finger positions
- `omnihand_pro_2025/demo_set_velocity.py` - Set finger velocity
- `omnihand_pro_2025/demo_canfd_id.py` - CANFD communication example (by device ID)
- `omnihand_pro_2025/demo_canfd_serial.py` - CANFD communication example (by serial number)
- `omnihand_pro_2025/demo_socketcan.py` - SocketCAN example (Linux only)

## OmniHand Dex UMI (10 DOF, UMI Protocol) Examples
- **Kinematics (Python):** `OmniHandDexUMI.get_all_active_joint_angles()`, `get_all_joint_angles()`, `get_all_joint_pos(...)` — same O10 solver as OmniHand 2025; read-only (no set-angle APIs).
- `omnihand_dex_umi/demo_get_hardware_info.py` - Get hardware information
- `omnihand_dex_umi/demo_tactile_sensor_raw.py` - Read tactile sensor raw data (uses Pn6 protocol)
- `omnihand_dex_umi/demo_periodic_report.py` - Receive periodic reports (position and tactile sensor)
- `omnihand_dex_umi/demo_set_max_min_calibration.py` - Set max/min position calibration
- `omnihand_dex_umi/demo_umi_to_o10.py` - Control O10 hand using UMI position data
- `omnihand_dex_umi/demo_canfd_id.py` - CANFD communication example (by device ID)
- `omnihand_dex_umi/demo_canfd_serial.py` - CANFD communication example (by serial number)
- `omnihand_dex_umi/demo_socketcan.py` - SocketCAN example (Linux only)

## Usage

```bash
# Make sure omnihand package is installed
pip list | findstr omnihand

# Run examples
python omnihand_2025\demo_get_hardware_info.py
python omnihand_pro_2025\demo_get_hardware_info.py
python omnihand_dex_umi\demo_get_hardware_info.py
```
