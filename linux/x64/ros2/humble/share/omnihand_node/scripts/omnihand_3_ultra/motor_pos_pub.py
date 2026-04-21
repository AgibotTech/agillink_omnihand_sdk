#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Publish motor position commands to OmniHand3Ultra (O20) via sensor_msgs/JointState.

约定：
  - 节点话题层级：/<product>/<side>/motor_pos_cmd，产品代号见各 launch 的 `namespace=`。
  - motor_pos_cmd 按 SDK 电机下标顺序填 position[0..19]；节点端忽略 name 字段。
  - "下标 <-> 关节名" 运行时查 /<product>/<side>/motor_pos_state（msg.name 权威）；
    源代码权威见 OmniHand3Ultra::GetJointNames()。

Usage:  python3 motor_pos_pub.py [left|right] [product]
        默认 side=left, product=h3u
"""

import sys
import time

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


NUM_JOINTS = 20


class MotorPosPublisher(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_motor_pos_publisher')
        self.hand_side = hand_side
        self.product = product
        self.publisher = self.create_publisher(
            JointState,
            f'/{product}/{hand_side}/motor_pos_cmd',
            10,
        )
        self.timer = self.create_timer(1.5, self.publish_motor_pos_cmd)
        self.get_logger().info(
            f'{product}/{hand_side} motor_pos_cmd publisher started (O20, {NUM_JOINTS} DOF)'
        )

    def _make_msg(self, positions):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in positions]
        return msg

    def publish_motor_pos_cmd(self):
        pos_open = [148.0] * NUM_JOINTS
        pos_close = [50.0] * NUM_JOINTS

        self.publisher.publish(self._make_msg(pos_open))
        time.sleep(0.5)
        self.publisher.publish(self._make_msg(pos_close))


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u'
    node = MotorPosPublisher(hand_side, product)
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
