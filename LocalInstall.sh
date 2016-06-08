export ROS_DISTRO=indigo
export ROS_CI_DESKTOP="`lsb_release -cs`"
export CI_SOURCE_PATH=$(pwd)
export ROSINSTALL_FILE=$CI_SOURCE_PATH/dependencies.rosinstall
export CATKIN_OPTIONS=$CI_SOURCE_PATH/catkin.options

# install barebones ROS
sudo sh -c "echo \"deb http://packages.ros.org/ros/ubuntu $ROS_CI_DESKTOP main\" > /etc/apt/sources.list.d/ros-latest.list"
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
sudo apt-get update -qq
sudo apt-get install -y python-catkin-pkg python-rosdep python-wstool ros-$ROS_DISTRO-catkin
source /opt/ros/$ROS_DISTRO/setup.bash
# rosdep will install dependencies
sudo rosdep init
rosdep update

sudo apt-get install python-pip
sudo pip install requests[security]
sudo pip install --upgrade pip
sudo apt-get install -y -qq lcov curl
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace
cd ~/catkin_ws
catkin_make
source devel/setup.bash
cd ~/catkin_ws/src
wstool init
if [[ -f $ROSINSTALL_FILE ]] ; then wstool merge $ROSINSTALL_FILE ; fi
wstool up
cd ~/catkin_ws
rosdep install -y --from-paths src --ignore-src --rosdistro $ROS_DISTRO
