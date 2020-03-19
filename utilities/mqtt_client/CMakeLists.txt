project(mqtt_client)

add_library(${PROJECT_NAME} mqtt_client.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} .)
target_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_ERROR_CODE_HEADER_ONLY)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

add_dependencies(${PROJECT_NAME} redboltz)

add_executable(${PROJECT_NAME}_test mqtt_client_unittest.cpp)
target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest gtest_main pthread ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME}_test gtest_framework)