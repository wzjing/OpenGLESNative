cmake_minimum_required(VERSION 3.4.0)

add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp )

set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

add_library( opencv_core STATIC IMPORTED )
set_target_properties( opencv_core PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libopencv_core.a )

add_library( opencv_features2d STATIC IMPORTED )
set_target_properties( opencv_features2d PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libopencv_features2d.a )

add_library( opencv_highgui STATIC IMPORTED )
set_target_properties( opencv_highgui PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libopencv_highgui.a )

add_library( opencv_imgcodecs STATIC IMPORTED )
set_target_properties( opencv_imgcodecs PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libopencv_imgcodecs.a )

add_library( opencv_imgproc STATIC IMPORTED )
set_target_properties( opencv_imgproc PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libopencv_imgproc.a )

add_library( opencv_objdetect STATIC IMPORTED )
set_target_properties( opencv_objdetect PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libopencv_objdetect.a )

add_library( IlmImf STATIC IMPORTED)
set_target_properties( IlmImf PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libIlmImf.a )

add_library( libtiff STATIC IMPORTED)
set_target_properties( libtiff PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/liblibtiff.a )

add_library( libjpeg STATIC IMPORTED)
set_target_properties( libjpeg PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/liblibjpeg.a )

add_library( libpng STATIC IMPORTED)
set_target_properties( libpng PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/liblibpng.a )

add_library( libwebp STATIC IMPORTED)
set_target_properties( libwebp PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/liblibwebp.a )

add_library( cpufeatures STATIC IMPORTED)
set_target_properties( cpufeatures PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libcpufeatures.a )

add_library( libjasper STATIC IMPORTED)
set_target_properties( libjasper PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/liblibjasper.a )

add_library( libprotobuf STATIC IMPORTED)
set_target_properties( libprotobuf PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/liblibprotobuf.a )

add_library( tbb STATIC IMPORTED)
set_target_properties( tbb PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libtbb.a )

add_library( tegra_hal STATIC IMPORTED)
set_target_properties( tegra_hal PROPERTIES IMPORTED_LOCATION ${JNI_LIBS_DIR}/libtegra_hal.a )

include_directories( ${CMAKE_SOURCE_DIR}/src/libs/opencv/include )

set(OPENCV_LIBS opencv_features2d opencv_core opencv_objdetect opencv_imgproc opencv_highgui opencv_imgcodecs tegra_hal)
set(THIRD_PART_LIBS cpufeatures IlmImf libjasper libtiff libjpeg libpng libwebp tbb tegra_hal)

# arm abi libs is diffrent from x86 abi libs, we need make diffrent configuration
#if(ANDROID_ABI STREQUAL "armeabi-v7a" OR ANDROID_ABI STREQUAL "arm64-v8a")
#    message("arm lib configuration")
#    set(OPENCV_LIBS ${OPENCV_LIBS} tegra_hal)
#endif()

#set(ANDROID_ALLOW_UNDEFINED_SYMBOLS TRUE)

target_link_libraries(native-lib ${OPENCV_LIBS} log jnigraphics android EGL GLESv2 z)