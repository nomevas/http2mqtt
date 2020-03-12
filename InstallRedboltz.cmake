include(ExternalProject)

ExternalProject_Add(redboltz
        GIT_REPOSITORY https://github.com/redboltz/mqtt_cpp.git
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        -DMQTT_BUILD_EXAMPLES=OFF
        -DMQTT_BUILD_TESTS=OFF
        -DBOOST_ROOT=${BOOST_ROOT})