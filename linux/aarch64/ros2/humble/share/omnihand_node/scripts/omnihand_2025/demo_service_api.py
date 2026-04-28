#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-01-XX
@Description: Demo script showing how to use the simplified Service API
              (set_joint_angles, get_joint_angles)
"""

import sys
import os

# Check if ROS2 environment is properly sourced
try:
    from omnihand_2025_node_msgs.srv import SetJointAngles, GetJointAngles
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
import time


class HandServiceDemo(Node):
    """Demo class showing how to use the simplified Service API"""
    
    def __init__(self, hand_side='left'):
        """
        初始化演示节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__('hand_service_demo')
        
        self.hand_side = hand_side
        self.topic_prefix = f'/omnihand/omnihand_2025/{hand_side}'
        
        # 创建Service客户端
        self.set_joint_angles_client = self.create_client(
            SetJointAngles,
            f'{self.topic_prefix}/set_joint_angles'
        )
        
        self.get_joint_angles_client = self.create_client(
            GetJointAngles,
            f'{self.topic_prefix}/get_joint_angles'
        )
        
        self.get_logger().info(f'{hand_side.capitalize()} Hand Service Demo Node started')
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
        else:
            self.get_logger().warn(f'⚠️  Hand not ready: {response.error_message}')
        
        return response
    
    def set_joint_angles(self, target_angles, timeout=5.0):
        """
        设置手部关节角度
        
        Args:
            target_angles: 目标关节角度列表（弧度）
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
    
    def run_demo(self):
        """运行完整演示"""
        self.get_logger().info('')
        self.get_logger().info('=' * 60)
        self.get_logger().info('OmniHand Service API Demo')
        self.get_logger().info('=' * 60)
        self.get_logger().info('')
        self.get_logger().info('Note: Hand is automatically initialized when node starts')
        self.get_logger().info('      Configuration is set via parameters/config file')
        self.get_logger().info('')
        
        # 等待服务可用
        if not self.wait_for_services():
            return
        
        # Step 1: 获取初始角度（验证连接）
        initial_angles = self.get_joint_angles()
        if not initial_angles or not initial_angles.is_ready:
            self.get_logger().error('Hand not ready. Please check:')
            self.get_logger().error('  1. Hardware connection')
            self.get_logger().error('  2. Node parameters/config file')
            self.get_logger().error('  3. Device permissions')
            return
        
        self.get_logger().info('')
        time.sleep(1.0)
        
        # Step 2: 设置角度 - 示例1：张开手（所有关节角度为0）
        self.get_logger().info('')
        self.get_logger().info('Moving to position 1: Open hand (all angles = 0)')
        import math
        target1 = [0.0] * len(initial_angles.angles)  # 张开手，所有角度为0
        if self.set_joint_angles(target1, timeout=3.0):
            time.sleep(1.0)
            self.get_joint_angles()
        
        time.sleep(2.0)
        
        # Step 3: 设置角度 - 示例2：半握（每个关节约1度）
        # O10 各关节最大角度（弧度）: [1.12, 0.05, 0.8416, 0, 1.48, 1.48, 0.17, 1.48, 0.19, 1.48]
        # 转换为度数: [64.1°, 2.9°, 48.2°, 0°, 84.8°, 84.8°, 9.7°, 84.8°, 10.9°, 84.8°]
        # 1度对所有关节都在有效范围内
        self.get_logger().info('')
        self.get_logger().info('Moving to position 2: Half grasp (~1 degree per joint)')
        target2 = [math.radians(1.0)] * len(initial_angles.angles)  # 1度转换为弧度
        if self.set_joint_angles(target2, timeout=3.0):
            time.sleep(1.0)
            self.get_joint_angles()
        
        time.sleep(2.0)
        
        # Step 4: 设置角度 - 示例3：握紧（每个关节约30度）
        # ⚠️ 注意：30度对某些关节可能超出角度范围！
        # O10 各关节最大角度（弧度）: [1.12, 0.05, 0.8416, 0, 1.48, 1.48, 0.17, 1.48, 0.19, 1.48]
        # 转换为度数: [64.1°, 2.9°, 48.2°, 0°, 84.8°, 84.8°, 9.7°, 84.8°, 10.9°, 84.8°]
        # 关节1最大只有2.9°，关节3为0°（不能动），关节6最大9.7°，关节8最大10.9°
        # 因此30度对这些关节来说是非法角度，可能导致超时或失败
        self.get_logger().info('')
        self.get_logger().info('Moving to position 3: Full grasp (~30 degrees per joint)')
        target3 = [math.radians(30.0)] * len(initial_angles.angles)  # 30度转换为弧度（⚠️ 非法：超出部分关节范围）
        if self.set_joint_angles(target3, timeout=3.0):
            time.sleep(1.0)
            self.get_joint_angles()
        
        time.sleep(2.0)
        
        # Step 5: 回到初始角度
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
    
    # 可以通过命令行参数指定左手或右手，默认为左手
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
