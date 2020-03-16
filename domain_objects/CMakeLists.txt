project(domain_objects)

add_library(${PROJECT_NAME} user.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC .)
add_dependencies(${PROJECT_NAME} taojson)