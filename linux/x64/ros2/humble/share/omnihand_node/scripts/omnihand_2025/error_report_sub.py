#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node (error_report_sub) (Omnihand2025)
"""
        初始化电机错误报告订阅节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_motor_error_report_subscriber')
        
        self.hand_side = hand_side
        
        # 创建电机错误报告反馈订阅器 (1Hz)
        self.motor_error_report_fb_subscriber = self.create_subscription(
            MotorErrorReport,
            f'/omnihand/omnihand_2025/{hand_side}/motor_error_report',
            self.motor_error_report_callback,
            10
        )
        
        self.get_logger().info(f'{hand_side.capitalize()} Motor Error Report Subscriber Node started')
        self.get_logger().info(f'Subscribing to: /omnihand/omnihand_2025/{hand_side}/motor_error_report')

    def motor_error_report_callback(self, msg):
        error_reports_str = ', '.join([f'{t}' for t in msg.error_reports])
        self.get_logger().info(
            f'Received motor error report for {self.hand_side} hand: [{error_reports_str}]'
        )

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    motor_error_report_subscriber = MotorErrorReportSubscriber(hand_side)
    
    try:
        rclpy.spin(motor_error_report_subscriber)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            motor_error_report_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            motor_error_report_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
