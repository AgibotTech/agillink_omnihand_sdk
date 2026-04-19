#!/usr/bin/env python3
"""
@Author: huangshiheng@agibot.com
@Date: 2025-11-06
@Description: Python node for tactile sensor (OmniHand3Ultra O20)
"""

import sys
import os

try:
    from omnihand_3_ultra_node_msgs.msg import TactileSensor
except ImportError:
    print("Error: Cannot import omnihand_3_ultra_node_msgs.msg")
    print("")
    print("Please make sure you have sourced the ROS2 setup script:")
    print("  source ros2/humble/setup.bash")
    print("  # or")
    print("  source ros2/setup.bash")
    print("")
    print("Current PYTHONPATH:", os.environ.get('PYTHONPATH', 'Not set'))
    print("")
    sys.exit(1)

import rclpy
from rclpy.node import Node


class TactileSensorSubscriber(Node):
    def __init__(self, hand_side='left'):
        super().__init__(f'{hand_side}_tactile_sensor_subscriber')
        
        self.hand_side = hand_side

        self.tactile_sensor_subscriber = self.create_subscription(
            TactileSensor,
            f'/omnihand/omnihand_3_ultra/{hand_side}/tactile_sensor',
            self.tactile_sensor_feedback_callback,
            10
        )

        self.get_logger().info(f'{hand_side.capitalize()} Tactile Sensor Subscriber Node started (O20)')
        self.get_logger().info(f'Subscribing to: /omnihand/omnihand_3_ultra/{hand_side}/tactile_sensor')

    def tactile_sensor_feedback_callback(self, msg):
        for i, sensor_data in enumerate(msg.tactile_datas):
            self.get_logger().info(
                f'Sensor {i}: online={sensor_data.online_state}, '
                f'normal_force={sensor_data.normal_force}, '
                f'tangent_force={sensor_data.tangent_force}'
            )

def main(args=None):
    rclpy.init(args=args)
    
    hand_side = 'left'
    if len(sys.argv) > 1:
        hand_side = sys.argv[1]
    
    tactile_sensor_subscriber = TactileSensorSubscriber(hand_side)
    
    try:
        rclpy.spin(tactile_sensor_subscriber)
    except KeyboardInterrupt:
        pass
    except Exception as e:
        try:
            tactile_sensor_subscriber.get_logger().error(f'Error: {str(e)}')
        except Exception:
            print(f'Error: {str(e)}')
    finally:
        try:
            tactile_sensor_subscriber.destroy_node()
        except Exception:
            pass
        try:
            rclpy.shutdown()
        except Exception:
            pass

if __name__ == '__main__':
    main()
