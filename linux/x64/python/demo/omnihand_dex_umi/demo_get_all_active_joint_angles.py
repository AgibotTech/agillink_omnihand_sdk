#!/usr/bin/env python3
# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 主动关节角度读取示例

此示例演示如何使用 get_all_active_joint_angles() 获取 UMI 10 个主动关节角度。
返回值从电机位置通过标定参数转换得到，单位为 rad。

运行方式：
    python3 demo_get_all_active_joint_angles.py left
    python3 demo_get_all_active_joint_angles.py right
"""

import sys
import time
from omnihand import OmniHandDexUMI, HandType


def main():
    if len(sys.argv) < 2 or sys.argv[1] not in ("left", "right"):
        print(f"Usage: {sys.argv[0]} [left|right]")
        sys.exit(1)

    hand_type = HandType.LEFT if sys.argv[1] == "left" else HandType.RIGHT
    hand = OmniHandDexUMI.create_hand_by_zlgcan(
        hand_type=hand_type,
        hand_device_id=1,
        canfd_device_id=0,
    )

    print(f"Connected to UMI ({sys.argv[1]} hand)")
    print("Reading active joint angles (rad) every 0.5s, press Ctrl+C to stop.\n")

    try:
        while True:
            angles = hand.get_all_active_joint_angles()
            formatted = [f"{a:+.4f}" for a in angles]
            print(f"  Active joint angles ({len(angles)}): [{', '.join(formatted)}]")
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("\nStopped.")


if __name__ == "__main__":
    main()
