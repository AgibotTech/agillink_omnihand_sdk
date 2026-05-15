import argparse
from glove_receiver import GloveReceiver
import time

def main():
    parser = argparse.ArgumentParser(description='Dexterous hand control main program')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='hcan',
                        help='CAN backend: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD); default zlgcan')
    parser.add_argument('--mode', choices=['o10', 'o10_hal'], default='o10', help='Hand stack selection')
    parser.add_argument('--ip', help='UDP bind IP (default 127.0.0.1)', default='127.0.0.1')
    parser.add_argument('--port', type=int, help='UDP port (default 7777)', default=7777)
    parser.add_argument('--hal_ip', help='HAL service IP (default 127.0.0.1)', default='127.0.0.1')
    parser.add_argument('--hal_port', type=int, help='HAL service port (default 56421)', default=56421)
    parser.add_argument('--hand', type=str, help='Glove side: left/right (default right); both for dual', default='both')
    parser.add_argument('--user_name', type=str, help='HandDrive username (default teleop)', default='teleop')
    args, unknown = parser.parse_known_args()

    # unified UDP receive
    sdk = GloveReceiver()
    sdk.server_addr = (args.ip, args.port)
    sdk.initialize()
    sdk.start_listening()

    # Lingxin dexterous-hand branch
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
        # New path: HAL service (solver + curl)
        # Use main.py ip/port as HAL_HOST/HAL_PORT
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
        print("\nInterrupted, exiting...")
    finally:
        sdk.end_listening()


if __name__ == "__main__":
    main()