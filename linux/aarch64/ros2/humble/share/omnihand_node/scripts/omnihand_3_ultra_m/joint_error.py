#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Periodically trigger error-report query and display results
              for OmniHand3UltraM (O20).

Topic:
  pub: /<product>/<side>/joint_error_cmd    (std_msgs/Empty)
  sub: /<product>/<side>/joint_error_states (omnihand_msgs/JointStateInt16)

H3U_M error bitmask per joint (16 bits):
  bit0  = encoder_comm_timeout
  bit1  = calibration_error
  bit2  = over_voltage
  bit3  = under_voltage
  bit4  = over_temperature
  bit5  = torque_saturation
  bit6  = param_crc_error
  bit7  = homing_error
  bit8  = position_following_error
  bit9  = velocity_following_error
  bit10 = over_current
  bit11 = inner_encoder_crc_error
  bit12 = outer_encoder_crc_error
  bit13 = encoder_multi_turn_error
  bit14 = angle_identify_fail
  bit15 = reserved

Usage:  python3 joint_error.py [left|right] [product] [hz]
        default: side=left, product=h3u_m, hz=1
"""

import sys

import rclpy
from rclpy.node import Node
from std_msgs.msg import Empty
from omnihand_msgs.msg import JointStateInt16
from omnihand import h3um_error_report_to_string

# topic:
# /h3u_m/left/joint_error_cmd; /h3u_m/right/joint_error_cmd;
# /h3u_m/left/joint_error_states; /h3u_m/right/joint_error_states


class JointErrorNode(Node):
    def __init__(self, hand_side: str, product: str, hz: float):
        super().__init__(f'{product}_{hand_side}_joint_error')
        self.product = product
        self.hand_side = hand_side
        self.publisher = self.create_publisher(
            Empty, f'/{product}/{hand_side}/joint_error_cmd', 10)
        self.subscription = self.create_subscription(
            JointStateInt16, f'/{product}/{hand_side}/joint_error_states',
            self.callback, 10)
        self.timer = self.create_timer(1.0 / hz, lambda: self.publisher.publish(Empty()))
        self.get_logger().info(
            f'{product}/{hand_side} joint_error started ({hz} Hz, O20 20 DOF)')

    def callback(self, msg: JointStateInt16):
        stamp = f'{msg.header.stamp.sec}.{msg.header.stamp.nanosec:09d}'
        lines = [f'{self.product}/{self.hand_side} joint_error_states (stamp={stamp}):']
        has_error = False
        for i, val in enumerate(msg.data):
            unsigned_val = val & 0xFFFF
            name = msg.name[i] if i < len(msg.name) else f'joint_{i}'
            decoded = h3um_error_report_to_string(unsigned_val)
            if unsigned_val != 0:
                has_error = True
            lines.append(f'  [{i:2d}] {name:25s} = 0x{unsigned_val:04X} ({decoded})')
        if has_error:
            self.get_logger().warn('\n'.join(lines))
        else:
            self.get_logger().info('\n'.join(lines))


def main(args=None):
    rclpy.init(args=args)
    hand_side = sys.argv[1].lower() if len(sys.argv) > 1 else 'left'
    product = sys.argv[2].lower() if len(sys.argv) > 2 else 'h3u_m'
    hz = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0
    node = JointErrorNode(hand_side, product, hz)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        try:
            rclpy.shutdown()
        except Exception:
            pass


if __name__ == '__main__':
    main()
