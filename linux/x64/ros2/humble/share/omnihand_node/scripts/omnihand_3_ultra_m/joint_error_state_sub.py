#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Description: Subscribe to joint error reports from OmniHand3UltraM (O20)
              via omnihand_msg/JointStateInt16 and decode H3U_M error bitmask.

Topic:  /<product>/<side>/joint_error_states  (omnihand_msg/JointStateInt16)

Each element in data[] is a 16-bit error bitmask per joint:
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

Usage:  python3 joint_error_state_sub.py [left|right] [product]
        default: side=left, product=h3u_m
"""

import sys

import rclpy
from rclpy.node import Node
from omnihand_msg.msg import JointStateInt16
from omnihand import h3um_error_report_to_string


class JointErrorStateSubscriber(Node):
    def __init__(self, hand_side: str, product: str):
        super().__init__(f'{product}_{hand_side}_joint_error_state_subscriber')
        self.hand_side = hand_side
        self.product = product
        self.subscription = self.create_subscription(
            JointStateInt16,
            f'/{product}/{hand_side}/joint_error_states',
            self.callback,
            10,
        )
        self.get_logger().info(
            f'{product}/{hand_side} joint_error_states subscriber started (O20, 20 DOF)'
        )

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
    node = JointErrorStateSubscriber(hand_side, product)
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
