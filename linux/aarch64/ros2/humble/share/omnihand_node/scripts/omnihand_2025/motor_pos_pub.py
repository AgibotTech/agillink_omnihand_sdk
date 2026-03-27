#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node to publish left hand motor position commands (Omnihand2025)
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
import time


class LeftMotorPosPublisher(Node):
    def __init__(self):
        super().__init__('left_motor_pos_publisher')
        
        # 创建左手电机位置命令发布器
        self.left_motor_pos_cmd_publisher = self.create_publisher(
            MotorPos,
            '/omnihand/omnihand_2025/left/motor_pos_cmd',
            10
        )

        self.timer = self.create_timer(1.5, self.publish_left_motor_pos_cmd)
        
        # 位置命令数据
        self.position_counter = 0
        
        self.get_logger().info('Left Motor Position Publisher Node started')

    def publish_left_motor_pos_cmd(self):
        """发布左手电机位置命令"""
        msg = MotorPos()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "left_hand_frame"
        
        import math
        self.position_counter += 1

        msg.pos = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
        self.left_motor_pos_cmd_publisher.publish(msg)
        time.sleep(0.2)

        msg.pos = [2000, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
        self.left_motor_pos_cmd_publisher.publish(msg)
        time.sleep(0.2)

        msg.pos = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
        self.left_motor_pos_cmd_publisher.publish(msg)
        time.sleep(0.2)

        msg.pos = [1500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094]
        self.left_motor_pos_cmd_publisher.publish(msg)

        self.get_logger().debug(
            f'Published left motor position command: {msg.pos}'
        )

def main(args=None):
    rclpy.init(args=args)
    
    left_motor_pos_publisher = LeftMotorPosPublisher()
    
    try:
        rclpy.spin(left_motor_pos_publisher)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            left_motor_pos_publisher.get_logger().error(f'Error: {str(e)}')
        except:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            left_motor_pos_publisher.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down


if __name__ == '__main__':
    main()
