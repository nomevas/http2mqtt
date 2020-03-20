project(user_manager_daemon)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} pthread mqtt_client domain_objects user_manager request_handler)