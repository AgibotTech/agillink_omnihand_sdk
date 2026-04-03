# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand Dex UMI 触觉传感器示例

此示例演示如何读取 OmniHand Dex UMI 产品的触觉传感器数据。
OmniHand Dex UMI 使用一维触觉传感器（1D sensors）。
注意：UMI 协议只支持 Pn6 (0x06)，不支持 Pn5 (0x05)。
因此必须使用 get_tactile_sensor_data_raw() 方法，而不是 get_tactile_sensor_data()。
"""

from omnihand import OmniHandDexUMI, Finger, HandType
import time

def main():
    print("=" * 50)
    print("OmniHand Dex UMI Tactile Sensor Demo")
    print("=" * 50)
    print("Note: OmniHand Dex UMI uses 1D tactile sensors")
    print("Protocol: Pn6 (0x06) - Tactile Sensor (read-only)")
    print("=" * 50)
    
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand Dex UMI Tactile Sensor Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # 创建 OmniHand Dex UMI 灵巧手实例 based on device type
    if args.device == 'hcan':
        hand = OmniHandDexUMI.create_hand_by_hcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandDexUMI.kDefaultHandDeviceId,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    else:  # default: zlgcan
        hand = OmniHandDexUMI.create_hand_by_zlgcan(
            hand_type=HandType.LEFT,
            hand_device_id=OmniHandDexUMI.kDefaultHandDeviceId,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    
    # 检查初始化状态
    if not hand.init():
        print("[Error]: Failed to initialize OmniHand Dex UMI hand!")
        return
    
    print("[OK]: OmniHand Dex UMI hand initialized successfully!\n")
    
    # 读取各个手指的触觉传感器数据
    print("Reading tactile sensor data for each finger...\n")
    
    # UMI has 6 sensors (no Dorsum/back of hand)
    fingers = [
        (Finger.THUMB, "Thumb"),
        (Finger.INDEX, "Index"),
        (Finger.MIDDLE, "Middle"),
        (Finger.RING, "Ring"),
        (Finger.LITTLE, "Little"),
        (Finger.PALM, "Palm"),
    ]
    
    for finger_enum, finger_name in fingers:
        try:
            sensor_data = hand.get_tactile_sensor_data_raw(finger_enum)
            if sensor_data and sensor_data.data:
                data = sensor_data.data
                print(f"{finger_name} ({finger_enum.name}):")
                print(f"  Data length: {len(data)} bytes")
                print(f"  Sum: {sum(data)} g")
                if len(data) > 0:
                    print(f"  First 10 values: {data[:10]}")
            else:
                print(f"{finger_name}: No data available")
        except Exception as e:
            print(f"{finger_name}: Error - {e}")
        print()
    
    # 读取所有触觉传感器原始数据
    print("=" * 50)
    print("Reading all tactile sensor raw data...")
    print("=" * 50)
    try:
        all_sensor_data = hand.get_all_tactile_sensor_data_raw()
        print(f"Total sensors: {len(all_sensor_data)}")
        for sensor_data in all_sensor_data:
            # sensor_id is an integer (Finger enum value)
            sensor_id = sensor_data.sensor_id
            sensor_name = "Unknown"
            for finger_enum, finger_name in fingers:
                if sensor_id == finger_enum:
                    sensor_name = finger_name
                    break
            data_len = len(sensor_data.data) if sensor_data.data else 0
            print(f"Sensor {sensor_name} (ID: {sensor_id}): {data_len} bytes")
    except Exception as e:
        print(f"Error reading all sensor data: {e}")
    
    # 连续读取示例
    print("\n" + "=" * 50)
    print("Continuous reading (5 samples)...")
    print("=" * 50)
    for i in range(5):
        try:
            thumb_sensor_data = hand.get_tactile_sensor_data_raw(Finger.THUMB)
            if thumb_sensor_data and thumb_sensor_data.data:
                thumb_sum = sum(thumb_sensor_data.data)
                print(f"Sample {i+1}: Thumb sum = {thumb_sum} g")
            else:
                print(f"Sample {i+1}: Thumb - No data available")
        except Exception as e:
            print(f"Sample {i+1}: Thumb - Error: {e}")
        time.sleep(0.2)
    
    print("\n[Done]: Tactile sensor demo completed!")

if __name__ == "__main__":
    main()
