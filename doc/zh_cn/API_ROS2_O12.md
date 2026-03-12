# OmniHand Pro 2025 (O12) ROS2 接口

> ⚠️ **仅限 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## ROS2 话题

| 话题名                                              | 话题描述 | 节点操作 | 消息类型                                                                                              | 备注 |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_pro_2025/left/motor_angle`  | 关节电机角度 |    发布（您订阅）    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle) | 
| `/omnihand/omnihand_pro_2025/left/motor_angle_cmd`  | 关节电机角度 |    订阅（您发布）    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle) | 
| `/omnihand/omnihand_pro_2025/right/motor_angle`  | 关节电机角度 |    发布（您订阅）    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle) | 
| `/omnihand/omnihand_pro_2025/right/motor_angle_cmd`  | 关节电机角度 |    订阅（您发布）    | [omnihand_pro_2025_node_msgs.msg.MotorAngle](#omnihand_pro_2025_nodemsgsmsgmotorangle) |

**注意**：O12 有 12 个自由度。所有消息中的数组包含 12 个值。

## ROS2 服务

| 服务名                                              | 服务描述 | 服务类型                                                                                              | 备注 |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_pro_2025/left/set_joint_angles`  | 设置关节角度 | [omnihand_pro_2025_node_msgs.srv.SetJointAngles](#omnihand_pro_2025_node_msgssrvsetjointangles) | 
| `/omnihand/omnihand_pro_2025/left/get_joint_angles`  | 获取关节角度 | [omnihand_pro_2025_node_msgs.srv.GetJointAngles](#omnihand_pro_2025_node_msgssrvgetjointangles) | 
| `/omnihand/omnihand_pro_2025/right/set_joint_angles`  | 设置关节角度 | [omnihand_pro_2025_node_msgs.srv.SetJointAngles](#omnihand_pro_2025_node_msgssrvsetjointangles) | 
| `/omnihand/omnihand_pro_2025/right/get_joint_angles`  | 获取关节角度 | [omnihand_pro_2025_node_msgs.srv.GetJointAngles](#omnihand_pro_2025_node_msgssrvgetjointangles) | 

**注意**：
- Service 接口使用关节角度（弧度），而不是电机位置
- `SetJointAngles` 会等待手部移动到目标角度，或超时返回
- `GetJointAngles` 返回当前关节角度和手部就绪状态
- O12 有 12 个自由度，角度数组包含 12 个值

## 使用方式

### 编译
参考根目录下的 README.md

### 运行
```bash
export LD_LIBRARY_PATH=$(pwd)/build/install/lib/:$LD_LIBRARY_PATH
cd build/install/bin/
./omnihand_pro_2025_node
```

启动节点后，您可以使用 Service 接口控制手部。

## 消息定义

### `omnihand_pro_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # O12 为 12 个值（单位：弧度）
```

## 服务定义

### `omnihand_pro_2025_node_msgs.srv.SetJointAngles`

设置所有关节的目标角度，并等待手部移动到目标位置。

**请求 (Request)**：
```python
# 目标关节角度（弧度），O12 为 12 个值
float64[] target_angles
# 超时时间（秒），0 表示使用默认值 5.0
float64 timeout
```

**响应 (Response)**：
```python
# 是否成功
bool success
# 最终到达的关节角度（弧度），O12 为 12 个值
float64[] final_angles
# 错误信息（如果失败）
string error_message
```

**使用示例**：
```python
from omnihand_pro_2025_node_msgs.srv import SetJointAngles
import rclpy
from rclpy.node import Node

# 创建服务客户端
client = node.create_client(SetJointAngles, '/omnihand/omnihand_pro_2025/left/set_joint_angles')

# 等待服务可用
client.wait_for_service()

# 创建请求
request = SetJointAngles.Request()
request.target_angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # 12 个值
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

### `omnihand_pro_2025_node_msgs.srv.GetJointAngles`

获取当前所有关节的角度和手部就绪状态。

**请求 (Request)**：
```python
# 无参数
```

**响应 (Response)**：
```python
# 当前关节角度（弧度），O12 为 12 个值
float64[] angles
# 手部是否就绪
bool is_ready
# 错误信息（如果有）
string error_message
```

**使用示例**：
```python
from omnihand_pro_2025_node_msgs.srv import GetJointAngles
import rclpy
from rclpy.node import Node

# 创建服务客户端
client = node.create_client(GetJointAngles, '/omnihand/omnihand_pro_2025/left/get_joint_angles')

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
- `is_ready` 为 `true` 表示手部已初始化并可以接收命令
- 如果手部未就绪，`error_message` 会包含原因

## 与 O10 的区别

1. **自由度**：O12 有 12 个自由度（O10 为 10 个）
2. **电机位置范围**：0-2000（O10 为 0-4096）
3. **触觉传感器**：3D 传感器（仅手指）vs 1D 传感器（手指、手心、手背）
4. **混合控制**：O12 **不支持** `mix_control_cmd` 话题
5. **控制模式**：所有控制模式均支持
6. **消息命名空间**：`omnihand_pro_2025_node_msgs`（O10 为 `omnihand_2025_node_msgs`）
7. **话题前缀**：`/omnihand/omnihand_pro_2025/`（O10 为 `/omnihand/omnihand_2025/`）

## 相关文档

- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md) - C++ API 文档
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md) - Python API 文档
