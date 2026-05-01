#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Periodically trigger 3D tactile sensor readback and display
              results for OmniHandPro2025 (O12).

Topic:
  pub: /<product>/<side>/tactile_cmd     (std_msgs/Empty)
  sub: /<product>/<side>/tactile_states  (omnihand_pro_2025_node_msgs/TactileSensor)

O12 has 5 fingers, each with 3D tactile data:
  - online_state: 1=online, 0=offline
  - channel_value[6]: 24-bit channel values
  - normal_force: 0.1N (max 3000)
  - tangent_force: 0.1N
  - tangent_force_angle: degrees (0-359)
  - capa_approach[4]: self-capacitance approach

Usage:  python3 tactile.py [left|right] [product] [hz]
        default: side=left, product=o12, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty
from omnihand_pro_2025_node_msgs.msg import TactileSensor

FINGER_NAMES = ['THUMB', 'INDEX', 'MIDDLE', 'RING', 'LITTLE']


class TactileNode(Node):
    def __init__(self, hand_side: str, product: str, hz: float):
        super().__init__(f'{product}_{hand_side}_tactile')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Empty, f'/{product}/{hand_side}/tactile_cmd', 10)
        self.subscription = self.create_subscription(
            TactileSensor, f'/{product}/{hand_side}/tactile_states',
            self.callback, 10)
        self.timer = self.create_timer(1.0 / hz, lambda: self.publisher.publish(Empty()))
        self.get_logger().info(
            f'{product}/{hand_side} tactile started ({hz} Hz, O12 3D)')

    def callback(self, msg: TactileSensor):
        stamp = f'{msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}'
        lines = [f'{self.product}/{self.hand_side} tactile_states (stamp={stamp}):']
        for i, td in enumerate(msg.tactile_datas):
            name = FINGER_NAMES[i] if i < len(FINGER_NAMES) else f'finger_{i}'
            ch_str = ', '.join(str(v) for v in td.channel_value)
            ca_str = ', '.join(str(v) for v in td.capa_approach)
            lines.append(f'  [{name:8s}] online={td.online_state} '
                         f'ch=[{ch_str}] '
                         f'Fn={td.normal_force * 0.1:.1f}N '
                         f'Ft={td.tangent_force * 0.1:.1f}N '
                         f'angle={td.tangent_force_angle}deg '
                         f'capa=[{ca_str}]')
        self.get_logger().info('\n'.join(lines))


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o12'
    hz = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0
    node = TactileNode(hand_side, product, hz)
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
