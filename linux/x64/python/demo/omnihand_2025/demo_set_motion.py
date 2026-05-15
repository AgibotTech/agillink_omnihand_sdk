# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, HandType, OmniHand2025Gesture
import time

# Interactive menu index -> SDK gesture (menu order matches demo labels, not raw enum values)
MENU_GESTURES = {
    0: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_RESET,
    1: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_PAPER,
    2: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST1,
    3: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST2,
    4: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_OK,
    5: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ONE_HANDED_FINGER_HEART,
    6: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_LIKE,
    7: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ILY,
    8: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM1,
    9: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM2,
    10: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM3,
    11: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM4,
    12: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM6,
    13: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM8,
    14: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART1,
    15: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART2,
    16: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART3,
    17: OmniHand2025Gesture.OMNIHAND_2025_GESTURE_CLASPING,
}

def print_menu():
    print("\n=== OmniHand gesture menu ===")
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
            
            if choice == 99:
                print("Exiting")
                break

            gesture = MENU_GESTURES.get(choice)
            if gesture is None:
                print("Invalid input; enter a valid menu number")
                continue
                
            print(f"\nRunning gesture: {gesture.name}")
            hand.set_hand_gesture(gesture)
            time.sleep(1)
            
            real_positions = hand.get_all_active_joint_angles()
            print("Current joint angles: ", real_positions)
            
        except ValueError:
            print("Invalid input; enter a valid number")
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    main()
