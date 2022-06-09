package com.wzjing.openglesnative

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.util.Log
import java.lang.Exception
import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.egl.EGLContext
import javax.microedition.khronos.egl.EGLDisplay
import javax.microedition.khronos.opengles.GL10

class GLESView(context: Context) : GLSurfaceView(context) {

    private val TAG = "GLESView"

    private val renderer = Renderer()

    private val bitmap: Bitmap

    init {
        val options = BitmapFactory.Options()
        options.inPreferredConfig = Bitmap.Config.ARGB_8888
//        bitmap = BitmapFactory.decodeResource(context.resources, R.drawable.pikachu, options)
        bitmap = BitmapFactory.decodeStream(context.assets.open("test.png"), null, options)!!

        setEGLContextClientVersion(2)
//        setEGLConfigChooser(8, 8, 8, 8, 24, 0)
//        holder.setFormat(PixelFormat.TRANSLUCENT)
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    private fun findConfigAttrib(egl: EGL10, display: EGLDisplay,
                                 config: EGLConfig, attribute: Int, defaultValue: Int): Int {
        val mValue: IntArray = intArrayOf(0)
        return if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
            mValue[0]
        } else defaultValue
    }

    inner class Renderer : GLSurfaceView.Renderer {
        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            try {
                val egl10 = EGLContext.getEGL() as EGL10
                val display = egl10.eglGetCurrentDisplay()
                val r = findConfigAttrib(egl10, display, config!!, EGL10.EGL_RED_SIZE, 0)
                val g = findConfigAttrib(egl10, display, config, EGL10.EGL_GREEN_SIZE, 0)
                val b = findConfigAttrib(egl10, display, config, EGL10.EGL_BLUE_SIZE, 0)
                val a = findConfigAttrib(egl10, display, config, EGL10.EGL_ALPHA_SIZE, 0)
                val depth = findConfigAttrib(egl10, display, config, EGL10.EGL_DEPTH_SIZE, 0)
                val stencil = findConfigAttrib(egl10, display, config, EGL10.EGL_STENCIL_SIZE, 0)
                Log.i(TAG, "onSurfaceCreated: $r $g $b $a $depth $stencil")

            } catch (e: Exception) {
                Log.e(TAG, "unable to get alpha size")
                e.printStackTrace()
            }
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
            System.loadLibrary("gl-engine")
        }
    }

}