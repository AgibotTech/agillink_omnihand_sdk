#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node for tactile sensor (Omnihand2025Pro)
"""
        初始化触觉传感器订阅节点
        
        Args:
            hand_side: 'left' 或 'right'，表示左手或右手
        """
        super().__init__(f'{hand_side}_tactile_sensor_subscriber')
        
        self.hand_side = hand_side

        # 创建触觉传感器订阅器 (10Hz)
        self.tactile_sensor_subscriber = self.create_subscription(
            TactileSensor,
            f'/omnihand/omnihand_pro_2025/{hand_side}/tactile_sensor',
            self.tactile_sensor_feedback_callback,
            10
        )

        self.get_logger().info(f'{hand_side.capitalize()} Tactile Sensor Subscriber Node started')
        self.get_logger().info(f'Subscribing to: /omnihand/omnihand_pro_2025/{hand_side}/tactile_sensor')

    def tactile_sensor_feedback_callback(self, msg):
        """处理触觉传感器反馈消息"""
        for i, sensor_data in enumerate(msg.tactile_datas):
            self.get_logger().info(
                f'Sensor {i}: online={sensor_data.online_state}, '
                f'normal_force={sensor_data.normal_force}, '
                f'tangent_force={sensor_data.tangent_force}'
            )

def main(args=None):
    rclpy.init(args=args)
    
    # 可以通过命令行参数指定左手或右手，默认为左手
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    tactile_sensor_subscriber = TactileSensorSubscriber(hand_side)
    
    try:
        rclpy.spin(tactile_sensor_subscriber)
    except KeyboardInterrupt:
        # Don't use logger here, context may be shutting down
        pass
    except Exception as e:
        try:
            tactile_sensor_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        # Safely destroy node and shutdown
        try:
            tactile_sensor_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass  # Context may already be shut down

if __name__ == '__main__':
    main()
