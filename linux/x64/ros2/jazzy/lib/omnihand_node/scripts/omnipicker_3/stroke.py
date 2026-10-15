#!/usr/bin/env python3
"""Send an OP3 distance command in millimetres."""

import sys
import time

import rclpy
from omnipicker_3_node_msgs.msg import StrokeCommand


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    if side not in {"left", "right"}:
        print(f"error: side must be left or right, got {side!r}", file=sys.stderr)
        raise SystemExit(2)
    stroke_mm = float(sys.argv[2]) if len(sys.argv) > 2 else 50.0
    speed = int(sys.argv[3]) if len(sys.argv) > 3 else None
    current = int(sys.argv[4]) if len(sys.argv) > 4 else None

    rclpy.init()
    node = rclpy.create_node("op3_stroke_cmd_sender")
    publisher = node.create_publisher(
        StrokeCommand, f"/op3/{side}/stroke_cmd", 10
    )
    message = StrokeCommand()
    message.stroke_mm = stroke_mm
    message.use_speed = speed is not None
    message.speed_rpm = speed or 0
    message.use_current = current is not None
    message.current_ma = current or 0

    deadline = time.monotonic() + 2.0
    while time.monotonic() < deadline:
        publisher.publish(message)
        rclpy.spin_once(node, timeout_sec=0.1)

    print(f"OP3 stroke command: {stroke_mm:g} mm")
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
