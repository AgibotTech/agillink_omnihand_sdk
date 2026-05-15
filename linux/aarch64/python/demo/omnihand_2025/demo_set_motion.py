# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, HandType, OmniHand2025Gesture
import time

# Menu index matches OmniHand2025Gesture numeric value (0=PAPER … 17=RESET)
GESTURE_MENU = (
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_PAPER, "Open palm"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST1, "Fist style 1"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_FIST2, "Fist style 2"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_OK, "OK gesture"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ONE_HANDED_FINGER_HEART, "One-hand heart"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_LIKE, "Thumbs up"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_ILY, "ILY gesture"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM1, "Digit 1"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM2, "Digit 2"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM3, "Digit 3"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM4, "Digit 4"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM6, "Digit 6"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_NUM8, "Digit 8"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART1, "Two-hand heart 1"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART2, "Two-hand heart 2"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_HAND_HEART3, "Two-hand heart 3"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_CLASPING, "Prayer hands"),
    (OmniHand2025Gesture.OMNIHAND_2025_GESTURE_RESET, "Reset pose"),
)


def print_menu():
    print("\n=== OmniHand gesture menu (index = OmniHand2025Gesture value) ===")
    for gesture, label in GESTURE_MENU:
        print(f"{int(gesture)}. {label}")
    print("99. Exit")
    print("Enter gesture index: ")


def main():
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand 2025 Gesture Control Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp', 'tj'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()

    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(hand_type=HandType.RIGHT)
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(hand_type=HandType.RIGHT, uart_port='/dev/ttyACM0')
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    elif args.device == 'tj':
        hand = OmniHand2025.create_hand_by_tj(hand_type=HandType.LEFT, marvin_controller_ip="192.168.10.190")
    else:
        hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.RIGHT)

    while True:
        print_menu()
        try:
            choice = int(input())

            if choice == 99:
                print("Exiting")
                break

            try:
                gesture = OmniHand2025Gesture(choice)
            except ValueError:
                print("Invalid input; enter a gesture index 0–17, or 99 to exit")
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
