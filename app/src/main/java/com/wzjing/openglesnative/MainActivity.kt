package com.wzjing.openglesnative

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.View

class MainActivity : AppCompatActivity() {

    private val TAG = "MainActivity"

    private val uiHide = (View.SYSTEM_UI_FLAG_IMMERSIVE
            or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
            or View.SYSTEM_UI_FLAG_FULLSCREEN)

    override fun onCreate(savedInstanceState: Bundle?) {
        window.decorView.setOnSystemUiVisibilityChangeListener { visibility ->
            if (visibility and View.SYSTEM_UI_FLAG_FULLSCREEN == 0) {
                Log.w(TAG, "System bars visible")
                Thread {
                    try {
                        Thread.sleep(1000)
                    } catch (e: InterruptedException) {
                        e.printStackTrace()
                    }
                    runOnUiThread {
                        window.decorView.systemUiVisibility = uiHide
                    }
                }.start()
            } else {
                Log.w(TAG, "System bars invisible")
            }
        }

        super.onCreate(savedInstanceState)
        setContentView(GLESView(applicationContext))
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        Log.w("MainActivity", "Windows Focus: $hasFocus")
        if (hasFocus) {
            window.decorView.systemUiVisibility = uiHide
        }
    }
}
