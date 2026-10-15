#!/usr/bin/env python3
"""Send one OP3 raw motor-position command (0..4095)."""

import sys
import time

import rclpy
from std_msgs.msg import Int16MultiArray


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    position = int(sys.argv[2]) if len(sys.argv) > 2 else 2048
    if side not in {"left", "right"}:
        raise SystemExit("side must be left or right")

    rclpy.init()
    node = rclpy.create_node("op3_joint_position_sender")
    publisher = node.create_publisher(
        Int16MultiArray, f"/op3/{side}/joint_position_cmd", 10
    )
    message = Int16MultiArray(data=[position])
    deadline = time.monotonic() + 2.0
    while time.monotonic() < deadline:
        publisher.publish(message)
        rclpy.spin_once(node, timeout_sec=0.1)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
