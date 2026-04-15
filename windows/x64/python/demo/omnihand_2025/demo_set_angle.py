# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 关节角度控制示例

此示例演示如何使用 `set_all_active_joint_angles()` 设置所有主动关节角度。
展示完整的动作序列：reset → FIST → reset
适合快速测试关节角度控制功能。
"""

import argparse
import time
from omnihand import OmniHand2025, HandType

def main():
    parser = argparse.ArgumentParser(description='OmniHand 2025 Joint Angle Control Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp', 'tj'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    print("=" * 50)
    print("OmniHand 2025 Joint Angle Control Demo")
    print("=" * 50)
    
    # 创建 OmniHand 2025 灵巧手实例 based on device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.RIGHT)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(hand_type=HandType.RIGHT, uart_port='COM7')
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    elif args.device == 'tj':
        hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.LEFT, marvin_controller_ip="192.168.10.190")
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT)
    
    # 检查初始化状态
    if not hand.init():
        print("[Error]: Failed to initialize OmniHand 2025 hand!")
        return
    
    hand.show_data_details(True)
    print("[OK]: OmniHand 2025 hand initialized successfully!\n")
    
    # reset - 所有关节回到初始位置
    print("Setting all joints to reset position...")
    aim_positions = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # O10 有 10 个关节
    hand.set_all_active_joint_angles(aim_positions)
    print(f"Set active joint angles: {aim_positions}")
    time.sleep(1)
    
    # 读取当前角度
    all_active_angles = hand.get_all_active_joint_angles()
    print(f"Current active joint angles: {all_active_angles}")
    
    # FIST - 握拳动作
    print("\nSetting all joints to FIST position...")
    aim_positions = [0.5, -0.2, 0.0, -1.2, 0.0, 1.35, 1.53, 0.0, 1.36, 1.82]  # O10 有 10 个关节
    hand.set_all_active_joint_angles(aim_positions)
    print(f"Set active joint angles: {aim_positions}")
    time.sleep(1)
    
    # 读取当前角度
    all_active_angles = hand.get_all_active_joint_angles()
    print(f"Current active joint angles: {all_active_angles}")
    
    # reset - 再次回到初始位置
    print("\nSetting all joints back to reset position...")
    aim_positions = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    hand.set_all_active_joint_angles(aim_positions)
    print(f"Set active joint angles: {aim_positions}")
    time.sleep(1)
    
    # 读取最终角度
    all_active_angles = hand.get_all_active_joint_angles()
    print(f"Final active joint angles: {all_active_angles}")
    
    # 读取所有关节角度（包括被动关节）
    all_angles = hand.get_all_joint_angles()
    print(f"\nAll joint angles (including passive joints): {all_angles}")
    print(f"Total joints: {len(all_angles)} (10 active + 6 passive)")
    
    print("\n[Done]: Joint angle control demo completed!")

if __name__ == "__main__":
    main()
