cmake_minimum_required(VERSION 3.5.0)

add_library(native-lib
        SHARED
        native-lib.cpp
        gl-utils.cpp
        native-utils.cpp)

target_link_libraries(native-lib log jnigraphics android EGL GLESv3 z)