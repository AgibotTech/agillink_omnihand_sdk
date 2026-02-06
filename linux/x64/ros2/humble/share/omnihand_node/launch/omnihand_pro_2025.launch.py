"""
OmniHand Pro 2025 (O12) ROS2 Launch File

Usage:
  # Default (left hand)
  ros2 launch omnihand_node omnihand_pro_2025.launch.py

  # Right hand
  ros2 launch omnihand_node omnihand_pro_2025.launch.py hand_type:=right

  # Custom configuration (parameter names match Python API)
  ros2 launch omnihand_node omnihand_pro_2025.launch.py hand_type:=left hand_device_id:=1 canfd_device_id:=0 canfd_channel_id:=0

  # Both hands
  ros2 launch omnihand_node omnihand_pro_2025.launch.py enable_both_hands:=true canfd_channel_id:=0 second_canfd_channel_id:=1
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # Declare launch arguments (names match Python API)
    hand_type_arg = DeclareLaunchArgument(
        'hand_type',
        default_value='left',
        description='Hand type: left or right'
    )
    
    hand_device_id_arg = DeclareLaunchArgument(
        'hand_device_id',
        default_value='1',
        description='Hand device ID (1-255)'
    )
    
    canfd_device_id_arg = DeclareLaunchArgument(
        'canfd_device_id',
        default_value='0',
        description='USB CANFD adapter device index'
    )
    
    canfd_channel_id_arg = DeclareLaunchArgument(
        'canfd_channel_id',
        default_value='0',
        description='CAN channel index (0 or 1)'
    )
    
    enable_both_hands_arg = DeclareLaunchArgument(
        'enable_both_hands',
        default_value='true',
        description='Enable dual-hand mode'
    )
    
    second_hand_type_arg = DeclareLaunchArgument(
        'second_hand_type',
        default_value='right',
        description='Second hand type when dual-hand mode is enabled'
    )
    
    second_hand_device_id_arg = DeclareLaunchArgument(
        'second_hand_device_id',
        default_value='1',
        description='Second hand device ID'
    )
    
    second_canfd_device_id_arg = DeclareLaunchArgument(
        'second_canfd_device_id',
        default_value='0',
        description='Second hand CANFD adapter device index'
    )
    
    second_canfd_channel_id_arg = DeclareLaunchArgument(
        'second_canfd_channel_id',
        default_value='1',
        description='Second hand CAN channel index'
    )

    # Create node with parameters
    omnihand_node = Node(
        package='omnihand_node',
        executable='omnihand_pro_2025_node',
        name='omnihand_pro_2025_node',
        output='screen',
        parameters=[{
            'hand_type': LaunchConfiguration('hand_type'),
            'hand_device_id': LaunchConfiguration('hand_device_id'),
            'canfd_device_id': LaunchConfiguration('canfd_device_id'),
            'canfd_channel_id': LaunchConfiguration('canfd_channel_id'),
            'enable_both_hands': LaunchConfiguration('enable_both_hands'),
            'second_hand_type': LaunchConfiguration('second_hand_type'),
            'second_hand_device_id': LaunchConfiguration('second_hand_device_id'),
            'second_canfd_device_id': LaunchConfiguration('second_canfd_device_id'),
            'second_canfd_channel_id': LaunchConfiguration('second_canfd_channel_id'),
        }]
    )

    return LaunchDescription([
        hand_type_arg,
        hand_device_id_arg,
        canfd_device_id_arg,
        canfd_channel_id_arg,
        enable_both_hands_arg,
        second_hand_type_arg,
        second_hand_device_id_arg,
        second_canfd_device_id_arg,
        second_canfd_channel_id_arg,
        omnihand_node,
    ])
