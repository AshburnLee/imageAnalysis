cmake_minimum_required(VERSION 3.5)
project(mission1Src)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(mission1Src main.cpp grayScale.h colorScale.h testing.h target.h)

target_link_libraries(mission1Src ${OpenCV_LIBS})