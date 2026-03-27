#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node for current threshold (Omnihand2025Pro)
"""
        初始化电流阈值节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_current_threshold_node')

        self.hand_side = hand_side

        # 创建电流阈值反馈发布器 (1Hz)
        self.current_threshold_fb_publisher = self.create_publisher(
            CurrentThreshold,
            f'/omnihand/omnihand_pro_2025/{hand_side}/current_threshold_cmd',
            10
        )

        # 创建电流阈值命令订阅器
        self.current_threshold_subscriber = self.create_subscription(
            CurrentThreshold,
            f'/omnihand/omnihand_pro_2025/{hand_side}/current_threshold',
            self.current_threshold_callback,
            10
        )

        # 存储当前电流阈值
        self.current_threshold = CurrentThreshold()

        self.get_logger().info(f'{hand_side.capitalize()} Current Threshold Node started')
        self.get_logger().info(f'Publishing to: /omnihand/omnihand_pro_2025/{hand_side}/current_threshold')
        self.get_logger().info(f'Subscribing to: /omnihand/omnihand_pro_2025/{hand_side}/current_threshold')

    def current_threshold_callback(self, msg):
        """处理电流阈值命令消息"""
        
        threshold_str = ', '.join([f'{t:.2f}' for t in msg.current_thresholds])
        self.get_logger().info(
            f'Received current threshold command for {self.hand_side} hand: [{threshold_str}]'
        )

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    current_threshold_node = CurrentThresholdNode(hand_side)
    
    try:
        rclpy.spin(current_threshold_node)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            current_threshold_node.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            current_threshold_node.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
