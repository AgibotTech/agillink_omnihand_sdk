#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish Empty messages to trigger error-report query on
              OmniHand3UltraM (O20). Each publish triggers one
              GetAllErrorReport() call inside the node, which responds
              on /<product>/<side>/joint_error_states.

Topic:  /<product>/<side>/joint_error_cmd  (std_msgs/Empty)

Usage:  python3 joint_error_cmd_pub.py [left|right] [product] [hz]
        default: side=left, product=h3u_m, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty


class JointErrorCmdPublisher(Node):
    def __init__(self, hand_side: str, product: str, hz: float):
        super().__init__(f'{product}_{hand_side}_joint_error_cmd_publisher')
        self.hand_side = hand_side
        self.product = product
        self.publisher = self.create_publisher(
            Empty,
            f'/{product}/{hand_side}/joint_error_cmd',
            10,
        )
        self.timer = self.create_timer(1.0 / hz, self.publish_cmd)
        self.get_logger().info(
            f'{product}/{hand_side} joint_error_cmd publisher started ({hz} Hz)'
        )

    def publish_cmd(self):
        self.publisher.publish(Empty())


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u_m'
    hz = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0
    node = JointErrorCmdPublisher(hand_side, product, hz)
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
