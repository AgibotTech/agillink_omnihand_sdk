# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 Tactile Sensor Raw Data Demo

This demo shows how to read raw tactile sensor data (Raw Data) for OmniHand 2025 (O10).
Raw data provides full resolution sensor readings, different from the downsampled data
returned by get_tactile_sensor_data().

Features:
1. Get raw data for individual sensors
2. Get raw data for all sensors
3. Continuous reading with data statistics
"""

from omnihand import OmniHand2025, Finger, HandType
import time

def print_sensor_raw_data(sensor_data, sensor_name=""):
    """Print sensor raw data information"""
    try:
        # Check if data is valid
        if not sensor_data:
            print(f"  {sensor_name}: No sensor data object")
            return
        
        # Access data and sensor_id attributes
        data = sensor_data.data
        sensor_id = sensor_data.sensor_id
        
        if not data or len(data) == 0:
            print(f"  {sensor_name} (Sensor ID: {sensor_id}): No data available (empty list)")
            return
        
        print(f"  {sensor_name} (Sensor ID: {sensor_id}):")
        print(f"    Data length: {len(data)} bytes")
        print(f"    Min value: {min(data)} g")
        print(f"    Max value: {max(data)} g")
        print(f"    Average: {sum(data) / len(data):.2f} g")
        print(f"    Sum: {sum(data)} g")
        print(f"    All values ({len(data)} points):")
        # Print all data, multiple values per line for readability
        for i in range(0, len(data), 16):  # 16 values per line
            chunk = data[i:i+16]
            values_str = ', '.join(f"{v:3d}" for v in chunk)
            print(f"      [{i:3d}-{i+len(chunk)-1:3d}]: {values_str}")
    except AttributeError as e:
        print(f"  {sensor_name}: Attribute error - {e}")
        if sensor_data:
            print(f"    Object type: {type(sensor_data)}")
            print(f"    Available attributes: {[x for x in dir(sensor_data) if not x.startswith('_')]}")
    except Exception as e:
        print(f"  {sensor_name}: Error processing data - {e}")

def main():
    print("=" * 60)
    print("OmniHand 2025 Tactile Sensor Raw Data Demo")
    print("=" * 60)
    print("Note: This demo reads RAW sensor data (full resolution)")
    print("      Data unit: 1g, Max value: 255g")
    print("=" * 60)
    print()
    
    import argparse
    parser = argparse.ArgumentParser(description='OmniHand 2025 Tactile Sensor Raw Data Demo')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'rs485', 'zlgcan_tcp'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create OmniHand 2025 hand instance based on device type
    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(
            hand_type=HandType.RIGHT,
            hand_device_id=1,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(
            hand_type=HandType.RIGHT,
            uart_port='/dev/ttyACM0'
        )
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(hand_type=HandType.RIGHT, host='192.168.0.178', port=8000)
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(
            hand_type=HandType.RIGHT,
            hand_device_id=1,
            canfd_device_id=0,
            canfd_channel_id=0
        )
    
    # Check initialization status
    if not hand.init():
        print("[Error]: Failed to initialize OmniHand 2025 hand!")
        return
    
    print("[OK]: OmniHand 2025 hand initialized successfully!\n")
    
    # ============================================
    # 1. Get raw data for individual sensors
    # ============================================
    print("=" * 60)
    print("1. Reading individual sensor raw data")
    print("=" * 60)
    
    sensors = [
        (Finger.THUMB, "Thumb"),
        (Finger.INDEX, "Index"),
        (Finger.MIDDLE, "Middle"),
        (Finger.RING, "Ring"),
        (Finger.LITTLE, "Little"),
        (Finger.PALM, "Palm"),
        (Finger.DORSUM, "Dorsum"),
    ]
    
    for finger_enum, finger_name in sensors:
        try:
            sensor_data = hand.get_tactile_sensor_data_raw(finger_enum)
            print_sensor_raw_data(sensor_data, finger_name)
            print()
        except Exception as e:
            print(f"  {finger_name}: Error - {e}\n")
    
    # ============================================
    # 2. Get raw data for all sensors (at once)
    # ============================================
    print("=" * 60)
    print("2. Reading all sensors raw data at once")
    print("=" * 60)
    
    try:
        all_sensor_data = hand.get_all_tactile_sensor_data_raw()
        print(f"Total sensors: {len(all_sensor_data)}\n")
        
        for sensor_data in all_sensor_data:
            # Find corresponding name based on sensor_id
            sensor_name = "Unknown"
            try:
                sensor_id = sensor_data.sensor_id
                for finger_enum, name in sensors:
                    if sensor_id == finger_enum:
                        sensor_name = name
                        break
            except AttributeError:
                pass
            
            print_sensor_raw_data(sensor_data, sensor_name)
            print()
    except Exception as e:
        print(f"Error reading all sensor data: {e}\n")
    
    # ============================================
    # 3. Continuous reading example (real-time monitoring)
    # ============================================
    print("=" * 60)
    print("3. Continuous reading (real-time monitoring)")
    print("=" * 60)
    print("Reading thumb sensor raw data for 5 samples...\n")
    
    for i in range(5):
        try:
            thumb_data = hand.get_tactile_sensor_data_raw(Finger.THUMB)
            if thumb_data:
                data = thumb_data.data
                if data and len(data) > 0:
                    print(f"Sample {i+1}:")
                    print(f"  Data points: {len(data)}")
                    print(f"  Sum: {sum(data)} g")
                    print(f"  Average: {sum(data) / len(data):.2f} g")
                    print(f"  Min/Max: {min(data)}/{max(data)} g")
                    print(f"  All values ({len(data)} points):")
                    # Print all data, multiple values per line for readability
                    for j in range(0, len(data), 16):  # 16 values per line
                        chunk = data[j:j+16]
                        values_str = ', '.join(f"{v:3d}" for v in chunk)
                        print(f"    [{j:3d}-{j+len(chunk)-1:3d}]: {values_str}")
                else:
                    print(f"Sample {i+1}: No data available")
            else:
                print(f"Sample {i+1}: No sensor data object")
        except Exception as e:
            print(f"Sample {i+1}: Error - {e}")
        
        if i < 4:  # Don't wait on last iteration
            time.sleep(0.2)
        print()
    
    # ============================================
    # 4. Comparison: Raw data vs Downsampled data (only Palm and Dorsum)
    # ============================================
    print("=" * 60)
    print("4. Comparison: Raw data vs Downsampled data")
    print("Note: Only comparing Palm and Dorsum (these are the only sensors")
    print("      that have different resolutions between raw and downsampled data)")
    print("=" * 60)
    
    # Compare Palm
    try:
        print("\n--- Palm Comparison ---")
        palm_raw = hand.get_tactile_sensor_data_raw(Finger.PALM)
        palm_downsampled = hand.get_tactile_sensor_data(Finger.PALM)
        
        if palm_raw:
            raw_data = palm_raw.data
            if raw_data and len(raw_data) > 0:
                print(f"Palm Raw Data:")
                print(f"  Data points: {len(raw_data)}")
                print(f"  Sum: {sum(raw_data)} g")
                print(f"  Average: {sum(raw_data) / len(raw_data):.2f} g")
                print(f"  All values ({len(raw_data)} points):")
                # Print all data, multiple values per line for readability
                for i in range(0, len(raw_data), 16):  # 16 values per line
                    chunk = raw_data[i:i+16]
                    values_str = ', '.join(f"{v:3d}" for v in chunk)
                    print(f"    [{i:3d}-{i+len(chunk)-1:3d}]: {values_str}")
                print()
        
        if palm_downsampled:
            print(f"Palm Downsampled Data:")
            print(f"  Data points: {len(palm_downsampled)}")
            print(f"  Sum: {sum(palm_downsampled)} g")
            print(f"  Average: {sum(palm_downsampled) / len(palm_downsampled):.2f} g")
            print(f"  All values ({len(palm_downsampled)} points):")
            # Print all downsampled data
            for i in range(0, len(palm_downsampled), 16):  # 16 values per line
                chunk = palm_downsampled[i:i+16]
                values_str = ', '.join(f"{v:3d}" for v in chunk)
                print(f"    [{i:3d}-{i+len(chunk)-1:3d}]: {values_str}")
            print()
            
            if palm_raw and len(palm_raw.data) > 0:
                print(f"Resolution difference:")
                print(f"  Raw: {len(palm_raw.data)} points")
                print(f"  Downsampled: {len(palm_downsampled)} points")
                print(f"  Ratio: {len(palm_raw.data) / len(palm_downsampled):.2f}x")
    except Exception as e:
        print(f"Error comparing Palm data: {e}\n")
    
    # Compare Dorsum
    try:
        print("\n--- Dorsum Comparison ---")
        dorsum_raw = hand.get_tactile_sensor_data_raw(Finger.DORSUM)
        dorsum_downsampled = hand.get_tactile_sensor_data(Finger.DORSUM)
        
        if dorsum_raw:
            raw_data = dorsum_raw.data
            if raw_data and len(raw_data) > 0:
                print(f"Dorsum Raw Data:")
                print(f"  Data points: {len(raw_data)}")
                print(f"  Sum: {sum(raw_data)} g")
                print(f"  Average: {sum(raw_data) / len(raw_data):.2f} g")
                print(f"  All values ({len(raw_data)} points):")
                # Print all data, multiple values per line for readability
                for i in range(0, len(raw_data), 16):  # 16 values per line
                    chunk = raw_data[i:i+16]
                    values_str = ', '.join(f"{v:3d}" for v in chunk)
                    print(f"    [{i:3d}-{i+len(chunk)-1:3d}]: {values_str}")
                print()
        
        if dorsum_downsampled:
            print(f"Dorsum Downsampled Data:")
            print(f"  Data points: {len(dorsum_downsampled)}")
            print(f"  Sum: {sum(dorsum_downsampled)} g")
            print(f"  Average: {sum(dorsum_downsampled) / len(dorsum_downsampled):.2f} g")
            print(f"  All values ({len(dorsum_downsampled)} points):")
            # Print all downsampled data
            for i in range(0, len(dorsum_downsampled), 16):  # 16 values per line
                chunk = dorsum_downsampled[i:i+16]
                values_str = ', '.join(f"{v:3d}" for v in chunk)
                print(f"    [{i:3d}-{i+len(chunk)-1:3d}]: {values_str}")
            print()
            
            if dorsum_raw and len(dorsum_raw.data) > 0:
                print(f"Resolution difference:")
                print(f"  Raw: {len(dorsum_raw.data)} points")
                print(f"  Downsampled: {len(dorsum_downsampled)} points")
                print(f"  Ratio: {len(dorsum_raw.data) / len(dorsum_downsampled):.2f}x")
    except Exception as e:
        print(f"Error comparing Dorsum data: {e}\n")
    
    print("\nNote: Fingers (Thumb, Index, Middle, Ring, Little) have the same")
    print("      resolution in both raw and downsampled data, so no comparison needed.")
    
    print("=" * 60)
    print("[Done]: Tactile sensor raw data demo completed!")
    print("=" * 60)

if __name__ == "__main__":
    main()
