#include <android/bitmap.h>
#include "native-lib.h"

JNIEXPORT void
JNICALL
Java_com_wzjing_paint_GLESView_initGLES(JNIEnv *env, jobject obj, jint w, jint h, jobject bitmap) {
    setGraphics(env, w, h, bitmap);
}

JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_step(JNIEnv *env, jobject obj) {
    renderFrame();
}

float textureBuffer[8] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
float vertexBuffer[8] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

GLuint sampler2DHandle;
GLuint vertexCoordHandle;
GLuint textureCoordHandle;
GLuint matrixHandle;

float projectionMatrix[16];

GLuint texture;

GLuint gProgram;

Frame frame;

bool setGraphics(JNIEnv *env, int w, int h, jobject bitmap) {

    printGlInfo();

    //Vertex Shader(Processing vertex position)
    const char *VERTEX_SHADER_CODE = loadAssetFile(env, "shaders/vertex_shader.shader");
    //Fragment Shader(Processing pixels)
    const char *FRAGMENT_SHADER_CODE = loadAssetFile(env, "shaders/fragment_shader.shader");


    AndroidBitmapInfo bmp_info = {0};
    if (AndroidBitmap_getInfo(env, bitmap, &bmp_info) < 0) {
        LOGD(TAG, "nativeProcess(): Unable to get bitmap info");
        return false;
    } else
        LOGD(TAG, "nativeProcess(): Bitmap Info: %d x %d <format: %d>", bmp_info.width,
             bmp_info.height,
             bmp_info.format);
    void *bmp_pixels = 0;
    if (AndroidBitmap_lockPixels(env, bitmap, &bmp_pixels) < 0) {
        LOGE(TAG, "nativeProcess(): Unable to lock bitmap pixels");
        return false;
    }

    if (!bmp_pixels) {
        LOGE(TAG, "nativeProcess(): didn't get any pixels");
        return false;
    }
    frame.w = bmp_info.width;
    frame.h = bmp_info.height;
    frame.pixels = bmp_pixels;
//    AndroidBitmap_unlockPixels(env, bitmap);

    LOGI(TAG, "setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(VERTEX_SHADER_CODE, FRAGMENT_SHADER_CODE);
    if (!gProgram) {
        LOGE(TAG, "Could not create program.");
        return false;
    }

    sampler2DHandle = (GLuint) glGetUniformLocation(gProgram, "tex");
    vertexCoordHandle = (GLuint) glGetAttribLocation(gProgram, "position");
    textureCoordHandle = (GLuint) glGetAttribLocation(gProgram, "textureCoord");
    matrixHandle = (GLuint) glGetUniformLocation(gProgram, "uMatrix");

    glGenTextures(1, &texture);
    checkGlError("gen Textures");
    glBindTexture(GL_TEXTURE_2D, texture);
    checkGlError("bind Textures");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.w, frame.h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, bmp_pixels);
    checkGlError("add a picture");

    //Init Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    if (vertexBuffer != NULL) {
        float imgAspectRatio = (float) frame.w / frame.h;
        float viewAspectRatio = (float) w / h;
        float relativeAspectRatio = viewAspectRatio / imgAspectRatio;
        float x0, y0, x1, y1;
        if (relativeAspectRatio > 1.0f) {
            x0 = -1.0f / relativeAspectRatio;
            y0 = -1.0f;
            x1 = 1.0f / relativeAspectRatio;
            y1 = 1.0f;
        } else {
            x0 = -1.0f;
            y0 = -relativeAspectRatio;
            x1 = 1.0f;
            y1 = relativeAspectRatio;
        }
        vertexBuffer[0] = x0;
        vertexBuffer[1] = y0;
        vertexBuffer[2] = x1;
        vertexBuffer[3] = y0;
        vertexBuffer[4] = x0;
        vertexBuffer[5] = y1;
        vertexBuffer[6] = x1;
        vertexBuffer[7] = y1;
    }

    return true;
}

void renderFrame() {
    long start = clock();

    // 1、Clear OpenGL
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    // 2、Attach GLSL shader program
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    // Matrix Handle
    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, projectionMatrix);

    // Vertex Handle
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), vertexBuffer);
    checkGlError("VertexBuffer");
    glEnableVertexAttribArray(vertexCoordHandle);
    checkGlError("vertexHandle");

    // Texture Handle
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), textureBuffer);
    checkGlError("PosVertex");
    glEnableVertexAttribArray(textureCoordHandle);
    checkGlError("PosVertexHandle");

    //
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(sampler2DHandle, 0);

    // Draw Texture
    glDrawElements(GL_TRIANGLE_STRIP, frame.w*frame.h* sizeof(GL_UNSIGNED_SHORT_5_6_5), GL_UNSIGNED_SHORT_5_6_5, frame.pixels);

    LOGD(TAG, "OpenGL ES frame: %d", (int) ((clock() - start) / 1000));
}