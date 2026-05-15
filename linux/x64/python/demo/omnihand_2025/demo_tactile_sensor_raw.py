# Copyright (c) 2025, Agibot Co., Ltd.
# AGILINK OmniHand SDK is licensed under Mulan PSL v2.

"""
Read raw tactile sensor data (full resolution) for OmniHand 2025 (O10).

Raw data provides full resolution sensor readings, different from the downsampled data
returned by get_tactile_sensor_data(). Includes individual/all sensor reading,
continuous monitoring with statistics, and raw vs downsampled comparison.

Supports multiple connection types: ZLG CANFD, HCAN, RS485, ZLG TCP, USB CDC serial, SocketCAN (Linux only).
Run with -h or --help to see all available options and usage examples.
"""

import argparse
import time
from omnihand import OmniHand2025, Finger, HandType

EXAMPLES = """\
examples:
  # ZLG CANFD, right hand
  python demo_tactile_sensor_raw.py -d zlgcan -t right --canfd-device-id 0 --canfd-channel-id 0

  # HCAN, device 0 channel 1
  python demo_tactile_sensor_raw.py -d hcan --canfd-device-id 0 --canfd-channel-id 1

  # SocketCAN (Linux only)
  python demo_tactile_sensor_raw.py -d socketcan --can-interface can0

  # RS485, specify serial port (e.g. COM3 or /dev/ttyUSB0)
  python demo_tactile_sensor_raw.py -d rs485 --uart-port COM3

  # USB CDC serial (e.g. COM3 or /dev/ttyACM0)
  python demo_tactile_sensor_raw.py -d usb --uart-port COM3
"""

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

    parser = argparse.ArgumentParser(
        description='Read raw tactile sensor data for OmniHand 2025',
        epilog=EXAMPLES,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp', 'rs485', 'usb'], default='zlgcan',
                        help='Device type: zlgcan, hcan, zlgcan_tcp, rs485, usb (CDC serial), socketcan (Linux only), default: zlgcan')
    parser.add_argument('-t', '--hand-type', choices=['left', 'right'], default='left',
                        help='Hand type: left or right, default: left')
    parser.add_argument('--hand-id', type=int, default=None,
                        help='Hand device ID, default: kDefaultHandDeviceId')
    parser.add_argument('--canfd-device-id', type=int, default=0,
                        help='CANFD device index, default: 0')
    parser.add_argument('--canfd-channel-id', type=int, default=0,
                        help='CANFD channel index, default: 0')
    parser.add_argument('--can-interface', type=str, default='can0',
                        help='SocketCAN interface (Linux only), default: can0')
    parser.add_argument('--uart-port', type=str, default='COM6',
                        help='Serial port for rs485/usb mode, default: COM6')
    parser.add_argument('--host', type=str, default='192.168.0.178',
                        help='ZLG CAN TCP host address, default: 192.168.0.178')
    parser.add_argument('--port', type=int, default=8000,
                        help='ZLG CAN TCP port, default: 8000')
    args = parser.parse_args()

    hand_type = HandType.LEFT if args.hand_type == 'left' else HandType.RIGHT
    hand_device_id = args.hand_id if args.hand_id is not None else OmniHand2025.kDefaultHandDeviceId

    if args.device == 'hcan':
        hand = OmniHand2025.create_hand_by_hcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
        )
    elif args.device == 'rs485':
        hand = OmniHand2025.create_hand_by_rs485(
            hand_type=hand_type,
            uart_port=args.uart_port
        )
    elif args.device == 'zlgcan_tcp':
        hand = OmniHand2025.create_hand_by_zlgcan_tcp(
            hand_type=hand_type,
            host=args.host,
            port=args.port
        )
    elif args.device == 'usb':
        hand = OmniHand2025.create_hand_by_usb(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            uart_port=args.uart_port
        )
    elif args.device == 'socketcan':
        hand = OmniHand2025.create_hand_socketcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            can_interface=args.can_interface
        )
    else:  # default: zlgcan
        hand = OmniHand2025.create_hand_by_zlgcan(
            hand_type=hand_type,
            hand_device_id=hand_device_id,
            canfd_device_id=args.canfd_device_id,
            canfd_channel_id=args.canfd_channel_id
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
