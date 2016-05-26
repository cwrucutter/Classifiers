cmake_minimum_required(VERSION 2.8.3)

message("RUNNING PYCLASSIFIERS CMAKELISTS")

project(pyclassifiers)

find_package(catkin REQUIRED
    rospy
)

catkin_package(

)

catkin_add_nosetests(scripts/unitTest)
