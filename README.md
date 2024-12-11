# Autodock_ROS2_Humble
This repository allows people to use apriltag code for positioning and automatic adjustment of pose in ROS2 humble version, and it has been successfully tested in real cars, but there may be some cumbersome adjustment process, and parameters need to be adjusted to improve. In the project, we use the Behavior Tree to integrate our code, so that our project can first navigate, navigate to the target point, execute Apriltag code positioning, and then return to the second target point. But in this repository, i just show how to get Apriltag code and Follow.

<p align="center">
  <img src="readme_source/readme1.gif" alt="说明文本">
</p>

## Device requirement
| 设备 | 描述               | 版本   | 依赖包     |
|----------|--------------------|--------|------------|
| REALSENSE DEPTH  CAMERA D435   | 深度相机    | D435   | realsense2_camera |
| Esp32   | 底盘控制板    | ESP32-S3   | micro_ros_agent |

## Install dependencies
```bash
$ cd ~/autodock_ros2_humble/
$ rosdep update
$ rosdep install --from-paths src --ignore-src -r -y --rosdistro humble
$ cd apriltag
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --target install
```

## Build BT_ros2 with main workspace:

```bash
$ git clone https://github.com/Y250HT/Behavior_Ros2.git
$ sudo apt-get install ros-humble-realsense2*
$ cd ~/autodock_ros2_humble/
$ source /opt/ros/humble/setup.bash
$ colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Release
```

## Use Physical Bot and RealSense D435

Step 1. Turn on MicroRos in your robot chRassis.

```bash
$ ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888 -v6
```

Step 2. Launch RealSenseD435 in real world.

```bash
$ ros2 launch realsense2_camera rs_launch.py
```

Step 3. Send request to start docking

```bash
$ ros2 launch bt_ros2 bt_ros2.launch.py
```

## BT configurations

```xml
<root main_tree_to_execute="MainTree">
  <BehaviorTree ID="MainTree">
    <Sequence>
      <AutodockClient action="start"/>
      <AutodockClient action="cancel"/>
    </Sequence>
  <BehaviorTree>
</root>
```

## Reference link
https://github.com/micro-ROS/micro-ROS-Agent/tree/humble

https://github.com/Adlink-ROS/apriltag_docking

https://github.com/wjwwood/serial


