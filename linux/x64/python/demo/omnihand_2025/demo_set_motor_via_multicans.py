# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

from omnihand import OmniHand2025, EFinger, EControlMode, EHandType
import time

def main():
    # 通过序列号直接创建 hand（推荐方式）
    left_hand = OmniHand2025.create_hand_by_zlgcan(EHandType.LEFT, 1, "201BFF2AF01202D44690")
    right_hand = OmniHand2025.create_hand_by_zlgcan(EHandType.RIGHT, 1, "A029A58630B30D14DBB")
    
    if left_hand is None or right_hand is None:
        print("Cannot find CANFD devices by serial numbers!")
        return
    
    # 或者如果已知 canfd_id，可以直接使用：
    # left_hand = OmniHand2025.create_hand_by_zlgcan(EHandType.LEFT, 1, 0)  # canfd_device_id= 0
    # right_hand = OmniHand2025.create_hand_by_zlgcan(EHandType.RIGHT, 1, 1)  # canfd_device_id= 1
    
    # 启用详细日志查看 CAN 通信
    left_hand.show_data_details(True)
    right_hand.show_data_details(True)

    left_hand.set_joint_position(2, 200)
    # time.sleep(1)
    right_hand.set_joint_position(10, 200)
    time.sleep(1)

    id2_joint_posi = left_hand.get_joint_position(2)
    print("Joint 2 position of left hand: ", id2_joint_posi)
    id2_joint_posi_right = right_hand.get_joint_position(10)
    print("Joint 10 position of right hand: ", id2_joint_posi_right)

    init_positions = [2048,2048,4096,2048,4096,4096,2048,4096,2048,4096]
    left_hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_positions = left_hand.get_all_joint_positions()
    print("All joint positions of left hand: ", real_positions)

    right_hand.set_all_joint_positions(init_positions)
    time.sleep(1)

    real_positions_right = right_hand.get_all_joint_positions()
    print("All joint positions of right hand: ", real_positions_right)


if __name__ == "__main__":
    main()
