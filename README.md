## 目标

根据每种情况下的所得出的数据（data文件夹）进行实验，并对每种实验情况进行截图，另外，需要记录机械臂末端运动的轨迹数据。

### 需要安装的软件

① ROS+Baxter simulator；

② MoveIt（[安装链接]：http://docs.ros.org/kinetic/api/moveit_tutorials/html/doc/getting_started/getting_started.html#install-moveit /"MoveIt教程"）；

③ 该package直接放在 `ros_ws/src` 目录下即可；

### 程序运行过程

#### 第一步：启动gazebo和moveit，启动后可拖动机械臂末端，在界面的planning选项下选择plan and execute即可使机械臂到达指定位置。

`终端1`：
`cd ros_ws`,
`./baxter.sh sim`,
`roslaunch baxter_gazebo baxter_world.launch`

`终端2`：
`cd ros_ws`,
`./baxter.sh sim`,
`rosrun baxter_interface joint_trajectory_action_server.py`

`终端3`：
`cd ros_ws`,
`./baxter.sh sim`,
`rosrun baxter_tools enable_robot.py -e`,
`roslaunch baxter_moveit_config demo_baxter.launch load_robot_description:=true right_electric_gripper:=true left_electric_gripper:=true`

#### 第二步，运行规划执行程序，在这个过程中挑一个合适的起始位姿。

`终端4`：
`cd ros_ws/src`,
`rosrun using_marker move_group_python_interface_tutorial.py joint_states:=/robot/joint_states`

程序执行过程中按回车即可，注意观察终端输出三个参数，分别是机械臂末端的x,y,z坐标。

#### 第三步，展示障碍物，执行程序前要先依据终端4给出的x,y,z坐标修改30-32行代码，替换前面的参数即可，另外33行的xx是用于动态障碍物的速度，使用静态障碍物时可将其注释掉。

`终端5`：
`cd ros_ws/src`,
`rosrun using_marker basic_shapes`

执行程序后记得在moveit界面找到add按钮，点击后找到marker并添加，然后修改frame为“/world”。

#### 第四步，展示轨迹，执行程序前要先依据终端4给出的x,y,z坐标修改89-91行代码，替换前面的参数即可。

`终端6`：
`cd ros_ws/src`,
`rosrun using_marker points_and_lines`

#### 第五步，保存机械臂执行时候的轨迹信息，这一步执行后要求再次执行第二步,因为机械臂动了后才需要记录轨迹变化信息。

`终端7`：
`cd ros_ws/src`,
`rosrun using_marker endpoint_recorder.py`

#### 注意：在第三，四步过程中注意修改读取数据文件路径，所有数据均在data文件夹下。

* 01-03表示修改起点，终点，起点和终点；新起点[0.15,-0.15],新终点[-1.1,0.2]

* 11-13表示障碍物于起始阶段，中间阶段和末尾阶段的避障轨迹，具体信息发图给你；

* 21-23表示障碍物旋转pi/4，pi/2，3pi/4得到的轨迹信息，此时要修改障碍物的姿态信息；

* 6表示动态避障过程，此时需要考虑xx的作用，x方向是-0.004，y方向是0.0035。
