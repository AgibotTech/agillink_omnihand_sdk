#!/usr/bin/env python3
"""
@Description: Publish voltage commands to OmniHandPro2025 (O12) and receive
              the read-back on joint_voltage_states.

Topic:
  pub: /<product>/<side>/joint_voltage_cmd  (std_msgs/Int16MultiArray)
  sub: /<product>/<side>/joint_voltage_states  (std_msgs/Int16MultiArray)

Note: Set all joints to ControlMode.VOLTAGE first, for example:
      python3 joint_control_mode_pub.py 4 left o12
      O12 firmware versions up to and including 1.2.15 do not support
      voltage read-back on joint_voltage_states.

Usage:  python3 joint_voltage_pub.py <voltage> [left|right] [product]
        default: voltage=0, side=left, product=o12
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

NUM_JOINTS = 12


class JointVoltagePubSub(Node):
    def __init__(self, hand_side: str, product: str, voltage: int):
        super().__init__(f'{product}_{hand_side}_joint_voltage_pub')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Int16MultiArray, f'/{product}/{hand_side}/joint_voltage_cmd', 10)
        self.subscription = self.create_subscription(
            Int16MultiArray, f'/{product}/{hand_side}/joint_voltage_states', self.callback, 10)

        self.publisher.publish(make_int16_multi_array([voltage] * NUM_JOINTS))
        self.get_logger().info(f'{product}/{hand_side} set voltage={voltage} for all {NUM_JOINTS} joints')

    def callback(self, msg: Int16MultiArray):
        pairs = [f'joint_{i}={msg.data[i]}' for i in range(len(msg.data))]
        self.get_logger().info(f'{self.product}/{self.hand_side} voltage read-back: [{", ".join(pairs)}]')
        raise SystemExit(0)


def main(args=None):
    rclpy.init(args=args)
    voltage = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    hand_side = sys.argv[2].lower() if len(sys.argv) > 2 else 'left'
    product = sys.argv[3].lower() if len(sys.argv) > 3 else 'o12'
    node = JointVoltagePubSub(hand_side, product, voltage)
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
