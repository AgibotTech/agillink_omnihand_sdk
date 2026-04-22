#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Subscribe to joint-position state of OmniHand3UltraM (O20)
              via sensor_msgs/JointState (position field = rad).

Topic:  /<product>/<side>/joint_states  (JointState, 10 Hz)

Usage:  python3 joint_state_sub.py [left|right] [product]
        default: side=left, product=h3u_m
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


class JointStateSubscriber(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_joint_state_subscriber')
        self.hand_side = hand_side
        self.product = product
        self.subscription = self.create_subscription(
            JointState,
            f'/{product}/{hand_side}/joint_states',
            self.joint_state_callback,
            10,
        )
        self.get_logger().info(
            f'{product}/{hand_side} joint_states subscriber started '
            f'(O20, 20 DOF, position in rad)'
        )

    def joint_state_callback(self, msg: JointState):
        self.get_logger().info(
            f'{self.product}/{self.hand_side} joint_states '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}, rad): '
            f'{[round(p, 3) for p in msg.position]}'
        )


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u_m'
    node = JointStateSubscriber(hand_side, product)
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
