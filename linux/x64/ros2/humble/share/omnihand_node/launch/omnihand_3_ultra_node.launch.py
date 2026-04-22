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
            'omnihand_3_ultra_node.yaml'
        ),
        description='Path to the configuration YAML file'
    )

    # 进程级 namespace 用产品代号 "h3u"，避免与 O10 ("o10") / O12 ("o12") 撞车。
    # 节点内部 sub-node 再用相对 ns "left"/"right"，最终话题为：
    #   /h3u/left/joint_states,  /h3u/left/joint_cmd
    #   /h3u/right/joint_states, /h3u/right/joint_cmd
    # JointState.position 单位：rad (REP-103)。
    omnihand_3_ultra_node = Node(
        package='omnihand_node',
        executable='omnihand_3_ultra_node',
        name='omnihand_3_ultra_param_reader',
        namespace='h3u',
        output='screen',
        parameters=[LaunchConfiguration('config_file')]
    )

    return LaunchDescription([
        config_file_arg,
        omnihand_3_ultra_node
    ])
