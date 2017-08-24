#ifndef PAINT_GLUTILS_H
#define PAINT_GLUTILS_H

#include <stdlib.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "native-utils.h"

//Check OpenGL ES error
void checkGlError(const char* op);

//print OpenGL ES String by enum
void printGlInfo();

//load shader by shader code
GLuint loadShader(GLenum shaderType, const char* pSource);

//create OpenGL ES program by [Vertex Shader code] and [Fragment Shader code]
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

#endif //PAINT_GLUTILS_H