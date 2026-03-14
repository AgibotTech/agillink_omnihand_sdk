# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 100Hz Control + 10Hz Tactile Sensor Reliability Test

This demo tests the reliability of:
1. 100Hz position control using set_all_joint_positions() with return value
2. 10Hz tactile sensor data acquisition (downsampled from 100Hz, every 10th iteration)

Control loop: 10ms period (100Hz)
Sensor query: every 10th control loop (10Hz)
"""

from omnihand import OmniHand2025, Finger, HandType
import time
from collections import defaultdict

def main():
    # Configuration parameters
    total_iterations = 10000
    control_period_ms = 10  # 100Hz control loop (10ms period)
    sensor_downsample = 10  # Query sensor every 10th iteration (10Hz)
    frame_recv_timeout_ms = 10

    print("=" * 60)
    print("OmniHand 2025 100Hz Control + 10Hz Sensor Test")
    print("=" * 60)
    print()

    import argparse
    parser = argparse.ArgumentParser(description='OmniHand 2025 100Hz Control + 10Hz Tactile Sensor Reliability Test')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance: left hand, channel 0 based on device type
    print("Creating hand instance (Left hand, channel 0)...")
    try:
        if args.device == 'hcan':
            hand = OmniHand2025.create_hand_by_hcan(
                hand_type=HandType.LEFT,
                hand_device_id=1,
                canfd_device_id=0,
                canfd_channel_id=0
            )
        else:  # default: zlgcan
            hand = OmniHand2025.create_hand_by_zlgcan(
                hand_type=HandType.LEFT,
                hand_device_id=1,
                canfd_device_id=0,
                canfd_channel_id=0
            )
    except Exception as e:
        print(f"Failed to create hand: {e}")
        return
    
    # Initialize hand
    print("Initializing hand...")
    if not hand.init():
        print("Failed to initialize hand")
        return
    
    # Set frame recv timeout
    hand.set_frame_recv_timeout(frame_recv_timeout_ms)
    
    print("Hand initialized successfully")
    print()
    
    # Define all 7 sensor positions with expected data lengths
    sensor_positions = [
        (Finger.THUMB, "Thumb", 16),
        (Finger.INDEX, "Index", 18),
        (Finger.MIDDLE, "Middle", 18),
        (Finger.RING, "Ring", 18),
        (Finger.LITTLE, "Little", 18),
        (Finger.PALM, "Palm", 78),
        (Finger.DORSUM, "Dorsum", 102),
    ]
    
    # Counters
    set_pos_success_count = 0
    set_pos_failure_count = 0
    sensor_success_count = 0
    sensor_failure_count = 0
    sensor_data_failures = defaultdict(int)  # Per-sensor failures
    
    # Timing statistics
    set_pos_times = []
    sensor_times = []
    loop_times = []
    slow_loop_count = 0  # Loops exceeding control_period_ms
    
    print(f"Starting reliability test:")
    print(f"  - Total iterations: {total_iterations}")
    print(f"  - Control period: {control_period_ms}ms (100Hz)")
    print(f"  - Sensor downsample: 1/{sensor_downsample} (10Hz)")
    print(f"  - Frame recv timeout: {frame_recv_timeout_ms}ms")
    print()
    
    # Initial position (neutral)
    target_positions = [2048, 2048, 4096, 2048, 4096, 4096, 2048, 4096, 2048, 4096]
    
    start_time = time.time()
    last_update_time = start_time
    
    try:
        for iteration in range(total_iterations):
            loop_start_time = time.time()
            
            # ========== 100Hz Position Control ==========
            set_pos_start = time.time()
            try:
                # set_all_joint_positions now returns actual positions
                actual_positions = hand.set_all_joint_positions(target_positions)
                set_pos_end = time.time()
                set_pos_times.append((set_pos_end - set_pos_start) * 1000.0)
                
                if len(actual_positions) == 10:
                    set_pos_success_count += 1
                else:
                    set_pos_failure_count += 1
            except Exception as e:
                set_pos_end = time.time()
                set_pos_times.append((set_pos_end - set_pos_start) * 1000.0)
                set_pos_failure_count += 1
                if iteration < 5:
                    print(f"\n[Iter {iteration}] set_all_joint_positions error: {e}")
            
            # ========== 10Hz Sensor Query (every 10th iteration) ==========
            if iteration % sensor_downsample == 0:
                sensor_start = time.time()
                all_valid = True
                
                # Query each sensor using non-raw interface
                for finger, name, _ in sensor_positions:
                    try:
                        sensor_data = hand.get_tactile_sensor_data(finger)
                        # get_tactile_sensor_data returns processed data (vector of uint8)
                        if sensor_data is None or len(sensor_data) == 0:
                            sensor_data_failures[name] += 1
                            all_valid = False
                    except Exception as e:
                        sensor_data_failures[name] += 1
                        all_valid = False
                        if iteration < 50:
                            print(f"\n[Iter {iteration}] get_tactile_sensor_data({name}) error: {e}")
                
                sensor_end = time.time()
                sensor_times.append((sensor_end - sensor_start) * 1000.0)
                
                if all_valid:
                    sensor_success_count += 1
                else:
                    sensor_failure_count += 1
            
            # Calculate loop time
            loop_end_time = time.time()
            loop_duration_ms = (loop_end_time - loop_start_time) * 1000.0
            loop_times.append(loop_duration_ms)
            
            if loop_duration_ms > control_period_ms:
                slow_loop_count += 1
            
            # Sleep to maintain 100Hz (if loop was faster than period)
            sleep_time = (control_period_ms / 1000.0) - (loop_end_time - loop_start_time)
            if sleep_time > 0:
                time.sleep(sleep_time)
            
            # Progress update
            current_time = time.time()
            if (iteration + 1) % 100 == 0 or current_time - last_update_time >= 1.0:
                elapsed = current_time - start_time
                progress_pct = (iteration + 1) * 100.0 / total_iterations
                eta = elapsed / (iteration + 1) * (total_iterations - iteration - 1) if iteration > 0 else 0
                
                bar_len = 50
                filled = int(bar_len * progress_pct / 100.0)
                bar = '=' * filled + '-' * (bar_len - filled)
                
                print(f"\r[{bar}] {iteration + 1}/{total_iterations} ({progress_pct:.1f}%) | "
                      f"Elapsed: {elapsed:.1f}s | ETA: {eta:.1f}s | "
                      f"SetPos fail: {set_pos_failure_count} | Sensor fail: {sensor_failure_count}", 
                      end='', flush=True)
                last_update_time = current_time
    
    except KeyboardInterrupt:
        print("\n\nTest interrupted by user")
    
    print()
    elapsed_time = time.time() - start_time
    
    # ========== Print Statistics ==========
    print()
    print("=" * 60)
    print("Test Results")
    print("=" * 60)
    print(f"Total iterations: {total_iterations}")
    print(f"Total time: {elapsed_time:.2f}s")
    print(f"Actual loop rate: {total_iterations / elapsed_time:.1f} Hz")
    print()
    
    # Position control statistics
    print("Position Control Statistics (100Hz target):")
    print("-" * 60)
    total_set = set_pos_success_count + set_pos_failure_count
    set_fail_rate = (set_pos_failure_count / total_set * 100.0) if total_set > 0 else 0
    print(f"  Success: {set_pos_success_count}")
    print(f"  Failure: {set_pos_failure_count} ({set_fail_rate:.2f}%)")
    if set_pos_times:
        print(f"  Timing: avg={sum(set_pos_times)/len(set_pos_times):.2f}ms, "
              f"min={min(set_pos_times):.2f}ms, max={max(set_pos_times):.2f}ms")
    print()
    
    # Sensor statistics
    total_sensor_queries = total_iterations // sensor_downsample
    print(f"Sensor Query Statistics (10Hz target, {total_sensor_queries} queries):")
    print("-" * 60)
    total_sensor = sensor_success_count + sensor_failure_count
    sensor_fail_rate = (sensor_failure_count / total_sensor * 100.0) if total_sensor > 0 else 0
    print(f"  Success: {sensor_success_count}")
    print(f"  Failure: {sensor_failure_count} ({sensor_fail_rate:.2f}%)")
    if sensor_times:
        print(f"  Timing: avg={sum(sensor_times)/len(sensor_times):.2f}ms, "
              f"min={min(sensor_times):.2f}ms, max={max(sensor_times):.2f}ms")
    print()
    
    # Per-sensor failures
    if any(sensor_data_failures.values()):
        print("Per-Sensor Failures:")
        for _, name, _ in sensor_positions:
            if sensor_data_failures[name] > 0:
                print(f"  {name}: {sensor_data_failures[name]}")
        print()
    
    # Loop timing statistics
    print("Loop Timing Statistics:")
    print("-" * 60)
    if loop_times:
        avg_loop = sum(loop_times) / len(loop_times)
        print(f"  Average loop time: {avg_loop:.2f}ms")
        print(f"  Min loop time: {min(loop_times):.2f}ms")
        print(f"  Max loop time: {max(loop_times):.2f}ms")
        slow_rate = (slow_loop_count / len(loop_times) * 100.0)
        print(f"  Loops exceeding {control_period_ms}ms: {slow_loop_count} ({slow_rate:.2f}%)")
    print()
    
    # Summary
    print("Summary:")
    print("-" * 60)
    if set_pos_failure_count == 0 and sensor_failure_count == 0:
        print("  ✓ Perfect reliability! No failures.")
    elif set_fail_rate < 0.1 and sensor_fail_rate < 0.1:
        print("  ✓ Excellent reliability (< 0.1% failure rate)")
    elif set_fail_rate < 1.0 and sensor_fail_rate < 1.0:
        print("  ⚠ Good reliability (< 1% failure rate)")
    else:
        print("  ✗ Needs investigation (>= 1% failure rate)")
    print()

if __name__ == "__main__":
    main()
