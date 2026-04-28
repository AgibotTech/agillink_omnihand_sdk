#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node (mix_control_pub) (Omnihand2025)
"""
        初始化混合控制发布节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_mix_control_publisher')
        
        self.hand_side = hand_side
        
        # 创建混合控制命令发布器
        self.mix_control_publisher = self.create_publisher(
            MixControl,
            f'/omnihand/omnihand_2025/{hand_side}/mix_control_cmd',
            10
        )

        self.timer = self.create_timer(1, self.timer_callback)
        self.get_logger().info(f'{hand_side.capitalize()} Mix Control Publisher Node started')
        self.get_logger().info(f'Publishing to: /omnihand/omnihand_2025/{hand_side}/mix_control_cmd')

    def publish_mix_control(self):
        """
        发布混合控制命令
        """
        msg = MixControl()
        mix_controls_str = ', '.join([f'{t}' for t in msg.mix_controls])
        self.get_logger().info(
            f'Publish {self.hand_side} hand: [{mix_controls_str}]'
        )

    def timer_callback(self):
        """定时器回调"""
        self.publish_mix_control()

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    mix_control_publisher = MixControlPublisher(hand_side)

    try:
        rclpy.spin(mix_control_publisher)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            mix_control_publisher.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            mix_control_publisher.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
