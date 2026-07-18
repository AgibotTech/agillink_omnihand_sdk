#!/usr/bin/env python3
"""
@Description: Publish control mode values to OmniHand3UltraM (O20) and
              receive the read-back on joint_control_mode_states.

Topic:
  pub: /<product>/<side>/joint_control_mode_cmd  (std_msgs/Int8MultiArray)
  sub: /<product>/<side>/joint_control_mode_states  (std_msgs/Int8MultiArray)

ControlMode values: 0=POSITION(CSP), 7=PROFILE_POSITION(PP)

Usage:  python3 joint_control_mode_pub.py <mode> [left|right] [product]
        default: mode=0, side=left, product=h3u_m
"""

import os
import sys

_scripts = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if _scripts not in sys.path:
    sys.path.insert(0, _scripts)

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int8MultiArray
from ros_multi_array_utils import make_int8_multi_array

NUM_JOINTS = 20

# topic:
# /h3u_m/left/joint_control_mode_cmd; /h3u_m/right/joint_control_mode_cmd;
# /h3u_m/left/joint_control_mode_states; /h3u_m/right/joint_control_mode_states


class ControlModePubSub(Node):
    def __init__(self, hand_side: str, product: str, mode: int):
        super().__init__(f'{product}_{hand_side}_control_mode_pub')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Int8MultiArray, f'/{product}/{hand_side}/joint_control_mode_cmd', 10)
        self.subscription = self.create_subscription(
            Int8MultiArray, f'/{product}/{hand_side}/joint_control_mode_states', self.callback, 10)

        payload = [mode] * NUM_JOINTS
        self.publisher.publish(make_int8_multi_array(payload))
        self.get_logger().info(f'{product}/{hand_side} set control_mode={mode} for all {NUM_JOINTS} joints')

    def callback(self, msg: Int8MultiArray):
        pairs = [f'joint_{i}={msg.data[i]}' for i in range(len(msg.data))]
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
