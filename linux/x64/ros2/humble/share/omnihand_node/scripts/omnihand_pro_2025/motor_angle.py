#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node for motor angle (Omnihand2025Pro)
"""
        初始化电机角度节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_motor_angle_node')
        
        self.hand_side = hand_side
        
        # 创建电机角度反馈发布器 (10Hz)
        self.motor_angle_fb_publisher = self.create_publisher(
            MotorAngle,
            f'/omnihand/omnihand_pro_2025/{hand_side}/motor_angle_cmd',
            10
        )
        
        # 创建电机角度命令订阅器
        self.motor_angle_subscriber = self.create_subscription(
            MotorAngle,
            f'/omnihand/omnihand_pro_2025/{hand_side}/motor_angle',
            self.motor_angle_callback,
            100
        )
        
        # 存储当前电机角度
        self.current_motor_angle = MotorAngle()

        self.get_logger().info(f'{hand_side.capitalize()} Motor Angle Node started')
        self.get_logger().info(f'Publishing to: /omnihand/omnihand_pro_2025/{hand_side}/motor_angle_cmd')
        self.get_logger().info(f'Subscribing to: /omnihand/omnihand_pro_2025/{hand_side}/motor_angle')

    def motor_angle_callback(self, msg):
        """处理电机角度命令消息"""
        angles_str = ', '.join([f'{t}' for t in msg.angles])
        self.get_logger().info(
            f'Receive {self.hand_side} hand: [{angles_str}]'
        )

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    motor_angle_node = MotorAngleNode(hand_side)
    
    try:
        rclpy.spin(motor_angle_node)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            motor_angle_node.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            motor_angle_node.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
