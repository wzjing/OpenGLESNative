#ifndef FACE_NATIVE_LIB_H
#define FACE_NATIVE_LIB_H

#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <android/bitmap.h>
#include "native-utils.h"
#include "gl-utils.h"

extern "C" {
JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_initGLES(JNIEnv * env, jobject, jint w, jint h, jobject bitmap);

JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_step(JNIEnv * env, jobject);
}

typedef struct Frame {
    int w;
    int h;
    void * pixels;
} Frame;

bool setGraphics(JNIEnv* env, int w, int h, jobject bitmap);

void renderFrame();

#endif
