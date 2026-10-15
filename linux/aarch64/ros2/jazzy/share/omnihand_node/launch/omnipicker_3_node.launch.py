import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    config_file = DeclareLaunchArgument(
        "config_file",
        default_value=os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
            "config",
            "omnipicker_3_node.yaml",
        ),
        description="Path to the OmniPicker 3 configuration YAML file",
    )

    node = Node(
        package="omnihand_node",
        executable="omnipicker_3_node",
        name="omnipicker_3_param_reader",
        namespace="op3",
        output="screen",
        parameters=[LaunchConfiguration("config_file")],
    )

    return LaunchDescription([config_file, node])
