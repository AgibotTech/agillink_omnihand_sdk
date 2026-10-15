#!/usr/bin/env python3
"""Enter OP3 gentle-grip mode, optionally with a current limit in mA."""

import sys
import time

import rclpy
from omnipicker_3_node_msgs.msg import GentleGripCommand


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    current_limit = int(sys.argv[2]) if len(sys.argv) > 2 else None
    if side not in {"left", "right"}:
        raise SystemExit("side must be left or right")

    rclpy.init()
    node = rclpy.create_node("op3_gentle_grip_sender")
    publisher = node.create_publisher(
        GentleGripCommand, f"/op3/{side}/gentle_grip_cmd", 10
    )
    message = GentleGripCommand()
    message.use_current_limit = current_limit is not None
    message.current_limit_ma = current_limit or 0
    deadline = time.monotonic() + 2.0
    while time.monotonic() < deadline:
        publisher.publish(message)
        rclpy.spin_once(node, timeout_sec=0.1)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
