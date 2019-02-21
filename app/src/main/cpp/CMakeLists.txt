cmake_minimum_required(VERSION 3.5.0)



add_library(native-lib
        SHARED
        native-lib.cpp
        gl-utils.cpp
        native-utils.cpp)

# glm
set(GLM_TEST_ENABLE OFF)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
#target_link_libraries(native-lib PRIVATE glm)

target_link_libraries(native-lib PRIVATE log jnigraphics android EGL GLESv3 z glm)