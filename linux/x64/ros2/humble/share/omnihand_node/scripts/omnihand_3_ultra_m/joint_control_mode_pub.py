#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish control mode values to OmniHand3UltraM (O20) and
              receive the read-back on joint_control_mode_states.

Topic:
  pub: /<product>/<side>/joint_control_mode_cmd  (omnihand_msgs/JointStateInt8)
  sub: /<product>/<side>/joint_control_mode_states  (omnihand_msgs/JointStateInt8)

ControlMode values: 0=POSITION(CSP), 7=PROFILE_POSITION(PP)

Usage:  python3 joint_control_mode_pub.py <mode> [left|right] [product]
        default: mode=0, side=left, product=h3u_m
"""

import sys

import rclpy
from rclpy.node import Node
from omnihand_msgs.msg import JointStateInt8

NUM_JOINTS = 20


class ControlModePubSub(Node):
    def __init__(self, hand_side: str, product: str, mode: int):
        super().__init__(f'{product}_{hand_side}_control_mode_pub')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            JointStateInt8, f'/{product}/{hand_side}/joint_control_mode_cmd', 10)
        self.subscription = self.create_subscription(
            JointStateInt8, f'/{product}/{hand_side}/joint_control_mode_states', self.callback, 10)

        msg = JointStateInt8()
        msg.data = [mode] * NUM_JOINTS
        self.publisher.publish(msg)
        self.get_logger().info(f'{product}/{hand_side} set control_mode={mode} for all {NUM_JOINTS} joints')

    def callback(self, msg: JointStateInt8):
        names = msg.name if msg.name else [f'joint_{i}' for i in range(len(msg.data))]
        pairs = [f'{names[i]}={msg.data[i]}' for i in range(len(msg.data))]
        self.get_logger().info(f'{self.product}/{self.hand_side} control_mode read-back: [{", ".join(pairs)}]')
        raise SystemExit(0)


def main(args=None):
    rclpy.init(args=args)
    mode = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    hand_side = sys.argv[2].lower() if len(sys.argv) > 2 else 'left'
    product = sys.argv[3].lower() if len(sys.argv) > 3 else 'h3u_m'
    node = ControlModePubSub(hand_side, product, mode)
    try:
        rclpy.spin(node)
    except SystemExit:
        pass
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
