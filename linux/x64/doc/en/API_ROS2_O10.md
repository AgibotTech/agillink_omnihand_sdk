# OmniHand 2025 (O10) ROS2 Interface

## ROS2 Topics

| Topic Name                                           | Description                         | Pub/Sub | Message Type                                                                                       | Notes |
|:----------------------------------------------------|:------------------------------------|:-------:|:---------------------------------------------------------------------------------------------------|:------|
| `/omnihand/omnihand_2025/left/control_mode`          | Joint motor control mode            |  Pub    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_nodemsgsmsgcontrolmode)                     |       |
| `/omnihand/omnihand_2025/left/current_report`        | Joint motor current report          |  Pub    | [omnihand_2025_node_msgs.msg.CurrentReport](#omnihand_2025_nodemsgsmsgcurrentreport)                 |       |
| `/omnihand/omnihand_2025/left/current_threshold`     | Joint motor current threshold       |  Pub    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_nodemsgsmsgcurrentthreshold)           |       |
| `/omnihand/omnihand_2025/left/motor_error_report`    | Joint motor error report            |  Pub    | [omnihand_2025_node_msgs.msg.MotorErrorReport](#omnihand_2025_nodemsgsmsgmotorerrorreport)           |       |
| `/omnihand/omnihand_2025/left/motor_angle`           | Joint motor angle                   |  Pub    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |
| `/omnihand/omnihand_2025/left/motor_pos`             | Joint motor position                |  Pub    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_nodemsgsmsgmotorpos)                           |       |
| `/omnihand/omnihand_2025/left/tactile_sensor`        | Tactile sensor data                 |  Pub    | [omnihand_2025_node_msgs.msg.TactileSensor](#omnihand_2025_nodemsgsmsgtactilesensor)                 |       |
| `/omnihand/omnihand_2025/left/temperature_report`    | Joint motor temperature report      |  Pub    | [omnihand_2025_node_msgs.msg.TemperatureReport](#omnihand_2025_nodemsgsmsgtemperaturereport)         |       |
| `/omnihand/omnihand_2025/left/control_mode_cmd`      | Joint motor control mode command    |  Sub    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_nodemsgsmsgcontrolmode)                     |       |
| `/omnihand/omnihand_2025/left/current_threshold_cmd` | Joint motor current threshold cmd   |  Sub    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_nodemsgsmsgcurrentthreshold)           |       |
| `/omnihand/omnihand_2025/left/mix_control_cmd`       | Hybrid control command              |  Sub    | [omnihand_2025_node_msgs.msg.MixControl](#omnihand_2025_nodemsgsmsgmixcontrol)                       |       |
| `/omnihand/omnihand_2025/left/motor_pos_cmd`         | Joint motor position command        |  Sub    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_nodemsgsmsgmotorpos)                           |       |
| `/omnihand/omnihand_2025/left/motor_angle_cmd`       | Joint motor angle command           |  Sub    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |
| `/omnihand/omnihand_2025/right/control_mode`         | Joint motor control mode            |  Pub    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_nodemsgsmsgcontrolmode)                     |       |
| `/omnihand/omnihand_2025/right/current_report`       | Joint motor current report          |  Pub    | [omnihand_2025_node_msgs.msg.CurrentReport](#omnihand_2025_nodemsgsmsgcurrentreport)                 |       |
| `/omnihand/omnihand_2025/right/current_threshold`    | Joint motor current threshold       |  Pub    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_nodemsgsmsgcurrentthreshold)           |       |
| `/omnihand/omnihand_2025/right/motor_error_report`   | Joint motor error report            |  Pub    | [omnihand_2025_node_msgs.msg.MotorErrorReport](#omnihand_2025_nodemsgsmsgmotorerrorreport)           |       |
| `/omnihand/omnihand_2025/right/motor_angle`          | Joint motor angle                   |  Pub    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |
| `/omnihand/omnihand_2025/right/motor_pos`            | Joint motor position                |  Pub    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_nodemsgsmsgmotorpos)                           |       |
| `/omnihand/omnihand_2025/right/tactile_sensor`       | Tactile sensor data                 |  Pub    | [omnihand_2025_node_msgs.msg.TactileSensor](#omnihand_2025_nodemsgsmsgtactilesensor)                 |       |
| `/omnihand/omnihand_2025/right/temperature_report`   | Joint motor temperature report      |  Pub    | [omnihand_2025_node_msgs.msg.TemperatureReport](#omnihand_2025_nodemsgsmsgtemperaturereport)         |       |
| `/omnihand/omnihand_2025/right/control_mode_cmd`     | Joint motor control mode command    |  Sub    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_nodemsgsmsgcontrolmode)                     |       |
| `/omnihand/omnihand_2025/right/current_threshold_cmd`| Joint motor current threshold cmd   |  Sub    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_nodemsgsmsgcurrentthreshold)           |       |
| `/omnihand/omnihand_2025/right/mix_control_cmd`      | Hybrid control command              |  Sub    | [omnihand_2025_node_msgs.msg.MixControl](#omnihand_2025_nodemsgsmsgmixcontrol)                       |       |
| `/omnihand/omnihand_2025/right/motor_pos_cmd`        | Joint motor position command        |  Sub    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_nodemsgsmsgmotorpos)                           |       |
| `/omnihand/omnihand_2025/right/motor_angle_cmd`      | Joint motor angle command           |  Sub    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |

