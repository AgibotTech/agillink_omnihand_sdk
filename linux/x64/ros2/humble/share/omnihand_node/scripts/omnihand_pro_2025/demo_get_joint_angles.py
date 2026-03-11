#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-01-XX
@Description: Demo script for getting joint angles using GetJointAngles service (OmniHand Pro 2025)
"""

import sys
import os

# Check if ROS2 environment is properly sourced
try:
    from omnihand_pro_2025_node_msgs.srv import GetJointAngles
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


class GetJointAnglesDemo(Node):
    """Demo for getting joint angles"""
    
    def __init__(self, hand_side='left'):
        """
        初始化演示节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__('get_joint_angles_demo_o12')
        
        self.hand_side = hand_side
        self.topic_prefix = f'/omnihand/omnihand_pro_2025/{hand_side}'
        
        # 创建Service客户端
        self.get_joint_angles_client = self.create_client(
            GetJointAngles,
            f'{self.topic_prefix}/get_joint_angles'
        )
        
        self.get_logger().info(f'{hand_side.capitalize()} Hand GetJointAngles Demo started (O12)')
        self.get_logger().info('Waiting for service to be available...')
    
    def wait_for_service(self, timeout=5.0):
        """等待Service可用"""
        if not self.get_joint_angles_client.wait_for_service(timeout_sec=timeout):
            self.get_logger().error('Service not available. Make sure the hand node is running.')
            return False
        
        self.get_logger().info('Service is ready!')
        return True
    
    def get_joint_angles(self):
        """获取手部关节角度"""
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


def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1].lower()
        if hand_side not in ['left', 'right']:
            print(f"Error: hand_side must be 'left' or 'right', got '{hand_side}'")
            sys.exit(1)
    
    demo = GetJointAnglesDemo(hand_side)
    
    try:
        if not demo.wait_for_service():
            sys.exit(1)
        
        # 获取关节角度
        result = demo.get_joint_angles()
        
        if result:
            demo.get_logger().info('')
            demo.get_logger().info('=' * 60)
            demo.get_logger().info('Demo completed successfully!')
            demo.get_logger().info('=' * 60)
        else:
            demo.get_logger().error('Failed to get joint angles')
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
