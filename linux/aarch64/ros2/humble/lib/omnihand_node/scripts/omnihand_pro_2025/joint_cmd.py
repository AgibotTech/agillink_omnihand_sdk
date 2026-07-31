#!/usr/bin/env python3
"""
@Description: Publish joint-position commands and display position readback
              for OmniHandPro2025 (O12).

Note: joint_cmd is the only topic that automatically triggers joint_states
      readback. Other state queries (temperature, current, etc.) require you
      to explicitly send a *_cmd first. This trigger-based design avoids
      consuming CAN bus bandwidth and ensures control loop real-time.

Topic:
  pub: /<product>/<side>/joint_cmd     (sensor_msgs/JointState, position[] = rad)
  sub: /<product>/<side>/joint_states  (sensor_msgs/JointState, position[] = rad)

Usage:  python3 joint_cmd.py [left|right] [product]
        default: side=left, product=o12
"""

import sys
import time

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


NUM_JOINTS = 12

# topic:
# /o12/left/joint_cmd; /o12/right/joint_cmd;
# /o12/left/joint_states; /o12/right/joint_states


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
        self.timer = self.create_timer(1.5, self.publish_joint_cmd)
        self.get_logger().info(
            f'{product}/{hand_side} joint_cmd started '
            f'(O12, {NUM_JOINTS} DOF, position in rad)')

    def _make_msg(self, positions_rad):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in positions_rad]
        return msg

    def publish_joint_cmd(self):
        pose_open = [0.0] * NUM_JOINTS
        pose_close = [0.6] * NUM_JOINTS
        self.publisher.publish(self._make_msg(pose_open))
        time.sleep(0.5)
        self.publisher.publish(self._make_msg(pose_close))

    def callback(self, msg: JointState):
        self.get_logger().info(
            f'{self.product}/{self.hand_side} joint_states '
            f'(stamp={msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}, rad): '
            f'{[round(p, 3) for p in msg.position]}')


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o12'
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
