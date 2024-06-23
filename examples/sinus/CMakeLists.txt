cmake_minimum_required(VERSION 3.16)

project(example_daq_zmq LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(PahoMqttCpp REQUIRED)
find_package(daqtt REQUIRED)

add_executable(sinus sinus.cpp)

target_link_libraries(sinus PRIVATE daqtt)
