# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, EFinger, EHandType
import time

def main():
    hand = OmniHand2025.create_hand_by_zlgcan(hand_type=EHandType.LEFT)

    thumb_tactile_data = hand.get_tactile_sensor_data(EFinger.THUMB)
    print("Thumb tactile data: {} g".format(sum(thumb_tactile_data)))

    index_tactile_data = hand.get_tactile_sensor_data(EFinger.INDEX)
    print("Index tactile data: {} g".format(sum(index_tactile_data)))

    middle_tactile_data = hand.get_tactile_sensor_data(EFinger.MIDDLE)
    print("Middle tactile data: {} g".format(sum(middle_tactile_data)))

    ring_tactile_data = hand.get_tactile_sensor_data(EFinger.RING)
    print("Ring tactile data: {} g".format(sum(ring_tactile_data)))

    little_tactile_data = hand.get_tactile_sensor_data(EFinger.LITTLE)
    print("Little tactile data: {} g".format(sum(little_tactile_data)))

    palm_tactile_data = hand.get_tactile_sensor_data(EFinger.PALM)
    print("Palm tactile data: {} g".format(sum(palm_tactile_data)))

    dorsum_tactile_data = hand.get_tactile_sensor_data(EFinger.DORSUM)
    print("Dorsum tactile data: {} g".format(sum(dorsum_tactile_data)))


if __name__ == "__main__":
    main()
