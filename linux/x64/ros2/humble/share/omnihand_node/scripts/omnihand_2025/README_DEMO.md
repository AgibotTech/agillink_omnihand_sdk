# Service API Demo 使用说明

## 简介

`demo_service_api.py` 是一个完整的演示脚本，展示如何使用简化后的Service API接口。

## 三个核心接口

1. **open_hand** - 创建/打开手部连接
2. **set_position** - 设置手部位置
3. **get_state** - 获取手部状态

## 使用方法

### 1. 启动ROS2节点

首先需要启动手部节点：

```bash
# 方式1：使用ros2 run
ros2 run omnihand_node omnihand_2025_node

# 方式2：使用launch文件
ros2 launch omnihand_node omnihand_2025_node.launch.py
```

### 2. 运行Demo脚本

在另一个终端运行demo：

```bash
# 使用左手（默认）
python3 demo_service_api.py

# 或指定右手
python3 demo_service_api.py right
```

### 3. Demo执行流程

Demo会自动执行以下步骤：

1. **打开手** - 初始化连接
2. **获取初始状态** - 查看当前位置
3. **移动到位置1** - 张开手
4. **移动到位置2** - 半握
5. **移动到位置3** - 握紧
6. **回到初始位置** - 恢复

## 代码示例

### 基本使用

```python
import rclpy
from rclpy.node import Node
from omnihand import OmniHand2025
from omnihand_2025_node_msgs.srv import OpenHand, SetPosition, GetState

class HandController(Node):
    def __init__(self):
        super().__init__('hand_controller')
        
        # 创建Service客户端
        self.open_client = self.create_client(
            OpenHand, '/omnihand/omnihand_2025/left/open_hand'
        )
        self.set_client = self.create_client(
            SetPosition, '/omnihand/omnihand_2025/left/set_position'
        )
        self.get_client = self.create_client(
            GetState, '/omnihand/omnihand_2025/left/get_state'
        )
    
    def open_hand(self):
        """打开手"""
        request = OpenHand.Request()
        request.hand_type = 'left'
        request.hand_device_id = OmniHand2025.kDefaultHandDeviceId
        request.canfd_device_id = 0
        request.canfd_channel_id = 0
        
        self.open_client.wait_for_service()
        future = self.open_client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        
        return future.result().success
    
    def set_position(self, positions, timeout=5.0):
        """设置位置"""
        request = SetPosition.Request()
        request.target_positions = positions
        request.timeout = timeout
        
        self.set_client.wait_for_service()
        future = self.set_client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        
        return future.result().success
    
    def get_state(self):
        """获取状态"""
        request = GetState.Request()
        
        self.get_client.wait_for_service()
        future = self.get_client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        
        return future.result()

def main():
    rclpy.init()
    controller = HandController()
    
    # 1. 打开手
    if controller.open_hand():
        print("Hand opened successfully")
    
    # 2. 设置位置
    target = [1000, 2000, 3000, 2000, 3000, 3000, 2000, 3000, 3000, 3000]
    if controller.set_position(target):
        print("Position set successfully")
    
    # 3. 获取状态
    state = controller.get_state()
    print(f"Current positions: {state.positions}")
    
    rclpy.shutdown()

if __name__ == '__main__':
    main()
```

## 注意事项

1. **确保节点运行**：运行demo前必须启动手部节点
2. **Python 示例中的 `OmniHand2025.kDefaultHandDeviceId`**：需安装 OmniHand Python 绑定（`omnihand` 包），与 C++ `OmniHand2025::kDefaultHandDeviceId` 一致；若未安装，可改为与节点参数一致的整型默认值（通常为 `1`）
3. **检查硬件连接**：确保手部硬件已连接
4. **权限问题**：如果使用USB设备，可能需要权限设置
5. **超时设置**：根据实际移动速度调整timeout参数

## 故障排除

### 问题1：Services not available
**原因**：手部节点未启动
**解决**：先运行 `ros2 run omnihand_node omnihand_2025_node`

### 问题2：Failed to open hand
**原因**：硬件未连接或配置错误
**解决**：检查硬件连接和设备ID配置

### 问题3：Timeout reached
**原因**：移动时间超过设置的timeout
**解决**：增加timeout值或检查目标位置是否合理

## 更多信息

- 详细API文档：`SIMPLIFIED_API_USAGE.md`
- 接口设计分析：`INTERFACE_DESIGN_ANALYSIS.md`
- 接口决策说明：`INTERFACE_DECISION.md`
