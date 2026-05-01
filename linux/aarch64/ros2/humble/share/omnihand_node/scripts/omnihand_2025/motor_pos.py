#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish motor-position commands (raw int16 ticks) and display
              readback for OmniHand2025 (O10).

Topic:
  pub: /<product>/<side>/motor_pos_cmd     (omnihand_msgs/JointStateInt16, data[] = tick)
  sub: /<product>/<side>/motor_pos_states  (omnihand_msgs/JointStateInt16, data[] = tick)

Usage:  python3 motor_pos.py [left|right] [product]
        default: side=left, product=o10
"""

import sys
import time

import rclpy
from rclpy.node import Node
from omnihand_msgs.msg import JointStateInt16


NUM_JOINTS = 10

# topic:
# /o10/left/motor_pos_cmd; /o10/right/motor_pos_cmd;
# /o10/left/motor_pos_states; /o10/right/motor_pos_states


class MotorPosNode(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_motor_pos')
        self.hand_side = hand_side
        self.product = product
        self.publisher = self.create_publisher(
            JointStateInt16, f'/{product}/{hand_side}/motor_pos_cmd', 10)
        self.subscription = self.create_subscription(
            JointStateInt16, f'/{product}/{hand_side}/motor_pos_states',
            self.callback, 10)
        self.timer = self.create_timer(1.5, self.publish_motor_pos)
        self.get_logger().info(
            f'{product}/{hand_side} motor_pos started '
            f'(O10, {NUM_JOINTS} DOF, raw int16 ticks)')

    def _make_msg(self, positions):
        msg = JointStateInt16()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.data = [int(p) for p in positions]
        return msg

    def publish_motor_pos(self):
        pos_a = [2048] * NUM_JOINTS
        pos_b = [2148] * NUM_JOINTS
        self.publisher.publish(self._make_msg(pos_a))
        time.sleep(0.5)
        self.publisher.publish(self._make_msg(pos_b))

    def callback(self, msg: JointStateInt16):
        self.get_logger().info(
            f'{self.product}/{self.hand_side} motor_pos_states '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}, tick): '
            f'{list(msg.data)}')


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o10'
    node = MotorPosNode(hand_side, product)
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
