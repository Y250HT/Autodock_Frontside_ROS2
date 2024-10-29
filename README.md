# autodock_ros2_humble
This repository allows people to use apriltag code for positioning and automatic adjustment of pose in ros2 humble version, and it has been successfully tested in real cars, but there may be some cumbersome adjustment process, and parameters need to be adjusted to improve. In the project, we use the behavior tree to integrate our code, so that our project can first navigate, navigate to the target point, execute Apriltag code positioning, and then return to the second target point.
## Device requirement
| 设备 | 描述               | 版本   | 依赖包     |
|----------|--------------------|--------|------------|
| REALSENSE DEPTH  CAMERA D435   | 深度相机    | D435   | realsense2_camera |
| Esp32   | 底盘控制板    | ESP32-S3   | micro_ros_agent |
## Reference link
https://github.com/micro-ROS/micro-ROS-Agent/tree/humble

https://github.com/Adlink-ROS/apriltag_docking
