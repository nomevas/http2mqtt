project(business_logic)

add_executable(${PROJECT_NAME} main.cpp business_logic.cpp route.cpp)
target_link_libraries(${PROJECT_NAME} pthread mqtt_client)

add_dependencies(${PROJECT_NAME} taojson)