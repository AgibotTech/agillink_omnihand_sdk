#!/usr/bin/env python3
"""
@Description: Periodically trigger 1D tactile sensor readback and display
              results for OmniHand2025 (O10).

Note: OmniHand node uses trigger-based readback — it does not publish states
      on a periodic timer. You must send a *_cmd to get one *_states response.
      This avoids consuming CAN bus bandwidth and ensures control loop real-time.

Topic:
  pub: /<product>/<side>/tactile_cmd     (std_msgs/Empty)
  sub: /<product>/<side>/tactile_states  (omnihand_2025_node_msgs/TactileSensor)

O10 has 7 tactile sensor regions:
  THUMB(16), INDEX(18), MIDDLE(18), RING(18), LITTLE(18), PALM(78), DORSUM(102)
Each element is uint8, unit: 1g, max: 255g.

Usage:  python3 tactile.py [left|right] [product] [hz]
        default: side=left, product=o10, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty
from omnihand_2025_node_msgs.msg import TactileSensor

REGION_NAMES = ['THUMB', 'INDEX', 'MIDDLE', 'RING', 'LITTLE', 'PALM', 'DORSUM']

# topic:
# /o10/left/tactile_cmd; /o10/right/tactile_cmd;
# /o10/left/tactile_states; /o10/right/tactile_states


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
            f'{product}/{hand_side} tactile started ({hz} Hz, O10 1D)')

    def callback(self, msg: TactileSensor):
        stamp = f'{msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}'
        lines = [f'{self.product}/{self.hand_side} tactile_states (stamp={stamp}):']
        for i, td in enumerate(msg.tactile_datas):
            name = REGION_NAMES[i] if i < len(REGION_NAMES) else f'region_{i}'
            data_str = ', '.join(str(v) for v in td.tactiles[:10])
            if len(td.tactiles) > 10:
                data_str += f', ... ({len(td.tactiles)} total)'
            lines.append(f'  [{name:8s}] ({len(td.tactiles):3d} pts): [{data_str}]')
        self.get_logger().info('\n'.join(lines))


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o10'
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
