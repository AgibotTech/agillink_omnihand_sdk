#!/usr/bin/env python3
"""Periodically open and close OP3 with stroke commands.

Usage: python3 stroke.py [left|right] [open_mm] [speed_rpm] [current_ma] [period_s]
       default: side=left, open_mm=50, period_s=1.5

The close stroke is 0 mm. Press Ctrl+C to stop the test.
"""

import sys

import rclpy
from omnipicker_3_node_msgs.msg import StrokeCommand
from rclpy.node import Node

CLOSE_STROKE_MM = 0.0
MIN_STROKE_MM = -5.0
MAX_STROKE_MM = 112.0


class StrokeTest(Node):
    def __init__(
        self,
        side: str,
        open_stroke_mm: float,
        speed: int | None,
        current: int | None,
        period_s: float,
    ) -> None:
        super().__init__("op3_stroke_test")
        self.publisher = self.create_publisher(
            StrokeCommand, f"/op3/{side}/stroke_cmd", 10
        )
        self.open_stroke_mm = open_stroke_mm
        self.speed = speed
        self.current = current
        self.publish_open = True
        self.timer = self.create_timer(period_s, self.publish_command)
        self.get_logger().info(
            f"OP3 {side} periodic stroke test started: "
            f"open={open_stroke_mm:g}mm, close={CLOSE_STROKE_MM:g}mm, "
            f"period={period_s:g}s"
        )

    def publish_command(self) -> None:
        stroke_mm = self.open_stroke_mm if self.publish_open else CLOSE_STROKE_MM
        state = "open" if self.publish_open else "close"
        message = StrokeCommand()
        message.stroke_mm = stroke_mm
        message.use_speed = self.speed is not None
        message.speed_rpm = self.speed or 0
        message.use_current = self.current is not None
        message.current_ma = self.current or 0
        self.publisher.publish(message)
        self.get_logger().info(f"publish {state}: stroke={stroke_mm:g}mm")
        self.publish_open = not self.publish_open


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    open_stroke_mm = float(sys.argv[2]) if len(sys.argv) > 2 else 50.0
    speed = int(sys.argv[3]) if len(sys.argv) > 3 else None
    current = int(sys.argv[4]) if len(sys.argv) > 4 else None
    period_s = float(sys.argv[5]) if len(sys.argv) > 5 else 1.5
    if side not in {"left", "right"}:
        raise SystemExit("side must be left or right")
    if not MIN_STROKE_MM <= open_stroke_mm <= MAX_STROKE_MM:
        raise SystemExit(
            f"open_mm must be in [{MIN_STROKE_MM:g}, {MAX_STROKE_MM:g}]"
        )
    if period_s <= 0:
        raise SystemExit("period_s must be greater than 0")

    rclpy.init()
    node = StrokeTest(side, open_stroke_mm, speed, current, period_s)
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
