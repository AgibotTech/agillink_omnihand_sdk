# OmniHand Pro 2025 (O12) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## ROS2 Topics

| Topic Name                                              | Description                         | Pub/Sub | Message Type                                                                                           | Notes |
|:-------------------------------------------------------|:------------------------------------|:-------:|:-------------------------------------------------------------------------------------------------------|:------|
| `/omnihand/omnihand_pro_2025/left/control_mode`          | Joint motor control mode            |  Pub    | [omnihand_pro_2025_node_msgs.msg.ControlMode](#omnihand_pro_2025_nodemsgsmsgcontrolmode)                 |       |
| `/omnihand/omnihand_pro_2025/left/current_report`        | Joint motor current report          |  Pub    | [omnihand_pro_2025_node_msgs.msg.CurrentReport](#omnihand_pro_2025_nodemsgsmsgcurrentreport)             |       |
| `/omnihand/omnihand_pro_2025/left/current_threshold`     | Joint motor current threshold       |  Pub    | [omnihand_pro_2025_node_msgs.msg.CurrentThreshold](#omnihand_pro_2025_nodemsgsmsgcurrentthreshold)         |       |
| `/omnihand/omnihand_pro_2025/left/motor_error_report`    | Joint motor error report            |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorErrorReport](#omnihand_pro_2025_nodemsgsmsgmotorerrorreport)       |       |
| `/omnihand/omnihand_pro_2025/left/motor_angle`           | Joint motor angle                   |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |
| `/omnihand/omnihand_pro_2025/left/motor_pos`             | Joint motor position                |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorPos](#omnihand_pro_2025_nodemsgsmsgmotorpos)                       |       |
| `/omnihand/omnihand_pro_2025/left/motor_vel`             | Joint motor velocity                |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorVel](#omnihand_pro_2025_nodemsgsmsgmotorvel)                       |       |
| `/omnihand/omnihand_pro_2025/left/tactile_sensor`        | Tactile sensor data                 |  Pub    | [omnihand_pro_2025_node_msgs.msg.TactileSensor](#omnihand_pro_2025_nodemsgsmsgtactilesensor)             |       |
| `/omnihand/omnihand_pro_2025/left/temperature_report`    | Joint motor temperature report      |  Pub    | [omnihand_pro_2025_node_msgs.msg.TemperatureReport](#omnihand_pro_2025_nodemsgsmsgtemperaturereport)     |       |
| `/omnihand/omnihand_pro_2025/left/control_mode_cmd`      | Joint motor control mode command    |  Sub    | [omnihand_pro_2025_node_msgs.msg.ControlMode](#omnihand_pro_2025_nodemsgsmsgcontrolmode)                 |       |
| `/omnihand/omnihand_pro_2025/left/current_threshold_cmd` | Joint motor current threshold cmd   |  Sub    | [omnihand_pro_2025_node_msgs.msg.CurrentThreshold](#omnihand_pro_2025_nodemsgsmsgcurrentthreshold)     |       |
| `/omnihand/omnihand_pro_2025/left/motor_pos_cmd`         | Joint motor position command        |  Sub    | [omnihand_pro_2025_node_msgs.msg.MotorPos](#omnihand_pro_2025_nodemsgsmsgmotorpos)                       |       |
| `/omnihand/omnihand_pro_2025/left/motor_vel_cmd`         | Joint motor velocity command        |  Sub    | [omnihand_pro_2025_node_msgs.msg.MotorVel](#omnihand_pro_2025_nodemsgsmsgmotorvel)                       |       |
| `/omnihand/omnihand_pro_2025/left/motor_angle_cmd`       | Joint motor angle command           |  Sub    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |
| `/omnihand/omnihand_pro_2025/right/control_mode`         | Joint motor control mode            |  Pub    | [omnihand_pro_2025_node_msgs.msg.ControlMode](#omnihand_pro_2025_nodemsgsmsgcontrolmode)                 |       |
| `/omnihand/omnihand_pro_2025/right/current_report`       | Joint motor current report          |  Pub    | [omnihand_pro_2025_node_msgs.msg.CurrentReport](#omnihand_pro_2025_nodemsgsmsgcurrentreport)             |       |
| `/omnihand/omnihand_pro_2025/right/current_threshold`    | Joint motor current threshold       |  Pub    | [omnihand_pro_2025_node_msgs.msg.CurrentThreshold](#omnihand_pro_2025_nodemsgsmsgcurrentthreshold)       |       |
| `/omnihand/omnihand_pro_2025/right/motor_error_report`   | Joint motor error report            |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorErrorReport](#omnihand_pro_2025_nodemsgsmsgmotorerrorreport)       |       |
| `/omnihand/omnihand_pro_2025/right/motor_angle`          | Joint motor angle                   |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |
| `/omnihand/omnihand_pro_2025/right/motor_pos`            | Joint motor position                |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorPos](#omnihand_pro_2025_nodemsgsmsgmotorpos)                       |       |
| `/omnihand/omnihand_pro_2025/right/motor_vel`            | Joint motor velocity                |  Pub    | [omnihand_pro_2025_node_msgs.msg.MotorVel](#omnihand_pro_2025_nodemsgsmsgmotorvel)                       |       |
| `/omnihand/omnihand_pro_2025/right/tactile_sensor`       | Tactile sensor data                 |  Pub    | [omnihand_pro_2025_node_msgs.msg.TactileSensor](#omnihand_pro_2025_nodemsgsmsgtactilesensor)             |       |
| `/omnihand/omnihand_pro_2025/right/temperature_report`   | Joint motor temperature report      |  Pub    | [omnihand_pro_2025_node_msgs.msg.TemperatureReport](#omnihand_pro_2025_nodemsgsmsgtemperaturereport)   |       |
| `/omnihand/omnihand_pro_2025/right/control_mode_cmd`     | Joint motor control mode command    |  Sub    | [omnihand_pro_2025_node_msgs.msg.ControlMode](#omnihand_pro_2025_nodemsgsmsgcontrolmode)                 |       |
| `/omnihand/omnihand_pro_2025/right/current_threshold_cmd`| Joint motor current threshold cmd   |  Sub    | [omnihand_pro_2025_node_msgs.msg.CurrentThreshold](#omnihand_pro_2025_nodemsgsmsgcurrentthreshold)     |       |
| `/omnihand/omnihand_pro_2025/right/motor_pos_cmd`        | Joint motor position command        |  Sub    | [omnihand_pro_2025_node_msgs.msg.MotorPos](#omnihand_pro_2025_nodemsgsmsgmotorpos)                       |       |
| `/omnihand/omnihand_pro_2025/right/motor_vel_cmd`        | Joint motor velocity command        |  Sub    | [omnihand_pro_2025_node_msgs.msg.MotorVel](#omnihand_pro_2025_nodemsgsmsgmotorvel)                       |       |
| `/omnihand/omnihand_pro_2025/right/motor_angle_cmd`      | Joint motor angle command           |  Sub    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |

**Note**: 
- O12 has 12 degrees of freedom. All arrays in messages contain 12 values.
- O12 does NOT support `mix_control_cmd` topic (unlike O10).
- O12 motor position range is 0-2000 (different from O10 which is 0-4096).
- O12 uses 3D tactile sensors (fingers only, not palm/dorsum).

## Usage

### Build

See the root `README.md` for how to build the ROS2 node together with the SDK.

### Run

```bash
export LD_LIBRARY_PATH=$(pwd)/build/install/lib/:$LD_LIBRARY_PATH
cd build/install/bin/
./omnihand_pro_2025_node
```

After starting the node, you can publish/subscribe to the above topics.
You can also use the helper Python scripts under `node/scripts/omnihand_pro_2025/` to publish or subscribe to these topics.

## Message Definitions

### `omnihand_pro_2025_node_msgs.msg.ControlMode`

```python
std_msgs/Header header

# POSITION = 0,                    # position control
# SERVO = 1,                   # servo mode
# VELOCITY = 2,                    # velocity control
# TORQUE = 3,                  # torque control (not supported, use mixed modes)
# POSITION_TORQUE = 4,              # position-torque hybrid control
# VELOCITY_TORQUE = 5,              # velocity-torque hybrid control
# POSITION_VELOCITY_TORQUE = 6,          # position-velocity-torque hybrid control
# UNKNOWN = 10                 # unknown mode

int8[] modes  # 12 values for O12
```

**Note**: 
- **SERVO mode (1)**: Servo control mode
- **Pure torque control (TORQUE) is not supported**: Use mixed control modes (POSITION_TORQUE, VELOCITY_TORQUE, POSITION_VELOCITY_TORQUE) instead

### `omnihand_pro_2025_node_msgs.msg.CurrentReport`

```python
std_msgs/Header header
uint16[] current_reports  # 12 values for O12
```

### `omnihand_pro_2025_node_msgs.msg.CurrentThreshold`

```python
std_msgs/Header header
int16[] current_thresholds  # 12 values for O12
```

### `omnihand_pro_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # 12 values for O12
```

### `omnihand_pro_2025_node_msgs.msg.MotorErrorReport`

```python
std_msgs/Header header

# unsigned char stalled_ : 1;      # stall flag
# unsigned char overheat_ : 1;     # overheat flag
# unsigned char over_current_ : 1; # over-current flag
# unsigned char motor_except_ : 1; # motor exception
# unsigned char commu_except_ : 1; # communication exception
# unsigned char res1_ : 3;         # reserved bits
# unsigned char res2_;             # reserved byte

uint16[] error_reports  # 12 values for O12
```

### `omnihand_pro_2025_node_msgs.msg.MotorPos`

```python
std_msgs/Header header
int16[] pos  # 12 values for O12, range: 0-2000
```

### `omnihand_pro_2025_node_msgs.msg.MotorVel`

```python
std_msgs/Header header
int16[] vels  # 12 values for O12
```

### `omnihand_pro_2025_node_msgs.msg.TactileSensor`

```python
std_msgs/Header header
TactileSensorData[] tactile_datas  # 3D tactile sensors for O12 (fingers only)
```

### `omnihand_pro_2025_node_msgs.msg.TactileSensorData`

```python
uint8[] tactiles  # 3D tactile sensor data
```

### `omnihand_pro_2025_node_msgs.msg.TemperatureReport`

```python
std_msgs/Header header
uint16[] temperature_reports  # 12 values for O12
```

## Differences from O10

1. **DOF**: O12 has 12 degrees of freedom (vs 10 for O10)
2. **Motor Position Range**: 0-2000 (vs 0-4096 for O10)
3. **Tactile Sensors**: 3D sensors (fingers only) vs 1D sensors (fingers, palm, dorsum)
4. **Mix Control**: O12 does NOT support `mix_control_cmd` topic
5. **Control Modes**: All control modes are supported
6. **Message Namespace**: `omnihand_pro_2025_node_msgs` (vs `omnihand_2025_node_msgs` for O10)
7. **Topic Prefix**: `/omnihand/omnihand_pro_2025/` (vs `/omnihand/omnihand_2025/` for O10)

## Related Documentation

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md) - C++ API documentation
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md) - Python API documentation
- [OmniHand 2025 (O10) ROS2 Interface](API_ROS2_O10.md) - O10 ROS2 interface
