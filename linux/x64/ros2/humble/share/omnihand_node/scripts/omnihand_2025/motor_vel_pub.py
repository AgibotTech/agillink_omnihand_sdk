#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node to publish motor velocity commands (Omnihand2025)
Usage: python3 motor_vel_pub.py [left|right]
"""

import sys
import os

try:
    from omnihand_2025_node_msgs.msg import MotorVel
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


class MotorVelPublisher(Node):
    def __init__(self, hand_side):
        super().__init__(f'{hand_side}_motor_vel_publisher')

        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            MotorVel,
            f'/omnihand/omnihand_2025/{hand_side}/motor_vel_cmd',
            10
        )

        self.timer = self.create_timer(1.5, self.publish_motor_vel_cmd)
        self.velocity_counter = 0

        self.get_logger().info(f'{hand_side.capitalize()} Motor Velocity Publisher Node started')

    def publish_motor_vel_cmd(self):
        msg = MotorVel()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = f"{self.hand_side}_hand_frame"

        self.velocity_counter += 1

        msg.vels = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        self.publisher.publish(msg)

        self.get_logger().debug(
            f'Published {self.hand_side} motor velocity command: {msg.vels}'
        )

def main(args=None):
    rclpy.init(args=args)

    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1].lower()

    node = MotorVelPublisher(hand_side)

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
