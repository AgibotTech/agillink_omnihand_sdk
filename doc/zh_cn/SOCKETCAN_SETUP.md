# SocketCAN 配置与使用说明

> ⚠️ **仅限 Linux**：SocketCAN 是 Linux 内核功能，在 Windows 上不可用。

**适用场景：**
- 你在 Linux 上已经有 SocketCAN 环境（板载 CAN、PCIe CAN、其他已适配 SocketCAN 的设备），或者
- 你确实需要把 ZLG USBCANFD-200U 之类的 USB 设备映射成 `can0` 来统一走 SocketCAN。

对于大多数只使用 ZLG USB 转 CANFD 设备的用户，**推荐直接使用 ZLG 库方式**，不需要本文件中的步骤。

---

## 1. 总体思路

- SocketCAN 在内核里把 CAN 设备抽象成网络接口（如 `can0`, `can1`）。
- SDK 通过产品特定的工厂方法：
  - C++：`OmniHand2025::createHandSocketCan(...)`、`OmniHandPro2025::createHandSocketCan(...)`、`OmniHandDexUMI::createHandSocketCan(...)`
  - Python：`OmniHand2025.create_hand_socketcan(...)`、`OmniHandPro2025.create_hand_socketcan(...)`、`OmniHandDexUMI.create_hand_socketcan(...)`
  
  直接使用这些接口名进行通信。

**注意：**
- 使用 SocketCAN 时，底层是内核驱动，不再通过 ZLG 用户态库；
- 同一块 USB 设备**不能同时**被 ZLG 库和 SocketCAN 驱动占用，二者是互斥关系。

---

## 2. 确认是否已有 SocketCAN 接口

```bash
ip link show | grep -E "can[0-9]"
ls /sys/class/net | grep -E "^can"
```

如果已经看到 `can0`、`can1` 等接口，并且可以用 `candump can0` 收发报文，可以直接跳到「第 4 节：在 SDK 中使用 SocketCAN」。

---

## 3. 为 ZLG USBCANFD 设备启用 SocketCAN（可选，高级用法）

如果你使用的是 ZLG USBCANFD-200U 之类的 USB 设备，并且想通过 SocketCAN 访问，可以使用仓库自带的内核驱动示例（位于 `cpp/third_party/zlgcan/socketcan`）。

### 3.1 安装编译依赖

```bash
sudo apt-get update
sudo apt-get install -y gcc-12 build-essential linux-headers-$(uname -r) can-utils
```

### 3.2 编译内核模块

```bash
cd cpp/third_party/zlgcan/socketcan
make clean
make
```

生成的 `usbcanfd.ko` 即为 SocketCAN 驱动模块。

### 3.3 加载驱动并创建 `can0`/`can1`

推荐使用厂商脚本：

```bash
cd cpp/third_party/zlgcan/socketcan
sudo bash driver_load.sh
```

脚本会：
- 卸载旧的 `usbcanfd` / `can_dev` 驱动（如果有）；
- 加载 `can_dev` 和刚刚编译好的 `usbcanfd.ko`；
- 为设备创建并配置 `can0`、`can1` 等接口（默认 bitrate 500 kbps，data bitrate 2 Mbps）；
- 将接口置为 `UP` 状态。

之后可以用：

```bash
ip link show can0
candump can0
```

来验证 SocketCAN 是否正常工作。

> 如果你想使用不同的速率，可以参考 `driver_load.sh` 或 `readme.txt` 中的示例，自行调整 `ip link set can0 type can ...` 的参数。

---

## 4. 在 SDK 中通过 SocketCAN 使用手爪

### 4.1 C++：`createHandSocketCan`

前提：
- 已安装 SDK（例如安装到 `/usr/local`）；
- `can0` 已经通过 SocketCAN 正常配置并 `up`。

示例：

OmniHand 2025 (O10) 示例：

```cpp
#include "omnihand/omnihand_2025.h"

int main() {
    // SocketCAN 示例：使用 can0 接口（OmniHand 2025 O10）
    // hand_type    : HandType::LEFT / HandType::RIGHT
    // device_id    : 手上固件中配置的设备 ID（通常为 1）
    // can0         : Linux SocketCAN 接口名
    auto hand = OmniHand2025::createHandSocketCan(
        HandType::LEFT,
        1,
        "can0"
    );

    if (!hand || !hand->Init()) {
        std::cerr << "[错误]: 初始化 SocketCAN 设备 (can0) 失败。" << std::endl;
        std::cerr << "请检查：" << std::endl;
        std::cerr << "  1. ip link show can0" << std::endl;
        std::cerr << "  2. sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on" << std::endl;
        std::cerr << "  3. sudo ip link set can0 up" << std::endl;
        return -1;
    }

    // 读取厂家信息
    auto vendor = hand->GetVendorInfo();
    std::cout << vendor.toString() << std::endl;

    // 设置全部关节的电机位置（O10 范围：0-4096）
    std::vector<int16_t> positions{500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094};
    hand->SetAllJointMotorPosi(positions);

    return 0;
}
```

