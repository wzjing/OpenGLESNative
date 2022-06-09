cmake_minimum_required(VERSION 3.5.0)

add_library(gl-engine
        SHARED
        gl-engine.cpp
        gl-utils.cpp
        native-utils.cpp)

# glm
SET(GLM_TEST_ENABLE OFF CACHE BOOL "disable glm test")
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
#target_link_libraries(native-lib PRIVATE glm)

target_link_libraries(gl-engine PRIVATE glm log jnigraphics android EGL GLESv3 z)