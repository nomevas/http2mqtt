cmake_minimum_required(VERSION 3.15)

project(Http2MqttBridge)

set(CMAKE_CXX_STANDARD 17)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include(InstallRedboltz.cmake)
include(InstallTaoJson.cmake)

find_package(Boost REQUIRED COMPONENTS system date_time thread)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

add_subdirectory(utilities/mqtt_client)
add_subdirectory(utilities/http2mqtt)
add_subdirectory(generated/domain_objects)
add_subdirectory(generated/server)
add_subdirectory(business_logic/user_manager)
add_subdirectory(business_logic/user_manager_daemon)