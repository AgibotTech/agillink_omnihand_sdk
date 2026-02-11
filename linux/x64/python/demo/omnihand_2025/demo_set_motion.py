# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, Finger, ControlMode, HandType
import time
from enum import Enum

class Gesture(Enum):
    RESET = 0
    PAPER = 1
    FIST1 = 2
    FIST2 = 3
    OK = 4
    ONE_HANDED_FINGER_HEART = 5
    LIKE = 6
    ILY = 7
    NUM1 = 8
    NUM2 = 9
    NUM3 = 10
    NUM4 = 11
    NUM6 = 12
    NUM8 = 13
    HAND_HEART1 = 14
    HAND_HEART2 = 15
    HAND_HEART3 = 16
    CLASPING = 17
    EXIT = 99

def print_menu():
    print("\n=== OmniHand 手势控制菜单(预置指令仅适合右手) ===")
    print("0. 重置位置")
    print("1. 手掌展开")
    print("2. 握拳方式1")
    print("3. 握拳方式2")
    print("4. OK手势")
    print("5. 单手比心")
    print("6. 点赞")
    print("7. ILY手势")
    print("8. 数字1")
    print("9. 数字2")
    print("10. 数字3")
    print("11. 数字4")
    print("12. 数字6")
    print("13. 数字8")
    print("14. 双手比心1")
    print("15. 双手比心2")
    print("16. 双手比心3")
    print("17. 合十")
    print("99. 退出程序")
    print("请输入对应的数字选择手势: ")

def get_gesture_positions(gesture):
    gesture_positions = {
        Gesture.RESET: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        Gesture.PAPER: [0.58, -0.21, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        Gesture.FIST1: [0.43, -0.3, 0.66, 0.0, 1.48, 1.48, 0.0, 1.48, 0.0, 1.48],
        Gesture.FIST2: [0.5, -1.0, 0.75, 0.0, 1.48, 1.48, 0.0, 1.48, 0.0, 1.48],
        Gesture.OK: [0.03, -1.51, 0.7, -0.16, 0.85, 0.21, 0.07, 0.153, 0.107, 0.1],
        Gesture.ONE_HANDED_FINGER_HEART: [0.8, -0.4, 0.47, 0.0, 0.82, 1.48, 0.0, 1.48, 0.0, 1.48],
        Gesture.LIKE: [0.27, 0.0, 0.0, 0.0, 1.48, 1.48, 0.0, 1.48, 0.0, 1.48],
        Gesture.ILY: [0.33, 0.0, 0.0, -0.1, 0.0, 1.48, 0.07, 1.48, 0.11, 0.0],
        Gesture.NUM1: [0.32, -1.12, 0.79, -0.06, 0.0, 1.48, 0.0, 1.48, 0.0, 1.48],
        Gesture.NUM2: [0.48, -1.5, 0.79, -0.16, 0.0, 0.0, 0.0, 1.48, 0.0, 1.48],
        Gesture.NUM3: [0.64, -1.48, 0.81, -0.16, 0.0, 0.0, 0.09, 0.0, 0.09, 1.48],
        Gesture.NUM4: [0.64, -1.48, 0.81, -0.16, 0.0, 0.0, 0.07, 0.0, 0.15, 0.0],
        Gesture.NUM6: [0.40, 0.0, 0.0, 0.0, 1.48, 1.48, 0.05, 1.48, 0.17, 0.0],
        Gesture.NUM8: [0.40, 0.0, 0.0, 0.0, 0.0, 1.48, 0.0, 1.48, 0.0, 1.48],
        Gesture.HAND_HEART1: [-0.03, -1.36, 0.0, 0.0, 0.65, 0.65, 0.0, 0.65, 0.0, 0.65],
        Gesture.HAND_HEART2: [0.30, -0.1, 0.66, 0.0, 1.1, 1.1, 0.0, 1.1, 0.0, 1.1],
        Gesture.HAND_HEART3: [0.0, -1.56, 0.46, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        Gesture.CLASPING: [0.5, -0.8, 0.2, -0.16, 0.6, 0.6, 0.17, 0.6, 0.17, 0.6]
    }
    return gesture_positions.get(gesture, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

def main():
    hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT, canfd_channel_id=1)
    
    while True:
        print_menu()
        try:
            choice = int(input())
            gesture = Gesture(choice)
            
            if gesture == Gesture.EXIT:
                print("程序退出")
                break
                
            print(f"\n执行手势: {gesture.name}")
            positions = get_gesture_positions(gesture)
            hand.set_all_active_joint_angles(positions)
            time.sleep(1)
            
            # 获取并打印实际位置
            real_positions = hand.get_all_active_joint_angles()
            print("当前关节角度: ", real_positions)
            
        except ValueError:
            print("输入无效，请输入正确的数字")
        except Exception as e:
            print(f"发生错误: {e}")

if __name__ == "__main__":
    main()
