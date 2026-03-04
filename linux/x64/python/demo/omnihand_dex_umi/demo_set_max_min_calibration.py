# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 最大最小位置校准示例

此示例演示如何设置 OmniHand Dex UMI 产品的最大和最小位置校准值。
校准功能用于设置位置传感器的最大和最小位置范围。

使用说明：
1. 将手指移动到最小位置（完全弯曲），然后调用 set_min_position_calibration()
2. 将手指移动到最大位置（完全伸直），然后调用 set_max_position_calibration()

注意：
- 校准操作是写操作，会修改设备内部参数
- 请确保在安全的环境下进行校准
- 校准后需要重新初始化设备才能生效
"""

from omnihand import OmniHandDexUMI, HandType
import time

def main():
    print("=" * 60)
    print("OmniHand Dex UMI Max/Min Position Calibration Demo")
    print("=" * 60)
    
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand Dex UMI Max/Min Position Calibration Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # 创建 OmniHand Dex UMI 灵巧手实例 based on device type
    if args.device == 'hcan':
        hand = OmniHandDexUMI.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=1,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    else:  # default: zlgcan
        hand = OmniHandDexUMI.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=1,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    
    # 检查初始化状态
    if not hand.init():
        print("[Error]: Failed to initialize OmniHand Dex UMI hand!")
        return
    
    print("[OK]: OmniHand Dex UMI hand initialized successfully!\n")
    
    print("=" * 60)
    print("Position Calibration Instructions")
    print("=" * 60)
    print("1. Minimum Position Calibration:")
    print("   - Move all fingers to their MINIMUM position (fully bent)")
    print("   - This will set the lower limit of position range")
    print("   - Protocol: Pn7, sub-register 0x00")
    print()
    print("2. Maximum Position Calibration:")
    print("   - Move all fingers to their MAXIMUM position (fully extended)")
    print("   - This will set the upper limit of position range")
    print("   - Protocol: Pn7, sub-register 0x01")
    print()
    
    # 最小位置校准
    print("\n" + "=" * 60)
    print("Step 1: Setting Minimum Position Calibration")
    print("=" * 60)
    print("[Info]: Please move all fingers to MINIMUM position (fully bent)...")
    
    # 询问用户是否继续最小位置校准
    print("=" * 60)
    print("WARNING: This will modify device calibration parameters!")
    print("=" * 60)
    user_input = input("Are fingers in MINIMUM position? Proceed with minimum calibration? (yes/no): ").strip().lower()
    
    if user_input == "n" or user_input == "no":
        print("[Info]: Minimum position calibration cancelled by user.")
        return
    
    countdown = 5
    for i in range(countdown, 0, -1):
        print(f"[Countdown]: {i} seconds remaining...")
        time.sleep(1)
    
    print("[Info]: Setting minimum position calibration...")
    try:
        hand.set_min_position_calibration()
        print("[OK]: Minimum position calibration set successfully!")
    except Exception as e:
        error_msg = str(e)
        print(f"[Error]: Failed to set minimum position calibration: {error_msg}")
        if "timeout" in error_msg.lower():
            print("[Warning]: Request timeout. This may indicate:")
            print("  - The device doesn't support this operation")
            print("  - Communication issue with the device")
            print("  - The device may need to be in a specific state")
        return
    
    time.sleep(1)
    
    # 最大位置校准
    print("\n" + "=" * 60)
    print("Step 2: Setting Maximum Position Calibration")
    print("=" * 60)
    print("[Info]: Please move all fingers to MAXIMUM position (fully extended)...")
    
    # 询问用户是否继续最大位置校准
    print("=" * 60)
    print("WARNING: This will modify device calibration parameters!")
    print("=" * 60)
    user_input = input("Are fingers in MAXIMUM position? Proceed with maximum calibration? (yes/no): ").strip().lower()
    
    if user_input == "n" or user_input == "no":
        print("[Info]: Maximum position calibration cancelled by user.")
        return
    
    countdown = 5
    for i in range(countdown, 0, -1):
        print(f"[Countdown]: {i} seconds remaining...")
        time.sleep(1)
    
    print("[Info]: Setting maximum position calibration...")
    try:
        hand.set_max_position_calibration()
        print("[OK]: Maximum position calibration set successfully!")
    except Exception as e:
        error_msg = str(e)
        print(f"[Error]: Failed to set maximum position calibration: {error_msg}")
        if "timeout" in error_msg.lower():
            print("[Warning]: Request timeout. This may indicate:")
            print("  - The device doesn't support this operation")
            print("  - Communication issue with the device")
            print("  - The device may need to be in a specific state")
        return
    
    # 完成提示
    print("\n" + "=" * 60)
    print("Calibration Complete!")
    print("=" * 60)
    print("[Info]: Both minimum and maximum position calibrations have been set.")
    print("[Info]: You may need to reinitialize the device for the changes to take effect.")
    print("[Note]: If you encountered timeout errors, please check:")
    print("  - Device firmware version supports calibration (Pn7 protocol)")
    print("  - Device is in the correct state for calibration")
    print("  - CAN communication is stable")
    print("[Done]: Calibration demo completed!")

if __name__ == "__main__":
    main()
