#ifndef FACE_NATIVE_LIB_H
#define FACE_NATIVE_LIB_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

#define TAG "native-log"
#define LOGD(tag, format, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, format, ## __VA_ARGS__)
#define LOGI(tag, format, ...) __android_log_print(ANDROID_LOG_INFO, tag, format, ## __VA_ARGS__)
#define LOGE(tag, format, ...) __android_log_print(ANDROID_LOG_ERROR, tag, format, ## __VA_ARGS__)

extern "C" {
JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_initGLES(JNIEnv * env, jobject obj, jint w, jint h, jobject bitmap);

JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_step(JNIEnv * env, jobject);
}

inline void checkGlError(const char* op) {
    for (GLint error = glGetError(); error ; error = glGetError()) {
        LOGE(TAG, "Operation: %s Error: 0x%x", op, error);
    }
}

inline void printGlString(const char* name, GLenum v) {
    const char* s = (const char *) glGetString(v);
    LOGI(TAG, "OpenGL ES: %s = %s", name, s);
}

typedef struct Frame {
    int w;
    int h;
    void * pixels;
} Frame;

auto VERTEX_SHADER_CODE =
        "attribute vec4 a_position;\n"
        "attribute vec2 a_texcoord;\n"
        "varying vec2 v_texcoord;\n"
        "void main() {\n"
        "  gl_Position = a_position;\n"
        "  v_texcoord = a_texcoord;\n"
        "}\n";
auto FRAGMENT_SHADER_CODE =
        "precision mediump float;\n"
        "uniform sampler2D tex_sampler;\n"
        "varying vec2 v_texcoord;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(tex_sampler, v_texcoord);\n"
        "}\n";

float mTexVertex[8] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
float mPosVertex[8] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

int mTexSamplehandle;
int mTexCoordHandle;
int mPosCoordHandle;

GLuint mTextures[2];

Frame frame;

GLuint loadShader(GLenum shaderType, const char* pSource);

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

void initTextureParams();

bool setGraphics(JNIEnv* env, int w, int h, jobject bitmap);

void renderFrame();

#endif
