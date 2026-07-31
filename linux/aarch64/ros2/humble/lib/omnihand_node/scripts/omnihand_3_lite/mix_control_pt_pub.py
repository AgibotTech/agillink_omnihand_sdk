#!/usr/bin/env python3
"""
@Description: Publish position + working-current mixed-control (PT) for H3L.

**NOT SUPPORTED YET** — H3L PT mixed control is not available on hardware/firmware.
This script is kept as an API example only. Use joint_cmd.py (position-only) until a future release.

Mixed control topic: /<product>/<side>/joint_mix_control_cmd  (JointState)
  position[] = motor tick (int16, 0~4095)
  effort[]   = working current threshold (mA)

Usage:  python3 mix_control_pt_pub.py [left|right] [product]
        default: side=left, product=h3l
        topic: /h3l/<side>/joint_mix_control_cmd
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

NUM_JOINTS = 4
WORK_CURRENT_MA = 300

# OmniHand3LiteSolver::SetHandGesture (OPEN / FIST), motor tick
POSE_OPEN = {
    'right': [4095, 4095, 4095, 4095],
    'left': [0, 4095, 4095, 0],
}
POSE_CLOSE = {
    'right': [1500, 1500, 2900, 400],
    'left': [2595, 1500, 2900, 3695],
}


class MixControlPtPublisher(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_mix_control_pt_publisher')
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
            f'{product}/{hand_side} mix_control PT started '
            f'(H3L, effort={WORK_CURRENT_MA}mA)'
        )

    def publish_mix_control(self):
        label = 'open' if self._publish_open else 'close'
        pose = POSE_OPEN[self.hand_side] if self._publish_open else POSE_CLOSE[self.hand_side]
        self._publish_open = not self._publish_open

        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.position = [float(p) for p in pose]
        msg.effort = [float(WORK_CURRENT_MA)] * NUM_JOINTS
        self.publisher.publish(msg)
        self.get_logger().info(
            f'Published mix_control PT ({label}, pos={pose}, current={WORK_CURRENT_MA}mA)'
        )


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3l'
    node = MixControlPtPublisher(hand_side, product)
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
