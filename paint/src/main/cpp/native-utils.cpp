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
    else
        LOGI(TAG, "Asset Content: \n%s", buffer);
    AAsset_close(asset);
    return buffer;
}

AAssetManager *getAssetManager(JNIEnv *env) {
    jclass App = env->FindClass("com/wzjing/paint/App");
    jfieldID assets_fieldId = env->GetStaticFieldID(App, "assetManager", "Landroid/content/res/AssetManager;");
    jobject assets = env->GetStaticObjectField(App, assets_fieldId);
    return AAssetManager_fromJava(env, assets);
}