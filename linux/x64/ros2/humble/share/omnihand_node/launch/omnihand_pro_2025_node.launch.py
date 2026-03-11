"""
OmniHand Pro 2025 (O12) ROS2 Launch File

Usage:
  # Default (uses config/omnihand_pro_2025_node.yaml - single hand, left)
  ros2 launch omnihand_node omnihand_pro_2025_node.launch.py

  # Use specific config file
  ros2 launch omnihand_node omnihand_pro_2025_node.launch.py config_file:=config/omnihand_pro_2025_node.yaml

  # Override parameters via launch arguments
  ros2 launch omnihand_node omnihand_pro_2025_node.launch.py hand_type:=right

  # Both hands (using config file)
  ros2 launch omnihand_node omnihand_pro_2025_node.launch.py config_file:=config/omnihand_pro_2025_node.yaml

Note: O12 only supports CANFD (zlg_can or hcan), not RS485
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
import os


def generate_launch_description():
    # Get package share directory
    pkg_share = FindPackageShare('omnihand_node').find('omnihand_node')
    config_dir = os.path.join(pkg_share, 'config')
    
    # Default config file path (can be overridden by config_file argument)
    # Default: omnihand_pro_2025_node.yaml (single hand, left - same as code defaults)
    default_config_file = PathJoinSubstitution([
        config_dir,
        'omnihand_pro_2025_node.yaml'
    ])
    
    # Declare config_file argument
    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value=default_config_file,
        description='Path to YAML configuration file'
    )
    
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
    
    connection_type_arg = DeclareLaunchArgument(
        'connection_type',
        default_value='zlg_can',
        description='Connection type: zlg_can or hcan (O12 only supports CANFD)'
    )
    
    canfd_serial_number_arg = DeclareLaunchArgument(
        'canfd_serial_number',
        default_value='',
        description='CANFD adapter serial number (recommended, stable after reboot)'
    )
    
    canfd_device_id_arg = DeclareLaunchArgument(
        'canfd_device_id',
        default_value='0',
        description='CANFD adapter device index (alternative, may change after reboot)'
    )
    
    canfd_channel_id_arg = DeclareLaunchArgument(
        'canfd_channel_id',
        default_value='0',
        description='CAN channel index (0 or 1)'
    )
    
    # Parameters for second hand (optional - if provided, second hand will be started)
    second_hand_type_arg = DeclareLaunchArgument(
        'second_hand_type',
        default_value='',
        description='Second hand type (left or right). If empty, only first hand is started'
    )
    
    second_hand_device_id_arg = DeclareLaunchArgument(
        'second_hand_device_id',
        default_value='1',
        description='Second hand device ID'
    )
    
    second_connection_type_arg = DeclareLaunchArgument(
        'second_connection_type',
        default_value='zlg_can',
        description='Second hand connection type: zlg_can or hcan'
    )
    
    second_canfd_serial_number_arg = DeclareLaunchArgument(
        'second_canfd_serial_number',
        default_value='',
        description='Second hand CANFD adapter serial number (recommended)'
    )
    
    second_canfd_device_id_arg = DeclareLaunchArgument(
        'second_canfd_device_id',
        default_value='0',
        description='Second hand CANFD adapter device index (alternative)'
    )
    
    second_canfd_channel_id_arg = DeclareLaunchArgument(
        'second_canfd_channel_id',
        default_value='1',
        description='Second hand CAN channel index'
    )

    # Create node with parameters
    # Note: Node name is set in hand_node.cpp constructor based on parameters
    omnihand_node = Node(
        package='omnihand_node',
        executable='omnihand_pro_2025_node',
        output='screen',
        parameters=[
            LaunchConfiguration('config_file'),
            {
                'hand_type': LaunchConfiguration('hand_type'),
                'hand_device_id': LaunchConfiguration('hand_device_id'),
                'connection_type': LaunchConfiguration('connection_type'),
                'canfd_serial_number': LaunchConfiguration('canfd_serial_number'),
                'canfd_device_id': LaunchConfiguration('canfd_device_id'),
                'canfd_channel_id': LaunchConfiguration('canfd_channel_id'),
                'second_hand_type': LaunchConfiguration('second_hand_type'),
                'second_hand_device_id': LaunchConfiguration('second_hand_device_id'),
                'second_connection_type': LaunchConfiguration('second_connection_type'),
                'second_canfd_serial_number': LaunchConfiguration('second_canfd_serial_number'),
                'second_canfd_device_id': LaunchConfiguration('second_canfd_device_id'),
                'second_canfd_channel_id': LaunchConfiguration('second_canfd_channel_id'),
            }
        ]
    )

    return LaunchDescription([
        config_file_arg,
        hand_type_arg,
        hand_device_id_arg,
        connection_type_arg,
        canfd_serial_number_arg,
        canfd_device_id_arg,
        canfd_channel_id_arg,
        second_hand_type_arg,
        second_hand_device_id_arg,
        second_connection_type_arg,
        second_canfd_serial_number_arg,
        second_canfd_device_id_arg,
        second_canfd_channel_id_arg,
        omnihand_node,
    ])
