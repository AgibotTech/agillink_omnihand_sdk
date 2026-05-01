#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Periodically trigger error-report query and display results
              for OmniHand2025 (O10).

Topic:
  pub: /<product>/<side>/joint_error_cmd    (std_msgs/Empty)
  sub: /<product>/<side>/joint_error_states (omnihand_msgs/JointStateInt16)

O10 error bitmask per joint (5 bits):
  bit0 = stalled
  bit1 = overheat
  bit2 = over_current
  bit3 = motor_except
  bit4 = commu_except

Usage:  python3 joint_error.py [left|right] [product] [hz]
        default: side=left, product=o10, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty
from omnihand_msgs.msg import JointStateInt16

O10_ERROR_BIT_NAMES = [
    'stalled',
    'overheat',
    'over_current',
    'motor_except',
    'commu_except',
]


def decode_o10_error(value: int) -> str:
    value &= 0xFFFF
    if value == 0:
        return '0'
    return ','.join(
        name for i, name in enumerate(O10_ERROR_BIT_NAMES) if value & (1 << i)
    )


class JointErrorNode(Node):
    def __init__(self, hand_side: str, product: str, hz: float):
        super().__init__(f'{product}_{hand_side}_joint_error')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Empty, f'/{product}/{hand_side}/joint_error_cmd', 10)
        self.subscription = self.create_subscription(
            JointStateInt16, f'/{product}/{hand_side}/joint_error_states',
            self.callback, 10)
        self.timer = self.create_timer(1.0 / hz, lambda: self.publisher.publish(Empty()))
        self.get_logger().info(
            f'{product}/{hand_side} joint_error started ({hz} Hz, O10 10 DOF)')

    def callback(self, msg: JointStateInt16):
        stamp = f'{msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}'
        lines = [f'{self.product}/{self.hand_side} joint_error_states (stamp={stamp}):']
        has_error = False
        for i, val in enumerate(msg.data):
            unsigned_val = val & 0xFFFF
            name = msg.name[i] if i < len(msg.name) else f'joint_{i}'
            decoded = decode_o10_error(unsigned_val)
            if unsigned_val != 0:
                has_error = True
            lines.append(f'  [{i:2d}] {name:25s} = 0x{unsigned_val:04X} ({decoded})')
        if has_error:
            self.get_logger().warn('\n'.join(lines))
        else:
            self.get_logger().info('\n'.join(lines))


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o10'
    hz = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0
    node = JointErrorNode(hand_side, product, hz)
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
