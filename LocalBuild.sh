export ROS_DISTRO=indigo
export CI_SOURCE_PATH=$(pwd)
export ROSINSTALL_FILE=$CI_SOURCE_PATH/dependencies.rosinstall
export CATKIN_OPTIONS=$CI_SOURCE_PATH/catkin.options
export ROS_PARALLEL_JOBS='-j8 -l6'

source /opt/ros/$ROS_DISTRO/setup.bash
sudo rosdep init
rosdep update

sudo pip install --upgrade pip


mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace

cd ~/catkin_ws
catkin_make
source devel/setup.bash

cd ~/catkin_ws/src
ln -s $CI_SOURCE_PATH .

source /opt/ros/$ROS_DISTRO/setup.bash
cd ~/catkin_ws
catkin_make $( [ -f $CATKIN_OPTIONS ] && cat $CATKIN_OPTIONS ) && catkin_make run_tests && catkin_test_results
