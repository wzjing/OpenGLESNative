package com.wzjing.paint

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Rect
import android.opengl.GLSurfaceView
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import org.jetbrains.anko.coroutines.experimental.bg

class MySurfaceView(context: Context): GLSurfaceView(context), SurfaceHolder.Callback2 {

    private val TAG = "MySurfaceView"

    private var bitmap: Bitmap? = null
    private var rending = false
    private var myRenderer: MyRenderer
    private var frame: Bitmap

    init {
        holder.addCallback(this)
        setEGLContextClientVersion(2)
        frame = BitmapFactory.decodeResource(context.resources, R.mipmap.ic_launcher)
        myRenderer = MyRenderer(frame)
        setRenderer(myRenderer)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    private fun drawFarme() {
        val canvas = holder.lockCanvas()
        if (canvas == null)
            return
        val start: Long = System.currentTimeMillis()
        canvas.drawBitmap(bitmap!!, Rect(0, 0, bitmap?.width?:0, bitmap?.height?:0), Rect(0, 0, canvas.width, canvas.height), null)
        Log.i(TAG, "Frame time: ${System.currentTimeMillis()-start}")
        holder.unlockCanvasAndPost(canvas)
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        super.surfaceCreated(holder)
        bitmap = BitmapFactory.decodeResource(context.resources, R.drawable.person)
        rending = true

        bg {
            while(rending) {
//                val start: Long = System.currentTimeMillis()
//                myRenderer.update(frame)
//                requestRender()
//                Log.i(TAG, "Frame time: ${System.currentTimeMillis()-start} ms")
                drawFarme()
            }
        }

    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, w: Int, h: Int) {
        super.surfaceChanged(holder, format, w, h)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        super.surfaceDestroyed(holder)
        rending = false
        bitmap?.recycle()
    }

}
