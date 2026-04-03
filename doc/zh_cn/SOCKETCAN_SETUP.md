# SocketCAN 使用手册

[English](../en/SOCKETCAN_SETUP.md)

> **仅适用于 Linux。** SocketCAN 是 Linux 内核能力，Windows 不支持。  
> 若您使用 **ZLG USBCANFD 用户态库**，**无需**阅读本文。

---

## 1. 从厂商处下载并安装 SocketCAN 驱动

请严格按 **厂商 PDF 或安装包说明** 操作（依赖、编译/安装内核模块、`insmod`/`modprobe`、USB 检查等）。

**说明：** 驱动需与内核版本匹配；若厂商提供安装脚本，优先使用。完成后应能看到 CAN 网络接口（见下一节）。

---

## 2. 使用 `ip link show` 检查接口

执行：

```bash
ip link show
```

在输出中查找 **`can0`**、**`can1`** 等（也可使用 `ip link show can0`）。

- 若已有接口，继续 **第 3 节** 设置 OmniHand 位时序。  
- 若 **没有** `can` 接口，请回到 **第 1 节**（驱动、USB 枚举）或查阅厂商文档。

可选：

```bash
ls /sys/class/net | grep -E '^can'
```

---

## 3. 设置 OmniHand 总线参数

OmniHand 典型 CAN FD 配置：

- **仲裁段**：1 Mbps，采样点 **80%**  
- **数据段**：5 Mbps，采样点 **75%**

```bash
sudo ip link set can0 down
sudo ip link set can0 type can fd on bitrate 1000000 dbitrate 5000000 sample-point 0.8 dsample-point 0.75
sudo ip link set can0 up
```

验证：

```bash
ip -details link show can0
```

---

## 4. 第一个程序

前提：`can0`（或您使用的接口）已按第 3 节配置并处于 **UP** 状态。

**C++：**

```cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(demo_omnihand_2025_socketcan O10_demo_socketcan.cc)
  target_link_libraries(demo_omnihand_2025_socketcan PRIVATE ${OMNIHAND_TARGET})
endif()
```

```cpp
#include "omnihand/omnihand_2025.h"
#include <vector>
// ...
auto hand = OmniHand2025::createHandSocketCan(HandType::LEFT, 1, "can0");
if (!hand || !hand->Init()) { /* 检查第 2、3 节 */ }
hand->SetAllActiveJointAngles(std::vector<double>(10, 0.0));  // O10, radians
```

**Python：**

```python
from omnihand import OmniHand2025, HandType
hand = OmniHand2025.create_hand_socketcan(
    hand_type=HandType.LEFT, hand_device_id=1, can_interface="can0")
if not hand.init():
    ...
hand.set_all_active_joint_angles([0.0] * 10)  # O10, radians
```

## 5. API 文档

**C++**

- [C++ API 索引](API_CPP.md)  
- [OmniHand 2025 (O10) C++ API](API_CPP_O10.md)  
- [OmniHand Pro 2025 (O12) C++ API](API_CPP_O12.md)  
- [OmniHand Dex UMI C++ API](API_CPP_O10_UMI.md)

**Python**

- [Python API 索引](API_PYTHON.md)  
- [OmniHand 2025 (O10) Python API](API_PYTHON_O10.md)  
- [OmniHand Pro 2025 (O12) Python API](API_PYTHON_O12.md)  
- [OmniHand Dex UMI Python API](API_PYTHON_UMI.md)

**ROS2（仅 Linux）**

- [ROS2 API 索引](API_ROS2.md)  
- [OmniHand 2025 (O10) ROS2](API_ROS2_O10.md)  
- [OmniHand Pro 2025 (O12) ROS2](API_ROS2_O12.md)

另见 [快速入门](QUICK_START.md)、[故障排除](TROUBLESHOOTING.md)。

---

## 6. 卸载 SocketCAN 内核模块

停止使用 SocketCAN 驱动时：

```bash
sudo rmmod usbcanfd
```

若需同时移除 `can_dev`（视系统而定）：

```bash
sudo rmmod can_dev
```

模块未加载时的提示可忽略。卸载后 `can0` 将消失；需再次使用 SocketCAN 时，请重复第 1～3 节或厂商脚本。
