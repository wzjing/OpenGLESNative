#include <android/bitmap.h>
#include "native-lib.h"
#include "native-utils.h"

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
GLuint vertexCoordHandle;
GLuint matrixHandle;
GLuint resolutionHandle;
GLuint timeHandle;

float projectionMatrix[16];
float modelMatrix[16];
float viewMatrix[16];

GLuint texture;

GLuint gProgram;

static Frame frame;

bool setGraphics(JNIEnv *env, int w, int h, jobject bitmap) {

    printGlInfo();

    size[0] = w;
    size[1] = h;

    //Vertex Shader(Processing vertex position)
    const char *VERTEX_SHADER_CODE = loadAssetFile(env, "shader/vertex_shader.glsl");
    //Fragment Shader(Processing pixels)
    const char *FRAGMENT_SHADER_CODE = loadAssetFile(env, "shader/fragment_shape.glsl");

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
    vertexCoordHandle = (GLuint) glGetAttribLocation(gProgram, "vertexCoord");
    matrixHandle = (GLuint) glGetUniformLocation(gProgram, "projectionMatrix");
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

    //Configure the texture data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.w, frame.h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 frame.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(sampler2DHandle, 0);
//    glBindTexture(GL_TEXTURE_2D, 0);


//    perspectiveM(projectionMatrix, 0, 45, (float)w/h, 2, 6);
    glViewport(0, 0, w, h);
    LOGI(TAG, "Width: %d, Height: %d", w, h);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearStencil(0);
    glClearDepthf(0);

    return true;
}

void renderFrame() {

    // Clear cache
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Attach GLSL shader program
    glUseProgram(gProgram);

    // Shader: iGlobalTime
    glUniform1f(timeHandle, (float) clock() / CLOCKS_PER_SEC);

    // Shader: iResolution
    glUniform2f(resolutionHandle, size[0], size[1]);

    // Shader: projectionMatrix;
    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, projectionMatrix);

    // Vertex Handle
    glEnableVertexAttribArray(vertexCoordHandle);
    glVertexAttribPointer(vertexCoordHandle, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          vertexBuffer);

    //Draw the basic rect
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}