package com.wzjing.paint

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GLESView(context: Context) : GLSurfaceView(context) {

    private val renderer = Renderer()
    private val bitmap = BitmapFactory.decodeResource(context.resources, R.mipmap.ic_launcher)

    init {
        setEGLContextClientVersion(2)
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
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