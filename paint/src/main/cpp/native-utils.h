#ifndef PAINT_NATIVE_UTILS_H
#define PAINT_NATIVE_UTILS_H

#include <stdlib.h>
#include <iostream>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <android/bitmap.h>

#define TAG "native-log"

#define LOGV(tag, format, ...) __android_log_print(ANDROID_LOG_VERBOSE, tag, format, ## __VA_ARGS__)
#define LOGD(tag, format, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, format, ## __VA_ARGS__)
#define LOGI(tag, format, ...) __android_log_print(ANDROID_LOG_INFO, tag, format, ## __VA_ARGS__)
#define LOGW(tag, format, ...) __android_log_print(ANDROID_LOG_WARNING, tag, format, ## __VA_ARGS__)
#define LOGE(tag, format, ...) __android_log_print(ANDROID_LOG_ERROR, tag, format, ## __VA_ARGS__)

using namespace std;

char* loadAssetFile(JNIEnv * env, const char * filename);

AAssetManager * getAssetManager(JNIEnv * env);

jobject getCurrentContext(JNIEnv * env);

typedef struct{
    int width;
    int height;
    int format;
    void* pixels = 0;
} Bitmap;

Bitmap* getBitmap(JNIEnv* env, jobject bitmap);

#endif //PAINT_NATIVE_UTILS_H
