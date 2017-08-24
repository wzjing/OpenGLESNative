#ifndef PAINT_NATIVE_UTILS_H
#define PAINT_NATIVE_UTILS_H

#include <stdlib.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

namespace nu {

#define TAG "native-log"

#define LOGV(tag, format, ...) __android_log_print(ANDROID_LOG_VERBOSE, tag, format, ## __VA_ARGS__)
#define LOGD(tag, format, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, format, ## __VA_ARGS__)
#define LOGI(tag, format, ...) __android_log_print(ANDROID_LOG_INFO, tag, format, ## __VA_ARGS__)
#define LOGW(tag, format, ...) __android_log_print(ANDROID_LOG_WARNING, tag, format, ## __VA_ARGS__)
#define LOGE(tag, format, ...) __android_log_print(ANDROID_LOG_ERROR, tag, format, ## __VA_ARGS__)

}

char * loadAssetFile(JNIEnv * env, const char * filename);

AAssetManager * getAssetManager(JNIEnv * env);


#endif //PAINT_NATIVE_UTILS_H
