"""
OmniHand 2025 (O10) ROS2 Launch File

Usage:
  # Default (uses config/omnihand_2025_node.yaml - single hand, left)
  ros2 launch omnihand_node omnihand_2025_node.launch.py

  # Use specific config file
  ros2 launch omnihand_node omnihand_2025_node.launch.py config_file:=config/omnihand_2025_node.yaml

  # Override parameters via launch arguments
  ros2 launch omnihand_node omnihand_2025_node.launch.py hand_type:=right

  # Both hands (using config file)
  ros2 launch omnihand_node omnihand_2025_node.launch.py config_file:=config/omnihand_2025_node.yaml
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
import os

# Default hand bus ID; must stay in sync with agilink::omnihand::OmniHand2025::kDefaultHandDeviceId (1u).
_DEFAULT_HAND_DEVICE_ID = '1'


def generate_launch_description():
    # Declare launch arguments (names match Python API)
    hand_type_arg = DeclareLaunchArgument(
        'hand_type',
        default_value='left',
        description='Hand type: left or right'
    )
    
    hand_device_id_arg = DeclareLaunchArgument(
        'hand_device_id',
        default_value=_DEFAULT_HAND_DEVICE_ID,
        description='Hand device ID (1-255); default matches OmniHand2025::kDefaultHandDeviceId'
    )
    
    connection_type_arg = DeclareLaunchArgument(
        'connection_type',
        default_value='zlg_can',
        description='Connection type: zlg_can, hcan, or rs485'
    )
    
    canfd_serial_number_arg = DeclareLaunchArgument(
        'canfd_serial_number',
        default_value='',
        description='CANFD adapter serial number (recommended, stable after reboot)'
    )
    
    canfd_device_id_arg = DeclareLaunchArgument(
        'canfd_device_id',
        default_value='0',
        description='USB CANFD adapter device index (alternative, for zlg_can/hcan)'
    )
    
    canfd_channel_id_arg = DeclareLaunchArgument(
        'canfd_channel_id',
        default_value='0',
        description='CAN channel index (0 or 1, for zlg_can/hcan)'
    )
    
    uart_port_arg = DeclareLaunchArgument(
        'uart_port',
        default_value='/dev/ttyUSB0',
        description='UART port path (for rs485)'
    )
    
    baudrate_arg = DeclareLaunchArgument(
        'baudrate',
        default_value='460800',
        description='Baud rate (for rs485)'
    )
    
    second_hand_type_arg = DeclareLaunchArgument(
        'second_hand_type',
        default_value='',
        description='Second hand type when dual-hand mode is enabled (empty string means single hand mode)'
    )
    
    second_hand_device_id_arg = DeclareLaunchArgument(
        'second_hand_device_id',
        default_value=_DEFAULT_HAND_DEVICE_ID,
        description='Second hand device ID; default matches OmniHand2025::kDefaultHandDeviceId'
    )
    
    second_connection_type_arg = DeclareLaunchArgument(
        'second_connection_type',
        default_value='zlg_can',
        description='Second hand connection type: zlg_can, hcan, or rs485'
    )
    
    second_canfd_serial_number_arg = DeclareLaunchArgument(
        'second_canfd_serial_number',
        default_value='',
        description='Second hand CANFD adapter serial number (recommended, stable after reboot)'
    )
    
    second_canfd_device_id_arg = DeclareLaunchArgument(
        'second_canfd_device_id',
        default_value='0',
        description='Second hand CANFD adapter device index (alternative, for zlg_can/hcan)'
    )
    
    second_canfd_channel_id_arg = DeclareLaunchArgument(
        'second_canfd_channel_id',
        default_value='1',
        description='Second hand CAN channel index (for zlg_can/hcan)'
    )
    
    second_uart_port_arg = DeclareLaunchArgument(
        'second_uart_port',
        default_value='/dev/ttyUSB1',
        description='Second hand UART port path (for rs485)'
    )
    
    second_baudrate_arg = DeclareLaunchArgument(
        'second_baudrate',
        default_value='460800',
        description='Second hand baud rate (for rs485)'
    )

    # Get package share directory for config files
    config_dir = PathJoinSubstitution([
        FindPackageShare('omnihand_node'),
        'config'
    ])
    
    # Default config file path (can be overridden by config_file argument)
    # Default: omnihand_2025_node.yaml (single hand, left - same as code defaults)
    default_config_file = PathJoinSubstitution([
        config_dir,
        'omnihand_2025_node.yaml'
    ])
    
    # Declare config_file argument
    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value=default_config_file,
        description='Path to YAML configuration file'
    )
    
    # Create node with parameters
    # Priority: YAML config file (loaded via config_file argument)
    # Note: Launch arguments with default values will override YAML values
    # To use YAML values, don't pass launch arguments, or modify YAML file directly
    # Note: Node name is set in hand_node.cpp constructor based on parameters
    omnihand_node = Node(
        package='omnihand_node',
        executable='omnihand_2025_node',
        output='screen',
        parameters=[
            LaunchConfiguration('config_file'),  # Load from YAML file first
        ]
    )

    return LaunchDescription([
        config_file_arg,  # Config file argument (highest priority)
        hand_type_arg,
        hand_device_id_arg,
        connection_type_arg,
        canfd_serial_number_arg,
        canfd_device_id_arg,
        canfd_channel_id_arg,
        uart_port_arg,
        baudrate_arg,
        second_hand_type_arg,
        second_hand_device_id_arg,
        second_connection_type_arg,
        second_canfd_serial_number_arg,
        second_canfd_device_id_arg,
        second_canfd_channel_id_arg,
        second_uart_port_arg,
        second_baudrate_arg,
        omnihand_node,
    ])
