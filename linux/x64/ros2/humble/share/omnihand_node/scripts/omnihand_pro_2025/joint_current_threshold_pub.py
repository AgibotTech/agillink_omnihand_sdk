#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish current threshold values to OmniHandPro2025 (O12) and
              receive the read-back on joint_current_threshold_states.

Topic:
  pub: /<product>/<side>/joint_current_threshold_cmd  (omnihand_msgs/JointStateInt16)
  sub: /<product>/<side>/joint_current_threshold_states  (omnihand_msgs/JointStateInt16)

Usage:  python3 joint_current_threshold_pub.py <threshold> [left|right] [product]
        default: threshold=500, side=left, product=o12
"""

import sys

import rclpy
from rclpy.node import Node
from omnihand_msgs.msg import JointStateInt16

NUM_JOINTS = 12

# topic:
# /o12/left/joint_current_threshold_cmd; /o12/right/joint_current_threshold_cmd;
# /o12/left/joint_current_threshold_states; /o12/right/joint_current_threshold_states


class CurrentThresholdPubSub(Node):
    def __init__(self, hand_side: str, product: str, threshold: int):
        super().__init__(f'{product}_{hand_side}_current_threshold_pub')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            JointStateInt16, f'/{product}/{hand_side}/joint_current_threshold_cmd', 10)
        self.subscription = self.create_subscription(
            JointStateInt16, f'/{product}/{hand_side}/joint_current_threshold_states', self.callback, 10)

        msg = JointStateInt16()
        msg.data = [threshold] * NUM_JOINTS
        self.publisher.publish(msg)
        self.get_logger().info(f'{product}/{hand_side} set current_threshold={threshold} for all {NUM_JOINTS} joints')

    def callback(self, msg: JointStateInt16):
        names = msg.name if msg.name else [f'joint_{i}' for i in range(len(msg.data))]
        pairs = [f'{names[i]}={msg.data[i]}' for i in range(len(msg.data))]
        self.get_logger().info(f'{self.product}/{self.hand_side} current_threshold read-back: [{", ".join(pairs)}]')
        raise SystemExit(0)


def main(args=None):
    rclpy.init(args=args)
    threshold = int(sys.argv[1]) if len(sys.argv) > 1 else 500
    hand_side = sys.argv[2].lower() if len(sys.argv) > 2 else 'left'
    product = sys.argv[3].lower() if len(sys.argv) > 3 else 'o12'
    node = CurrentThresholdPubSub(hand_side, product, threshold)
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
