#!/usr/bin/env python3
"""
Set O12 tactile stream rate on omnihand_node (no local subscriber).

  pub  /<product>/<side>/tactile_cmd   std_msgs/Float32   data = Hz (>0 start, 0 stop)
  sub  /<product>/<side>/tactile_states   (use ros2 topic echo / PlotJuggler elsewhere)

Usage:  python3 tactile.py [left|right] [o12] [hz]
        default: side=left, product=o12, hz=10

Examples:
  python3 tactile.py left o12 10    # start 10 Hz stream, then exit
  python3 tactile.py left o12 0     # stop stream
"""

import sys
import time

import rclpy
from std_msgs.msg import Float32

_VALID_SIDES = frozenset({'left', 'right'})


def main() -> None:
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    if hand_side not in _VALID_SIDES:
        print(f'error: side must be left or right, got {hand_side!r}', file=sys.stderr)
        sys.exit(2)

    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'o12'
    hz = float(sys.argv[3]) if len(sys.argv) > 3 else 10.0

    rclpy.init()
    node = rclpy.create_node('tactile_cmd_setter')
    pub = node.create_publisher(Float32, f'/{product}/{hand_side}/tactile_cmd', 10)

    msg = Float32()
    msg.data = hz
    deadline = time.monotonic() + 2.0
    while time.monotonic() < deadline:
        pub.publish(msg)
        rclpy.spin_once(node, timeout_sec=0.1)

    if hz > 0:
        print(f'tactile stream {hz} Hz on /{product}/{hand_side} (node publishes tactile_states)')
    else:
        print(f'tactile stream stopped on /{product}/{hand_side}')

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
