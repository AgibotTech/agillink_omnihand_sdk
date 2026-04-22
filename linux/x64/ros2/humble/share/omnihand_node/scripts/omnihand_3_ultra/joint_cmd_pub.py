#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish joint-position commands to OmniHand3Ultra (O20)
              via sensor_msgs/JointState (position field = rad, per ROS /
              REP-103 convention for revolute joints).

Topic:
  /<product>/<side>/joint_cmd  (JointState)

Conventions:
  - position[0..19] follows SDK joint index order. Name field is ignored
    by the node; source-of-truth is OmniHand3Ultra::GetJointNames(), which
    also populates /<product>/<side>/joint_states.
  - Out-of-URDF-range values are clamped per joint inside the SDK solver,
    so the demo values below are safe even for thumb_abad etc.

Usage:  python3 joint_cmd_pub.py [left|right] [product]
        default: side=left, product=h3u
"""

import sys
import time

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


NUM_JOINTS = 20


class JointCmdPublisher(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_joint_cmd_publisher')
        self.hand_side = hand_side
        self.product = product
        self.publisher = self.create_publisher(
            JointState,
            f'/{product}/{hand_side}/joint_cmd',
            10,
        )
        self.timer = self.create_timer(1.5, self.publish_joint_cmd)
        self.get_logger().info(
            f'{product}/{hand_side} joint_cmd publisher started '
            f'(O20, {NUM_JOINTS} DOF, position in rad)'
        )

    def _make_msg(self, positions_rad):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in positions_rad]
        return msg

    def publish_joint_cmd(self):
        # Open pose (~ extended): 0 rad for every joint. SDK solver clamps
        # out-of-range joints (e.g. thumb_abad on right hand clamps to -0.1745).
        pose_open = [0.0] * NUM_JOINTS
        # Partial close: ~35 deg of flexion across the board. Abad joints get
        # clamped as needed.
        pose_close = [0.6] * NUM_JOINTS

        self.publisher.publish(self._make_msg(pose_open))
        time.sleep(0.5)
        self.publisher.publish(self._make_msg(pose_close))


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u'
    node = JointCmdPublisher(hand_side, product)
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
