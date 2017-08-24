#include "native-utils.h"

char *loadAssetFile(JNIEnv *env, const char *filename) {
    AAssetManager *mgr = getAssetManager(env);
    AAsset *asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
    if (asset == NULL) {
        LOGE(TAG, "Asset not found");
        exit(0);
    }
    long size = AAsset_getLength(asset);
    char *buffer = (char *) malloc(sizeof(char) * size);
    int ret = AAsset_read(asset, buffer, size);
    if (ret < 0) {
        LOGE(TAG, "Read assset error: %d", ret);
        exit(0);
    }
    else
        LOGI(TAG, "Asset Content: %s", buffer);
    AAsset_close(asset);
    return buffer;
}

AAssetManager *getAssetManager(JNIEnv *env) {
    jclass App = env->FindClass("com/wzjing/paint/App");
    jfieldID assets_fieldId = env->GetFieldID(App, "assetManager", "Landroid/content/res/AssetManager");
    jobject assets = env->GetStaticObjectField(App, assets_fieldId);
    return AAssetManager_fromJava(env, assets);
}