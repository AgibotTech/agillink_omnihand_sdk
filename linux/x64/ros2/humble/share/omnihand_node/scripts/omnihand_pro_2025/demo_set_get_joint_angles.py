#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-01-XX
@Description: Demo script for setting and getting joint angles using SetJointAngles and GetJointAngles services (OmniHand Pro 2025)
"""

import sys
import os
import math

# Check if ROS2 environment is properly sourced
try:
    from omnihand_pro_2025_node_msgs.srv import SetJointAngles, GetJointAngles
except ImportError:
    print("Error: Cannot import omnihand_pro_2025_node_msgs.srv")
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


class SetGetJointAnglesDemo(Node):
    """Demo for setting and getting joint angles"""
    
    def __init__(self, hand_side='left'):
        """
        初始化演示节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__('set_get_joint_angles_demo_o12')
        
        self.hand_side = hand_side
        self.topic_prefix = f'/omnihand/omnihand_pro_2025/{hand_side}'
        
        # 创建Service客户端
        self.set_joint_angles_client = self.create_client(
            SetJointAngles,
            f'{self.topic_prefix}/set_joint_angles'
        )
        
        self.get_joint_angles_client = self.create_client(
            GetJointAngles,
            f'{self.topic_prefix}/get_joint_angles'
        )
        
        self.get_logger().info(f'{hand_side.capitalize()} Hand Set/Get JointAngles Demo started (O12)')
        self.get_logger().info('Waiting for services to be available...')
    
    def wait_for_services(self, timeout=5.0):
        """等待所有Service可用"""
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
        """
        获取手部关节角度
        
        Returns:
            GetJointAngles.Response: 角度响应对象，如果失败返回None
        """
        self.get_logger().info('=' * 60)
        self.get_logger().info('Getting joint angles...')
        self.get_logger().info('=' * 60)
        
        request = GetJointAngles.Request()
        future = self.get_joint_angles_client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        
        response = future.result()
        
        if response.is_ready:
            self.get_logger().info('✅ Hand is ready')
            self.get_logger().info(f'   Joint angles (rad): {[f"{a:.3f}" for a in response.angles]}')
            self.get_logger().info(f'   Joint angles (deg): {[f"{a*180/3.14159:.2f}" for a in response.angles]}')
            return response
        else:
            self.get_logger().warn(f'⚠️  Hand not ready: {response.error_message}')
            return None
    
    def set_joint_angles(self, target_angles, timeout=5.0):
        """
        设置手部关节角度
        
        Args:
            target_angles: 目标关节角度列表（弧度），O12有12个关节
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
    
    demo = SetGetJointAnglesDemo(hand_side)
    
    try:
        if not demo.wait_for_services():
            sys.exit(1)
        
        # Step 1: 获取当前关节角度
        demo.get_logger().info('')
        demo.get_logger().info('Step 1: Get current joint angles')
        current_angles = demo.get_joint_angles()
        
        if not current_angles or not current_angles.is_ready:
            demo.get_logger().error('Hand not ready. Please check hardware connection and node configuration.')
            sys.exit(1)
        
        time.sleep(1.0)
        
        # Step 2: 设置关节角度（第一次：张开手，所有角度为0）
        demo.get_logger().info('')
        demo.get_logger().info('Step 2: Set joint angles (first time: open hand, all angles = 0)')
        # O12有12个关节
        target_angles_1 = [0.0] * 12
        success = demo.set_joint_angles(target_angles_1, timeout=5.0)
        
        if not success:
            demo.get_logger().error('Failed to set joint angles')
            sys.exit(1)
        
        time.sleep(1.0)
        
        # Step 3: 获取关节角度验证第一次设置
        demo.get_logger().info('')
        demo.get_logger().info('Step 3: Get joint angles to verify first setting')
        demo.get_joint_angles()
        
        time.sleep(1.0)
        
        # Step 4: 设置关节角度（第二次：半握，每个关节约1度）
        demo.get_logger().info('')
        demo.get_logger().info('Step 4: Set joint angles (second time: half grasp, ~1 degree per joint)')
        target_angles_2 = [math.radians(1.0)] * 12  # 1度转换为弧度
        success = demo.set_joint_angles(target_angles_2, timeout=5.0)
        
        if not success:
            demo.get_logger().error('Failed to set joint angles')
            sys.exit(1)
        
        time.sleep(1.0)
        
        # Step 5: 获取关节角度验证第二次设置
        demo.get_logger().info('')
        demo.get_logger().info('Step 5: Get joint angles to verify second setting')
        demo.get_joint_angles()
        
        demo.get_logger().info('')
        demo.get_logger().info('=' * 60)
        demo.get_logger().info('Demo completed successfully!')
        demo.get_logger().info('=' * 60)
            
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
