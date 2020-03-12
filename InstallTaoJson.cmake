include(ExternalProject)

ExternalProject_Add(taojson
        GIT_REPOSITORY https://github.com/taocpp/json.git
        CMAKE_ARGS
        -DTAOCPP_JSON_BUILD_EXAMPLES=OFF
        -DTAOCPP_JSON_BUILD_TESTS=OFF
        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})