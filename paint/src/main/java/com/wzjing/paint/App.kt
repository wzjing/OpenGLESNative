package com.wzjing.paint

import android.app.Application
import android.content.Context
import android.content.res.AssetManager

public class App: Application() {

    override fun onCreate() {
        super.onCreate()
        assetManager = applicationContext.assets

    }

    companion object {
        @JvmStatic public var assetManager :AssetManager? = null
    }
}
