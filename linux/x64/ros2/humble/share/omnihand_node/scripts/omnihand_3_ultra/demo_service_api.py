#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-01-XX
@Description: Demo script showing how to use the simplified Service API for OmniHand 3 Ultra (O20)
              (set_joint_angles, get_joint_angles)
"""

import sys
import os

try:
    from omnihand_3_ultra_node_msgs.srv import SetJointAngles, GetJointAngles
except ImportError:
    print("Error: Cannot import omnihand_3_ultra_node_msgs.srv")
    print("")
    print("Please make sure you have sourced the ROS2 setup script:")
    print("  source ros2/humble/setup.bash")
    print("  # or")
    print("  source ros2/setup.bash")
    print("")
    print("Current PYTHONPATH:", os.environ.get('PYTHONPATH', 'Not set'))
    print("")
    sys.exit(1)

import rclpy
from rclpy.node import Node
import time


class HandServiceDemo(Node):
    
    def __init__(self, hand_side='left'):
        super().__init__('hand_service_demo')
        
        self.hand_side = hand_side
        self.topic_prefix = f'/omnihand/omnihand_3_ultra/{hand_side}'
        
        self.set_joint_angles_client = self.create_client(
            SetJointAngles,
            f'{self.topic_prefix}/set_joint_angles'
        )
        
        self.get_joint_angles_client = self.create_client(
            GetJointAngles,
            f'{self.topic_prefix}/get_joint_angles'
        )
        
        self.get_logger().info(f'{hand_side.capitalize()} Hand Service Demo Node started (O20)')
        self.get_logger().info('Waiting for services to be available...')
    
    def wait_for_services(self, timeout=5.0):
        services_ready = (
            self.set_joint_angles_client.wait_for_service(timeout_sec=timeout) and
            self.get_joint_angles_client.wait_for_service(timeout_sec=timeout)
        )
        
        if not services_ready:
            self.get_logger().error('Services not available. Make sure the hand node is running.')
            return False
        
        self.get_logger().info('All services are ready!')
        return True
    
    def get_joint_angles(self):
        self.get_logger().info('=' * 60)
        self.get_logger().info('Getting joint angles...')
        self.get_logger().info('=' * 60)
        
        request = GetJointAngles.Request()
        future = self.get_joint_angles_client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        
        response = future.result()
        
        if response.is_ready:
            self.get_logger().info('Hand is ready')
            self.get_logger().info(f'   Joint angles (rad): {[f"{a:.3f}" for a in response.angles]}')
            self.get_logger().info(f'   Joint angles (deg): {[f"{a*180/3.14159:.2f}" for a in response.angles]}')
        else:
            self.get_logger().warn(f'Hand not ready: {response.error_message}')
        
        return response
    
    def set_joint_angles(self, target_angles, timeout=5.0):
        self.get_logger().info('=' * 60)
        self.get_logger().info('Setting joint angles...')
        self.get_logger().info('=' * 60)
        self.get_logger().info(f'   Target angles (rad): {[f"{a:.3f}" for a in target_angles]}')
        self.get_logger().info(f'   Target angles (deg): {[f"{a*180/3.14159:.2f}" for a in target_angles]}')
        self.get_logger().info(f'   Timeout: {timeout}s')
        
        request = SetJointAngles.Request()
        request.target_angles = target_angles
        request.timeout = timeout
        
        future = self.set_joint_angles_client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        
        response = future.result()
        
        if response.success:
            self.get_logger().info('Joint angles set successfully!')
            self.get_logger().info(f'   Final angles (rad): {[f"{a:.3f}" for a in response.final_angles]}')
            self.get_logger().info(f'   Final angles (deg): {[f"{a*180/3.14159:.2f}" for a in response.final_angles]}')
            return True
        else:
            self.get_logger().error(f'Failed to set joint angles: {response.error_message}')
            if response.final_angles:
                self.get_logger().info(f'   Final angles: {[f"{a:.3f}" for a in response.final_angles]}')
            return False
    
    def run_demo(self):
        self.get_logger().info('')
        self.get_logger().info('=' * 60)
        self.get_logger().info('OmniHand 3 Ultra (O20) Service API Demo')
        self.get_logger().info('=' * 60)
        self.get_logger().info('')
        self.get_logger().info('Note: Hand is automatically initialized when node starts')
        self.get_logger().info('      Configuration is set via parameters/config file')
        self.get_logger().info('      O20 has 20 active joints')
        self.get_logger().info('')
        
        if not self.wait_for_services():
            return
        
        initial_angles = self.get_joint_angles()
        if not initial_angles or not initial_angles.is_ready:
            self.get_logger().error('Hand not ready. Please check:')
            self.get_logger().error('  1. Hardware connection')
            self.get_logger().error('  2. Node parameters/config file')
            self.get_logger().error('  3. Device permissions')
            return
        
        self.get_logger().info('')
        time.sleep(1.0)
        
        self.get_logger().info('')
        self.get_logger().info('Moving to position 1: Open hand (all angles = 0)')
        import math
        target1 = [0.0] * len(initial_angles.angles)
        if self.set_joint_angles(target1, timeout=3.0):
            time.sleep(1.0)
            self.get_joint_angles()
        
        time.sleep(2.0)
        
        self.get_logger().info('')
        self.get_logger().info('Moving to position 2: Half grasp (~1 degree per joint)')
        target2 = [math.radians(1.0)] * len(initial_angles.angles)
        if self.set_joint_angles(target2, timeout=3.0):
            time.sleep(1.0)
            self.get_joint_angles()
        
        time.sleep(2.0)
        
        self.get_logger().info('')
        self.get_logger().info('Moving to position 3: Full grasp (~30 degrees per joint)')
        target3 = [math.radians(30.0)] * len(initial_angles.angles)
        if self.set_joint_angles(target3, timeout=3.0):
            time.sleep(1.0)
            self.get_joint_angles()
        
        time.sleep(2.0)
        
        if initial_angles and initial_angles.angles:
            self.get_logger().info('')
            self.get_logger().info('Moving back to initial angles')
            if self.set_joint_angles(initial_angles.angles, timeout=3.0):
                time.sleep(1.0)
                self.get_joint_angles()
        
        self.get_logger().info('')
        self.get_logger().info('=' * 60)
        self.get_logger().info('Demo completed!')
        self.get_logger().info('=' * 60)


def main(args=None):
    rclpy.init(args=args)
    
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1].lower()
        if hand_side not in ['left', 'right']:
            print(f"Error: hand_side must be 'left' or 'right', got '{hand_side}'")
            sys.exit(1)
    
    demo = HandServiceDemo(hand_side)
    
    try:
        demo.run_demo()
    except KeyboardInterrupt:
        demo.get_logger().info('Demo interrupted by user')
    except Exception as e:
        demo.get_logger().error(f'Error during demo: {str(e)}')
        import traceback
        traceback.print_exc()
    finally:
        try:
            demo.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass


if __name__ == '__main__':
    main()
