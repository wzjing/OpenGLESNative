#include <android/bitmap.h>
#include "native-lib.h"

JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_initGLES(JNIEnv *env, jobject obj, jint w, jint h, jobject bitmap) {
    setGraphics(env, w, h, bitmap);
}

JNIEXPORT void
JNICALL Java_com_wzjing_paint_GLESView_step(JNIEnv *env, jobject obj) {
    renderFrame();
}

bool setGraphics(JNIEnv* env, int w, int h, jobject bitmap) {
    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

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

    mTexSamplehandle = glGetUniformLocation(gProgram, "tex_sampler");
    mTexCoordHandle = glGetAttribLocation(gProgram, "a_texcoord");
    mPosCoordHandle = glGetAttribLocation(gProgram, "a_position");

    glGenTextures(2, mTextures);
    checkGlError("gen Textures");
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    checkGlError("bind Textures");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.w, frame.h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frame.pixels);
    checkGlError("add a picture");
    initTextureParams();


    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    if (mPosVertex != NULL) {
        float imgAspectRatio = (float)frame.w / frame.h;
        float viewAspectRatio = (float)w / h;
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
        mPosVertex[0] = x0;
        mPosVertex[1] = y0;
        mPosVertex[2] = x1;
        mPosVertex[3] = y0;
        mPosVertex[4] = x0;
        mPosVertex[5] = y1;
        mPosVertex[6] = x1;
        mPosVertex[7] = y1;
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

    // 3、vertex
    glVertexAttribPointer(mTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, mTexVertex);
    checkGlError("TexVertex");
    glEnableVertexAttribArray(mTexCoordHandle);
    checkGlError("TexVertexHandle");

    // 4、position
    glVertexAttribPointer(mPosCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, mPosVertex);
    checkGlError("PosVertex");
    glEnableVertexAttribArray(mPosCoordHandle);
    checkGlError("PosVertexHandle");

    //5、Enable Texture draw
    glActiveTexture(GL_TEXTURE0);
    checkGlError("Active texture");
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    checkGlError("Bind texture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.w, frame.h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frame.pixels);
    checkGlError("draw image");
    glUniform1i(mTexSamplehandle, 0);
    checkGlError("glUniform1i");

    //5、Draw vertex
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("clear color");
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("clear");
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("draw Arrays");

    LOGD(TAG, "OpenGL ES frame: %d", (int) ((clock() - start) / 1000));
}


GLuint loadShader(GLenum shaderType, const char *pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char *buf = (char *) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE(TAG, "Could not compile shader %d: \n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}


GLuint createProgram(const char *pVertexSource, const char *pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader)
        return 0;
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader)
        return 0;

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachVertexShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachPixelShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char *buf = (char *) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE(TAG, "Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void initTextureParams() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}