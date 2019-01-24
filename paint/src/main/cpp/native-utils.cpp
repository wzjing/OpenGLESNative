#include <android/bitmap.h>
#include "native-utils.h"

char* loadAssetFile(JNIEnv *env, const char *filename) {
    AAssetManager *mgr = getAssetManager(env);
    AAsset *asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
    if (asset == NULL) {
        LOGE(TAG, "Asset not found");
        exit(0);
    }
    long size = AAsset_getRemainingLength(asset);
    char * buffer = (char *) malloc(sizeof(char) * size);
    memset(buffer, 0, size);
    int ret = AAsset_read(asset, buffer, size);
    buffer[size] = '\0';\
    if (ret < 0) {
        LOGE(TAG, "Read assset error: %d", ret);
        exit(0);
    }
    AAsset_close(asset);
    return buffer;
}

AAssetManager *getAssetManager(JNIEnv *env) {
    jclass AndroidContext = env->FindClass("android/content/Context");
    jobject androidContext = getCurrentContext(env);
    jmethodID getAssets = env->GetMethodID(AndroidContext, "getAssets", "()Landroid/content/res/AssetManager;");
    return AAssetManager_fromJava(env, env->CallObjectMethod(androidContext, getAssets));
}

jobject getCurrentContext(JNIEnv * env) {
    jclass ActivityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(ActivityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject activityThread = env->CallStaticObjectMethod(ActivityThread, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(ActivityThread, "getApplication", "()Landroid/app/Application;");
    return env->CallObjectMethod(activityThread, getApplication);
}

Bitmap* getBitmap(JNIEnv* env, jobject bitmap) {

    Bitmap * mBitmap = (Bitmap *) malloc(sizeof(Bitmap));

    AndroidBitmapInfo bmp_info = {0};
    if (AndroidBitmap_getInfo(env, bitmap, &bmp_info) < 0) {
        LOGD(TAG, "nativeProcess(): Unable to get bitmap info");
        return 0;
    }
    LOGD(TAG, "nativeProcess(): Bitmap Info: %d x %d <format: %d>", bmp_info.width,
         bmp_info.height,
         bmp_info.format);
    mBitmap->width = bmp_info.width;
    mBitmap->height = bmp_info.height;
    mBitmap->format = bmp_info.format;
    if (AndroidBitmap_lockPixels(env, bitmap, &mBitmap->pixels) < 0) {
        LOGE(TAG, "nativeProcess(): Unable to lock bitmap pixels");
        return 0;
    }

    if (!mBitmap->pixels) {
        LOGE(TAG, "nativeProcess(): didn't get any pixels");
        return 0;
    }

    return mBitmap;
}