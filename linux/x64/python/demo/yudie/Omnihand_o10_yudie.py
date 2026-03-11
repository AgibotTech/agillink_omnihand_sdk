# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, Finger, ControlMode, HandType
from enum import Enum

def init_hand(hand_type: str = "right"):
    """
    初始化手
    """
    if hand_type.lower() == "right":
        hand = OmniHand2025.create_hand_by_zlgcan(hand_device_id= 1,canfd_device_id= 0,hand_type=HandType.RIGHT)
        return hand
    elif hand_type.lower() == "left":
        hand = OmniHand2025.create_hand_by_zlgcan(hand_device_id= 1,canfd_device_id= 0,hand_type=HandType.LEFT)
        return hand
    else:
        left_hand = OmniHand2025.create_hand_by_zlgcan(canfd_device_id=0, hand_type=HandType.LEFT, canfd_channel_id=0)
        right_hand = OmniHand2025.create_hand_by_zlgcan(canfd_device_id=0, hand_type=HandType.RIGHT, canfd_channel_id=1)
        # 启用详细日志查看 CAN 通信
        left_hand.show_data_details(True)
        right_hand.show_data_details(True)
        return left_hand, right_hand

def set_hand_position(hand: OmniHand2025, positions: list):
    """
    设置手的位置
    """
    hand.set_all_active_joint_angles(positions)
    print("get active joint angles:", hand.get_all_active_joint_angles())
