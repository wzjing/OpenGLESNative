#include <android/bitmap.h>
#include <EGL/egl.h>
#include "native-lib.h"
#include "native-utils.h"

#define GLM_FORCE_CXX11

#include "glm/glm.hpp"
#include "glm/ext.hpp"

extern "C" JNIEXPORT void
JNICALL
Java_com_wzjing_paint_GLESView_initGLES(JNIEnv *env, jobject, jint w, jint h, jobject bitmap) {
    setGraphics(env, w, h, bitmap);
}

extern "C" JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_step(JNIEnv *, jobject) {
    renderFrame();
}

float size[2];

float vertexBuffer[8] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

GLuint sampler2DHandle;
GLuint vertexPositionHandle;
GLuint mvpHandle;
GLuint resolutionHandle;
GLuint timeHandle;

GLuint texture;

GLuint gProgram;

static Frame frame;

bool setGraphics(JNIEnv *env, int w, int h, jobject bitmap) {

    printGlInfo();

    EGLContext mEGLContext = eglGetCurrentContext();
    EGLDisplay display = eglGetCurrentDisplay();

    EGLint attrs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };

    GLint numberOfConfigs;
    EGLConfig  mConfigs[10];
//    EGLBoolean eglSuccess = eglChooseConfig(display, attrs, mConfigs, 10, &numberOfConfigs);
//    if (eglSuccess) {
//        LOGD(TAG, "choose config success");
//    } else {
//        LOGD(TAG, "choose config fail");
//    }

    size[0] = w;
    size[1] = h;

    //Vertex Shader(Processing vertex position)
    const char *VERTEX_SHADER_CODE = loadAssetFile(env, "shader/vertex_shader.glsl");
//    LOGI(TAG, "Vertex Shader: \n%s", VERTEX_SHADER_CODE);
    //Fragment Shader(Processing pixels)
    const char *FRAGMENT_SHADER_CODE = loadAssetFile(env, "shader/fragment_texture.glsl");
//    LOGI(TAG, "Fragment Shader: \n%s", FRAGMENT_SHADER_CODE);

    Bitmap *mBitmap = getBitmap(env, bitmap);
    frame.w = mBitmap->width;
    frame.h = mBitmap->height;
    frame.pixels = mBitmap->pixels;

    LOGI(TAG, "setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(VERTEX_SHADER_CODE, FRAGMENT_SHADER_CODE);
    if (!gProgram) {
        LOGE(TAG, "Could not create program.");
        return false;
    }

    sampler2DHandle = (GLuint) glGetUniformLocation(gProgram, "iChannel0");
    vertexPositionHandle = (GLuint) glGetAttribLocation(gProgram, "vertexPosition");
    mvpHandle = (GLuint) glGetUniformLocation(gProgram, "mvp");
    resolutionHandle = (GLuint) glGetUniformLocation(gProgram, "iResolution");
    timeHandle = (GLuint) glGetUniformLocation(gProgram, "iGlobalTime");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.w, frame.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 frame.pixels);

    //Init Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);

    //Configure the texture data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.w, frame.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 frame.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(sampler2DHandle, 0);


    glViewport(0, 0, w, h);
    LOGI(TAG, "Width: %d, Height: %d", w, h);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearStencil(0);
    glClearDepthf(0);

    return true;
}

void renderFrame() {

    float sec = (float) clock() / CLOCKS_PER_SEC;

    // Clear cache
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Attach GLSL shader program
    glUseProgram(gProgram);

    // Shader: iGlobalTime
    glUniform1f(timeHandle, sec);

    // Shader: iResolution
    glUniform2f(resolutionHandle, size[0], size[1]);

    // Shader: projectionMatrix;
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) size[0] / size[1], 0.1f,
                                            100.0f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(4, 3, 3),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );
    glm::mat4 Model = glm::mat4(1.0f);

    float degree = 360 * sin((float) clock() / (10 * CLOCKS_PER_SEC));
    glm::mat4 rotatedView = glm::rotate(View, degree, glm::vec3(0, 1, 0));

    glm::mat4 mvp = Projection * rotatedView * Model;
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &mvp[0][0]);

    // Vertex Handle
    glEnableVertexAttribArray(vertexPositionHandle);
    glVertexAttribPointer(vertexPositionHandle, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          vertexBuffer);

    //Draw the basic rect
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}