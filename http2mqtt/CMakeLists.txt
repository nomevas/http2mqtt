project(http2mqtt)

add_executable(${PROJECT_NAME} main.cpp http_server.cpp http2mqtt_bridge.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_ERROR_CODE_HEADER_ONLY)
target_link_libraries(${PROJECT_NAME} pthread mqtt_client)

add_dependencies(${PROJECT_NAME} taojson)