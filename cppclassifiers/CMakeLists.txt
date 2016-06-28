cmake_minimum_required(VERSION 2.8.3)

message("RUNNING CPPCLASSIFIERS CMAKELISTS")

set( CMAKE_VERBOSE_MAKEFILE "ON" )

# check c++11 / c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

get_filename_component( CMAKE_LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cmake ABSOLUTE )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_LOCAL_ROOT}/modules )

project( cppclassifiers )


get_filename_component( SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cpp/src ABSOLUTE )

get_filename_component( INC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include ABSOLUTE )

get_filename_component( TEST_ROOT ${SRC_ROOT}/unitTest ABSOLUTE )


find_package( catkin REQUIRED
    roscpp
)

catkin_package(
)


set( ${PROJECT_NAME}_SRCS
    ${SRC_ROOT}/Neural/INeuron.cpp
    ${SRC_ROOT}/Neural/Synapse.cpp
    ${SRC_ROOT}/Neural/CppNeuron.cpp
    ${SRC_ROOT}/Neural/FFNeuralNet.cpp
)

set( ${PROJECT_NAME}_PUBLIC_HEADERS
    ${INC_ROOT}/Neural/INeuron.hpp
    ${INC_ROOT}/Neural/FFNeuralNet.hpp
)

set( ${PROJECT_NAME}_PRIVATE_HEADERS
    ${INC_ROOT}/Neural/Synapse.hpp
    ${INC_ROOT}/Neural/CppNeuron.hpp
)

set( ${PROJECT_NAME}_TEST_SRCS
    ${TEST_ROOT}/Common/TestFunctions.cpp

    ${TEST_ROOT}/TestFixtures/NeuronTestFixtures.cpp
    ${TEST_ROOT}/TestFixtures/NetTestFixtures.cpp

    ${TEST_ROOT}/CppNeuron_unit.cpp
    ${TEST_ROOT}/FFNeuralNet_unit.cpp
    ${TEST_ROOT}/main.cpp
)

set( ${PROJECT_NAME}_TEST_HEADERS
    ${TEST_ROOT}/Common/TestFunctions.hpp
    ${TEST_ROOT}/Common/FriendDeclarations.hpp

    ${TEST_ROOT}/TestFixtures/NeuronTestFixtures.hpp
    ${TEST_ROOT}/TestFixtures/NetTestFixtures.hpp
)

source_group( PublicHeaders FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} )

set( INCLUDE_DIRS
    ${INC_ROOT}
    ${SRC_ROOT}
)

include_directories( ${INCLUDE_DIRS} )

add_library( ${PROJECT_NAME}
    ${${PROJECT_NAME}_SRCS}
    ${${PROJECT_NAME}_PUBLIC_HEADERS}
    ${${PROJECT_NAME}_PRIVATE_HEADERS}
)

catkin_add_gtest(${PROJECT_NAME}_UNIT
    ${${PROJECT_NAME}_TEST_SRCS}
    ${${PROJECT_NAME}_TEST_HEADERS}
)
if( TARGET ${PROJECT_NAME}_UNIT )
    target_link_libraries( ${PROJECT_NAME}_UNIT ${PROJECT_NAME} )
endif()
