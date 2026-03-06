# Copyright (c) 2025, Agibot Co., Ltd.
# OmniHand 2025 SDK is licensed under Mulan PSL v2.

"""
OmniHand 3 Lite S (O4) Position Control Reliability Test

This demo tests the reliability of position control by:
1. Creating a left hand on channel 0
2. Setting all 4 joint positions
3. Getting all 4 joint positions
4. Repeating every 3ms for 10000 iterations
5. Counting data failures or losses

This helps identify communication stability and data integrity issues.
Note: O4 does not support tactile sensors, so this test only covers position control.
"""

from omnihand import OmniHand3Lite, HandType
import time
from collections import defaultdict

def main():
    # Configuration parameters
    total_iterations = 10000
    interval_ms = 3  # Request interval in milliseconds
    frame_recv_timeout_ms = 30

    print("=" * 60)
    print("OmniHand 3 Lite S (O4) Position Control Reliability Test")
    print("=" * 60)
    print()

    import argparse
    parser = argparse.ArgumentParser(description='OmniHand 3 Lite S (O4) Set/Get Position Reliability Test')
    parser.add_argument('-d', '--device', choices=['zlgcan', 'hcan'], default='zlgcan',
                        help='CAN device type: zlgcan (ZLG USB CANFD) or hcan (HCAN USB CANFD), default: zlgcan')
    args = parser.parse_args()
    
    # Create hand instance: left hand, channel 0 based on device type
    print("Creating hand instance (Left hand, channel 0)...")
    try:
        if args.device == 'hcan':
            hand = OmniHand3Lite.create_hand_by_hcan(
                hand_type=HandType.LEFT,
                hand_device_id=1,
                canfd_device_id=0,
                canfd_channel_id=0
            )
        else:  # default: zlgcan
            hand = OmniHand3Lite.create_hand_by_zlgcan(
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
    
    # Set request interval to control CAN bus communication rate
    # This ensures minimum interval between requests at the C++ level
    hand.set_request_interval(interval_ms)
    hand.set_frame_recv_timeout(frame_recv_timeout_ms)
    
    print("Hand initialized successfully")
    print()
    
    # O4 has 4 joints
    num_joints = 4
    
    # Counters for failures per joint
    failure_count = defaultdict(int)  # {joint_index: count}
    total_failures = 0
    
    # Track data validity
    valid_count = defaultdict(int)  # {joint_index: count}
    
    # Track timing statistics
    slow_query_count = 0  # Count of queries that exceed 33ms
    query_times = []  # Store all query times for statistics
    
    # Track position control timing statistics
    set_position_times = []  # Store all set_position operation times
    get_position_times = []  # Store all get_position operation times
    total_operation_times = []  # Store total time for both operations (set position + get position)
    
    # Track success/failure for each operation
    set_position_success_count = 0
    set_position_failure_count = 0
    get_position_success_count = 0
    get_position_failure_count = 0
    
    print(f"Starting reliability test:")
    print(f"  - Total iterations: {total_iterations}")
    print(f"  - Request Interval: {interval_ms}ms")
    print(f"  - Frame reception timeout: {frame_recv_timeout_ms}ms")
    print(f"  - Joints: {num_joints} (O4 has 4 DOF)")
    print()
    
    start_time = time.time()
    last_update_time = start_time
    
    try:
        # Initialize positions for position control (4 positions for O4)
        # Set to a neutral position (2048 is middle position for most motors)
        initial_positions = [2048] * num_joints
        
        for iteration in range(total_iterations):
            # Start timing for all operations combined
            total_operation_start_time = time.time()
            
            # Set all joint positions
            try:
                set_position_start_time = time.time()
                actual_positions = hand.set_all_joint_positions(initial_positions)
                set_position_end_time = time.time()
                set_position_duration_ms = (set_position_end_time - set_position_start_time) * 1000.0
                set_position_times.append(set_position_duration_ms)
                set_position_success_count += 1
                
                # Check if we got valid response (should have 4 positions)
                if len(actual_positions) != num_joints:
                    set_position_failure_count += 1
                    if iteration < 10:  # Only print first few errors to avoid spam
                        print(f"\n[Iteration {iteration}] Warning: set_all_joint_positions returned {len(actual_positions)} positions, expected {num_joints}")
            except Exception as e:
                set_position_failure_count += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error setting all joint positions: {e}")
            
            # Get all joint positions
            all_positions = None
            try:
                get_position_start_time = time.time()
                all_positions = hand.get_all_joint_positions()
                get_position_end_time = time.time()
                get_position_duration_ms = (get_position_end_time - get_position_start_time) * 1000.0
                get_position_times.append(get_position_duration_ms)
                get_position_success_count += 1
            except Exception as e:
                # Exception during data acquisition - still record query time
                get_position_end_time = time.time()
                get_position_duration_ms = (get_position_end_time - get_position_start_time) * 1000.0
                get_position_times.append(get_position_duration_ms)
                get_position_failure_count += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error getting all joint positions: {e}")
            
            # End timing for all operations combined
            total_operation_end_time = time.time()
            total_operation_duration_ms = (total_operation_end_time - total_operation_start_time) * 1000.0
            total_operation_times.append(total_operation_duration_ms)
            
            # Check if total operation time exceeded 33ms
            if total_operation_duration_ms > 33.0:
                slow_query_count += 1
            
            # Process position data (only if we successfully got it)
            try:
                # Check each joint position
                if all_positions is None:
                    # No data received - count all joints as failed
                    for joint_idx in range(1, num_joints + 1):
                        failure_count[joint_idx] += 1
                        total_failures += 1
                elif len(all_positions) != num_joints:
                    # Incomplete data (wrong number of positions)
                    for joint_idx in range(1, num_joints + 1):
                        failure_count[joint_idx] += 1
                        total_failures += 1
                    if iteration < 10:
                        print(f"\n[Warning]: Incomplete data at iteration {iteration}. "
                              f"Expected {num_joints} positions, got {len(all_positions)}")
                else:
                    # Valid data with correct number of positions
                    for joint_idx in range(1, num_joints + 1):
                        position = all_positions[joint_idx - 1]
                        if position is None or position < 0:
                            # Invalid position value
                            failure_count[joint_idx] += 1
                            total_failures += 1
                        else:
                            # Valid position
                            valid_count[joint_idx] += 1
            except Exception as e:
                # Exception during position data processing - count all joints as failed
                for joint_idx in range(1, num_joints + 1):
                    failure_count[joint_idx] += 1
                    total_failures += 1
                if iteration < 10:  # Only print first few errors to avoid spam
                    print(f"\n[Iteration {iteration}] Error processing position data: {e}")
            
            # Note: No need to sleep here - request interval is controlled by set_request_interval()
            # Each operation will automatically enforce the interval at C++ level
            
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
    
    # ========== Joint Statistics (Per-Joint + Position Timing) ==========
    print("Per-Joint Statistics:")
    print("-" * 60)
    print(f"{'Joint':<12} {'Valid':<10} {'Failures':<10} {'Failure Rate':<12}")
    print("-" * 60)
    
    for joint_idx in range(1, num_joints + 1):
        valid = valid_count[joint_idx]
        failures = failure_count[joint_idx]
        total_queries = valid + failures
        failure_rate = (failures / total_queries * 100.0) if total_queries > 0 else 0.0
        
        print(f"Joint {joint_idx:<8} {valid:<10} {failures:<10} {failure_rate:>6.2f}%")
    
    print("-" * 60)
    print()
    
    # Position control timing statistics
    if set_position_times:
        avg_set_position_time = sum(set_position_times) / len(set_position_times)
        max_set_position_time = max(set_position_times)
        min_set_position_time = min(set_position_times)
        
        print("Position Control Timing Statistics:")
        print(f"  - Set all joint positions:")
        print(f"    * Total operations: {len(set_position_times)}")
        print(f"    * Average time: {avg_set_position_time:.2f}ms")
        print(f"    * Min time: {min_set_position_time:.2f}ms")
        print(f"    * Max time: {max_set_position_time:.2f}ms")
    
    if get_position_times:
        avg_get_position_time = sum(get_position_times) / len(get_position_times)
        max_get_position_time = max(get_position_times)
        min_get_position_time = min(get_position_times)
        
        print(f"  - Get all joint positions:")
        print(f"    * Total operations: {len(get_position_times)}")
        print(f"    * Average time: {avg_get_position_time:.2f}ms")
        print(f"    * Min time: {min_get_position_time:.2f}ms")
        print(f"    * Max time: {max_get_position_time:.2f}ms")
        print()
    
    # ========== Total Statistics ==========
    # Operation reliability statistics
    print("Operation Reliability Statistics:")
    print("-" * 60)
    print(f"{'Operation':<30} {'Success':<10} {'Failures':<10} {'Failure Rate':<12}")
    print("-" * 60)
    
    # Set position operation
    set_position_total = set_position_success_count + set_position_failure_count
    set_position_failure_rate = (set_position_failure_count / set_position_total * 100.0) if set_position_total > 0 else 0.0
    print(f"{'Set All Joint Positions':<30} {set_position_success_count:<10} {set_position_failure_count:<10} {set_position_failure_rate:>6.2f}%")
    
    # Get position operation
    get_position_total = get_position_success_count + get_position_failure_count
    get_position_failure_rate = (get_position_failure_count / get_position_total * 100.0) if get_position_total > 0 else 0.0
    print(f"{'Get All Joint Positions':<30} {get_position_success_count:<10} {get_position_failure_count:<10} {get_position_failure_rate:>6.2f}%")
    
    print("-" * 60)
    print()
    
    # Total operation timing statistics (set position + get position)
    if total_operation_times:
        avg_total_time = sum(total_operation_times) / len(total_operation_times)
        max_total_time = max(total_operation_times)
        min_total_time = min(total_operation_times)
        slow_query_rate = (slow_query_count / len(total_operation_times) * 100.0) if total_operation_times else 0.0
        
        print("Total Operation Timing Statistics (set position + get position):")
        print(f"  - Total operations: {len(total_operation_times)}")
        print(f"  - Average total time: {avg_total_time:.2f}ms")
        print(f"  - Min total time: {min_total_time:.2f}ms")
        print(f"  - Max total time: {max_total_time:.2f}ms")
        print(f"  - Operations exceeding 33ms: {slow_query_count} ({slow_query_rate:.2f}%)")
        print()
    
    # Position summary
    total_valid = sum(valid_count.values())
    total_position_data_points = total_iterations * num_joints  # Total position data points checked
    total_queries = total_iterations  # Actual query count (one set+get per iteration)
    overall_failure_rate = (total_failures / total_position_data_points * 100.0) if total_position_data_points > 0 else 0.0
    
    print("Position Summary:")
    print(f"  - Total queries: {total_queries}")
    print(f"  - Total position data points checked: {total_position_data_points}")
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
