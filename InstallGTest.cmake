include(ExternalProject)

ExternalProject_Add(gtest_framework
GIT_REPOSITORY https://github.com/google/googletest.git
CMAKE_ARGS
-DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})