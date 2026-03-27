#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node (motor_pos_pub) (Omnihand2025Pro)
"""发布左手电机位置命令"""
        msg = MotorPos()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "left_hand_frame"
        
        import math
        self.position_counter += 1

        # Omnihand2025Pro has 12 joints
        msg.pos = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094, 2000, 2000]
        self.left_motor_pos_cmd_publisher.publish(msg)
        time.sleep(0.2)

        msg.pos = [2000, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094, 2000, 2000]
        self.left_motor_pos_cmd_publisher.publish(msg)
        time.sleep(0.2)

        msg.pos = [500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094, 2000, 2000]
        self.left_motor_pos_cmd_publisher.publish(msg)
        time.sleep(0.2)

        msg.pos = [1500, 2081, 4094, 2029, 4094, 4094, 2048, 4094, 4000, 4094, 2000, 2000]
        self.left_motor_pos_cmd_publisher.publish(msg)

        self.get_logger().debug(
            f'Published left motor position command: {msg.pos}'
        )

def main(args=None):
    rclpy.init(args=args)
    
    left_motor_pos_publisher = LeftMotorPosPublisher()
    
    try:
        rclpy.spin(left_motor_pos_publisher)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            left_motor_pos_publisher.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            left_motor_pos_publisher.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
