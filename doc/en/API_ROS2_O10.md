# OmniHand 2025 (O10) ROS2 Interface

> ⚠️ **Linux Only**: ROS2 interface is only available on Linux. Windows is not supported.

## ROS2 Topics

| Topic Name                                           | Description                         | Node Action | Message Type                                                                                       | Notes |
|:----------------------------------------------------|:------------------------------------|:-----------:|:---------------------------------------------------------------------------------------------------|:------|
| `/omnihand/omnihand_2025/left/motor_angle`           | Joint motor angle                   |  Pub (You Sub)    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |
| `/omnihand/omnihand_2025/left/motor_angle_cmd`       | Joint motor angle command           |  Sub (You Pub)    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |
| `/omnihand/omnihand_2025/right/motor_angle`          | Joint motor angle                   |  Pub (You Sub)    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |
| `/omnihand/omnihand_2025/right/motor_angle_cmd`      | Joint motor angle command           |  Sub (You Pub)    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_nodemsgsmsgmotorangle)                       |       |

**Note**: O10 has 10 degrees of freedom. All arrays in messages contain 10 values.

## ROS2 Services

| Service Name                                           | Description | Service Type                                                                                              | Notes |  
|:----------------------------------------------|  :----:  |:--------:|-------------------------------------------------
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

After starting the node, you can use the Service interfaces to control the hand.

## Message Definitions

### `omnihand_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # 10 values for O10 (in radians)
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
