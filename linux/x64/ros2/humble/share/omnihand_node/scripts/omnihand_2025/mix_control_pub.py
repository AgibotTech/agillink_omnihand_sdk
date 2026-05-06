#!/usr/bin/env python3
"""
@Description: Publish position+torque mixed-control commands to
              OmniHand2025 (O10) via sensor_msgs/JointState.

Mixed control uses a separate topic joint_mix_control_cmd.
position[] = raw int16 motor position, effort[] = raw int16 torque.

Topic:  /<product>/<side>/joint_mix_control_cmd  (JointState)

Usage:  python3 mix_control_pub.py [left|right] [product]
        default: side=left, product=o10
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState


NUM_JOINTS = 10

# topic:
# /o10/left/joint_mix_control_cmd; /o10/right/joint_mix_control_cmd;


class MixControlPublisher(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_mix_control_publisher')
        self.hand_side = hand_side
        self.product = product
        self.publisher = self.create_publisher(
            JointState,
            f'/{product}/{hand_side}/joint_mix_control_cmd',
            10,
        )
        self.timer = self.create_timer(1.0, self.publish_mix_control)
        self.get_logger().info(
            f'{product}/{hand_side} mix_control publisher started '
            f'(O10, {NUM_JOINTS} DOF, POSITION_TORQUE)'
        )

    def publish_mix_control(self):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [2000.0] * NUM_JOINTS
        msg.effort = [100.0] * NUM_JOINTS
        self.publisher.publish(msg)
        self.get_logger().info('Published mix_control (position+torque)')


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o10'
    node = MixControlPublisher(hand_side, product)
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
