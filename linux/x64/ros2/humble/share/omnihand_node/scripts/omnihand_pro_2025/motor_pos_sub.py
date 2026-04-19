#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node to subscribe to motor_pos topic (Omnihand2025Pro)
Usage: python3 motor_pos_sub.py [left|right]
"""

import sys
import os

try:
    from omnihand_pro_2025_node_msgs.msg import MotorPos
except ImportError:
    print("Error: Cannot import omnihand_pro_2025_node_msgs.msg")
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
    def __init__(self, hand_side):
        super().__init__(f'{hand_side}_motor_pos_subscriber')

        self.hand_side = hand_side
        self.subscription = self.create_subscription(
            MotorPos,
            f'/omnihand/omnihand_pro_2025/{hand_side}/motor_pos',
            self.motor_pos_callback,
            10
        )

        self.get_logger().info(f'{hand_side.capitalize()} Motor Position Subscriber Node started (O12, 12 DOF)')

    def motor_pos_callback(self, msg):
        self.get_logger().info(
            f'Received {self.hand_side} motor positions: {msg.pos}, '
            f'frame_id: {msg.header.frame_id}, '
            f'timestamp: {msg.header.stamp.sec}.{msg.header.stamp.nanosec}'
        )

def main(args=None):
    rclpy.init(args=args)

    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1].lower()

    node = MotorPosSubscriber(hand_side)

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    except Exception as e:
        try:
            node.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        try:
            node.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass

if __name__ == '__main__':
    main()
