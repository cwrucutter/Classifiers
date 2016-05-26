cmake_minimum_required(VERSION 3.3)
project(pyclassifiers)

find_package(catkin REQUIRED
    rospy
)

catkin_package(

)

catkin_add_nosetests(scripts/unitTest)
