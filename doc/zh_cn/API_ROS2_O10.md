# OmniHand 2025 (O10) ROS2 接口

> ⚠️ **仅限 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## ROS2 话题

| 话题名                                           | 话题描述 | 节点操作 | 消息类型                                                                                              | 备注 |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_2025/left/motor_angle`  | 关节电机角度 |    发布（您订阅）    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 
| `/omnihand/omnihand_2025/left/motor_angle_cmd`  | 关节电机角度 |    订阅（您发布）    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 
| `/omnihand/omnihand_2025/right/motor_angle`  | 关节电机角度 |    发布（您订阅）    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 
| `/omnihand/omnihand_2025/right/motor_angle_cmd`  | 关节电机角度 |    订阅（您发布）    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) |

**注意**：O10 有 10 个自由度。所有消息中的数组包含 10 个值。

## ROS2 服务

| 服务名                                           | 服务描述 | 服务类型                                                                                              | 备注 |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_2025/left/set_joint_angles`  | 设置关节角度 |    服务    | [omnihand_2025_node_msgs.srv.SetJointAngles](#omnihand_2025_node_msgs_srv_SetJointAngles) | 
| `/omnihand/omnihand_2025/left/get_joint_angles`  | 获取关节角度 |    服务    | [omnihand_2025_node_msgs.srv.GetJointAngles](#omnihand_2025_node_msgs_srv_GetJointAngles) | 
| `/omnihand/omnihand_2025/right/set_joint_angles`  | 设置关节角度 |    服务    | [omnihand_2025_node_msgs.srv.SetJointAngles](#omnihand_2025_node_msgs_srv_SetJointAngles) | 
| `/omnihand/omnihand_2025/right/get_joint_angles`  | 获取关节角度 |    服务    | [omnihand_2025_node_msgs.srv.GetJointAngles](#omnihand_2025_node_msgs_srv_GetJointAngles) | 

**注意**：
- Service 接口使用关节角度（弧度），而不是电机位置
- `SetJointAngles` 会等待手部移动到目标角度，或超时返回
- `GetJointAngles` 返回当前关节角度和手部就绪状态
- O10 有 10 个自由度，角度数组包含 10 个值

## 使用方式

### 编译
参考根目录下的 README.md

### 运行
```bash
export LD_LIBRARY_PATH=$(pwd)/build/install/lib/:$LD_LIBRARY_PATH
cd build/install/bin/
./omnihand_2025_node
```

启动节点后，您可以使用 Service 接口控制手部。

## 消息定义

### `omnihand_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # O10 为 10 个值（单位：弧度）
```

## ROS2 服务

### `omnihand_2025_node_msgs.srv.SetJointAngles`

设置所有关节的目标角度，并等待手部移动到目标位置。

**请求 (Request)**：
```python
# 目标关节角度（弧度），O10 为 10 个值
float64[] target_angles
# 超时时间（秒），0 表示使用默认值 5.0
float64 timeout
```

**响应 (Response)**：
```python
# 是否成功
bool success
# 最终到达的关节角度（弧度），O10 为 10 个值
float64[] final_angles
# 错误信息（如果失败）
string error_message
```

**使用示例**：
```python
from omnihand_2025_node_msgs.srv import SetJointAngles
import rclpy
from rclpy.node import Node

# 创建服务客户端
client = node.create_client(SetJointAngles, '/omnihand/omnihand_2025/left/set_joint_angles')

# 等待服务可用
client.wait_for_service()

# 创建请求
request = SetJointAngles.Request()
request.target_angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # 10 个值
request.timeout = 5.0

# 调用服务
future = client.call_async(request)
rclpy.spin_until_future_complete(node, future)
response = future.result()

if response.success:
    print(f"成功移动到目标角度: {response.final_angles}")
else:
    print(f"失败: {response.error_message}")
```

**注意**：
- 角度单位为弧度
- 如果目标角度超出关节范围，服务会返回失败
- 超时时间从调用服务开始计算，如果手部在超时时间内未到达目标角度，会返回失败

### `omnihand_2025_node_msgs.srv.GetJointAngles`

获取当前所有关节的角度和手部就绪状态。

**请求 (Request)**：
```python
# (空 - 无需参数)
```

**响应 (Response)**：
```python
# 当前关节角度（弧度），O10 为 10 个值
float64[] angles
# 手部是否就绪
bool is_ready
# 错误信息（如果有）
string error_message
```

**使用示例**：
```python
from omnihand_2025_node_msgs.srv import GetJointAngles
import rclpy
from rclpy.node import Node

# 创建服务客户端
client = node.create_client(GetJointAngles, '/omnihand/omnihand_2025/left/get_joint_angles')

# 等待服务可用
client.wait_for_service()

# 创建请求（空请求）
request = GetJointAngles.Request()

# 调用服务
future = client.call_async(request)
rclpy.spin_until_future_complete(node, future)
response = future.result()

if response.is_ready:
    print(f"当前关节角度: {response.angles}")
    print(f"角度（度）: {[a * 180 / 3.14159 for a in response.angles]}")
else:
    print(f"手部未就绪: {response.error_message}")
```

**注意**：
- 角度单位为弧度
- `is_ready` 为 `true` 时表示手部已初始化并可以接收命令
- 如果手部未就绪，`error_message` 包含原因

## 相关文档

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md) - C++ API 文档
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md) - Python API 文档
- [OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md) - O12 ROS2 接口
