#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node for control mode (Omnihand2025Pro)
"""
        初始化控制模式节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_control_mode_node')
        
        self.hand_side = hand_side
        
        # 创建控制模式反馈发布器 (1Hz)
        self.control_mode_fb_publisher = self.create_publisher(
            ControlMode,
            f'/omnihand/omnihand_pro_2025/{hand_side}/control_mode_fb',
            10
        )
        
        # 创建控制模式命令订阅器
        self.control_mode_subscriber = self.create_subscription(
            ControlMode,
            f'/omnihand/omnihand_pro_2025/{hand_side}/control_mode',
            self.control_mode_callback,
            10
        )
        
        # 存储当前控制模式
        self.current_control_mode = ControlMode()
        
        self.get_logger().info(f'{hand_side.capitalize()} Control Mode Node started')

    def control_mode_callback(self, msg):
        """处理控制模式命令消息"""
        self.get_logger().info(
            f'Received control mode command for {self.hand_side} hand: mode={msg.modes}'
        )
        
        # 更新当前控制模式
        self.current_control_mode = msg

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    control_mode_node = ControlModeNode(hand_side)
    
    try:
        rclpy.spin(control_mode_node)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            control_mode_node.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            control_mode_node.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
