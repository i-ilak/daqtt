cmake_minimum_required(VERSION 3.22)
project(daqtt VERSION 1.0.0 LANGUAGES CXX)

find_package(PahoMqttCpp REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


include(GNUInstallDirs)

add_library(${PROJECT_NAME}
    src/sender.cpp
    src/subscriber.cpp
    src/connector.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_link_libraries(${PROJECT_NAME} PUBLIC
    PahoMqttCpp::paho-mqttpp3-static
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1)

install(TARGETS ${PROJECT_NAME} EXPORT daqttConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT daqttConfig DESTINATION share/daqtt/cmake)

export(TARGETS ${PROJECT_NAME} FILE daqttConfig.cmake)
