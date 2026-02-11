# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, HandType
import time

def main():
    hand = OmniHand2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)
        
    # get temperature report for finger 8
    temp = hand.get_temperature_report(8)
    print(f"Joint 8 temperature: {temp}°C")

    # get temperature report for all fingers
    all_temps = hand.get_all_temperature_reports()
    print(f"All joint temperatures: {all_temps}")



if __name__ == "__main__":
    main()
