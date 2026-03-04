# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

import argparse
from omnihand import OmniHandPro2025, Finger, HandType

def main():
    parser = argparse.ArgumentParser(description='Get touch sensor data for OmniHand Pro 2025')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHandPro2025.create_hand_by_hcan(hand_type=HandType.LEFT)
    else:  # default: zlgcan
        hand = OmniHandPro2025.create_hand_by_zlgcan(hand_type=HandType.LEFT)

    # O12 uses get_tactile_sensor_3d_data (returns TactileSensor3DData object)
    # O10 uses get_tactile_sensor_data (returns List[int])
    touch_data = hand.get_tactile_sensor_3d_data(Finger.THUMB)
    print(f"Thumb touch sensor data:")
    print(f"  Online state: {touch_data.online_state}")
    print(f"  Normal force: {touch_data.normal_force}")
    print(f"  Tangent force: {touch_data.tangent_force}")
    print(f"  Tangent force angle: {touch_data.tangent_force_angle}")
    print(f"  Channel values: {touch_data.channel_values}")
    print(f"  Capacitive approach: {touch_data.capacitive_approach}")

if __name__ == "__main__":
    main()
