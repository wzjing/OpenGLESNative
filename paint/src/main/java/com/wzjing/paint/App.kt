package com.wzjing.paint

import android.app.Application
import android.content.Context
import android.content.res.AssetManager

public class App: Application() {

    override fun onCreate() {
        super.onCreate()
        appContext = applicationContext
    }

    companion object {
        public lateinit var appContext: Context
        @JvmStatic public var assetManager = appContext.assets
    }
}
