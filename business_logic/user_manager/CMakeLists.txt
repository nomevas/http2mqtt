project(user_manager)

add_library(${PROJECT_NAME} src/user_manager.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread mqtt_client router domain_objects)

add_executable(${PROJECT_NAME}_test test/unittest_user_manager.cpp)
target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest gtest_main pthread)

add_dependencies(${PROJECT_NAME}_test gtest_framework)