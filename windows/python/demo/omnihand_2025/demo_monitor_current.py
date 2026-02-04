# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, EHandType
import time

def main():
    hand = OmniHand2025.create_hand_by_zlgcan(hand_type=EHandType.LEFT)
    
    # get current report for finger 8
    current = hand.get_current_report(8)
    print(f"Joint 8 current: {current}mA")

    all_currents = hand.get_all_current_reports()
    print(f"All joint currents: {all_currents}")


if __name__ == "__main__":
    main()
