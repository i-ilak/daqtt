cmake_minimum_required(VERSION 3.15)

project(subscriber VERSION 1.0)

find_package(PahoMqttCpp REQUIRED)
find_package(daqtt REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(subscriber main.cpp)

target_link_libraries(subscriber PRIVATE daqtt PahoMqttCpp::paho-mqttpp3-static)
