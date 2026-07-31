import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value=os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
            'config',
            'omnihand_3_lite_node.yaml'
        ),
        description='Path to the configuration YAML file'
    )

    omnihand_3_lite_node = Node(
        package='omnihand_node',
        executable='omnihand_3_lite_node',
        name='omnihand_3_lite_param_reader',
        namespace='h3l',
        output='screen',
        parameters=[LaunchConfiguration('config_file')]
    )

    return LaunchDescription([
        config_file_arg,
        omnihand_3_lite_node
    ])
