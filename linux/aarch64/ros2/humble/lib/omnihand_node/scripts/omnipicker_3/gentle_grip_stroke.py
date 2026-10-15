#!/usr/bin/env python3
"""Alternate between an OP3 stroke-open command and gentle gripping.

The stroke command opens the gripper but does not leave gentle-grip mode.
Power-cycle the device after the test to leave gentle-grip mode completely.

Usage:
  python3 gentle_grip_stroke.py [left|right] [options]

Example:
  python3 gentle_grip_stroke.py left --open-mm 50 --grip-current-ma 1000
"""

import argparse
import time

import rclpy
from omnipicker_3_node_msgs.msg import (
    GentleGripCommand,
    GentleGripState,
    StrokeCommand,
    StrokeState,
)
from rclpy.node import Node

MIN_STROKE_MM = -5.0
MAX_STROKE_MM = 112.0
MAX_UINT16 = 65535
MAX_STROKE_SPEED_RPM = 5000
MAX_STROKE_CURRENT_MA = 3500


class GentleGripStrokeTest(Node):
    def __init__(
        self,
        side: str,
        open_stroke_mm: float,
        grip_current_ma: int | None,
        speed_rpm: int | None,
        stroke_current_ma: int | None,
        period_s: float,
    ) -> None:
        super().__init__("op3_gentle_grip_stroke_test")
        prefix = f"/op3/{side}"
        self.stroke_publisher = self.create_publisher(
            StrokeCommand, f"{prefix}/stroke_cmd", 10
        )
        self.gentle_grip_publisher = self.create_publisher(
            GentleGripCommand, f"{prefix}/gentle_grip_cmd", 10
        )
        self.stroke_subscription = self.create_subscription(
            StrokeState,
            f"{prefix}/stroke_states",
            self.stroke_state_callback,
            10,
        )
        self.gentle_grip_subscription = self.create_subscription(
            GentleGripState,
            f"{prefix}/gentle_grip_states",
            self.gentle_grip_state_callback,
            10,
        )

        self.side = side
        self.open_stroke_mm = open_stroke_mm
        self.grip_current_ma = grip_current_ma
        self.speed_rpm = speed_rpm
        self.stroke_current_ma = stroke_current_ma
        self.period_s = period_s
        self.publish_open = True
        self.timer = None

    def wait_for_command_subscribers(self, timeout_s: float = 3.0) -> bool:
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            if (
                self.stroke_publisher.get_subscription_count() > 0
                and self.gentle_grip_publisher.get_subscription_count() > 0
            ):
                return True
            rclpy.spin_once(self, timeout_sec=0.1)
        return False

    def start(self) -> None:
        self.get_logger().info(
            f"OP3 {self.side} gentle-grip/stroke test started: "
            f"open={self.open_stroke_mm:g}mm, period={self.period_s:g}s"
        )
        self.publish_command()
        self.timer = self.create_timer(self.period_s, self.publish_command)

    def publish_command(self) -> None:
        if self.publish_open:
            message = StrokeCommand()
            message.stroke_mm = self.open_stroke_mm
            message.use_speed = self.speed_rpm is not None
            message.speed_rpm = self.speed_rpm or 0
            message.use_current = self.stroke_current_ma is not None
            message.current_ma = self.stroke_current_ma or 0
            self.stroke_publisher.publish(message)
            self.get_logger().info(
                f"publish open: stroke={self.open_stroke_mm:g}mm"
            )
        else:
            message = GentleGripCommand()
            message.use_current_limit = self.grip_current_ma is not None
            message.current_limit_ma = self.grip_current_ma or 0
            self.gentle_grip_publisher.publish(message)
            limit = (
                f"{self.grip_current_ma}mA"
                if self.grip_current_ma is not None
                else "firmware default"
            )
            self.get_logger().info(f"publish gentle grip: current={limit}")
        self.publish_open = not self.publish_open

    def stroke_state_callback(self, message: StrokeState) -> None:
        self.get_logger().info(
            f"stroke feedback: {message.stroke_mm:g}mm"
        )

    def gentle_grip_state_callback(self, message: GentleGripState) -> None:
        self.get_logger().info(
            f"gentle-grip feedback: {list(message.raw_response)}"
        )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Periodically open OP3, then trigger gentle gripping."
    )
    parser.add_argument("side", nargs="?", choices=("left", "right"), default="left")
    parser.add_argument("--open-mm", type=float, default=50.0)
    parser.add_argument("--grip-current-ma", type=int, default=1000)
    parser.add_argument("--speed-rpm", type=int)
    parser.add_argument("--stroke-current-ma", type=int)
    parser.add_argument("--period", type=float, default=2.0)
    args = parser.parse_args()

    if not MIN_STROKE_MM <= args.open_mm <= MAX_STROKE_MM:
        parser.error(
            f"--open-mm must be in [{MIN_STROKE_MM:g}, {MAX_STROKE_MM:g}]"
        )
    if not 0 <= args.grip_current_ma <= MAX_UINT16:
        parser.error(f"--grip-current-ma must be in [0, {MAX_UINT16}]")
    if (
        args.speed_rpm is not None
        and not 0 <= args.speed_rpm <= MAX_STROKE_SPEED_RPM
    ):
        parser.error(f"--speed-rpm must be in [0, {MAX_STROKE_SPEED_RPM}]")
    if (
        args.stroke_current_ma is not None
        and not 0 <= args.stroke_current_ma <= MAX_STROKE_CURRENT_MA
    ):
        parser.error(
            f"--stroke-current-ma must be in [0, {MAX_STROKE_CURRENT_MA}]"
        )
    if args.period <= 0:
        parser.error("--period must be greater than 0")
    return args


def main() -> None:
    args = parse_args()
    rclpy.init()
    node = GentleGripStrokeTest(
        args.side,
        args.open_mm,
        args.grip_current_ma,
        args.speed_rpm,
        args.stroke_current_ma,
        args.period,
    )
    started = False
    try:
        if not node.wait_for_command_subscribers():
            raise SystemExit(
                "OP3 node did not subscribe to both stroke_cmd and gentle_grip_cmd"
            )
        node.start()
        started = True
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        if started:
            node.get_logger().warning(
                "Test stopped; power-cycle OP3 to leave gentle-grip mode."
            )
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == "__main__":
    main()
