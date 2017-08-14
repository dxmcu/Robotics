cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(robot ${SOURCE_FILES})

target_link_libraries(robot HamsterAPICommon opencv_core opencv_highgui CogniAlgCPP HamsterAPIClientCPP)
