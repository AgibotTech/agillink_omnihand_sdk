import argparse
from glove_receiver import GloveReceiver
import time

def main():
    parser = argparse.ArgumentParser(description='机械手控制主程序')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='hcan',
                        help='CAN设备类型：zlgcan（ZLG USB CANFD）或 hcan（HCAN USB CANFD），默认 zlgcan')
    parser.add_argument('--mode', choices=['o10', 'o10_hal'], default='o10', help='选择机械手类型')
    parser.add_argument('--ip', help='输入udp信息传输的ip地址，默认为127.0.0.1', default='127.0.0.1')
    parser.add_argument('--port', type=int, help='输入udp信息传输的端口号，默认为7777', default=7777)
    parser.add_argument('--hal_ip', help='HAL服务ip地址，默认为127.0.0.1', default='127.0.0.1')
    parser.add_argument('--hal_port', type=int, help='HAL服务端口号，默认为56421', default=56421)
    parser.add_argument('--hand', type=str, help='输入操控手套是左手还是右手，默认为right，控制双手时为both', default='both')
    parser.add_argument('--user_name', type=str, help='输入HandDrive端中的用户名，默认为teleop', default='teleop')
    args, unknown = parser.parse_known_args()

    # 统一UDP数据接收
    sdk = GloveReceiver()
    sdk.server_addr = (args.ip, args.port)
    sdk.initialize()
    sdk.start_listening()

    # 灵心巧手机械手分支
    if args.mode == 'o10':
        from Omnihand_o10_yudie import init_hand, set_hand_position
        if args.hand == 'both':
            left_hand, right_hand = init_hand(args.hand, args.device)
        else:
            hand = init_hand(args.hand, args.device)
        def update():
            role_list = sdk.get_role_name_list()
            if len(role_list) > 0:
                if args.hand == 'both':
                    left_joints = sdk.get_finger_data_for_o10hand(args.user_name, 'left')
                    right_joints = sdk.get_finger_data_for_o10hand(args.user_name, 'right')
                    print(f"Finger Data: {left_joints} | {right_joints}")
                    set_hand_position(left_hand, left_joints)
                    set_hand_position(right_hand, right_joints)
                else:
                    joints = sdk.get_finger_data_for_o10hand(args.user_name, args.hand)
                    print(f"Finger Data: {joints}")
                    set_hand_position(hand, joints)

    elif args.mode == 'o10_hal':
        # 新接口：通过 HAL 服务控制（使用 Solver + curl）
        # 这里将 main.py 传入的 ip/port 作为 HAL_HOST/HAL_PORT 使用
        from o10_hand_service import set_hand_position
        def update():
            role_list = sdk.get_role_name_list()
            if len(role_list) > 0:
                if args.hand == 'both':
                    left_joints = sdk.get_finger_data_for_o10hand(args.user_name, 'left')
                    right_joints = sdk.get_finger_data_for_o10hand(args.user_name, 'right')
                    print(f"Finger Data: {left_joints} | {right_joints}")
                    set_hand_position('left', left_joints, args.hal_ip, args.hal_port)
                    set_hand_position('right', right_joints, args.hal_ip, args.hal_port)
                else:
                    joints = sdk.get_finger_data_for_o10hand(args.user_name, args.hand)
                    print(f"Finger Data: {joints}")
                    set_hand_position(args.hand, joints, args.hal_ip, args.hal_port)

    try:
        while True:
            update()
            time.sleep(0.05)
    except KeyboardInterrupt:
        print("\n已中断，正在退出...")
    finally:
        sdk.end_listening()


if __name__ == "__main__":
    main()