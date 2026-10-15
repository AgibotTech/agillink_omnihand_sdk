#!/usr/bin/env python3
"""Periodically open and close OP3 with raw motor-position commands.

Usage: python3 joint_position.py [left|right] [open_position] [period_s]
       default: side=left, open_position=2048, period_s=1.5

The close position is 0. Press Ctrl+C to stop the test.
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16MultiArray

CLOSE_POSITION = 0
MIN_POSITION = 0
MAX_POSITION = 4095


class JointPositionTest(Node):
    def __init__(self, side: str, open_position: int, period_s: float) -> None:
        super().__init__("op3_joint_position_test")
        self.publisher = self.create_publisher(
            Int16MultiArray, f"/op3/{side}/joint_position_cmd", 10
        )
        self.open_position = open_position
        self.publish_open = True
        self.timer = self.create_timer(period_s, self.publish_command)
        self.get_logger().info(
            f"OP3 {side} periodic joint-position test started: "
            f"open={open_position}, close={CLOSE_POSITION}, period={period_s:g}s"
        )

    def publish_command(self) -> None:
        position = self.open_position if self.publish_open else CLOSE_POSITION
        state = "open" if self.publish_open else "close"
        self.publisher.publish(Int16MultiArray(data=[position]))
        self.get_logger().info(f"publish {state}: position={position}")
        self.publish_open = not self.publish_open


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    open_position = int(sys.argv[2]) if len(sys.argv) > 2 else 2048
    period_s = float(sys.argv[3]) if len(sys.argv) > 3 else 1.5
    if side not in {"left", "right"}:
        raise SystemExit("side must be left or right")
    if not MIN_POSITION <= open_position <= MAX_POSITION:
        raise SystemExit(
            f"open_position must be in [{MIN_POSITION}, {MAX_POSITION}]"
        )
    if period_s <= 0:
        raise SystemExit("period_s must be greater than 0")

    rclpy.init()
    node = JointPositionTest(side, open_position, period_s)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == "__main__":
    main()
