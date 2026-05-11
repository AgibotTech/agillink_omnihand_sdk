# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

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
    print("\n=== OmniHand gesture menu (presets tuned for right hand) ===")
    print("0. Reset pose")
    print("1. Open palm")
    print("2. Fist style 1")
    print("3. Fist style 2")
    print("4. OK gesture")
    print("5. One-hand heart")
    print("6. Thumbs up")
    print("7. ILY gesture")
    print("8. Digit 1")
    print("9. Digit 2")
    print("10. Digit 3")
    print("11. Digit 4")
    print("12. Digit 6")
    print("13. Digit 8")
    print("14. Two-hand heart 1")
    print("15. Two-hand heart 2")
    print("16. Two-hand heart 3")
    print("17. Prayer hands")
    print("99. Exit")
    print("Enter number to select gesture: ")

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
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand 2025 Gesture Control Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp', 'tj'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.RIGHT, canfd_channel_id=1)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(hand_type=HandType.RIGHT, uart_port='/dev/ttyACM0')
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    elif args.device == 'tj':
        hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.LEFT, marvin_controller_ip="192.168.10.190")
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT, canfd_channel_id=1)
    
    while True:
        print_menu()
        try:
            choice = int(input())
            gesture = Gesture(choice)
            
            if gesture == Gesture.EXIT:
                print("Exiting")
                break
                
            print(f"\nRunning gesture: {gesture.name}")
            positions = get_gesture_positions(gesture)
            hand.set_all_active_joint_angles(positions)
            time.sleep(1)
            
            # read and print actual positions
            real_positions = hand.get_all_active_joint_angles()
            print("Current joint angles: ", real_positions)
            
        except ValueError:
            print("Invalid input; enter a valid number")
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    main()
