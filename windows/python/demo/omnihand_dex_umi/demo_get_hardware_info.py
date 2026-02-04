# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 硬件信息获取示例

此示例演示如何获取 OmniHand Dex UMI 产品的厂家信息和设备信息。
"""

from omnihand import OmniHandDexUMI, EHandType
import time

def main():
    print("=" * 50)
    print("OmniHand Dex UMI Hardware Info Demo")
    print("=" * 50)
    
    # 创建 OmniHand Dex UMI 灵巧手实例
    hand = OmniHandDexUMI.create_hand_by_zlgcan(
        hand_type=EHandType.LEFT,
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
    print(f"Hardware Version: {vendor_info.hardware_version.major_}.{vendor_info.hardware_version.minor_}.{vendor_info.hardware_version.patch_}")
    print(f"Software Version: {vendor_info.software_version.major_}.{vendor_info.software_version.minor_}.{vendor_info.software_version.patch_}")
    print(f"Voltage: {vendor_info.voltage} mV")
    print(f"DOF: {vendor_info.dof}")
    
    # 获取设备信息
    print("\n" + "=" * 50)
    print("Device Info (Pn2)")
    print("=" * 50)
    device_info = hand.get_device_info()
    print(f"Device ID: {device_info.hand_device_id}")
    print(f"Communication Parameters:")
    print(f"  CAN FD Arbitration Bitrate: {device_info.commu_params.bitrate_}")
    print(f"  CAN FD Arbitration Sample Point: {device_info.commu_params.sample_point_}")
    print(f"  CAN FD Data Bitrate: {device_info.commu_params.dbitrate_}")
    print(f"  CAN FD Data Sample Point: {device_info.commu_params.dsample_point_}")
    
    # UMI-specific fields (Pn2.03, Pn2.04, Pn2.05, Pn2.06)
    print("\nUMI-Specific Device Info:")
    if device_info.position_report_frequency is not None:
        print(f"  Position Report Frequency (Pn2.03): {device_info.position_report_frequency} Hz")
    else:
        print(f"  Position Report Frequency (Pn2.03): Not available")
    
    if device_info.tactile_sensor_report_frequency is not None:
        print(f"  Tactile Sensor Report Frequency (Pn2.04): {device_info.tactile_sensor_report_frequency} Hz")
    else:
        print(f"  Tactile Sensor Report Frequency (Pn2.04): Not available")
    
    if device_info.adc_channel_count is not None:
        print(f"  ADC Channel Count (Pn2.05): {device_info.adc_channel_count}")
    else:
        print(f"  ADC Channel Count (Pn2.05): Not available")
    
    if device_info.tactile_sensor_info is not None:
        print(f"  Tactile Sensor Info (Pn2.06): {len(device_info.tactile_sensor_info)} bytes")
        if len(device_info.tactile_sensor_info) > 0:
            print(f"    First 20 bytes: {device_info.tactile_sensor_info[:20]}")
    else:
        print(f"  Tactile Sensor Info (Pn2.06): Not available")
    
    # 使用 toString() 方法显示完整信息
    print("\n" + "=" * 50)
    print("Full Device Info (toString):")
    print("=" * 50)
    print(device_info)
    
    print("\n[Done]: Hardware info demo completed!")

if __name__ == "__main__":
    main()
