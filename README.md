# Microros-on-DIscovery-board
stm32 discovery board from zhengdianyuanzi to control Panasonic A6 servo controller

## Prepare
1. copy config/freertos/stm32MakefileTest To $(ROS2_WORKSPACE)/src/micro_ros_setup/config/freertos/

2. copy freertos_apps/stm32MakefileTest/ To $(ROS2_WORKSPACE)/firmware/freertos_apps/

3. change the $(ROS2_WORKSPACE)/firmware/PLATFORM like this

```
freertos
stm32MakefileTest
```

## Build

1. 

```
cd microros_ws
source install/local_setup.bash
ros2 run micro_ros_setup configure_firmware.sh ping_pong -t serial -d 1
ros2 run micro_ros_setup build_firmware.sh
```
