#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-01-XX
@Description: Demo script for setting joint angles using SetJointAngles service (OmniHand 2025)
"""

import sys
import os
import math

# Check if ROS2 environment is properly sourced
try:
    from omnihand_2025_node_msgs.srv import SetJointAngles
except ImportError:
    print("Error: Cannot import omnihand_2025_node_msgs.srv")
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


class SetJointAnglesDemo(Node):
    """Demo for setting joint angles"""
    
    def __init__(self, hand_side='left'):
        """
        初始化演示节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__('set_joint_angles_demo')
        
        self.hand_side = hand_side
        self.topic_prefix = f'/omnihand/omnihand_2025/{hand_side}'
        
        # 创建Service客户端
        self.set_joint_angles_client = self.create_client(
            SetJointAngles,
            f'{self.topic_prefix}/set_joint_angles'
        )
        
        self.get_logger().info(f'{hand_side.capitalize()} Hand SetJointAngles Demo started')
        self.get_logger().info('Waiting for service to be available...')
    
    def wait_for_service(self, timeout=5.0):
        """等待Service可用"""
        if not self.set_joint_angles_client.wait_for_service(timeout_sec=timeout):
            self.get_logger().error('Service not available. Make sure the hand node is running.')
            return False
        
        self.get_logger().info('Service is ready!')
        return True
    
    def set_joint_angles(self, target_angles, timeout=5.0):
        """
        设置手部关节角度
        
        Args:
            target_angles: 目标关节角度列表（弧度），O10有10个关节
            timeout: 超时时间（秒）
        
        Returns:
            bool: 是否成功
        """
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
            self.get_logger().info('✅ Joint angles set successfully!')
            self.get_logger().info(f'   Final angles (rad): {[f"{a:.3f}" for a in response.final_angles]}')
            self.get_logger().info(f'   Final angles (deg): {[f"{a*180/3.14159:.2f}" for a in response.final_angles]}')
            return True
        else:
            self.get_logger().error(f'❌ Failed to set joint angles: {response.error_message}')
            if response.final_angles:
                self.get_logger().info(f'   Final angles: {[f"{a:.3f}" for a in response.final_angles]}')
            return False


def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1].lower()
        if hand_side not in ['left', 'right']:
            print(f"Error: hand_side must be 'left' or 'right', got '{hand_side}'")
            sys.exit(1)
    
    demo = SetJointAnglesDemo(hand_side)
    
    try:
        if not demo.wait_for_service():
            sys.exit(1)
        
        # 示例：设置所有关节角度为0（张开手）
        # O10有10个关节
        target_angles = [0.0] * 10
        
        demo.get_logger().info('')
        demo.get_logger().info('Example: Setting all joints to 0 (open hand)')
        success = demo.set_joint_angles(target_angles, timeout=5.0)
        
        if success:
            demo.get_logger().info('')
            demo.get_logger().info('=' * 60)
            demo.get_logger().info('Demo completed successfully!')
            demo.get_logger().info('=' * 60)
        else:
            self.get_logger().error('Failed to set joint angles')
            sys.exit(1)
            
    except KeyboardInterrupt:
        demo.get_logger().info('Demo interrupted by user')
    except Exception as e:
        demo.get_logger().error(f'Error during demo: {str(e)}')
        import traceback
        traceback.print_exc()
        sys.exit(1)
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
