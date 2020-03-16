project(user_manager)

add_library(${PROJECT_NAME} user_manager.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread mqtt_client router domain_objects)