#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node (motor_vel_sub) (Omnihand2025)
"""处理电机速度反馈消息"""
        self.get_logger().info(
            f'Received left motor velocity feedback: {msg.vels}'
        )

def main(args=None):
    rclpy.init(args=args)
    
    left_motor_vel_subscriber = LeftMotorVelSubscriber()
    
    try:
        rclpy.spin(left_motor_vel_subscriber)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            left_motor_vel_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            left_motor_vel_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
