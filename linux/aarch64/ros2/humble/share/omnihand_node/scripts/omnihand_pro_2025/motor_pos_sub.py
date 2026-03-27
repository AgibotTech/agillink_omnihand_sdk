#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node (motor_pos_sub) (Omnihand2025Pro)
"""处理接收到的MotorPos消息"""
        self.get_logger().info(
            f'Received motor positions: {msg.pos}, '
            f'frame_id: {msg.header.frame_id}, '
            f'timestamp: {msg.header.stamp.sec}.{msg.header.stamp.nanosec}'
        )

def main(args=None):
    rclpy.init(args=args)
    
    motor_pos_subscriber = MotorPosSubscriber()
    
    try:
        rclpy.spin(motor_pos_subscriber)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            motor_pos_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            motor_pos_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
