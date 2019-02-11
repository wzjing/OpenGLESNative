#ifndef PAINT_GLUTILS_H
#define PAINT_GLUTILS_H

#include <stdlib.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "native-utils.h"

//Check OpenGL ES error
void checkGlError(const char* op);

//print OpenGL ES String by enum
void printGlInfo();

//load shader by shader code
GLuint loadShader(GLenum shaderType, const char* pSource);

//create OpenGL ES program by [Vertex Shader code] and [Fragment Shader code]
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

//void perspectiveM(float* mat, int offset,
//                  float fovy, float aspect, float zNear, float zFar);

#endif //PAINT_GLUTILS_H