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

char *pix16(char *data) {
    char pixs[16][255];
    for (int i = 0; i < 16; i++) {
        sprintf(pixs[i], "%s %02x", i < 1 ? "" : pixs[i - 1], data[i]);
    }
    return pixs[15];
}

float vertexBuffer[8] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};
float textureBuffer[8] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

GLint sampler2DHandle;
GLint vertexCoordHandle;
GLint textureCoordHandle;
GLint matrixHandle;
GLint timeHandle;

float projectionMatrix[16];

GLuint texture;

GLuint gProgram;

static Frame frame;

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
    if (AndroidBitmap_lockPixels(env, bitmap, &frame.pixels) < 0) {
        LOGE(TAG, "nativeProcess(): Unable to lock bitmap pixels");
        return false;
    }

    if (!frame.pixels) {
        LOGE(TAG, "nativeProcess(): didn't get any pixels");
        return false;
    }
    frame.w = bmp_info.width;
    frame.h = bmp_info.height;

    LOGI(TAG, "Pix: %s", pix16((char*)frame.pixels));

    LOGI(TAG, "setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(VERTEX_SHADER_CODE, FRAGMENT_SHADER_CODE);
    if (!gProgram) {
        LOGE(TAG, "Could not create program.");
        return false;
    }

    sampler2DHandle = glGetUniformLocation(gProgram, "tex");
    vertexCoordHandle = glGetAttribLocation(gProgram, "vertexCoord");
    textureCoordHandle = glGetAttribLocation(gProgram, "textureCoord");
    matrixHandle = glGetUniformLocation(gProgram, "uMatrix");
    timeHandle = glGetUniformLocation(gProgram, "time");

    LOGI(TAG, "samper2DHandle:     %d", sampler2DHandle);
    LOGI(TAG, "vertexCoordhandle:  %d", vertexCoordHandle);
    LOGI(TAG, "textureCoordhandle: %d", textureCoordHandle);
    LOGI(TAG, "matrixHandle:       %d", matrixHandle);
    LOGI(TAG, "timeHandle:         %d", timeHandle);

    glGenTextures(1, &texture);
    checkGlError("gen Textures");
    glBindTexture(GL_TEXTURE_2D, texture);
    checkGlError("bind Textures");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.w, frame.h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frame.pixels);
    checkGlError("add a picture");

    //Init Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    LOGI(TAG, "Width: %d, Height: %d", w, h);

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

    glUniform1f(timeHandle, (float)clock()/CLOCKS_PER_SEC);

    // Matrix Handle
    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, projectionMatrix);
    checkGlError("set Matrix");

    // Vertex Handle
    glVertexAttribPointer(vertexCoordHandle, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), vertexBuffer);
    checkGlError("VertexBuffer");
    glEnableVertexAttribArray(vertexCoordHandle);
    checkGlError("vertexHandle");

    // Texture Handle
    glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 2* sizeof(GL_UNSIGNED_BYTE), textureBuffer);
    checkGlError("textureBuffer");
    glEnableVertexAttribArray(textureCoordHandle);
    checkGlError("textureHandle");

    //
    glActiveTexture(GL_TEXTURE0);
    checkGlError("activeTexture");
    glBindTexture(GL_TEXTURE_2D, texture);
    checkGlError("bind Texture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.w, frame.h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frame.pixels);
    checkGlError("glTexImage2D");
    glUniform1i(glGetUniformLocation(gProgram, "tex"), 0);
    checkGlError("set tex sampler");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("glDrawArray");

    LOGD(TAG, "Frame Info: %d x %d Data: %02x", frame.w, frame.h, ((unsigned char*)frame.pixels)[100]);

    //LOGD(TAG, "OpenGL ES frame: %d", (int) ((clock() - start) / 1000));
}