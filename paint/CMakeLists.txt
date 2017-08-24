cmake_minimum_required(VERSION 3.4.0)

add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp
             src/main/cpp/gl-utils.cpp
             src/main/cpp/native-utils.cpp)

target_link_libraries(native-lib log jnigraphics android EGL GLESv2 z)