# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 2025 Tactile Sensor Raw Data Reliability Test

This demo tests the reliability of raw tactile sensor data acquisition by:
1. Creating a left hand on channel 0
2. Querying raw data for all 7 sensor positions (Thumb, Index, Middle, Ring, Little, Palm, Dorsum)
3. Repeating every 10ms for 5000 iterations
4. Counting data failures or losses

This helps identify communication stability and data integrity issues.
"""

from omnihand import OmniHand2025, EFinger, EHandType
import time
from collections import defaultdict

def main():
    # Configuration parameters
    total_iterations = 10000
    interval_ms = 3  # Request interval in milliseconds (0 = no limit, since we use getAllTactileSensorDataRaw which is a single request)
    frame_recv_timeout_ms = 30

    print("=" * 60)
    print("OmniHand 2025 Tactile Sensor Raw Data Reliability Test")
    print("=" * 60)
    print()

    # Create hand instance: left hand, channel 0
    print("Creating hand instance (Left hand, channel 0)...")
    try:
        hand = OmniHand2025.create_hand_by_zlgcan(
            hand_type=EHandType.LEFT,
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
    
    # Set request interval to control CAN bus communication rate
    # This ensures minimum interval between requests at the C++ level
    hand.set_request_interval(interval_ms)
    hand.set_frame_recv_timeout(frame_recv_timeout_ms)
    
    print("Hand initialized successfully")
    print()
    
    # Define all 7 sensor positions with expected data lengths
    # Expected lengths: Thumb=16, Index=18, Middle=18, Ring=18, Little=18, Palm=78, Dorsum=102
    sensor_positions = [
        (EFinger.THUMB, "Thumb", 16),
        (EFinger.INDEX, "Index", 18),
        (EFinger.MIDDLE, "Middle", 18),
        (EFinger.RING, "Ring", 18),
        (EFinger.LITTLE, "Little", 18),
        (EFinger.PALM, "Palm", 78),
        (EFinger.DORSUM, "Dorsum", 102),
    ]
    
    # Counters for failures per sensor
    failure_count = defaultdict(int)  # {sensor_name: count}
    total_failures = 0
    
    # Track data validity
    valid_count = defaultdict(int)  # {sensor_name: count}
    
    # Track timing statistics
    slow_query_count = 0  # Count of queries that exceed 33ms
    query_times = []  # Store all query times for statistics
    
    # Track angle control timing statistics
    set_angle_times = []  # Store all set_angle operation times
    get_angle_times = []  # Store all get_angle operation times
    total_operation_times = []  # Store total time for all three operations (set angle + get angle + get sensor)
    
    # Track success/failure for each operation
    set_angle_success_count = 0
    set_angle_failure_count = 0
    get_angle_success_count = 0
    get_angle_failure_count = 0
    get_sensor_success_count = 0
    get_sensor_failure_count = 0
    
    print(f"Starting reliability test:")
    print(f"  - Total iterations: {total_iterations}")
    print(f"  - Request Interval: {interval_ms}ms")
    print(f"  - Frame reception timeout: {frame_recv_timeout_ms}ms")
    print(f"  - Sensors: {len(sensor_positions)} positions")
    print()
    
    start_time = time.time()
    last_update_time = start_time
    
    try:
        # Initialize angles for angle control (10 angles for O10)
        # Set to a neutral position (all zeros or small initial values)
        initial_angles = [0.0] * 10
        
        for iteration in range(total_iterations):
            # Start timing for all three operations combined
            total_operation_start_time = time.time()
            
            # Set all active joint angles
            try:
                set_angle_start_time = time.time()
                hand.set_all_active_joint_angles(initial_angles)
                set_angle_end_time = time.time()
                set_angle_duration_ms = (set_angle_end_time - set_angle_start_time) * 1000.0
                set_angle_times.append(set_angle_duration_ms)
                set_angle_success_count += 1
            except Exception as e:
                set_angle_failure_count += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error setting all active joint angles: {e}")
            
            # Get all active joint angles
            try:
                get_angle_start_time = time.time()
                all_active_angles = hand.get_all_active_joint_angles()
                get_angle_end_time = time.time()
                get_angle_duration_ms = (get_angle_end_time - get_angle_start_time) * 1000.0
                get_angle_times.append(get_angle_duration_ms)
                get_angle_success_count += 1
            except Exception as e:
                get_angle_failure_count += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error getting all active joint angles: {e}")
            
            # Use getAllTactileSensorDataRaw() to get all 7 sensors in one request
            # This is much faster than requesting each sensor individually
            all_sensor_data = None
            try:
                query_start_time = time.time()
                all_sensor_data = hand.get_all_tactile_sensor_data_raw()
                query_end_time = time.time()
                query_duration_ms = (query_end_time - query_start_time) * 1000.0
                query_times.append(query_duration_ms)
                get_sensor_success_count += 1
            except Exception as e:
                # Exception during data acquisition - still record query time
                query_end_time = time.time()
                query_duration_ms = (query_end_time - query_start_time) * 1000.0
                query_times.append(query_duration_ms)
                get_sensor_failure_count += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error getting all sensor data: {e}")
            
            # End timing for all three operations combined
            total_operation_end_time = time.time()
            total_operation_duration_ms = (total_operation_end_time - total_operation_start_time) * 1000.0
            total_operation_times.append(total_operation_duration_ms)
            
            # Check if total operation time exceeded 33ms
            if total_operation_duration_ms > 33.0:
                slow_query_count += 1
            
            # Process sensor data (only if we successfully got it)
            try:
                # Create a map for quick lookup: sensor_id -> sensor_data
                # Note: sensor_id is EFinger enum type, not int
                sensor_data_map = {}
                if all_sensor_data is not None:
                    for sensor_data in all_sensor_data:
                        if hasattr(sensor_data, 'sensor_id'):
                            sensor_data_map[sensor_data.sensor_id] = sensor_data
                
                # Check each sensor position
                for finger, sensor_name, expected_length in sensor_positions:
                    # Use finger enum directly as key (not int(finger))
                    if finger in sensor_data_map:
                        sensor_data = sensor_data_map[finger]
                        
                        # Check if data is valid
                        if sensor_data is None:
                            failure_count[sensor_name] += 1
                            total_failures += 1
                        elif not hasattr(sensor_data, 'data'):
                            failure_count[sensor_name] += 1
                            total_failures += 1
                        elif sensor_data.data is None or len(sensor_data.data) == 0:
                            failure_count[sensor_name] += 1
                            total_failures += 1
                        elif len(sensor_data.data) < expected_length:
                            # Incomplete data (data length mismatch)
                            failure_count[sensor_name] += 1
                            total_failures += 1
                            # Only print first few warnings to avoid spam
                            if iteration < 10:
                                print(f"\n[Warning]: Incomplete data for {sensor_name} at iteration {iteration}. "
                                      f"Expected {expected_length} bytes, got {len(sensor_data.data)}")
                        else:
                            # Valid data with correct length
                            valid_count[sensor_name] += 1
                    else:
                        # Sensor data not found in response
                        failure_count[sensor_name] += 1
                        total_failures += 1
                        if iteration < 10:
                            print(f"\n[Warning]: Sensor {sensor_name} not found in response at iteration {iteration}")
            except Exception as e:
                # Exception during sensor data processing - count all sensors as failed
                for finger, sensor_name, expected_length in sensor_positions:
                    failure_count[sensor_name] += 1
                    total_failures += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error processing sensor data: {e}")
            
            # Note: No need to sleep here - request interval is controlled by set_request_interval()
            # Each get_tactile_sensor_data_raw() call will automatically enforce the interval at C++ level
            
            # Progress update every 100 iterations or every 1 second
            current_time = time.time()
            should_update = False
            if (iteration + 1) % 100 == 0:
                should_update = True
            elif current_time - last_update_time >= 1.0:
                should_update = True
            
            if should_update:
                elapsed = current_time - start_time
                progress_pct = (iteration + 1) * 100.0 / total_iterations
                avg_time_per_iter = elapsed / (iteration + 1) if iteration > 0 else 0
                remaining_iterations = total_iterations - (iteration + 1)
                estimated_remaining = avg_time_per_iter * remaining_iterations
                
                # Create progress bar (50 characters)
                bar_length = 50
                filled = int(bar_length * progress_pct / 100.0)
                bar = '=' * filled + '-' * (bar_length - filled)
                
                # Print progress with carriage return to overwrite same line
                print(f"\r[{bar}] {iteration + 1}/{total_iterations} ({progress_pct:.1f}%) | "
                      f"Elapsed: {elapsed:.1f}s | "
                      f"ETA: {estimated_remaining:.1f}s | "
                      f"Failures: {total_failures}", end='', flush=True)
                
                last_update_time = current_time
    
    except KeyboardInterrupt:
        print("\n\nTest interrupted by user")
    
    # Print newline to move to next line after progress bar
    print()
    
    elapsed_time = time.time() - start_time
    
    # Print statistics
    print()
    print("=" * 60)
    print("Test Results")
    print("=" * 60)
    print(f"Total iterations: {total_iterations}")
    print(f"Total time: {elapsed_time:.2f}s")
    print(f"Average iteration time: {elapsed_time / total_iterations * 1000:.2f}ms")
    print()
    
    # ========== Sensor Statistics (Per-Sensor + Sensor Timing) ==========
    print("Per-Sensor Statistics:")
    print("-" * 60)
    print(f"{'Sensor':<12} {'Valid':<10} {'Failures':<10} {'Failure Rate':<12}")
    print("-" * 60)
    
    for finger, sensor_name, expected_length in sensor_positions:
        valid = valid_count[sensor_name]
        failures = failure_count[sensor_name]
        total_queries = valid + failures
        failure_rate = (failures / total_queries * 100.0) if total_queries > 0 else 0.0
        
        print(f"{sensor_name:<12} {valid:<10} {failures:<10} {failure_rate:>6.2f}%")
    
    print("-" * 60)
    print()
    
    # Sensor timing statistics
    if query_times:
        avg_query_time = sum(query_times) / len(query_times)
        max_query_time = max(query_times)
        min_query_time = min(query_times)
        
        print("Sensor Timing Statistics (per query of 7 sensors):")
        print(f"  - Total queries: {len(query_times)}")
        print(f"  - Average query time: {avg_query_time:.2f}ms")
        print(f"  - Min query time: {min_query_time:.2f}ms")
        print(f"  - Max query time: {max_query_time:.2f}ms")
        print()
    
    # Angle control timing statistics
    if set_angle_times:
        avg_set_angle_time = sum(set_angle_times) / len(set_angle_times)
        max_set_angle_time = max(set_angle_times)
        min_set_angle_time = min(set_angle_times)
        
        print("Angle Control Timing Statistics:")
        print(f"  - Set all active joint angles:")
        print(f"    * Total operations: {len(set_angle_times)}")
        print(f"    * Average time: {avg_set_angle_time:.2f}ms")
        print(f"    * Min time: {min_set_angle_time:.2f}ms")
        print(f"    * Max time: {max_set_angle_time:.2f}ms")
    
    if get_angle_times:
        avg_get_angle_time = sum(get_angle_times) / len(get_angle_times)
        max_get_angle_time = max(get_angle_times)
        min_get_angle_time = min(get_angle_times)
        
        print(f"  - Get all active joint angles:")
        print(f"    * Total operations: {len(get_angle_times)}")
        print(f"    * Average time: {avg_get_angle_time:.2f}ms")
        print(f"    * Min time: {min_get_angle_time:.2f}ms")
        print(f"    * Max time: {max_get_angle_time:.2f}ms")
        print()
    
    # ========== Total Statistics ==========
    # Operation reliability statistics
    print("Operation Reliability Statistics:")
    print("-" * 60)
    print(f"{'Operation':<30} {'Success':<10} {'Failures':<10} {'Failure Rate':<12}")
    print("-" * 60)
    
    # Set angle operation
    set_angle_total = set_angle_success_count + set_angle_failure_count
    set_angle_failure_rate = (set_angle_failure_count / set_angle_total * 100.0) if set_angle_total > 0 else 0.0
    print(f"{'Set All Active Joint Angles':<30} {set_angle_success_count:<10} {set_angle_failure_count:<10} {set_angle_failure_rate:>6.2f}%")
    
    # Get angle operation
    get_angle_total = get_angle_success_count + get_angle_failure_count
    get_angle_failure_rate = (get_angle_failure_count / get_angle_total * 100.0) if get_angle_total > 0 else 0.0
    print(f"{'Get All Active Joint Angles':<30} {get_angle_success_count:<10} {get_angle_failure_count:<10} {get_angle_failure_rate:>6.2f}%")
    
    # Get sensor operation
    get_sensor_total = get_sensor_success_count + get_sensor_failure_count
    get_sensor_failure_rate = (get_sensor_failure_count / get_sensor_total * 100.0) if get_sensor_total > 0 else 0.0
    print(f"{'Get All Tactile Sensor Data':<30} {get_sensor_success_count:<10} {get_sensor_failure_count:<10} {get_sensor_failure_rate:>6.2f}%")
    
    print("-" * 60)
    print()
    
    # Total operation timing statistics (set angle + get angle + get sensor)
    if total_operation_times:
        avg_total_time = sum(total_operation_times) / len(total_operation_times)
        max_total_time = max(total_operation_times)
        min_total_time = min(total_operation_times)
        slow_query_rate = (slow_query_count / len(total_operation_times) * 100.0) if total_operation_times else 0.0
        
        print("Total Operation Timing Statistics (set angle + get angle + get sensor):")
        print(f"  - Total operations: {len(total_operation_times)}")
        print(f"  - Average total time: {avg_total_time:.2f}ms")
        print(f"  - Min total time: {min_total_time:.2f}ms")
        print(f"  - Max total time: {max_total_time:.2f}ms")
        print(f"  - Operations exceeding 33ms: {slow_query_count} ({slow_query_rate:.2f}%)")
        print()
    
    # Sensor summary
    total_valid = sum(valid_count.values())
    # Note: We use get_all_tactile_sensor_data_raw() which is ONE query per iteration
    # So total_queries = total_iterations (not total_iterations * len(sensor_positions))
    total_sensor_data_points = total_iterations * len(sensor_positions)  # Total sensor data points checked
    total_queries = total_iterations  # Actual query count (one get_all_tactile_sensor_data_raw() per iteration)
    overall_failure_rate = (total_failures / total_sensor_data_points * 100.0) if total_sensor_data_points > 0 else 0.0
    
    print("Sensor Summary:")
    print(f"  - Total queries: {total_queries}")
    print(f"  - Total sensor data points checked: {total_sensor_data_points}")
    print(f"  - Successful data points: {total_valid} ({100.0 - overall_failure_rate:.2f}%)")
    print(f"  - Failed data points: {total_failures} ({overall_failure_rate:.2f}%)")
    
    if total_failures == 0:
        print("  ✓ All queries successful! Perfect reliability.")
    elif overall_failure_rate < 0.1:
        print("  ✓ Excellent reliability (< 0.1% failure rate)")
    elif overall_failure_rate < 1.0:
        print("  ⚠ Good reliability (< 1% failure rate)")
    elif overall_failure_rate < 5.0:
        print("  ⚠ Moderate reliability (< 5% failure rate)")
    else:
        print("  ✗ Poor reliability (>= 5% failure rate)")
    
    print()

if __name__ == "__main__":
    main()
