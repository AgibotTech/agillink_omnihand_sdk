# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, EHandType
import time

def main():
    hand = OmniHand2025.create_hand_by_zlgcan(hand_type=EHandType.LEFT)
    
    hand.show_data_details(True)
    
    # get error report for joint 1
    error = hand.get_error_report(1)
    print(f"Joint 1 error info: motor_except={error.motor_except}")

    # get error report for all joints
    all_errors = hand.get_all_error_reports()
    for i, error in enumerate(all_errors):
        print(f"Joint {i+1} error info: motor_except={error.motor_except}")


if __name__ == "__main__":
    main()
