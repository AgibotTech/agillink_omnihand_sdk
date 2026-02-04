#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node to subscribe to motor_pos topic (Omnihand2025)
"""

# Note: This script requires ROS2 environment to be sourced.
# Please run: source /path/to/ros2/setup.bash (or source ros2/setup.bash)
# The setup.bash script sets PYTHONPATH and LD_LIBRARY_PATH automatically.

import sys
import os

# Check if ROS2 environment is properly sourced
try:
    from omnihand_2025_node_msgs.msg import MotorPos
except ImportError:
    print("Error: Cannot import omnihand_2025_node_msgs.msg")
    print("")
    print("Please make sure you have sourced the ROS2 setup script:")
    print("  source ros2/humble/setup.bash")
    print("  # or")
    print("  source ros2/setup.bash")
    print("")
    print("Current PYTHONPATH:", os.environ.get('PYTHONPATH', 'Not set'))
    print("")
    sys.exit(1)

import rclpy
from rclpy.node import Node


class MotorPosSubscriber(Node):
    def __init__(self):
        super().__init__('motor_pos_subscriber')
        
        # 创建订阅器
        self.subscription = self.create_subscription(
            MotorPos,
            '/omnihand/omnihand_2025/left/motor_pos',
            self.motor_pos_callback,
            10
        )
        self.subscription  # prevent unused variable warning

        self.get_logger().info('Motor Position Subscriber Node started')

    def motor_pos_callback(self, msg):
        """处理接收到的MotorPos消息"""
        self.get_logger().info(
            f'Received motor positions: {msg.pos}, '
            f'frame_id: {msg.header.frame_id}, '
            f'timestamp: {msg.header.stamp.sec}.{msg.header.stamp.nanosec}'
        )

def main(args=None):
    rclpy.init(args=args)
    
    motor_pos_subscriber = MotorPosSubscriber()
    
    try:
        rclpy.spin(motor_pos_subscriber)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            motor_pos_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            motor_pos_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down


if __name__ == '__main__':
    main()
