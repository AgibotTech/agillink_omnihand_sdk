#!/usr/bin/env python3
"""Start or stop the H3L tactile stream.

Usage: tactile.py [left|right] [hz]
  hz > 0 starts/restarts publishing; hz == 0 stops it.
"""

import sys
import time

import rclpy
from std_msgs.msg import Float32


def main() -> None:
    side = sys.argv[1].lower() if len(sys.argv) > 1 else "left"
    if side not in {"left", "right"}:
        print(f"error: side must be left or right, got {side!r}", file=sys.stderr)
        raise SystemExit(2)

    hz = float(sys.argv[2]) if len(sys.argv) > 2 else 10.0
    topic_prefix = f"/h3l/{side}"

    rclpy.init()
    node = rclpy.create_node("h3l_tactile_cmd_setter")
    publisher = node.create_publisher(Float32, f"{topic_prefix}/tactile_cmd", 10)

    message = Float32()
    message.data = hz
    deadline = time.monotonic() + 2.0
    while time.monotonic() < deadline:
        publisher.publish(message)
        rclpy.spin_once(node, timeout_sec=0.1)

    state_topic = f"{topic_prefix}/tactile_states"
    action = "started" if hz > 0 else "stopped"
    print(f"H3L tactile stream {action}: {state_topic}")

    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