**Note**: O10 has 10 degrees of freedom. All arrays in messages contain 10 values.

## Usage

### Build

See the root `README.md` for how to build the ROS2 node together with the SDK.

### Run

```bash
export LD_LIBRARY_PATH=$(pwd)/build/install/lib/:$LD_LIBRARY_PATH
cd build/install/bin/
./omnihand_2025_node
```

After starting the node, you can publish/subscribe to the above topics.
You can also use the helper Python scripts under `node/scripts/omnihand_2025/` to publish or subscribe to these topics.

## Message Definitions

### `omnihand_2025_node_msgs.msg.ControlMode`

```python
std_msgs/Header header

# ePosi = 0,                    # position control
# eServo = 1,                   # servo mode
# eVelo = 2,                    # velocity control
# eTorque = 3,                  # torque control (not supported, use mixed modes)
# ePosiTorque = 4,              # position-torque hybrid control
# eVeloTorque = 5,              # velocity-torque hybrid control
# ePosiVeloTorque = 6,          # position-velocity-torque hybrid control
# eUnknown = 10                 # unknown mode

int8[] modes  # 10 values for O10
```

**Note**: 
- **eServo mode (1)**: Servo control mode
- **Pure torque control (eTorque) is not supported**: Use mixed control modes (ePosiTorque, eVeloTorque, ePosiVeloTorque) instead

### `omnihand_2025_node_msgs.msg.CurrentReport`

```python
std_msgs/Header header
uint16[] current_reports  # 10 values for O10
```

### `omnihand_2025_node_msgs.msg.CurrentThreshold`

```python
std_msgs/Header header
int16[] current_thresholds  # 10 values for O10
```

### `omnihand_2025_node_msgs.msg.MixControl`

```python
std_msgs/Header header
uint64[] mix_controls  # 10 values for O10
```

### `omnihand_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # 10 values for O10
```

### `omnihand_2025_node_msgs.msg.MotorErrorReport`

```python
std_msgs/Header header

# unsigned char stalled_ : 1;      # stall flag
# unsigned char overheat_ : 1;     # overheat flag
# unsigned char over_current_ : 1; # over-current flag
# unsigned char motor_except_ : 1; # motor exception
# unsigned char commu_except_ : 1; # communication exception
# unsigned char res1_ : 3;         # reserved bits
# unsigned char res2_;             # reserved byte

uint16[] error_reports  # 10 values for O10
```

### `omnihand_2025_node_msgs.msg.MotorPos`

```python
std_msgs/Header header
int16[] pos  # 10 values for O10, range: 0-4096
```

### `omnihand_2025_node_msgs.msg.MotorVel`

```python
std_msgs/Header header
int16[] vels  # 10 values for O10
```

### `omnihand_2025_node_msgs.msg.TactileSensor`

```python
std_msgs/Header header
TactileSensorData[] tactile_datas  # 1D tactile sensors for O10
```

### `omnihand_2025_node_msgs.msg.TactileSensorData`

```python
uint8[] tactiles  # 1D tactile sensor data
```

### `omnihand_2025_node_msgs.msg.TemperatureReport`

```python
std_msgs/Header header
uint16[] temperature_reports  # 10 values for O10
```

## Related Documentation

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md) - C++ API documentation
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md) - Python API documentation
- [OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md) - O12 ROS2 interface
