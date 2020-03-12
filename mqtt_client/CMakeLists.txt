project(mqtt_client)


include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
ExternalProject_Add(redboltz
        GIT_REPOSITORY https://github.com/redboltz/mqtt_cpp.git
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        -DMQTT_BUILD_EXAMPLES=OFF
        -DMQTT_BUILD_TESTS=OFF
        -DBOOST_ROOT=${BOOST_ROOT})

add_library(${PROJECT_NAME} mqtt_client.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} . ${EXTERNAL_INSTALL_LOCATION}/include)
target_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_ERROR_CODE_HEADER_ONLY)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

add_dependencies(${PROJECT_NAME} redboltz)