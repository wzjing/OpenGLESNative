package com.wzjing.paint

import android.graphics.Bitmap
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.GLUtils
import android.util.Log
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyRenderer(var frame: Bitmap) : GLSurfaceView.Renderer {

    private var mProgram: Int? = null
    private var mTexSampleHandle: Int? = null
    private var mTexCoordHandle: Int? = null
    private var mPosCoordHandle: Int? = null
    private var mTexVertices: FloatBuffer? = null
    private var mPosVertices: FloatBuffer? = null
    private val mTextures = IntArray(2)

    public fun update(frame: Bitmap) {
        this.frame = frame
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        val TEX_VERTICES = floatArrayOf( 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f)
        mTexVertices = ByteBuffer.allocateDirect(TEX_VERTICES.size * 4).order(ByteOrder.nativeOrder()).asFloatBuffer()
        mTexVertices!!.put(TEX_VERTICES).position(0)

        val POS_VERTICES = floatArrayOf(-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f )
        mPosVertices = ByteBuffer.allocateDirect(POS_VERTICES.size * 4).order(ByteOrder.nativeOrder()).asFloatBuffer()
        mPosVertices!!.put(POS_VERTICES).position(0)

        val VERTEX_SHADER_CODE =
                "attribute vec4 a_position;\n" +
                "attribute vec2 a_texcoord;\n" +
                "varying vec2 v_texcoord;\n" +
                "void main() {\n" +
                "  gl_Position = a_position;\n" +
                "  v_texcoord = a_texcoord;\n" +
                "}\n"
        val FRAGMENT_SHADER_CODE =
                "precision mediump float;\n" +
                "uniform sampler2D tex_sampler;\n" +
                "varying vec2 v_texcoord;\n" +
                "void main() {\n" +
                "  gl_FragColor = texture2D(tex_sampler, v_texcoord);\n" +
                "}\n"
        mProgram = createProgram(VERTEX_SHADER_CODE, FRAGMENT_SHADER_CODE)
        mTexSampleHandle = GLES20.glGetUniformLocation(mProgram!!, "tex_sampler")
        mTexCoordHandle = GLES20.glGetAttribLocation(mProgram!!, "a_texcoord")
        mPosCoordHandle = GLES20.glGetAttribLocation(mProgram!!, "a_position")

        GLES20.glGenTextures(2, mTextures, 0)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextures[0])
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, frame, 0)
        initTextureNeedParams()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        checkGLerror("glViewport")

        if (mPosVertices != null) {
            val imgAspectRatio:Float = frame.width / frame.height.toFloat()
            val viewAspectRatio: Float = width/ height.toFloat()
            val relativeAspectRatio = viewAspectRatio / imgAspectRatio
            val x0: Float
            val y0: Float
            val x1: Float
            val y1: Float
            if (relativeAspectRatio > 1.0f) {
                x0 = -1.0f / relativeAspectRatio
                y0 = -1.0f
                x1 = 1.0f / relativeAspectRatio
                y1 = 1.0f
            } else {
                x0 = -1.0f
                y0 = -relativeAspectRatio
                x1 = 1.0f
                y1 = relativeAspectRatio
            }
            val coords = floatArrayOf(x0, y0, x1, y0, x0, y1, x1, y1)
            mPosVertices?.put(coords)?.position(0)
        }
    }

    override fun onDrawFrame(gl: GL10?) {
        val start = System.currentTimeMillis()
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT and GLES20.GL_DEPTH_BUFFER_BIT)
        GLES20.glUseProgram(mProgram!!)
        checkGLerror("glUseProgram")

        GLES20.glVertexAttribPointer(mTexCoordHandle!!, 2, GLES20.GL_FLOAT, false, 0, mTexVertices)
        GLES20.glEnableVertexAttribArray(mTexCoordHandle!!)
        checkGLerror("vertex attribute setup(texCoord)")

        GLES20.glVertexAttribPointer(mPosCoordHandle!!, 2, GLES20.GL_FLOAT, false, 0, mPosVertices)
        GLES20.glEnableVertexAttribArray(mPosCoordHandle!!)
        checkGLerror("vertex attribute setup(posCoord)")

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        checkGLerror("glActiveTexture")
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextures[0])
        checkGLerror("glBindTexture")
        GLES20.glUniform1i(mTexSampleHandle!!, 0)

        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f)
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4)
        Log.i("MyRenderer", "drawFrame: ${System.currentTimeMillis()-start} ms")
    }

    companion object {

        public fun loadShader(type: Int, source: String): Int {
            val shader = GLES20.glCreateShader(type)
            if (shader != 0) {
                GLES20.glShaderSource(shader, source)
                GLES20.glCompileShader(shader)
                val compiled = intArrayOf(1)
                GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0)
                if (compiled[0] == 0) {
                    val info = GLES20.glGetShaderInfoLog(shader)
                    GLES20.glDeleteShader(shader)
                    throw RuntimeException("Could not compile shader: $type ($info)")
                }
            }
            return shader
        }

        public fun createProgram(vertexSourceCode: String, fragmentSourceCode: String):Int {
            val vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSourceCode)
            if (vertexShader == 0) return 0
            val pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSourceCode)
            if (pixelShader == 0) return 0
            val program = GLES20.glCreateProgram()
            if (program != 0) {
                GLES20.glAttachShader(program, vertexShader)
                checkGLerror("glAttachShader")
                GLES20.glAttachShader(program, pixelShader)
                checkGLerror("glLinkProgram")
                GLES20.glLinkProgram(program)
                val linkStatus = IntArray(1)
                GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0)
                if (linkStatus[0] != GLES20.GL_TRUE) {
                    val info = GLES20.glGetProgramInfoLog(program)
                    GLES20.glDeleteProgram(program)
                    throw RuntimeException("Could not link program: $info")
                }
            }
            return program
        }

        public fun checkGLerror(op: String) {
            while (true) {
                val error = GLES20.glGetError()
                if (error != GLES20.GL_NO_ERROR) throw RuntimeException("$op: glError $error") else break
            }

        }

        public fun initTextureNeedParams() {
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
        }
    }

}