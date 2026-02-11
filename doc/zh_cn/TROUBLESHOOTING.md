# 故障排除指南

[English](../en/TROUBLESHOOTING.md)

## 常见问题

### 1. 设备未找到 / 连接失败

**症状：**
- `Failed to initialize`
- `CANFD device not found`
- `USB device not found`

**解决方案：**

#### 检查硬件连接
```bash
# Linux: 检查设备是否被识别
lsusb | grep -i zlg
# 预期输出: Bus XXX Device YYY: ID 3068:0009 ZLG USBCANFD-200U

# 检查串口
ls -la /dev/ttyACM* /dev/ttyUSB*
```

#### 配置 USB 权限（Linux）
```bash
# 运行配置脚本
sudo ./setup_udev.sh

# 注销并重新登录（必须！）

# 验证权限
ls -la /dev/ttyACM0  # 应显示 rw-rw-rw-
```

#### 检查设备索引
如果有多个 CANFD 适配器：
```python
# 尝试不同的设备索引
hand = OmniHand2025.create_hand_by_zlgcan(HandType.LEFT, 1, 0, 0)  # 第一个适配器
hand = OmniHand2025.create_hand_by_zlgcan(HandType.LEFT, 1, 1, 0)  # 第二个适配器
```

---

### 2. 权限被拒绝

**症状：**
- `Permission denied: /dev/ttyACM0`
- `LIBUSB_ERROR_ACCESS`
- 需要使用 `sudo` 运行

**解决方案：**

#### Linux
```bash
# 运行 udev 配置
sudo ./setup_udev.sh

# 手动添加用户到组
sudo usermod -aG dialout $USER
sudo usermod -aG plugdev $USER

# 注销并重新登录

# 验证组成员资格
groups  # 应包含 'dialout' 和 'plugdev'
```

#### Windows
- 右键点击"以管理员身份运行"
- 如未安装，请安装 ZLG USB 驱动程序

---

### 3. 通信超时

**症状：**
- `Request timeout`
- `No response from device`
- 间歇性失败

**解决方案：**

#### 检查 CANFD 参数
```python
# 验证 hand_device_id 与实际设备匹配
info = hand.get_device_info()
print(f"实际设备 ID: {info.hand_device_id}")

# 如果不匹配，使用正确的 ID 创建
hand = OmniHand2025.create_hand_by_zlgcan(HandType.LEFT, actual_id, 0, 0)

# 如果不知道 canfd_device_id，可以使用 SN（序列号）接口：
hand = OmniHand2025.create_hand_by_zlgcan_sn(HandType.LEFT, 1, "YOUR_DEVICE_SN", 0)
```

> **关于 `canfd_channel_id` 说明**：
> - 双通道适配器（USBCANFD-200U）：`can0` → `channel_id=0`，`can1` → `channel_id=1`
> - 单通道适配器（USBCANFD-100U）：始终使用 `channel_id=0`

#### 检查电源
- 确保 24V 电源稳定
- 检查电源连接是否松动
- 灵巧手上的 LED 应亮起

#### 降低通信频率
```python
# 增加请求间隔
hand.set_request_interval(100)  # 请求间隔 100ms

# 增加响应超时阈值
hand.set_request_timeout(500)  # 每次请求超时 500ms
```

---

### 4. 电机不动

**症状：**
- 命令已发送但无移动
- 位置读数不变

**解决方案：**

#### 检查控制模式
```python
# 使用伺服模式（推荐）或位置模式
from omnihand import ControlMode

# 伺服模式 - 带速度/力矩限制的平滑运动
hand.set_all_control_mode(ControlMode.SERVO)

# 或位置模式 - 直接位置控制
hand.set_all_control_mode(ControlMode.POSITIONTION)
```

#### 检查位置范围
```python
# 有效范围通常是 0-4096
# 警告：由于手指机械限位，部分电机无法达到极限值（0 或 4096）
# 使用极限值可能导致电机堵转或触发保护

# 从安全的中间位置开始（demo 中测试过的值）
safe_pos = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
hand.set_all_joint_motor_posi(safe_pos)

# 或使用关节角度接口（推荐，自动处理限位）
angles = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
hand.set_all_active_joint_angles(angles)
```

#### 检查错误状态
```python
# 读取错误报告
errors = hand.get_all_error_report()
for i, err in enumerate(errors):
    if err:
        print(f"关节 {i+1} 错误: {err}")
```

---

### 5. 导入错误（Python）

**症状：**
- `ModuleNotFoundError: No module named 'omnihand'`
- `ImportError: DLL load failed`

**解决方案：**

#### 重新安装包
```bash
# 卸载并重新安装
pip uninstall omnihand
pip install python/omnihand-*.whl
```

#### 检查 Python 版本
```bash
python3 --version  # 应为 3.10+
```

#### 安装测试依赖（运行测试时需要）
```bash
pip install pytest numpy
```

#### Windows DLL 问题
- 安装 Visual C++ Redistributable 2019+
- 确保 `zlgcan.dll` 可访问

---

### 6. CMake 找不到包（C++）

**症状：**
- `Could not find package omnihand`
- `omnihand_DIR not set`

**解决方案：**

```cmake
# 设置正确的路径
set(OMNIHAND_ROOT "/usr/local")  # Linux 默认
# set(OMNIHAND_ROOT "C:/Program Files/omnihand2025")  # Windows

list(APPEND CMAKE_MODULE_PATH "${OMNIHAND_ROOT}/share/cmake/omnihand")
find_package(omnihand REQUIRED)
```

---

### 7. ROS2 节点无法启动（Linux）

**症状：**
- `Package 'omnihand_node' not found`
- 节点无法通信

**解决方案：**

```bash
# 先 source ROS2
source /opt/ros/humble/setup.bash

# 再 source omnihand
source ros2/setup.bash

# 验证包
ros2 pkg list | grep omnihand

# 检查节点
ros2 run omnihand_node omnihand_2025_node
```

---

## 诊断命令

### Linux

```bash
# 检查 USB 设备
lsusb -v | grep -A 20 "ZLG\|3068"

# 检查串口
dmesg | tail -20

# 检查 udev 规则
cat /etc/udev/rules.d/99-omnihand-usb.rules

# 检查用户组
groups $USER

# 测试设备访问
python3 -c "from omnihand import OmniHand2025; print('OK')"
```

### Windows

```cmd
# 在设备管理器中检查 USB
devmgmt.msc

# 检查 Python
python --version
pip show omnihand
```

---

## 获取帮助

如果问题仍然存在：

1. **收集信息：**
   - 操作系统版本
   - SDK 版本（查看 `VERSION` 文件）
   - 错误消息（完整输出）
   - 硬件型号（O10/O12，适配器类型）

2. **检查日志：**
   ```python
   # 启用详细输出
   import logging
   logging.basicConfig(level=logging.DEBUG)
   ```

3. **联系支持：**
   - 邮箱：support@agibot.com
   - 请附上收集到的信息
