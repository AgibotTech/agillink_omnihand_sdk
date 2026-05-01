#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish joint-position commands and display position readback
              for OmniHand3UltraM (O20).

Topic:
  pub: /<product>/<side>/joint_cmd     (sensor_msgs/JointState, position[] = rad)
  sub: /<product>/<side>/joint_states  (sensor_msgs/JointState, position[] = rad)

Usage:  python3 joint_cmd.py [left|right] [product]
        default: side=left, product=h3u_m
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


NUM_JOINTS = 20


class JointCmdNode(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_joint_cmd')
        self.hand_side = hand_side
        self.product = product
        self.publisher = self.create_publisher(
            JointState, f'/{product}/{hand_side}/joint_cmd', 10)
        self.subscription = self.create_subscription(
            JointState, f'/{product}/{hand_side}/joint_states',
            self.callback, 10)
        self.pose_open = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        self.pose_close = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6]
        self.use_close = False
        self.cycle_count = 0
        self.timer = self.create_timer(0.002, self.publish_joint_cmd)
        self.get_logger().info(
            f'{product}/{hand_side} joint_cmd started '
            f'(O20, {NUM_JOINTS} DOF, 500Hz, position in rad)')

    def _make_msg(self, positions_rad):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in positions_rad]
        return msg

    def publish_joint_cmd(self):
        pose = self.pose_close if self.use_close else self.pose_open
        self.publisher.publish(self._make_msg(pose))
        self.cycle_count += 1
        if self.cycle_count >= 250:
            self.use_close = not self.use_close
            self.cycle_count = 0

    def callback(self, msg: JointState):
        self.get_logger().info(
            f'{self.product}/{self.hand_side} joint_states '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}, rad): '
            f'{[round(p, 3) for p in msg.position]}')


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u_m'
    node = JointCmdNode(hand_side, product)
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
