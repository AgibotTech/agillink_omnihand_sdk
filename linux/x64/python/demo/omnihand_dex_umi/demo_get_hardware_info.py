# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 硬件信息获取示例

此示例演示如何获取 OmniHand Dex UMI 产品的厂家信息和设备信息。
"""

import argparse
import time
from omnihand import OmniHandDexUMI, HandType

def main():
    parser = argparse.ArgumentParser(description='Get hardware info from OmniHand Dex UMI')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    print("=" * 50)
    print("OmniHand Dex UMI Hardware Info Demo")
    print("=" * 50)
    
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
    
    # 获取厂家信息
    print("=" * 50)
    print("Vendor Info (Pn1)")
    print("=" * 50)
    vendor_info = hand.get_vendor_info()
    print(f"Product Model: {vendor_info.product_model}")
    print(f"Serial Number: {vendor_info.product_seq_num}")
    print(f"Hardware Version: {vendor_info.hardware_version.major}.{vendor_info.hardware_version.minor}.{vendor_info.hardware_version.patch}")
    print(f"Software Version: {vendor_info.software_version.major}.{vendor_info.software_version.minor}.{vendor_info.software_version.patch}")
    print(f"Voltage: {vendor_info.voltage} mV")
    print(f"DOF: {vendor_info.dof}")
    
    # 获取设备信息
    print("\n" + "=" * 50)
    print("Device Info (Pn2)")
    print("=" * 50)
    device_info = hand.get_device_info()
    print(f"Device ID: {device_info.hand_device_id}")
    print(f"Communication Parameters:")
    print(f"  CAN FD Arbitration Bitrate: {device_info.commu_params.bitrate}")
    print(f"  CAN FD Arbitration Sample Point: {device_info.commu_params.sample_point}")
    print(f"  CAN FD Data Bitrate: {device_info.commu_params.dbitrate}")
    print(f"  CAN FD Data Sample Point: {device_info.commu_params.dsample_point}")
    
    # 使用 toString() 方法显示完整信息
    print("\n" + "=" * 50)
    print("Full Device Info (toString):")
    print("=" * 50)
    print(device_info)
    
    print("\n[Done]: Hardware info demo completed!")

if __name__ == "__main__":
    main()
