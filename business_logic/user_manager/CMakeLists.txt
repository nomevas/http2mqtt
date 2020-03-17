project(user_manager)

add_library(${PROJECT_NAME} user_manager.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC user_manager)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread mqtt_client domain_objects)

add_executable(${PROJECT_NAME}_test user_manager_unittest.cpp)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest gtest_main pthread ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME}_test gtest_framework)