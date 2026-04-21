#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Subscribe to motor position state of OmniHand3Ultra (O20) via sensor_msgs/JointState.

Usage:  python3 motor_pos_sub.py [left|right] [product]
        默认 side=left, product=h3u
Topic:  /<product>/<side>/motor_pos_state
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


class MotorPosSubscriber(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_motor_pos_subscriber')
        self.hand_side = hand_side
        self.product = product
        self.subscription = self.create_subscription(
            JointState,
            f'/{product}/{hand_side}/motor_pos_state',
            self.motor_pos_callback,
            10,
        )
        self.get_logger().info(
            f'{product}/{hand_side} motor_pos_state subscriber started (O20, 20 DOF)'
        )

    def motor_pos_callback(self, msg: JointState):
        self.get_logger().info(
            f'{self.product}/{self.hand_side} motor_pos_state '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}): '
            f'{[round(p, 2) for p in msg.position]}'
        )


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u'
    node = MotorPosSubscriber(hand_side, product)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        try:
            rclpy.shutdown()
        except Exception:
            pass


if __name__ == '__main__':
    main()
