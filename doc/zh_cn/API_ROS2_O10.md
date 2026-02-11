# OmniHand 2025 (O10) ROS2 接口

> ⚠️ **仅限 Linux**：ROS2 接口仅在 Linux 上可用，不支持 Windows。

## ROS2 话题

| 话题名                                           | 话题描述 | 订阅 or 发布 | 消息类型                                                                                              | 备注 |  
|:----------------------------------------------|  :----:  |:--------:|---------------------------------------------------------------------------------------------------|  ---  |
| `/omnihand/omnihand_2025/left/control_mode`  | 关节电机控制模式 |    发布    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_node_msgsmsgcontrolmode) | 
| `/omnihand/omnihand_2025/left/current_report`  | 关节电机电流上报 |    发布    | [omnihand_2025_node_msgs.msg.CurrentReport](#omnihand_2025_node_msgsmsgcurrentreport) | 
| `/omnihand/omnihand_2025/left/current_threshold`  | 关节电机电流阈值 |    发布    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_node_msgsmsgcurrentthreshold) | 
| `/omnihand/omnihand_2025/left/motor_error_report`  | 关节电机错误上报 |    发布    | [omnihand_2025_node_msgs.msg.MotorErrorReport](#omnihand_2025_node_msgsmsgmotorerrorreport) | 
| `/omnihand/omnihand_2025/left/motor_angle`  | 关节电机角度 |    发布    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 
| `/omnihand/omnihand_2025/left/motor_pos`  | 关节电机位置 |    发布    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_node_msgsmsgmotorpos) | 
| `/omnihand/omnihand_2025/left/tactile_sensor`  | 触觉传感器数据 |    发布    | [omnihand_2025_node_msgs.msg.TactileSensor](#omnihand_2025_node_msgsmsgtactilesensor) | 
| `/omnihand/omnihand_2025/left/temperature_report`  | 关节电机温度上报 |    发布    | [omnihand_2025_node_msgs.msg.TemperatureReport](#omnihand_2025_node_msgsmsgtemperaturereport) | 
| `/omnihand/omnihand_2025/left/control_mode_cmd`  | 关节电机控制模式 |    订阅    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_node_msgsmsgcontrolmode) | 
| `/omnihand/omnihand_2025/left/current_threshold_cmd`  | 关节电机电流阈值 |    订阅    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_node_msgsmsgcurrentthreshold) | 
| `/omnihand/omnihand_2025/left/mix_control_cmd`  | 混合控制模式 |    订阅    | [omnihand_2025_node_msgs.msg.MixControl](#omnihand_2025_node_msgsmsgmixcontrol) | 
| `/omnihand/omnihand_2025/left/motor_pos_cmd`  | 关节电机位置 |    订阅    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_node_msgsmsgmotorpos) | 
| `/omnihand/omnihand_2025/left/motor_angle_cmd`  | 关节电机角度 |    订阅    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 
| `/omnihand/omnihand_2025/right/control_mode`  | 关节电机控制模式 |    发布    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_node_msgsmsgcontrolmode) | 
| `/omnihand/omnihand_2025/right/current_report`  | 关节电机电流上报 |    发布    | [omnihand_2025_node_msgs.msg.CurrentReport](#omnihand_2025_node_msgsmsgcurrentreport) | 
| `/omnihand/omnihand_2025/right/current_threshold`  | 关节电机电流阈值 |    发布    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_node_msgsmsgcurrentthreshold) | 
| `/omnihand/omnihand_2025/right/motor_error_report`  | 关节电机错误上报 |    发布    | [omnihand_2025_node_msgs.msg.MotorErrorReport](#omnihand_2025_node_msgsmsgmotorerrorreport) | 
| `/omnihand/omnihand_2025/right/motor_angle`  | 关节电机角度 |    发布    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 
| `/omnihand/omnihand_2025/right/motor_pos`  | 关节电机位置 |    发布    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_node_msgsmsgmotorpos) | 
| `/omnihand/omnihand_2025/right/tactile_sensor`  | 触觉传感器数据 |    发布    | [omnihand_2025_node_msgs.msg.TactileSensor](#omnihand_2025_node_msgsmsgtactilesensor) | 
| `/omnihand/omnihand_2025/right/temperature_report`  | 关节电机温度上报 |    发布    | [omnihand_2025_node_msgs.msg.TemperatureReport](#omnihand_2025_node_msgsmsgtemperaturereport) | 
| `/omnihand/omnihand_2025/right/control_mode_cmd`  | 关节电机控制模式 |    订阅    | [omnihand_2025_node_msgs.msg.ControlMode](#omnihand_2025_node_msgsmsgcontrolmode) | 
| `/omnihand/omnihand_2025/right/current_threshold_cmd`  | 关节电机电流阈值 |    订阅    | [omnihand_2025_node_msgs.msg.CurrentThreshold](#omnihand_2025_node_msgsmsgcurrentthreshold) | 
| `/omnihand/omnihand_2025/right/mix_control_cmd`  | 混合控制模式 |    订阅    | [omnihand_2025_node_msgs.msg.MixControl](#omnihand_2025_node_msgsmsgmixcontrol) | 
| `/omnihand/omnihand_2025/right/motor_pos_cmd`  | 关节电机位置 |    订阅    | [omnihand_2025_node_msgs.msg.MotorPos](#omnihand_2025_node_msgsmsgmotorpos) | 
| `/omnihand/omnihand_2025/right/motor_angle_cmd`  | 关节电机角度 |    订阅    | [omnihand_2025_node_msgs.msg.MotorAngle](#omnihand_2025_node_msgsmsgmotorangle) | 

**注意**：O10 有 10 个自由度。所有消息中的数组包含 10 个值。

## 使用方式

### 编译
参考根目录下的 README.md

### 运行
```bash
export LD_LIBRARY_PATH=$(pwd)/build/install/lib/:$LD_LIBRARY_PATH
cd build/install/bin/
./omnihand_2025_node
```

启动节点后，您可以订阅和发布上述话题。也可以使用 `node/scripts/omnihand_2025/` 下的 Python 脚本来订阅和发布话题。

## 消息定义

### `omnihand_2025_node_msgs.msg.ControlMode`

```python
std_msgs/Header header

# POSITION = 0,                    // 位置控制
# SERVO = 1,                       // 伺服模式
# VELOCITY = 2,                    // 速度控制
# TORQUE = 3,                      // 力矩控制（不支持，使用混合模式）
# POSITION_TORQUE = 4,             // 位置-力矩混合控制
# VELOCITY_TORQUE = 5,             // 速度-力矩混合控制
# POSITION_VELOCITY_TORQUE = 6,    // 位置-速度-力矩混合控制
# UNKNOWN = 10                     // 未知模式

int8[] modes  # O10 为 10 个值
```

**注意**：
- **SERVO 模式（1）**：伺服控制模式
- **纯力矩控制（TORQUE）不支持**：请使用混合控制模式（POSITION_TORQUE、VELOCITY_TORQUE、POSITION_VELOCITY_TORQUE）

### `omnihand_2025_node_msgs.msg.CurrentReport`

```python
std_msgs/Header header
uint16[] current_reports  # O10 为 10 个值
```

### `omnihand_2025_node_msgs.msg.CurrentThreshold`

```python
std_msgs/Header header
int16[] current_thresholds  # O10 为 10 个值
```

### `omnihand_2025_node_msgs.msg.MixControl`

```python
std_msgs/Header header
uint64[] mix_controls  # O10 为 10 个值
```

### `omnihand_2025_node_msgs.msg.MotorAngle`

```python
std_msgs/Header header
float64[] angles  # O10 为 10 个值
```

### `omnihand_2025_node_msgs.msg.MotorErrorReport`

```python
std_msgs/Header header

# unsigned char stalled_ : 1;      // 堵转标志
# unsigned char overheat_ : 1;     // 过热标志
# unsigned char over_current_ : 1; // 过流标志
# unsigned char motor_except_ : 1; // 电机异常
# unsigned char commu_except_ : 1; // 通信异常
# unsigned char res1_ : 3;         // 保留位
# unsigned char res2_;             // 保留字节

uint16[] error_reports  # O10 为 10 个值
```

### `omnihand_2025_node_msgs.msg.MotorPos`

```python
std_msgs/Header header
int16[] pos  # O10 为 10 个值，范围：0-4096
```

### `omnihand_2025_node_msgs.msg.MotorVel`

```python
std_msgs/Header header
int16[] vels  # O10 为 10 个值
```

### `omnihand_2025_node_msgs.msg.TactileSensor`

```python
std_msgs/Header header
TactileSensorData[] tactile_datas  # O10 使用 1D 触觉传感器
```

### `omnihand_2025_node_msgs.msg.TactileSensorData`

```python
uint8[] tactiles  # 1D 触觉传感器数据
```

### `omnihand_2025_node_msgs.msg.TemperatureReport`

```python
std_msgs/Header header
uint16[] temperature_reports  # O10 为 10 个值
```

## 相关文档

- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md) - C++ API 文档
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md) - Python API 文档
- [OmniHand Pro 2025 (O12) ROS2 接口](API_ROS2_O12.md) - O12 ROS2 接口
