# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHandPro2025, HandType
import time

def main():
    hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)
    
    # set current report period for all joints
    periods = [500] * 12
    hand.set_all_current_report_periods(periods)
    
    time.sleep(1)
    
    # get current report for joint 8
    current = hand.get_current_report(8)
    print(f"Joint 8 current: {current}mA")

    # get current report for all joints
    all_currents = hand.get_all_current_reports()
    print(f"All joint currents: {all_currents}")


if __name__ == "__main__":
    main()
