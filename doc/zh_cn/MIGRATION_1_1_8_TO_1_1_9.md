# 从 1.1.8 迁移到 1.1.9

本文列出 SDK 1.1.8 之后影响用户源码的公开 API 变化，不包含内部实现重构。

## 设备 ID 的发现与设置

产品类中的静态 `GetDeviceInfoFromBroadcast(...)` 已删除。现在应先创建并初始化对象，再通过实例查询地址：

```cpp
auto hand = OmniHand2025::createHandByHcan(HandType::LEFT, 0, 0, 0);
if (!hand || !hand->Init()) {
  return false;
}

const int id = hand->GetHandDeviceIdByBroadcast();
```

公共基类 `OmniHand` 新增：

```cpp
uint8_t GetNonPrivateHandDeviceIdByBroadcast();
int GetHandDeviceIdByBroadcast();
void SetBroadcast(bool broadcast);
bool SetHandDeviceIdByBroadcast(uint8_t id);
```

同时支持标准协议和私有协议的产品还提供：

```cpp
uint16_t GetPrivateHandDeviceIdByBroadcast();
uint16_t GetPrivateHandDeviceId();
```

广播常量分别是 `kBroadcastHandDeviceId == 0x00` 和 `kPrivateBroadcastHandDeviceId == 0x7ff`。

对 O10 和 H3L，`GetHandDeviceIdByBroadcast()` 的返回语义为：

- `0`：设备使用各协议的出厂默认 ID；
- `1..0x7f`：标准协议和私有协议查询到相同的自定义 ID；
- `-1`：查询失败，或两种协议的 ID 不一致。

`SetHandDeviceIdByBroadcast(0)` 恢复产品对应的出厂默认 ID；传入非零值则通过广播地址写入该 ID。`SetBroadcast()` 只改变 SDK 本地后续请求使用的目标地址，不会修改或保存设备参数。

`SetDeviceId()` 为兼容旧代码而保留，但已废弃。新代码应使用上述广播查询和设置接口。

## 工厂函数变化

- H3L 新增 `createHandByRs485(hand_type, hand_device_id, serial_port, baud_rate)`。
- OP3 新增 `createHandByRs485(hand_type, hand_device_id, uart_port, baudrate)`。
- OP3 公开头文件更名为 `omnihand/omnipicker_3.h`（原为 `omni_picker_3.h`）。
- H3UM 的手掌与 Xense 触觉共用同一个 SoC 端点：

```cpp
const std::string& tactile_soc_ip = OmniHand3UltraM::kDefaultTactileSocIp;
uint16_t tactile_soc_port = OmniHand3UltraM::kDefaultTactileSocPort;
```

默认值为 `192.168.99.2:19009`。SDK 会在内部探测该端点；使用默认地址时，调用者无需填写这两个参数。

## 通用信息接口

`OmniHand` 新增 `GetSN()`。`DeviceInfo` 新增 `hand_type`，其 `ToString()` 也会输出手型。`Version` 新增 `<` 和 `<=`，`Version::ToString()` 现在固定输出四段版本号（`major.minor.patch.res`）。

`ProductSerialNumber` 从 `private_omnihand.h` 移到了公开头文件 `proto.h`。

## 触觉接口

O10 的 1D 触觉接口与 H3L 的触觉兼容接口新增：

```cpp
size_t GetNumOfTactileSensors() const;
size_t GetNumOfTactilePoints(Finger finger) const;
size_t GetLenOfTactileDatum(Finger finger) const;
size_t GetNumOfRepliedTactileFrames(Finger finger) const;
std::string GetSNOfTactileSensor(Finger finger) const;
```

UMI 也实现了这些元数据查询。`GetSensorDataLength()` 已废弃；需要获取触觉点数时请使用 `GetNumOfTactilePoints()`。

H3UM 的 `PalmFrame` 新增 `resultant[3]` 和 `timestamp`。H3UM 的手掌与 Xense 服务现在使用相同的触觉 SoC IP。

## H3UM 控制接口

H3UM 新增位置 + 力矩 `MixControlByPT`，以及位置 + 速度 + 力矩 `MixControlByPVT`。力矩/电流按协议直接使用有符号 16 位 mA 值，公开范围为 `0..10000`，默认值为 `1000`。

H3UM 还新增 `SetAllMotor2Zero()` 和 `SetMotor2Zero(joint_motor_index)`。其他产品继承安全的“不支持”默认实现，并返回 `false`。

## OP3 新增接口

除 RS485 外，OP3 新增 `SetJointMotorPosiWithStatus()` 和 `GetJointDetailInfo()`，分别返回新的 `JointPosiStatus` 和 `JointDetailInfo` 结构。OP3 只有标准协议，不存在私有协议广播 ID。

## Python 变化

- 新增 `OmniHand.set_broadcast(bool)`。
- H3L 新增 `create_hand_by_rs485(...)`。
- O10 和 H3L 暴露了上述触觉元数据接口；UMI 提供对应 C++ 接口，但目前未绑定到 Python。
- 新增 `OmniHand3UltraM` 和 `OmniHand3UltraMSolver`；求解器位于 `omnihand.omnihand_3_ultra_m`。

Python 当前没有暴露所有 C++ 设备 ID 接口；需要这些操作时请使用 C++ API 或随 SDK 提供的设备 ID 工具。

## ROS2 消息兼容性

原来使用 `std_msgs/Int16MultiArray` 或 `std_msgs/Int8MultiArray` 的状态 topic，现改用 `omnihand_node_msgs` 中带时间戳的消息：

- `omnihand_node_msgs/msg/Int16MultiArrayStamped`
- `omnihand_node_msgs/msg/Int8MultiArrayStamped`

两者均包含 `header`、`layout` 和 `data`；采集时间从 `header.stamp` 获取。命令 topic 仍使用原来的 `std_msgs` 类型。

节点还新增 `joint_cmd_source`（`0=NORMAL`、`1=ANGLE`、`2=POSITION`）、`joint_angle_cmd`、`joint_position_cmd` 和带时间戳的 `joint_position_states`。`joint_states` 仍为 `sensor_msgs/JointState`，并通过 `header.stamp` 提供时间戳。
