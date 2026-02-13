import launch
import launch_ros
from ament_index_python.packages import get_package_share_directory#拼接出URDF目录
import os
import launch_ros.parameter_descriptions
def generate_launch_description():
    #1.获取URDF路径
    urdf_package_path= get_package_share_directory('fishbot_description')
    urdf_path=os.path.join(urdf_package_path,'urdf','fishbot','fishbot.urdf.xacro')
    #rviz2_path=os.path.join(urdf_package_path,'rviz2','display_robot_model.rviz')
    gazebo_path=os.path.join(urdf_package_path,'world','custom_room.world')
    #2.声明一个URDF目录的参数 方便修改 

    declare_arg_mode_path=launch.actions.DeclareLaunchArgument(
        name='model',
        default_value=str(urdf_path)
    )


    #通过文件路径获取内容，并转化成参数值对象。
    robot_state_publisher_content=launch.substitutions.Command(['xacro ',launch.substitutions.LaunchConfiguration('model')])
    robot_state_publisher_param=launch_ros.parameter_descriptions.ParameterValue(
    robot_state_publisher_content,  # 传入URDF文本内容
    value_type=str  # 明确告诉ROS2：这个参数的值类型是字符串
    )

    robot_state_publisher_node=launch_ros.actions.Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description':robot_state_publisher_param}]
    )

    # joint_state_publisher_node=launch_ros.actions.Node(
    #     package='joint_state_publisher',
    #     executable='joint_state_publisher'
    # )

    # rviz2_node=launch_ros.actions.Node(
    #     package='rviz2',
    #     executable='rviz2',
    #     arguments=['-d',rviz2_path]
    # )

    gazebo_node=launch.actions.IncludeLaunchDescription(
        launch.launch_description_sources.PythonLaunchDescriptionSource(
            [get_package_share_directory('gazebo_ros'),'/launch','/gazebo.launch.py']),
            launch_arguments=[('world',gazebo_path)]
    )

    spawn_entity_node=launch_ros.actions.Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-topic','/robot_description','-entity','fishbot']
    )

    return launch.LaunchDescription([
        declare_arg_mode_path,
        robot_state_publisher_node,
        # joint_state_publisher_node,
        #rviz2_node
        gazebo_node,
        spawn_entity_node
    ])