cmake_minimum_required(VERSION 3.15)

project(Http2MqttBridge)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS system date_time thread)

add_subdirectory(mqtt_client)
add_subdirectory(client)
add_subdirectory(http2mqtt)
add_subdirectory(request_handler)