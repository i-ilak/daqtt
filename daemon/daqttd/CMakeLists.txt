cmake_minimum_required(VERSION 3.15)

project(daqttd VERSION 1.0)

find_package(libconfig REQUIRED)

add_subdirectory(lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(daqttd src/main.cpp src/daemon.cpp)

target_link_libraries(daqttd PRIVATE daqtt libconfig::libconfig)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/config/daqttd.conf DESTINATION /etc/daqttd)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/systemd/daqttd.service DESTINATION /etc/systemd/system)
install(TARGETS daqttd DESTINATION /usr/bin)