OmniHand Pro 2025 (O12) 示例：

```cpp
#include "omnihand/omnihand_pro_2025.h"

int main() {
    // SocketCAN 示例：使用 can0 接口（OmniHand Pro 2025 O12）
    auto hand = OmniHandPro2025::createHandSocketCan(
        HandType::LEFT,
        1,
        "can0"
    );

    if (!hand || !hand->Init()) {
        std::cerr << "[错误]: 初始化 SocketCAN 设备 (can0) 失败。" << std::endl;
        return -1;
    }

    // 设置全部关节的电机位置（O12 范围：0-2000）
    std::vector<int16_t> positions{500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500};
    hand->SetAllJointMotorPosi(positions);

    return 0;
}
```

### 4.2 Python：`create_hand_socketcan`

前提同上：`can0` 已经配置好。

OmniHand 2025 (O10) 示例：

```python
from omnihand import OmniHand2025, HandType

def main():
    # SocketCAN 示例（OmniHand 2025 O10）
    hand = OmniHand2025.create_hand_socketcan(
        hand_type=HandType.LEFT,
        device_id=1,       # 手固件中的设备 ID
        can_interface="can0"
    )

    if not hand.init():
        print("[错误]: 初始化 SocketCAN 设备 (can0) 失败。")
        print("请检查：")
        print("  1. ip link show can0")
        print("  2. sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on")
        print("  3. sudo ip link set can0 up")
        return

    vendor = hand.get_vendor_info()
    print(vendor)

    # 设置电机位置（O10 范围：0-4096）
    positions = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
    hand.set_all_joint_positions(positions)

if __name__ == "__main__":
    main()
```

OmniHand Pro 2025 (O12) 示例：

```python
from omnihand import OmniHandPro2025, HandType

def main():
    # SocketCAN 示例（OmniHand Pro 2025 O12）
    hand = OmniHandPro2025.create_hand_socketcan(
        hand_type=HandType.LEFT,
        device_id=1,
        can_interface="can0"
    )

    if not hand.init():
        print("[错误]: 初始化 SocketCAN 设备 (can0) 失败。")
        return

    # 设置电机位置（O12 范围：0-2000）
    positions = [500, 1000, 1500, 2000, 1000, 1500, 500, 1000, 1500, 2000, 1000, 1500]
    hand.set_all_joint_positions(positions)

if __name__ == "__main__":
    main()
```

> 更完整的 Python 示例可以参考 `python/demo/omnihand_2025/demo_socketcan.py` 或 `python/demo/omnihand_pro_2025/demo_socketcan.py`。

---

## 5. 与 ZLG 库方式的切换

同一块 ZLG USB 设备：
- **使用 ZLG 库方式（推荐）时**：不要加载 `usbcanfd.ko`，直接用产品特定的工厂方法，如 `OmniHand2025::createHandByZlgcan(..., canfd_device_id, canfd_channel_id)` / `OmniHand2025.create_hand_by_zlgcan(..., canfd_device_id, canfd_channel_id)`；
- **使用 SocketCAN 方式时**：需要先确保内核驱动已加载并创建 `can0`，然后用产品特定的工厂方法，如 `OmniHand2025::createHandSocketCan` / `OmniHand2025.create_hand_socketcan`。

切换思路：
- 如果当前加载了 `usbcanfd.ko`，想回到 ZLG 库方式：

```bash
sudo rmmod usbcanfd
sudo rmmod can_dev   # 如无报错可忽略
```

然后重新运行使用 `canfd_device_id` / `canfd_channel_id` 的示例或自己写的程序即可。

---

## 6. 快速配置命令

对于板载 CAN 或其他 SocketCAN 设备，配置接口：

```bash
# 配置 CAN 接口
sudo ip link set can0 type can bitrate 1000000 dbitrate 5000000 fd on

# 启动接口
sudo ip link set can0 up

# 验证接口状态
ip link show can0

# 使用 candump 测试（可选）
candump can0
```

对于使用内核驱动的 USB CANFD 设备，使用厂商脚本，如第 3.3 节所述。
