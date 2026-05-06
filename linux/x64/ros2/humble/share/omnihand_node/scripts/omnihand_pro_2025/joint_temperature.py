#!/usr/bin/env python3
"""
@Description: Periodically trigger temperature query and display results
              for OmniHandPro2025 (O12).

Topic:
  pub: /<product>/<side>/joint_temperature_cmd    (std_msgs/Empty)
  sub: /<product>/<side>/joint_temperature_states  (omnihand_msgs/JointStateInt16)

Usage:  python3 joint_temperature.py [left|right] [product] [hz]
        default: side=left, product=o12, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty
from omnihand_msgs.msg import JointStateInt16

# topic:
# /o12/left/joint_temperature_cmd; /o12/right/joint_temperature_cmd;
# /o12/left/joint_temperature_states; /o12/right/joint_temperature_states


class JointTemperatureNode(Node):
    def __init__(self, hand_side: str, product: str, hz: float):
        super().__init__(f'{product}_{hand_side}_joint_temperature')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Empty, f'/{product}/{hand_side}/joint_temperature_cmd', 10)
        self.subscription = self.create_subscription(
            JointStateInt16, f'/{product}/{hand_side}/joint_temperature_states',
            self.callback, 10)
        self.timer = self.create_timer(1.0 / hz, lambda: self.publisher.publish(Empty()))
        self.get_logger().info(f'{product}/{hand_side} joint_temperature started ({hz} Hz)')

    def callback(self, msg: JointStateInt16):
        stamp = f'{msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}'
        names = msg.name if msg.name else [f'joint_{i}' for i in range(len(msg.data))]
        pairs = [f'{names[i]}={msg.data[i]}' for i in range(len(msg.data))]
        self.get_logger().info(
            f'{self.product}/{self.hand_side} temperature (stamp={stamp}): [{", ".join(pairs)}]')


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o12'
    hz = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0
    node = JointTemperatureNode(hand_side, product, hz)
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
