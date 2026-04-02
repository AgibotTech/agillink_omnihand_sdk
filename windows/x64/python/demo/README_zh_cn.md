# Python 示例

## OmniHand 2025（10 自由度）示例
- `omnihand_2025/demo_get_hardware_info.py` - 获取硬件信息
- `omnihand_2025/demo_monitor_current.py` - 监控电机电流
- `omnihand_2025/demo_monitor_error.py` - 监控错误
- `omnihand_2025/demo_monitor_temperature.py` - 监控温度
- `omnihand_2025/demo_set_angle.py` - 设置关节角度
- `omnihand_2025/demo_set_motion.py` - 设置运动参数
- `omnihand_2025/demo_set_motor.py` - 电机直接控制
- `omnihand_2025/demo_set_motor_via_multicans.py` - 通过多个CANFD适配器控制
- `omnihand_2025/demo_set_motor_via_multichannels.py` - 通过多个CAN通道控制
- `omnihand_2025/demo_set_motor_via_multisocketcans.py` - 通过多个SocketCAN接口控制
- `omnihand_2025/demo_set_torque.py` - 设置力矩
- `omnihand_2025/demo_tactile_sensor.py` - 读取触觉传感器
- `omnihand_2025/demo_tactile_sensor_raw.py` - 读取触觉传感器原始数据
- `omnihand_2025/demo_tactile_sensor_raw_reliability.py` - 触觉传感器可靠性测试
- `omnihand_2025/demo_all_tactile_sensor_raw_reliability.py` - 所有触觉传感器可靠性测试
- `omnihand_2025/demo_set_angle_and_tactile_sensor_reliability.py` - 角度和触觉传感器可靠性测试
- `omnihand_2025/demo_100set_and_20tactile_reliability.py` - 高频控制和传感器测试
- `omnihand_2025/demo_canfd_id.py` - CANFD通信示例（通过设备ID）
- `omnihand_2025/demo_canfd_serial.py` - CANFD通信示例（通过序列号）
- `omnihand_2025/demo_socketcan.py` - SocketCAN 示例（仅 Linux）

## OmniHand Pro 2025（12 自由度）示例
- `omnihand_pro_2025/demo_get_hardware_info.py` - 获取硬件信息
- `omnihand_pro_2025/demo_monitor_current.py` - 监控电机电流
- `omnihand_pro_2025/demo_monitor_error.py` - 监控错误
- `omnihand_pro_2025/demo_monitor_temperature.py` - 监控温度
- `omnihand_pro_2025/demo_sensor_touch.py` - 读取触觉传感器
- `omnihand_pro_2025/demo_set_angle.py` - 设置关节角度
- `omnihand_pro_2025/demo_set_position.py` - 设置关节位置
- `omnihand_pro_2025/demo_set_velocity.py` - 设置关节速度
- `omnihand_pro_2025/demo_canfd_id.py` - CANFD通信示例（通过设备ID）
- `omnihand_pro_2025/demo_canfd_serial.py` - CANFD通信示例（通过序列号）
- `omnihand_pro_2025/demo_socketcan.py` - SocketCAN 示例（仅 Linux）

## OmniHand Dex UMI（10 自由度，UMI 协议）示例
- **运动学（Python）：** `OmniHandDexUMI.get_all_active_joint_angles()`、`get_all_joint_angles()`、`get_all_joint_pos(...)` 与 O10 共用求解器，只读（无 set 角度类接口）。
- `omnihand_dex_umi/demo_get_hardware_info.py` - 获取硬件信息
- `omnihand_dex_umi/demo_tactile_sensor_raw.py` - 读取触觉原始数据（Pn6 协议）
- `omnihand_dex_umi/demo_periodic_report.py` - 接收周期上报（位置与触觉）
- `omnihand_dex_umi/demo_set_max_min_calibration.py` - 设置最大/最小位置标定
- `omnihand_dex_umi/demo_umi_to_o10.py` - 使用UMI位置数据控制O10灵巧手
- `omnihand_dex_umi/demo_canfd_id.py` - CANFD通信示例（通过设备ID）
- `omnihand_dex_umi/demo_canfd_serial.py` - CANFD通信示例（通过序列号）
- `omnihand_dex_umi/demo_socketcan.py` - SocketCAN 示例（仅 Linux）

## 使用说明

```bash
# 确保已安装 omnihand 包
pip list | findstr omnihand

# 运行示例
python omnihand_2025\demo_get_hardware_info.py
python omnihand_pro_2025\demo_get_hardware_info.py
python omnihand_dex_umi\demo_get_hardware_info.py
```
