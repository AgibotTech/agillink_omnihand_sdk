#!/usr/bin/env python3
"""
@Description: Publish position + velocity + torque mixed-control commands for O10.

O10 node selects MixControlByPVT when JointState.velocity[] has 10 elements
(position[] + velocity[] + effort[] on joint_mix_control_cmd).

  position[] = motor tick (int16, calibrated open/close)
  velocity[] = motor velocity setpoint (int16, device raw unit)
  effort[]   = working current threshold (mA)

Usage:  python3 mix_control_pvt_pub.py [left|right] [product]
        default: side=left, product=o10
        topic: /o10/<side>/joint_mix_control_cmd
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

NUM_JOINTS = 10
WORK_CURRENT_MA = 300
MOTOR_VELOCITY = 8000

# Calibrated motor ticks (pos_axis_1 .. pos_axis_10)
POSE_OPEN = {
    'right': [4087, 18, 3963, 4085, 4094, 4094, 2072, 4094, 39, 4093],
    'left': [4087, 4077, 3963, 10, 4094, 4094, 2023, 4094, 4056, 4093],
}
POSE_CLOSE = {
    'right': [4087, 4085, 1634, 4085, 3, 3, 2035, 2, 9, 3],
    'left': [4087, 10, 1634, 10, 3, 3, 2060, 2, 4086, 3],
}


class MixControlPvtPublisher(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_mix_control_pvt_publisher')
        if hand_side not in POSE_OPEN:
            raise ValueError(f"hand_side must be 'left' or 'right', got {hand_side!r}")
        self.hand_side = hand_side
        self.product = product
        self._publish_open = True
        self.publisher = self.create_publisher(
            JointState,
            f'/{product}/{hand_side}/joint_mix_control_cmd',
            10,
        )
        self.timer = self.create_timer(1.5, self.publish_mix_control)
        self.get_logger().info(
            f'{product}/{hand_side} mix_control PVT started '
            f'(vel={MOTOR_VELOCITY}, effort={WORK_CURRENT_MA}mA)'
        )

    def publish_mix_control(self):
        label = 'open' if self._publish_open else 'close'
        pose = POSE_OPEN[self.hand_side] if self._publish_open else POSE_CLOSE[self.hand_side]
        self._publish_open = not self._publish_open

        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in pose]
        msg.velocity = [float(MOTOR_VELOCITY)] * NUM_JOINTS
        msg.effort = [float(WORK_CURRENT_MA)] * NUM_JOINTS
        self.publisher.publish(msg)
        self.get_logger().info(
            f'Published mix_control PVT ({label}, pos={pose}, '
            f'vel={MOTOR_VELOCITY}, current={WORK_CURRENT_MA}mA)'
        )


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o10'
    node = MixControlPvtPublisher(hand_side, product)
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
