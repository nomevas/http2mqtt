project(mqtt_client)

add_library(${PROJECT_NAME} mqtt_client.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} .)
target_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_ERROR_CODE_HEADER_ONLY)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

add_dependencies(${PROJECT_NAME} redboltz)