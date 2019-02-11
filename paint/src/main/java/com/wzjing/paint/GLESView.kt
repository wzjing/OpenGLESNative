package com.wzjing.paint

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GLESView(context: Context) : GLSurfaceView(context) {

    private val TAG = "GLESView"

    private val renderer = Renderer()

    private val bitmap : Bitmap

    init {
        val options = BitmapFactory.Options()
        options.inPreferredConfig = Bitmap.Config.ARGB_8888
        bitmap = BitmapFactory.decodeResource(context.resources, R.drawable.pikachu, options)

        setEGLContextClientVersion(2)
//        setEGLConfigChooser()
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
        Log.i(TAG, "ByteCount: ${bitmap.byteCount} ARGB8888:${bitmap.width*bitmap.height*32/4} RGB565:${bitmap.width*bitmap.height*16/4}")
    }

    inner class Renderer: GLSurfaceView.Renderer {
        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            initGLES(width, height, bitmap)
        }

        override fun onDrawFrame(gl: GL10?) {
            step()
        }

    }

    external fun initGLES(w: Int, h: Int, bitmap: Bitmap)
    external fun step()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

}