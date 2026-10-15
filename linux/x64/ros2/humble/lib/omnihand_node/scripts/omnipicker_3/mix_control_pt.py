#!/usr/bin/env python3
"""Send one OP3 PT command: raw position ticks plus current in mA."""

import sys
import time

import rclpy
from sensor_msgs.msg import JointState


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    position = float(sys.argv[2]) if len(sys.argv) > 2 else 2048.0
    current = float(sys.argv[3]) if len(sys.argv) > 3 else 3000.0
    if side not in {"left", "right"}:
        raise SystemExit("side must be left or right")

    rclpy.init()
    node = rclpy.create_node("op3_mix_control_pt_sender")
    publisher = node.create_publisher(
        JointState, f"/op3/{side}/joint_mix_control_cmd", 10
    )
    message = JointState()
    message.name = ["joint1"]
    message.position = [position]
    message.effort = [current]
    deadline = time.monotonic() + 2.0
    while time.monotonic() < deadline:
        message.header.stamp = node.get_clock().now().to_msg()
        publisher.publish(message)
        rclpy.spin_once(node, timeout_sec=0.1)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
