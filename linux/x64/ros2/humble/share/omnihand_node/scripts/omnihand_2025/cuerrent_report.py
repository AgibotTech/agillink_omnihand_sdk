#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node for current report (Omnihand2025)
"""
        初始化电流报告订阅节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_current_report_subscriber')
        
        self.hand_side = hand_side
        
        # 创建电流报告反馈订阅器 (1Hz)
        self.current_report_fb_subscriber = self.create_subscription(
            CurrentReport,
            f'/omnihand/omnihand_2025/{hand_side}/current_report',
            self.current_report_feedback_callback,
            10
        )

        self.get_logger().info(f'{hand_side.capitalize()} Current Report Subscriber Node started')
        self.get_logger().info(f'Subscribing to: /omnihand/omnihand_2025/{hand_side}/current_report')

    def current_report_feedback_callback(self, msg):
        """处理电流报告反馈消息"""
        self.get_logger().info(
            f'Current report: [{msg.current_reports}]'
        )

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    current_report_subscriber = CurrentReportSubscriber(hand_side)
    
    try:
        rclpy.spin(current_report_subscriber)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            current_report_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            current_report_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
