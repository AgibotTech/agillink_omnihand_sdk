#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node (motor_vel_pub) (Omnihand2025Pro)
"""发布左手电机速度命令"""
        msg = MotorVel()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "left_hand_frame"
        
        import math
        self.velocity_counter += 1

        # Omnihand2025Pro has 12 joints
        msg.vels = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

        self.left_motor_vel_cmd_publisher.publish(msg)
        
        self.get_logger().debug(
            f'Published left motor velocity command: {msg.vels}'
        )

def main(args=None):
    rclpy.init(args=args)
    
    left_motor_vel_publisher = LeftMotorVelPublisher()
    
    try:
        rclpy.spin(left_motor_vel_publisher)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            left_motor_vel_publisher.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            left_motor_vel_publisher.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
