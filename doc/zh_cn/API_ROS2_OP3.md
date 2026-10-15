# OmniPicker 3（OP3）ROS2 接口

> ⚠️ **仅支持 Linux**：ROS2 节点随 Linux x64/aarch64 发布包提供，支持 Humble 和 Jazzy。

## 概述

OP3 ROS2 节点提供单轴夹爪的位置、PT 混合控制、行程控制、温柔夹持、状态查询和双区域触觉流。Topic 前缀为：

```text
/op3/<side>/
```

`<side>` 为 `left` 或 `right`。节点支持 `zlgcan`、`hcan`、`socketcan` 和 `rs485`。

## Topic

| Topic | 消息类型 | 方向 | 说明 |
|---|---|---|---|
| `joint_position_cmd` | `std_msgs/Int16MultiArray` | 订阅 | 单元素原始电机位置，范围 0–4095 |
| `joint_position_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | 发布 | 位置命令返回的单元素位置回读 |
| `joint_mix_control_cmd` | `sensor_msgs/JointState` | 订阅 | 单轴 PT；`position[0]` 为 ticks，`effort[0]` 为电流 mA |
| `stroke_cmd` | `omnipicker_3_node_msgs/StrokeCommand` | 订阅 | 目标行程 mm，可选速度和电流限制 |
| `stroke_states` | `omnipicker_3_node_msgs/StrokeState` | 发布 | `header` + 实际行程 `stroke_mm` |
| `gentle_grip_cmd` | `omnipicker_3_node_msgs/GentleGripCommand` | 订阅 | 进入温柔夹持，可选 `current_limit_ma` |
| `gentle_grip_states` | `omnipicker_3_node_msgs/GentleGripState` | 发布 | `header` + 固件原始回包 `raw_response` |
| `joint_error_cmd` | `std_msgs/Empty` | 订阅 | 触发错误码查询 |
| `joint_error_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | 发布 | 单元素错误码 |
| `joint_temperature_cmd` | `std_msgs/Empty` | 订阅 | 触发温度查询 |
| `joint_temperature_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | 发布 | 单元素温度 |
| `joint_current_cmd` | `std_msgs/Empty` | 订阅 | 触发电流查询 |
| `joint_current_states` | `omnihand_node_msgs/Int16MultiArrayStamped` | 发布 | 单元素电流 |
| `tactile_cmd` | `std_msgs/Float32` | 订阅 | `data` 为 Hz；>0 启动/改频，0 停止，上限 50 Hz |
| `tactile_states` | `omnipicker_3_node_msgs/TactileSensor` | 发布 | `header` + `thumb`、`index` 两个 `uint16[]` 原始区域 |

状态查询采用触发式回读；触觉流在收到非零频率后周期发布，直到收到 0。

PT 控制的位置范围为 0–4095 ticks，电流范围为 0–6500 mA；节点会将超出范围的值限制到有效区间。

## 专用消息

### StrokeCommand

```text
float64 stroke_mm
bool use_speed
int16 speed_rpm
bool use_current
int16 current_ma
```

行程范围为 -5.00–112.00 mm。节点会将行程转换为 SDK 使用的 0.01 mm 单位并限制到有效范围。速度范围为 0–5000 rpm，电流范围为 0–3500 mA。

### GentleGripCommand

```text
bool use_current_limit
uint16 current_limit_ma
```

> **重要**：OP3 协议只有进入温柔夹持的命令，没有退出命令。行程命令仍可用于重新张开，但设备会保持在温柔夹持模式；彻底退出需要断电重启。

### TactileSensor

```text
std_msgs/Header header
uint16[] thumb
uint16[] index
```

## 启动

```bash
# 默认配置
ros2 launch omnihand_node omnipicker_3_node.launch.py

# 指定配置
ros2 launch omnihand_node omnipicker_3_node.launch.py \
  config_file:=/path/to/omnipicker_3_node.yaml
```

默认配置文件为 `share/omnihand_node/config/omnipicker_3_node.yaml`。仅配置 `left_hand` 或 `right_hand` 中需要使用的一侧；`connection_type` 为空时跳过该侧。

## 配置参数

| 参数 | 类型 | 默认值 | 说明 |
|---|---|---|---|
| `hand_device_id` | int | 1 | 设备 ID |
| `connection_type` | string | `""` | `zlgcan` / `hcan` / `socketcan` / `rs485` |
| `canfd_serial_number` | string | `""` | ZLG/HCAN 适配器序列号，优先于设备索引 |
| `canfd_device_id` | int | 0 | ZLG/HCAN 适配器索引 |
| `canfd_channel_id` | int | 0 | CAN 通道索引 |
| `can_interface` | string | `can0` | SocketCAN 接口 |
| `uart_port` | string | `""` | RS485 串口路径 |
| `request_interval_ms` | int | -1 | SDK 请求最小间隔；-1 使用 SDK 默认值 |
| `frame_recv_timeout_ms` | int | -1 | 接收超时；-1 使用 SDK 默认值 |
| `show_data_details` | bool | false | 打印底层通信数据 |

## Python 测试脚本

脚本安装在 `lib/omnihand_node/scripts/omnipicker_3/`：

```bash
# 原始位置周期测试：0 ↔ 2048
python3 joint_position.py left 2048 1.5

# PT 周期测试：位置 0 ↔ 2048，电流 1000 mA
python3 mix_control_pt.py left 2048 1000 1.5

# 行程周期测试：0 ↔ 50 mm
python3 stroke.py left 50 3000 1000 2

# 行程张开与温柔夹持交替
python3 gentle_grip_stroke.py left \
  --open-mm 50 --grip-current-ma 1000 --period 2

# 10 Hz 启动触觉流；发送 0 停止
python3 tactile.py left 10
python3 tactile.py left 0
```

## 命令行示例

```bash
# 设置原始位置
ros2 topic pub --once /op3/left/joint_position_cmd \
  std_msgs/msg/Int16MultiArray '{data: [2048]}'

# 查询温度
ros2 topic pub --once /op3/left/joint_temperature_cmd std_msgs/msg/Empty '{}'
ros2 topic echo /op3/left/joint_temperature_states

# 以 10 Hz 发布触觉
ros2 topic pub --once /op3/left/tactile_cmd std_msgs/msg/Float32 '{data: 10.0}'
ros2 topic echo /op3/left/tactile_states
```

## 相关文档

- [ROS2 接口总览](API_ROS2.md)
- [OmniPicker 3 C++ API](API_CPP_OP3.md)
- [SocketCAN 设置指南](SOCKETCAN_SETUP.md)

