#!/usr/bin/env python3
"""
@Description: Publish current threshold values to OmniHand3UltraM (O20) and
              receive the read-back on joint_current_threshold_states.

Topic:
  pub: /<product>/<side>/joint_current_threshold_cmd  (std_msgs/Int16MultiArray)
  sub: /<product>/<side>/joint_current_threshold_states  (std_msgs/Int16MultiArray)

Usage:  python3 joint_current_threshold_pub.py <threshold> [left|right] [product]
        default: threshold=500, side=left, product=h3u_m
"""

import os
import sys

_scripts = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if _scripts not in sys.path:
    sys.path.insert(0, _scripts)

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16MultiArray
from ros_multi_array_utils import make_int16_multi_array

NUM_JOINTS = 20

# topic:
# /h3u_m/left/joint_current_threshold_cmd; /h3u_m/right/joint_current_threshold_cmd;
# /h3u_m/left/joint_current_threshold_states; /h3u_m/right/joint_current_threshold_states


class CurrentThresholdPubSub(Node):
    def __init__(self, hand_side: str, product: str, threshold: int):
        super().__init__(f'{product}_{hand_side}_current_threshold_pub')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Int16MultiArray, f'/{product}/{hand_side}/joint_current_threshold_cmd', 10)
        self.subscription = self.create_subscription(
            Int16MultiArray, f'/{product}/{hand_side}/joint_current_threshold_states', self.callback, 10)

        self.publisher.publish(make_int16_multi_array([threshold] * NUM_JOINTS))
        self.get_logger().info(f'{product}/{hand_side} set current_threshold={threshold} for all {NUM_JOINTS} joints')

    def callback(self, msg: Int16MultiArray):
        pairs = [f'joint_{i}={msg.data[i]}' for i in range(len(msg.data))]
        self.get_logger().info(f'{self.product}/{self.hand_side} current_threshold read-back: [{", ".join(pairs)}]')
        raise SystemExit(0)


def main(args=None):
    rclpy.init(args=args)
    threshold = int(sys.argv[1]) if len(sys.argv) > 1 else 500
    hand_side = sys.argv[2].lower() if len(sys.argv) > 2 else 'left'
    product = sys.argv[3].lower() if len(sys.argv) > 3 else 'h3u_m'
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
