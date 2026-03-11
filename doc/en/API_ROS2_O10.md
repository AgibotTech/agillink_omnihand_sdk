# OmniHand 2025 (O10) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

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

## ROS2 Services

| Service Name                                           | Description | Service Type                                                                                              | Notes |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_2025/left/set_joint_angles`  | Set joint angles |    Service    | [omnihand_2025_node_msgs.srv.SetJointAngles](#omnihand_2025_node_msgs_srv_SetJointAngles) | 
| `/omnihand/omnihand_2025/left/get_joint_angles`  | Get joint angles |    Service    | [omnihand_2025_node_msgs.srv.GetJointAngles](#omnihand_2025_node_msgs_srv_GetJointAngles) | 
| `/omnihand/omnihand_2025/right/set_joint_angles`  | Set joint angles |    Service    | [omnihand_2025_node_msgs.srv.SetJointAngles](#omnihand_2025_node_msgs_srv_SetJointAngles) | 
| `/omnihand/omnihand_2025/right/get_joint_angles`  | Get joint angles |    Service    | [omnihand_2025_node_msgs.srv.GetJointAngles](#omnihand_2025_node_msgs_srv_GetJointAngles) | 

**Note**:
- Service interfaces use joint angles (radians), not motor positions
- `SetJointAngles` waits for the hand to reach target angles, or returns on timeout
- `GetJointAngles` returns current joint angles and hand ready status
- O10 has 10 degrees of freedom, angle arrays contain 10 values

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

# POSITION = 0,                    # position control
# SERVO = 1,                   # servo mode
# VELOCITY = 2,                    # velocity control
# TORQUE = 3,                  # torque control (not supported, use mixed modes)
# POSITION_TORQUE = 4,              # position-torque hybrid control
# VELOCITY_TORQUE = 5,              # velocity-torque hybrid control
# POSITION_VELOCITY_TORQUE = 6,          # position-velocity-torque hybrid control
# UNKNOWN = 10                 # unknown mode

int8[] modes  # 10 values for O10
```

**Note**: 
- **SERVO mode (1)**: Servo control mode
- **Pure torque control (TORQUE) is not supported**: Use mixed control modes (POSITION_TORQUE, VELOCITY_TORQUE, POSITION_VELOCITY_TORQUE) instead

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

## Service Definitions

### `omnihand_2025_node_msgs.srv.SetJointAngles`

Set target angles for all joints and wait for the hand to reach the target position.

**Request**:
```python
# Target joint angles (radians), 10 values for O10
float64[] target_angles
# Timeout in seconds (0 means use default: 5.0)
float64 timeout
```

**Response**:
```python
# Whether the movement was successful
bool success
# Final joint angles reached (radians), 10 values for O10
float64[] final_angles
# Error message if failed
string error_message
```

**Usage Example**:
```python
from omnihand_2025_node_msgs.srv import SetJointAngles
import rclpy
from rclpy.node import Node

# Create service client
client = node.create_client(SetJointAngles, '/omnihand/omnihand_2025/left/set_joint_angles')

# Wait for service to be available
client.wait_for_service()

# Create request
request = SetJointAngles.Request()
request.target_angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # 10 values
request.timeout = 5.0

# Call service
future = client.call_async(request)
rclpy.spin_until_future_complete(node, future)
response = future.result()

if response.success:
    print(f"Successfully moved to target angles: {response.final_angles}")
else:
    print(f"Failed: {response.error_message}")
```

**Note**:
- Angles are in radians
- If target angles exceed joint limits, the service will return failure
- Timeout is calculated from service call start. If the hand does not reach target angles within timeout, it returns failure

### `omnihand_2025_node_msgs.srv.GetJointAngles`

Get current joint angles and hand ready status.

**Request**:
```python
# No parameters
```

**Response**:
```python
# Current joint angles (radians), 10 values for O10
float64[] angles
# Whether the hand is ready
bool is_ready
# Error message if any
string error_message
```

**Usage Example**:
```python
from omnihand_2025_node_msgs.srv import GetJointAngles
import rclpy
from rclpy.node import Node

# Create service client
client = node.create_client(GetJointAngles, '/omnihand/omnihand_2025/left/get_joint_angles')

# Wait for service to be available
client.wait_for_service()

# Create request (empty request)
request = GetJointAngles.Request()

# Call service
future = client.call_async(request)
rclpy.spin_until_future_complete(node, future)
response = future.result()

if response.is_ready:
    print(f"Current joint angles: {response.angles}")
    print(f"Angles (degrees): {[a * 180 / 3.14159 for a in response.angles]}")
else:
    print(f"Hand not ready: {response.error_message}")
```

**Note**:
- Angles are in radians
- `is_ready` is `true` when the hand is initialized and can receive commands
- If the hand is not ready, `error_message` contains the reason

## Related Documentation

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md) - C++ API documentation
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md) - Python API documentation
- [OmniHand Pro 2025 (O12) ROS2 Interface](API_ROS2_O12.md) - O12 ROS2 interface
