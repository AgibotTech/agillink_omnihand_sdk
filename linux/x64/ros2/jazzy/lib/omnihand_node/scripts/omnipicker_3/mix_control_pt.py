#!/usr/bin/env python3
"""Periodically open and close OP3 with position-current (PT) commands.

Usage: python3 mix_control_pt.py [left|right] [open_position] [current_ma] [period_s]
       default: side=left, open_position=2048, current_ma=3000, period_s=1.5

The close position is 0. Press Ctrl+C to stop the test.
"""

import sys

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

CLOSE_POSITION = 0.0
MIN_POSITION = 0.0
MAX_POSITION = 4095.0


class MixControlPtTest(Node):
    def __init__(
        self, side: str, open_position: float, current: float, period_s: float
    ) -> None:
        super().__init__("op3_mix_control_pt_test")
        self.publisher = self.create_publisher(
            JointState, f"/op3/{side}/joint_mix_control_cmd", 10
        )
        self.open_position = open_position
        self.current = current
        self.publish_open = True
        self.timer = self.create_timer(period_s, self.publish_command)
        self.get_logger().info(
            f"OP3 {side} periodic PT test started: open={open_position:g}, "
            f"close={CLOSE_POSITION:g}, current={current:g}mA, period={period_s:g}s"
        )

    def publish_command(self) -> None:
        position = self.open_position if self.publish_open else CLOSE_POSITION
        state = "open" if self.publish_open else "close"
        message = JointState()
        message.header.stamp = self.get_clock().now().to_msg()
        message.name = ["joint1"]
        message.position = [position]
        message.effort = [self.current]
        self.publisher.publish(message)
        self.get_logger().info(f"publish {state}: position={position:g}")
        self.publish_open = not self.publish_open


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    open_position = float(sys.argv[2]) if len(sys.argv) > 2 else 2048.0
    current = float(sys.argv[3]) if len(sys.argv) > 3 else 3000.0
    period_s = float(sys.argv[4]) if len(sys.argv) > 4 else 1.5
    if side not in {"left", "right"}:
        raise SystemExit("side must be left or right")
    if not MIN_POSITION <= open_position <= MAX_POSITION:
        raise SystemExit(
            f"open_position must be in [{MIN_POSITION:g}, {MAX_POSITION:g}]"
        )
    if period_s <= 0:
        raise SystemExit("period_s must be greater than 0")

    rclpy.init()
    node = MixControlPtTest(side, open_position, current, period_s)
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
