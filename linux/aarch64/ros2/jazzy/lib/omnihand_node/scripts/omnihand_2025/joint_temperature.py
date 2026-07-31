#!/usr/bin/env python3
"""
@Description: Periodically trigger temperature query and display results
              for OmniHand2025 (O10).

Note: OmniHand node uses trigger-based readback — it does not publish states
      on a periodic timer. You must send a *_cmd to get one *_states response.
      This avoids consuming CAN bus bandwidth and ensures control loop real-time.

Topic:
  pub: /<product>/<side>/joint_temperature_cmd    (std_msgs/Empty)
  sub: /<product>/<side>/joint_temperature_states  (std_msgs/Int16MultiArray)

Usage:  python3 joint_temperature.py [left|right] [product] [hz]
        default: side=left, product=o10, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty, Int16MultiArray

# topic:
# /o10/left/joint_temperature_cmd; /o10/right/joint_temperature_cmd;
# /o10/left/joint_temperature_states; /o10/right/joint_temperature_states


class JointTemperatureNode(Node):
    def __init__(self, hand_side: str, product: str, hz: float):
        super().__init__(f'{product}_{hand_side}_joint_temperature')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Empty, f'/{product}/{hand_side}/joint_temperature_cmd', 10)
        self.subscription = self.create_subscription(
            Int16MultiArray, f'/{product}/{hand_side}/joint_temperature_states',
            self.callback, 10)
        self.timer = self.create_timer(1.0 / hz, lambda: self.publisher.publish(Empty()))
        self.get_logger().info(f'{product}/{hand_side} joint_temperature started ({hz} Hz)')

    def callback(self, msg: Int16MultiArray):
        pairs = [f'joint_{i}={msg.data[i]}' for i in range(len(msg.data))]
        self.get_logger().info(
            f'{self.product}/{self.hand_side} temperature: [{", ".join(pairs)}]')


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o10'
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
