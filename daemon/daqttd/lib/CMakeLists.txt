find_package(PahoMqttCpp REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(DEFAULT_BUILD_TYPE "Release")

include(GNUInstallDirs)

add_library(daqtt STATIC
    src/sender.cpp
    src/subscriber.cpp
    src/connector.cpp
)
target_include_directories(daqtt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_link_libraries(daqtt PUBLIC
    PahoMqttCpp::paho-mqttpp3-static
)

set_target_properties(daqtt PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1)

install(TARGETS daqtt EXPORT daqttConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT daqttConfig DESTINATION share/daqtt/cmake)

export(TARGETS daqtt FILE daqttConfig.cmake)
