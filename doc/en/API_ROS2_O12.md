# OmniHand Pro 2025 (O12) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## ROS2 Topics

| Topic Name                                              | Description                         | Node Action | Message Type                                                                                           | Notes |
|:-------------------------------------------------------|:------------------------------------|:-----------:|:-------------------------------------------------------------------------------------------------------|:------|
| `/omnihand/omnihand_pro_2025/left/motor_angle`           | Joint motor angle                   |  Pub (You Sub)    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |
| `/omnihand/omnihand_pro_2025/left/motor_angle_cmd`       | Joint motor angle command           |  Sub (You Pub)    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |
| `/omnihand/omnihand_pro_2025/right/motor_angle`          | Joint motor angle                   |  Pub (You Sub)    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |
| `/omnihand/omnihand_pro_2025/right/motor_angle_cmd`      | Joint motor angle command           |  Sub (You Pub)    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle)                   |       |

**Note**: O12 has 12 degrees of freedom. All arrays in messages contain 12 values.

## ROS2 Services

| Service Name                                              | Description | Service Type                                                                                              | Notes |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_pro_2025/left/set_joint_angles`  | Set joint angles | [omnihand_pro_2025_node_msgs.srv.SetJointAngles](#omnihand_pro_2025_node_msgssrvsetjointangles) | 
| `/omnihand/omnihand_pro_2025/left/get_joint_angles`  | Get joint angles | [omnihand_pro_2025_node_msgs.srv.GetJointAngles](#omnihand_pro_2025_node_msgssrvgetjointangles) | 
| `/omnihand/omnihand_pro_2025/right/set_joint_angles`  | Set joint angles | [omnihand_pro_2025_node_msgs.srv.SetJointAngles](#omnihand_pro_2025_node_msgssrvsetjointangles) | 
| `/omnihand/omnihand_pro_2025/right/get_joint_angles`  | Get joint angles | [omnihand_pro_2025_node_msgs.srv.GetJointAngles](#omnihand_pro_2025_node_msgssrvgetjointangles) | 

**Note**:
- Service interfaces use joint angles (radians), not motor positions
- `SetJointAngles` waits for the hand to reach target angles, or returns on timeout
- `GetJointAngles` returns current joint angles and hand ready status
- O12 has 12 degrees of freedom, angle arrays contain 12 values

## Usage

### Build

See the root `README.md` for how to build the ROS2 node together with the SDK.

### Run

```bash
export LD_LIBRARY_PATH=$(pwd)/build/install/lib/:$LD_LIBRARY_PATH
cd build/install/bin/
./omnihand_pro_2025_node
```

After starting the node, you can use the Service interfaces to control the hand.

## Message Definitions

### `omnihand_pro_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # 12 values for O12 (in radians)
```

## Service Definitions

### `omnihand_pro_2025_node_msgs.srv.SetJointAngles`

Set target angles for all joints and wait for the hand to reach the target position.

**Request**:
```python
# Target joint angles (radians), 12 values for O12
float64[] target_angles
# Timeout in seconds (0 means use default: 5.0)
float64 timeout
```

**Response**:
```python
# Whether the movement was successful
bool success
# Final joint angles reached (radians), 12 values for O12
float64[] final_angles
# Error message if failed
string error_message
```

**Usage Example**:
```python
from omnihand_pro_2025_node_msgs.srv import SetJointAngles
import rclpy
from rclpy.node import Node

# Create service client
client = node.create_client(SetJointAngles, '/omnihand/omnihand_pro_2025/left/set_joint_angles')

# Wait for service to be available
client.wait_for_service()

# Create request
request = SetJointAngles.Request()
request.target_angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # 12 values
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

### `omnihand_pro_2025_node_msgs.srv.GetJointAngles`

Get current joint angles and hand ready status.

**Request**:
```python
# No parameters
```

**Response**:
```python
# Current joint angles (radians), 12 values for O12
float64[] angles
# Whether the hand is ready
bool is_ready
# Error message if any
string error_message
```

**Usage Example**:
```python
from omnihand_pro_2025_node_msgs.srv import GetJointAngles
import rclpy
from rclpy.node import Node

# Create service client
client = node.create_client(GetJointAngles, '/omnihand/omnihand_pro_2025/left/get_joint_angles')

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
