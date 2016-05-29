cmake_minimum_required(VERSION 2.8.3)

message("RUNNING PYCLASSIFIERS CMAKELISTS")

project(pyclassifiers)

find_package(catkin REQUIRED
    rospy
)

catkin_package(

)

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/../coveralls-cmake/cmake )

# include(Coveralls)
# coveralls_turn_on_coverage()
# set( COVERAGE_SRCS ${PROJECT_SOURCE_DIR}/scripts/testFile.py )
# coveralls_setup(
#     "${COVERAGE_SRCS}"                          # the source files
#     ON                                          # upload coverage
#     "${PROJECT_SOURCE_DIR}/../coveralls-cmake/cmake"    # path to project cmake modules
# )

catkin_add_nosetests(unitTest)
