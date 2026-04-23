# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

import argparse
import time
from omnihand import OmniHandPro2025, HandType

def main():
    parser = argparse.ArgumentParser(description='Monitor current for OmniHand Pro 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(hand_type=HandType.LEFT)
    else:  # default: zlgcan
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
