#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node to publish motor position commands (OmniHand3Ultra O20)
Usage: python3 motor_pos_pub.py [left|right]
"""

import sys
import os

try:
    from omnihand_3_ultra_node_msgs.msg import MotorPos
except ImportError:
    print("Error: Cannot import omnihand_3_ultra_node_msgs.msg")
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


class MotorPosPublisher(Node):
    def __init__(self, hand_side):
        super().__init__(f'{hand_side}_motor_pos_publisher')

        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            MotorPos,
            f'/omnihand/omnihand_3_ultra/{hand_side}/motor_pos_cmd',
            10
        )

        self.timer = self.create_timer(1.5, self.publish_motor_pos_cmd)
        self.position_counter = 0

        self.get_logger().info(f'{hand_side.capitalize()} Motor Position Publisher Node started (O20, 20 DOF)')

    def publish_motor_pos_cmd(self):
        msg = MotorPos()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = f"{self.hand_side}_hand_frame"

        self.position_counter += 1

        pos_open = [2048] * 20
        pos_close = [500] * 20

        msg.pos = pos_open
        self.publisher.publish(msg)
        time.sleep(0.5)

        msg.pos = pos_close
        self.publisher.publish(msg)

        self.get_logger().debug(
            f'Published {self.hand_side} motor position command: {msg.pos}'
        )

def main(args=None):
    rclpy.init(args=args)

    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1].lower()

    node = MotorPosPublisher(hand_side)

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
