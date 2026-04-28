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
            'omnihand_3_ultra_m_node.yaml'
        ),
        description='Path to the configuration YAML file'
    )

    # 进程级 namespace 用产品代号 "h3u_m"（与 URDF 包 h3u_m_description / C++ 命名空间
    # h3um 对齐），避免与 O10 ("o10") / O12 ("o12") 撞车。
    # 节点内部 sub-node 再用相对 ns "left"/"right"，最终话题为：
    #   /h3u_m/left/joint_states,  /h3u_m/left/joint_cmd
    #   /h3u_m/right/joint_states, /h3u_m/right/joint_cmd
    # JointState.position 单位：rad (REP-103)。
    omnihand_3_ultra_m_node = Node(
        package='omnihand_node',
        executable='omnihand_3_ultra_m_node',
        name='omnihand_3_ultra_m_param_reader',
        namespace='h3u_m',
        output='screen',
        parameters=[LaunchConfiguration('config_file')]
    )

    return LaunchDescription([
        config_file_arg,
        omnihand_3_ultra_m_node
    ])
